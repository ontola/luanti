// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
struct GameNightViewport { int x, y, width, height; };
inline GameNightViewport gamenightViewport(int players, int seat, int width, int height) {
    if (players < 1 || players > 4 || seat < 0 || seat >= players)
        return {0, 0, width, height};
    const int halfW = width / 2, halfH = height / 2;
    if (players == 1) return {0, 0, width, height};
    if (players == 2 || (players == 3 && seat == 0))
        return {seat * halfW, 0, seat == 0 ? halfW : width-halfW, height};
    const int column = players == 3 ? 1 : seat % 2;
    const int row = players == 3 ? seat-1 : seat / 2;
    return {column*halfW, row*halfH, column ? width-halfW : halfW,
            row ? height-halfH : halfH};
}
