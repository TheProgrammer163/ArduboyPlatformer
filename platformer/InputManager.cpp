
#include "InputManager.h"

extern Arduboy2 arduboy;

void InputManager::resetJumpPressed() {
    jumpPressed = 0;
    jumpHeld = 0;
}

void InputManager::update() {

    arduboy.pollButtons();

    // Running
    if (arduboy.pressed(runButton)) {
        runHeld=10;
    } else if (0 < runHeld) {
        runHeld--;
    }

    // Jumping
    jumpHeld = (arduboy.pressed(jumpButton));
    if (arduboy.justPressed(jumpButton)) {
        jumpPressed = 10;
    } else if (0 < jumpPressed) {
        jumpPressed--;
    }

    // Moving left and right
    previousXInput = xInput;
    xInput = 0;
    if (arduboy.pressed(LEFT_BUTTON)) {
        xInput--;
    }
    if (arduboy.pressed(RIGHT_BUTTON)) {
        xInput++;
    }
}

void InputManager::configureRunAndJumpButtons(uint8_t r, uint8_t j) {
    runButton = r;
    jumpButton = j;
}

bool InputManager::isJumpPressed() {
    return (0 < jumpPressed);
}
bool InputManager::isJumpHeld() {
    return (0 < jumpHeld);
}
bool InputManager::isRunHeld() {
    return (0 < runHeld);
}
