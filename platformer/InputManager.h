#pragma once

#include <Arduino.h>
#include <Arduboy2.h>

extern Arduboy2 arduboy;

class InputManager {
    private:
        uint8_t runButton = A_BUTTON;
        uint8_t jumpButton = B_BUTTON;
    private:
        int8_t runHeld = 0;
        int8_t jumpPressed = 0;
        int8_t jumpHeld = 0;
    public:
        int8_t previousXInput = 0;
        int8_t xInput = 0;
    public:
        void update();
        void configureRunAndJumpButtons(uint8_t r, uint8_t j);
        bool isJumpPressed();
        bool isJumpHeld();
        bool isRunHeld();
        void resetJumpPressed();
};






/// End of file
