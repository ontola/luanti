// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include <IVideoDriver.h>
#include <IImage.h>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

// Explicit capture-only opt-in. Records actual renderer frames at wall-clock speed.
// The harness supplies an existing directory in <root>/capture.txt. Each client
// writes independent frames and timestamps. Normal installations never enable it.
inline void gamenightCaptureFrame(video::IVideoDriver *driver) {
    const char *root = std::getenv("GAMENIGHT_CAPTURE_ROOT");
    const char *seat = std::getenv("GAMENIGHT_COUCH_SEAT");
    if (!root || !*root || !seat || !*seat) return;
    using Clock = std::chrono::steady_clock;
    static auto last = Clock::time_point{};
    auto now = Clock::now();
    if (now - last < std::chrono::milliseconds(32)) return;
    last = now;
    std::ifstream control(std::string(root) + "/capture.txt");
    std::string directory;
    if (!std::getline(control, directory) || directory.empty()) return;
    if (!directory.empty() && directory.back() == '\r') directory.pop_back();
    std::string selected;
    std::getline(control, selected);
    if (!selected.empty() && selected.back() == '\r') selected.pop_back();
    if (!selected.empty() && selected != seat) return;
    static std::string previous;
    static unsigned frame = 0;
    static auto start = now;
    if (directory != previous) {previous=directory;frame=0;start=now;}
    if (frame >= 900) return;
    auto *raw = driver->createScreenShot();
    if (!raw) return;
    std::ostringstream file;
    file << directory << "/seat-" << seat << "-" << std::setfill('0')
        << std::setw(5) << frame << ".jpg";
    const auto size = raw->getDimension();
    const unsigned width = size.Width > 960 ? 960 : size.Width;
    auto *scaled = driver->createImage(video::ECF_R8G8B8,
        {width, size.Height * width / size.Width});
    if (!scaled) {raw->drop();return;}
    raw->copyToScaling(scaled);
    const bool saved = driver->writeImageToFile(scaled, file.str().c_str(), 95);
    scaled->drop();
    raw->drop();
    if (!saved) return;
    std::ofstream log(directory + "/seat-" + seat + ".csv", std::ios::app);
    log << frame << "," << std::chrono::duration<double>(now-start).count() << "\n";
    ++frame;
}
