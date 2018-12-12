
#include <Arduino.h>
#include <Arduboy2.h>
#include <FixedPoints.h>
#include "Player.h"
#include "InputManager.h"
#include "Images.h"
#include "View.h"

extern InputManager input;

int16_t Player::getX() {
    return this->x.getInteger();
}
int16_t Player::getY() {
    return this->y.getInteger();
}


int16_t Player::getBBoxTop() {
    return (this->getY() - 4);
}
int16_t Player::getBBoxBottom() {
    return (this->getBBoxTop() + 7);
}
int16_t Player::getBBoxLeft() {
    return (this->getX() - 4);
}
int16_t Player::getBBoxRight() {
    return (this->getBBoxLeft() + 7);
}

bool Player::detectWall(SQ11x4 xpos, SQ11x4 ypos) {
    return (Level::detectWall(xpos.getInteger()-4, ypos.getInteger()-4));
}

void Player::jump(SQ11x4 jumpStrength) {
    changeState(PlayerState::Falling);
    yvel = -jumpStrength;
    coyoteTimeLeft = 0;
    input.resetJumpPressed();
}

void Player::init(int16_t xpos, int16_t ypos) {
    x = static_cast<SQ11x4>(xpos);
    y = static_cast<SQ11x4>(xpos);
}
bool Player::isOnGround() {
    return (detectWall(this->x, this->y+1));
}
int8_t Player::velSign(SQ11x4 vel) {
    if (vel < 0)
        return -1;
    if (0 < vel)
        return 1;
    return 0;
}
SQ11x4 Player::velClamp(SQ11x4 value, SQ11x4 minimum, SQ11x4 maximum) {
    if (value < minimum)
        return minimum;
    if (maximum < value)
        return maximum;
    return value;
}
void Player::move() {
    if (this->detectWall(x+xvel, y)) {
        int8_t sign = velSign(xvel);
        int8_t dist = ceilFixed(absFixed(xvel)).getInteger();
        int8_t count = 0;
        xvel = 0;
        while(count < dist && !this->detectWall(x+sign, y)) {
            x+=sign;
            count++;
        }
    } else {
        x+=xvel;
    }
    if (this->detectWall(x, y+yvel)) {
        int8_t sign = velSign(yvel);
        int8_t dist = ceilFixed(absFixed(yvel)).getInteger();
        int8_t count = 0;
        yvel = 0;
        while(count < dist && !this->detectWall(x, y+sign)) {
            y+=sign;
            count++;
        }
    } else {
        y+=yvel;
    }
}

void Player::changeState(PlayerState newState) {
    if (previousState != newState) {
        previousState = state;
        state = newState;
    }
}



void Player::executeWalkingState() {
    if (!isOnGround()) {
        changeState(PlayerState::Falling);
    }
    if (canJump() && input.isJumpPressed()) {
        jump(jumpStrengthWalking);
    }
    if (input.xInput == 0) {
        xvel -= deceleration * velSign(xvel);
    } else {
        if (input.isRunHeld()) {
            xvel += input.xInput*accel;
            xvel = velClamp(xvel, -velRunMax, velRunMax);
            if (isFastEnoughToRun()) {
                changeState(PlayerState::Running);
            }
        } else {
            xvel += input.xInput*accel;
            xvel = velClamp(xvel, -velWalkMax, velWalkMax);
        }
    }
    move();
}

void Player::executeRunningState() {
    if (isSlowEnoughToWalk() || !input.isRunHeld()) {
        changeState(PlayerState::Walking);
    }
    if (!isOnGround()) {
        changeState(PlayerState::Falling);
    }
    if (input.isJumpPressed() && canJump()) {
        jump(jumpStrengthRunning);
    }

    if (input.xInput == 0) {
        xvel -= deceleration * velSign(xvel);
    } else {
        xvel += input.xInput * accel;
        xvel = velClamp(xvel, -velRunMax, velRunMax);
        if (input.xInput == -velSign(xvel)) {
            changeState(PlayerState::Breaking);
        }
    }
    move();
}

