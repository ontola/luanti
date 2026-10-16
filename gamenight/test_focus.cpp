// SPDX-License-Identifier: LGPL-2.1-or-later
#include "gamenight_focus.h"
#include <cassert>
#include <iostream>
int main() {
    GameNightFocusHandoff h;
    assert(!h.retry(100, 0, false));
    h.begin(100, 20);
    assert(!h.retry(199, 20, false));
    assert(h.retry(200, 20, false)); // delayed grant
    assert(!h.retry(201, 20, true)); // either view acquired focus
    assert(!h.retry(400, 20, false)); // do not fight later Alt-Tab
    h.begin(500, 20);
    assert(!h.retry(600, 21, false)); // user input during handoff
    assert(!h.retry(700, 21, false));
    h.begin(800, 21);
    h.cancel(); // pause/back before focus succeeds
    assert(!h.retry(900, 21, false));
    h.begin(1000, 21);
    assert(h.retry(1900, 21, false));
    assert(!h.retry(2000, 21, false)); // finite retry window
    h.begin(UINT32_MAX - 50, 21);
    assert(h.retry(50, 21, false)); // SDL ticks wrap
    std::cout << "Focus handoff deadline, success, pause and user override passed\n";
}
