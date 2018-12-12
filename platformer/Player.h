#pragma once

#include <Arduino.h>
#include <Arduboy2.h>
#include <FixedPoints.h>

enum class PlayerState : uint8_t {
    Walking,
    Running,
    Falling,
    Breaking,
    Dead,
};

using SQ11x4 = SFixed<11, 4>;

class Player {
    private:
        SQ11x4 x = 32;
        SQ11x4 y = 32;
        SQ11x4 xvel = 0;
        SQ11x4 yvel = 0;
        SQ11x4 testForBug = 0;
        const SQ11x4 accel = 1.0/16.0;
        const SQ11x4 deceleration = accel;
        const SQ11x4 grav = 1.0/16.0;
        const SQ11x4 velWalkMax = 1.0;
        const SQ11x4 velRunMax = 1.0 + (8.0/16.0);
        const SQ11x4 velFallMax = 4.0;
        const SQ11x4 jumpStrengthWalking = 1.0 + (2.0/16.0);
        const SQ11x4 jumpStrengthRunning = 1.0 + (8.0/16.0);
        const uint8_t coyoteTime = 7;
        uint8_t coyoteTimeLeft = 0;
        int8_t facing = 1;
        uint16_t coins = 0;
        PlayerState state = PlayerState::Falling;
        PlayerState previousState = PlayerState::Walking;
    public:
        void init(int16_t xpos, int16_t ypos);
        void update();
        void draw();
        int16_t getX();
        int16_t getY();
        int16_t getBBoxTop();
        int16_t getBBoxRight();
        int16_t getBBoxBottom();
        int16_t getBBoxLeft();
    private:
        SQ11x4 velClamp(SQ11x4, SQ11x4, SQ11x4);
        int8_t velSign(SQ11x4);
        void move();
        void jump(SQ11x4 jumpStrenght);
        bool canJump();
        void changeState(PlayerState s);
        void collectCoins();
        bool isFastEnoughToRun();
        bool isSlowEnoughToWalk();
        bool isOnGround();
        bool detectWall(SQ11x4 xpos, SQ11x4 ypos);
    private:
        void executeWalkingState();
        void executeRunningState();
        void executeFallingState();
        void executeBreakingState();
        void executeDeadState();
};