void Player::executeFallingState() {
    bool wasRunning = (isFastEnoughToRun() && (previousState == PlayerState::Running));

    if (input.isJumpHeld() && yvel <= 0) {
        yvel += grav;
    } else {
        yvel += grav*2;
    }

    if (input.xInput == 0) {
        xvel -= deceleration * velSign(xvel);
    } else {
        if (wasRunning) {
            xvel += input.xInput * accel;
            xvel = velClamp(xvel, - velRunMax, velRunMax);
        } else {
            xvel += input.xInput * accel;
            xvel = velClamp(xvel, -velWalkMax, velWalkMax);
        }
    }
    if (isOnGround()) {
        if (input.xInput == -velSign(xvel)) {
            changeState(PlayerState::Breaking);
        } else {
            if (wasRunning) {
                changeState(PlayerState::Running);
            } else {
                changeState(PlayerState::Walking);
            }
        }
    }
    if (canJump() && input.isJumpPressed()) {
        if (wasRunning) {
            jump(jumpStrengthRunning);
        } else {
            jump(jumpStrengthWalking);
        }
    }
    move();
}

void Player::executeBreakingState() {
    if (!isOnGround()) {
        changeState(PlayerState::Falling);
    }
    if (input.isJumpPressed() && canJump()) {
        jump(jumpStrengthWalking);
    }

    if (input.xInput == 0 || xvel == 0) {
        xvel -= deceleration * velSign(xvel);
        changeState(PlayerState::Walking);
    } else {
        if (input.xInput == -velSign(xvel)) {
            xvel -= velSign(xvel) * deceleration * 2;
            xvel = velClamp(xvel, -velRunMax, velRunMax);
        } else {
            if (isFastEnoughToRun()) {
                changeState(PlayerState::Running);
            } else {
                changeState(PlayerState::Walking);
            }
        }
    }
    move();
}

void Player::executeDeadState() {
    changeState(PlayerState::Walking);
    x = 32;
    y = 32;
}

void Player::update() {

    if (isOnGround()) {
        coyoteTimeLeft = coyoteTime;
    } else if (0 < coyoteTimeLeft) {
        coyoteTimeLeft--;
    }
    if (input.xInput != 0) {
        facing = input.xInput;
    }
    switch(state) {
        case PlayerState::Walking:
            executeWalkingState();
            break;
        case PlayerState::Running:
            executeRunningState();
            break;
        case PlayerState::Falling:
            executeFallingState();
            break;
        case PlayerState::Breaking:
            executeBreakingState();
            break;
        case PlayerState::Dead:
            executeDeadState();
            break;
    }
}
void Player::draw() {
    extern View view;
    int16_t drawX = this->getBBoxLeft()-view.getX();
    int16_t drawY = this->getBBoxTop();
    switch(state) {
        case PlayerState::Walking:
            Sprites::drawExternalMask(drawX, drawY, Images::playerRun, Masks::playerRun, 1, 1);
            break;
        case PlayerState::Running:
            Sprites::drawExternalMask(drawX, drawY, Images::playerAir, Masks::playerAir, 0, 0);
            break;
        case PlayerState::Falling:
            Sprites::drawExternalMask(drawX, drawY, Images::playerAir, Masks::playerAir, 0, 0);
            break;
        case PlayerState::Breaking:
            Sprites::drawExternalMask(drawX, drawY, Images::playerBreaking, Masks::playerAir, 0, 0);
            break;
        case PlayerState::Dead:
            //Sprites::drawExternalMask(drawX, drawY, Images::playerAir, Masks::playerAir, 0, 0);
            break;
    }
}



bool Player::canJump() {
    return (0 < coyoteTimeLeft);
}

bool Player::isFastEnoughToRun() {
    return (velWalkMax < absFixed(xvel));
}
bool Player::isSlowEnoughToWalk() {
    return (!isFastEnoughToRun());
}
