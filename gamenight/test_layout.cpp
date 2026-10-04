// SPDX-License-Identifier: LGPL-2.1-or-later
#include "gamenight_layout.h"
#include <cassert>
#include <vector>
int main() {
    for (int width : {1920, 1919}) for (int height : {1080, 1079}) {
        for (int players=1; players<=4; ++players) {
            std::vector<GameNightViewport> views;
            int area=0;
            for (int seat=0; seat<players; ++seat) {
                const auto v=gamenightViewport(players,seat,width,height);
                assert(v.x>=0 && v.y>=0 && v.width>0 && v.height>0);
                assert(v.x+v.width<=width && v.y+v.height<=height);
                for (const auto &other: views)
                    assert(v.x+v.width<=other.x || other.x+other.width<=v.x ||
                           v.y+v.height<=other.y || other.y+other.height<=v.y);
                area+=v.width*v.height;
                views.push_back(v);
            }
            assert(area==width*height);
        }
    }
    const auto solo=gamenightViewport(1,0,1920,1080);
    assert(solo.width==1920 && solo.height==1080);
    const auto third=gamenightViewport(3,2,1920,1080);
    assert(third.x==960 && third.y==540 && third.width==960 && third.height==540);
}
