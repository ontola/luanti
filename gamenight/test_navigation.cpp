// SPDX-License-Identifier: LGPL-2.1-or-later
#include "gamenight_navigation.h"
#include <cassert>
int main() {
    std::vector<GameNightFocusPoint> menu={{50,20},{50,60},{50,100}};
    assert(gamenightNextFocus(menu,0,0,1)==1);
    assert(gamenightNextFocus(menu,1,0,-1)==0);
    assert(gamenightNextFocus(menu,2,0,1)==2);
    std::vector<GameNightFocusPoint> slots={{0,0},{40,0},{80,0},{0,40},{40,40},{80,40},{150,20}};
    assert(gamenightNextFocus(slots,0,1,0)==1);
    assert(gamenightNextFocus(slots,1,0,1)==4);
    assert(gamenightNextFocus(slots,4,-1,0)==3);
    assert(gamenightNextFocus(slots,2,1,0)==6);
    assert(gamenightNextFocus({},0,1,0)==-1);
    assert(gamenightNextFocus(menu,-1,0,1)==0);
}
