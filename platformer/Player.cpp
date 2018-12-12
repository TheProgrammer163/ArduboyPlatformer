
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
        xvel = 0;
        while(!this->detectWall(x+sign, y)) {
            x+=sign;
        }
    } else {
        x+=xvel;
    }
    if (this->detectWall(x, y+yvel)) {
        int8_t sign = velSign(yvel);
        yvel = 0;
        while(!this->detectWall(x, y+sign)) {
            y+=sign;
        }
    } else {
        y+=yvel;
    }
}

void Player::changeState(PlayerState newState) {
    if (state != newState) {
        previousState = state;
        state = newState;
    }
}



void Player::executeWalkingState() {
    if (!isOnGround()) {
        changeState(PlayerState::Falling);
    }
    if (input.isJumpPressed() && canJump()) {
        jump(jumpStrengthWalking);
    }

    if (input.xInput == 0) {
        xvel -= deceleration * velSign(xvel);
    } else {
        if (input.isRunHeld()) {
            xvel += input.xInput * accel;
            xvel = velClamp(xvel, -velRunMax, velRunMax);
            if (isFastEnoughToRun()) {
                changeState(PlayerState::Running);
            }
        } else {
            xvel += input.xInput * accel;
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
        if (input.xInput == -velSign(xvel) && velSign(xvel) != 0) {
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
    } else if (input.xInput == -velSign(xvel)) {
        if (deceleration*2 <= absFixed(xvel)) {
            xvel -= velSign(xvel) * deceleration*2;
        } else {
            xvel = 0;
        }
    } else {
        if (isFastEnoughToRun()) {
            changeState(PlayerState::Running);
        } else {
            changeState(PlayerState::Walking);
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
    if (0 < coyoteTimeLeft) {
        coyoteTimeLeft--;
    }
    if (isOnGround()) {
        coyoteTimeLeft = coyoteTime;
    } else {
        changeState(PlayerState::Falling);
    }
    if (input.xInput != 0) {
        facing = input.xInput;
    }
    switch(state) {
        case PlayerState::Walking:
            executeWalkingState();
            break;
        case PlayerState::Running:;
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


extern View view;

int16_t Player::getDrawX() {
    return this->getBBoxLeft()-view.getX();
}
int16_t Player::getDrawY() {
    return this->getBBoxTop();
}

void Player::draw() {
    int16_t drawX = getDrawX();
    int16_t drawY = getDrawY();
    
    switch(state) {
        case PlayerState::Walking:
            Sprites::drawExternalMask(drawX, drawY, Images::playerRun, Masks::playerRun, 1, 1);
            break;
        case PlayerState::Running:
            Sprites::drawExternalMask(drawX, drawY, Images::playerRun, Masks::playerRun, 0, 0);
            break;
        case PlayerState::Falling:
            Sprites::drawExternalMask(drawX, drawY, Images::playerAir, Masks::playerAir, 0, 0);
            break;
        case PlayerState::Breaking:
            Sprites::drawExternalMask(drawX, drawY, Images::playerBreaking, Masks::playerBreaking, 0, 0);
            break;
        case PlayerState::Dead:
            //Sprites::drawExternalMask(drawX, drawY, Images::playerAir, Masks::playerAir, 0, 0);
            break;
    }
    arduboy.println(x.getInteger()-4);
    arduboy.print(y.getInteger()-4);
}

// Draw F when the player isn't on solid ground
void Player::debugDrawIsOnGround() {
    int16_t drawX = getDrawX();
    int16_t drawY = getDrawY();
    if (!isOnGround()) {
        Sprites::drawOverwrite(drawX, drawY, Images::playerDebugging, static_cast<uint8_t>(PlayerState::Falling));
    }
}
// Draw a letter corresponding to the current state
void Player::debugDrawState() {
    int16_t drawX = getDrawX();
    int16_t drawY = getDrawY();
    Sprites::drawOverwrite(drawX, drawY, Images::playerDebugging, static_cast<uint8_t>(state));
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






/// End of file
