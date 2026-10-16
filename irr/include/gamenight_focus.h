// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include <cstdint>

// Retry only an explicit resume. Stop on success, user desktop input, pause,
// or after one second. Unsigned elapsed arithmetic also handles tick wrap.
class GameNightFocusHandoff {
public:
    void begin(uint32_t now, uint32_t input) {
        pending = true; started = lastAttempt = now; initialInput = input;
    }
    void cancel() { pending = false; }
    bool retry(uint32_t now, uint32_t input, bool owned) {
        if (!pending) return false;
        if (owned || input != initialInput || uint32_t(now - started) >= 1000) {
            cancel(); return false;
        }
        if (uint32_t(now - lastAttempt) < 100) return false;
        lastAttempt = now;
        return true;
    }
private:
    bool pending = false;
    uint32_t started = 0, lastAttempt = 0, initialInput = 0;
};
