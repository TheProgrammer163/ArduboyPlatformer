#pragma once

#include "Arduino.h"
#include "Level.h"

class View {
    private:
        int16_t x = 0;
        int16_t y = 4;
    public:
        const uint8_t viewWidthInPixels = 128;
        const uint8_t viewHeightInPixels = 64;
        const uint8_t viewWidthInTiles = 16;
        const uint8_t viewHeightInTiles = 8;
    public:
        void setPosition(int16_t xpos);
        void draw();
        void update(int16_t newx);
        void clearViewToWhite();
        int16_t getX();
        int16_t getY();
        uint8_t timer = 0;
};






/// End of file
