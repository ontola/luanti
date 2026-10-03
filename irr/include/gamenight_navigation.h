// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include <vector>
#include <cmath>
#include <limits>
struct GameNightFocusPoint { float x, y; };
// Move in one cardinal direction. Prefer the same row/column over diagonal jumps.
inline int gamenightNextFocus(const std::vector<GameNightFocusPoint> &points,
        int current, int dx, int dy) {
    if (points.empty()) return -1;
    if (current < 0 || current >= static_cast<int>(points.size())) return 0;
    int best = current;
    float score = std::numeric_limits<float>::max();
    for (int i=0; i<static_cast<int>(points.size()); ++i) {
        float x=points[i].x-points[current].x, y=points[i].y-points[current].y;
        float forward=x*dx+y*dy, sideways=std::abs(x*dy-y*dx);
        if (forward <= 1.f) continue;
        float candidate=forward+sideways*4.f;
        if (candidate<score) {score=candidate;best=i;}
    }
    return best;
}
