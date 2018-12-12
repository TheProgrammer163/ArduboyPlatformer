
#include <Arduino.h>
#include <Arduboy2.h>
#include "Level.h"


int16_t Level::xOnGrid(int16_t xpos) {
    return (xpos / LEVEL_TILE_WIDTH);
}
int16_t Level::yOnGrid(int16_t ypos) {
    return (ypos / LEVEL_TILE_HEIGHT);
}

bool Level::isIndexOutOfBoundsAtTileX(int16_t xIndex) {
    return ((xIndex < 0) || (LEVEL_WIDTH_IN_TILES <= xIndex));
}
bool Level::isIndexOutOfBoundsAtTileY(int16_t yIndex) {
    return ((yIndex < 0) || (LEVEL_HEIGHT_IN_TILES <= yIndex));
}
bool Level::isTileOutOfBoundsAtTile(int16_t xIndex, int16_t yIndex) {
    return (isIndexOutOfBoundsAtTileX(xIndex) || isIndexOutOfBoundsAtTileY(yIndex));
}

uint8_t Level::getTileAtTile(int16_t xIndex, int16_t yIndex) {
    /*
    if (Level::isTileOutOfBoundsAtTile(xIndex,yIndex)) {
        return 0;
    }*/
    if (yIndex >= 7) return 1;
    return pgm_read_byte(&(levelData[LEVEL_WIDTH_IN_TILES * yIndex + xIndex]));
}
uint8_t Level::getTileAtPixel(int16_t xpos, int16_t ypos) {
    int16_t xIndex = Level::xOnGrid(xpos);
    int16_t yIndex = Level::yOnGrid(ypos);
    return Level::getTileAtTile(xIndex, yIndex);
}

bool Level::isTileSolidAtTile(int16_t xIndex, int16_t yIndex) {
    return (getTileAtTile(xIndex, yIndex) != 0);
}

bool Level::detectWall(int16_t x, int16_t y) {
    int16_t headProtection = 4; // 3 is also a good number
    int16_t x1 = xOnGrid(x);
    int16_t x2 = xOnGrid(x+LEVEL_TILE_WIDTH-1);
    int16_t y1 = yOnGrid(y+headProtection);
    int16_t y2 = yOnGrid(y+LEVEL_TILE_HEIGHT-1);
    
    return (getTileAtTile(x1, y1) != 0 || getTileAtTile(x2, y1) != 0 || getTileAtTile(x2, y2) != 0 || getTileAtTile(x1, y2) != 0);
}






/// End of file
