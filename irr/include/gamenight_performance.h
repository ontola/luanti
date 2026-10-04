// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include <chrono>
#include <cstdlib>
#include <fstream>

// Opt-in render completion timing. No framebuffer readback or image encoding.
// The adapter supplies a different output file for each client. Completion
// intervals include pacing, simulation, rendering and OS scheduling, not GPU
// execution time. Pauses/loading are excluded by the benchmark's time window.
inline void gamenightPerformanceFrame(unsigned width, unsigned height)
{
    static const char *path = std::getenv("GAMENIGHT_PERFORMANCE_LOG");
    if (!path || !*path) return;
    using Clock = std::chrono::steady_clock;
    static std::ofstream output(path, std::ios::trunc);
    static auto previous = Clock::now();
    static unsigned count = 0;
    const auto now = Clock::now();
    const auto interval = std::chrono::duration_cast<std::chrono::microseconds>(now - previous).count();
    previous = now;
    const auto unix_us = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    if (output) {
        output << unix_us << ',' << interval << ',' << width << ',' << height << '\n';
        if (++count % 60 == 0) output.flush();
    }
}
