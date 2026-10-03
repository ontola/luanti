// SPDX-License-Identifier: LGPL-2.1-or-later
// GameNight's opt-in, per-process controller binding. No OS input injection.
#pragma once
#include <cstdlib>
#include <string>
#include <array>
#include <chrono>
#include <fstream>
#include <cstdint>
inline const char *gamenightFramePath() {
    const char *p=std::getenv("GAMENIGHT_CONTROLLER_FRAME");
    return p && *p ? p : nullptr;
}
struct GameNightHostFrame {
    uint64_t sequence=0;
    bool active=false, connected=false;
    uint32_t buttons=0;
    std::array<int,6> axes{};
};
inline GameNightHostFrame gamenightHostFrame() {
    using Clock=std::chrono::steady_clock;
    static GameNightHostFrame frame;
    static auto checked=Clock::time_point{}, changed=Clock::time_point{};
    const auto now=Clock::now();
    const char *path=gamenightFramePath();
    if(!path) return frame;
    if(now-checked>=std::chrono::milliseconds(5)) {
        checked=now;
        std::ifstream file(path);
        GameNightHostFrame next;
        int active=0, connected=0;
        if(file >> next.sequence >> active >> connected >> next.buttons
            >> next.axes[0] >> next.axes[1] >> next.axes[2]
            >> next.axes[3] >> next.axes[4] >> next.axes[5]) {
            bool valid=active>=0 && active<=1 && connected>=0 && connected<=1 && next.buttons<16384;
            for(int axis:next.axes) valid &= axis>=-32768 && axis<=32767;
            if(valid && next.sequence>frame.sequence) {
                next.active=active;next.connected=connected;frame=next;changed=now;
            }
        }
    }
    auto result=frame;
    if(now-changed>std::chrono::milliseconds(250)) {
        result.connected=false;result.buttons=0;result.axes.fill(0);
    }
    if(now-changed>std::chrono::seconds(2)) result.active=false;
    return result;
}
inline bool gamenightControllerMode() {
    const char *p=std::getenv("GAMENIGHT_CONTROLLER_PATH");
    return (p && *p) || gamenightFramePath();
}
struct GameNightControllerBinding {
    std::string path;
    int id=-1;
    bool armed=false;
    bool attach(int instance, const std::string &candidate, bool neutral) {
        if (candidate.empty() || candidate != path || id >= 0) return false;
        id=instance; armed=neutral; return true;
    }
    bool accept(int instance, bool neutral) {
        if (instance != id || id < 0) return false;
        if (!armed) { armed=neutral; return false; }
        return true;
    }
    bool detach(int instance) {
        if(instance != id) return false;
        id=-1;armed=false;return true;
    }
};
