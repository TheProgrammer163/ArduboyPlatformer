#pragma once


#include <Arduino.h>
#include <Arduboy2.h>
#include <FixedPoints.h>
#include "Player.h"


namespace global {
    int16_t clamp(int16_t val, int16_t lower, int16_t upper) {
        if (val <= lower) return lower;
        if (upper <= val) return upper;
        return val;
    }
    SQ11x4 clamp(SQ11x4 val, SQ11x4 lower, SQ11x4 upper) {
        if (val <= lower) return lower;
        if (upper <= val) return upper;
        return val;
    }
    SQ11x4 lerp(SQ11x4 val, SQ11x4 target, SQ11x4 percent) {
        return ((target - val) * percent);
    }
}
