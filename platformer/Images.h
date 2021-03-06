#pragma once

#include <Arduino.h>
#include <Arduboy2.h>


namespace Images {
    
    /**** LEVELS ****/
    const uint8_t level[] PROGMEM  = {
        // Dimensions
        8, 8,
        // Tileset
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xfe, 0x56, 0x2a, 0x56, 0x2a, 0x56, 0x2, 0x6, 0xe2, 0x66, 0xa2, 0x60, 0x2e, 0x66, 0x2a, 0x7e, 0xbd, 0xdb, 0xe7, 0xe7, 0xdb, 0xbd, 0x7e, 0xff, 0x00, 0xdd, 0xbb, 0x77, 0xee, 0x00, 0xff, 0xff, 0x00, 0x77, 0xee, 0xdd, 0xbb, 0x00, 0xff, 
    };
    /*** PLAYER ***/
    const uint8_t playerRun[] PROGMEM  = {
        // Dimensions
        8, 8,
        0x00, 0x7c, 0x3e, 0x26, 0x3e, 0x26, 0x7c, 0x00,
        0x00, 0x3c, 0x7e, 0x26, 0x3e, 0x66, 0x3c, 0x00, 
    };
    const uint8_t playerAir[] PROGMEM  = {
        // Dimensions
        8, 8,
        0x00, 0x7c, 0x3e, 0x26, 0x3e, 0x26, 0x3c, 0x00, 
    };
    const uint8_t playerBreaking[] PROGMEM = {
        8, 8,
        0x00, 0x7c, 0x3e, 0x26, 0x7e, 0x26, 0x3c, 0x00, 
    };
    const uint8_t playerDebugging[] PROGMEM = {
        8, 8,
        0x00, 0x7e, 0x60, 0x18, 0x18, 0x60, 0x7e, 0x00, 0x00, 0x7e, 0x12, 0x12, 0x12, 0x12, 0x6c, 0x00, 0x00, 0x7e, 0x12, 0x12, 0x12, 0x2, 0x2, 0x00, 0x00, 0x7e, 0x52, 0x52, 0x52, 0x52, 0x6c, 0x00, 0x00, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 
    };
}
namespace Masks {
    /*** PLAYER ***/
    const uint8_t playerRun[] PROGMEM  = {
        // Dimensions
        0xfe, 0xff, 0xff, 0x7f, 0x7f, 0xff, 0xff, 0xfe,
        0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e, 
    };
    const uint8_t playerAir[] PROGMEM  = {
        // Dimensions
        0xfe, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7e, 
    };
    const uint8_t playerBreaking[] PROGMEM = {
        0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7e, 
    };
}






/// End of file
