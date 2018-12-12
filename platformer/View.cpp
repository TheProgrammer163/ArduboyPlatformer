



#include <Arduboy2.h>
#include "View.h"
#include "Level.h"
#include "Images.h"


int16_t View::getX() {
    return x;
}

// Move the view
void View::setPosition(int16_t xpos) {
    int16_t xtarget = xpos - viewWidthInPixels/2;
    
    xtarget = max(xtarget, 0);
    xtarget = min(xtarget, LEVEL_WIDTH_IN_PIXELS-viewWidthInPixels);
    
    this->x = xtarget;
}

void View::update(int16_t newx) {
    this->setPosition(newx);
}

// draw the world
void View::draw() {
    int8_t xOffset = (this->x % LEVEL_TILE_WIDTH);
    int16_t xStart = this->x/LEVEL_TILE_WIDTH;
    for(uint8_t i = xStart; i <= viewWidthInTiles+xStart; i++) {
        for(uint8_t j = 0; j < viewHeightInTiles; j++) {
            //uint8_t tile = Level::getTileAtTile(i, j);
            //Sprites::drawSelfMasked((i-xStart)*LEVEL_TILE_WIDTH-xOffset, j*LEVEL_TILE_HEIGHT, Images::level, tile);
            int16_t drawI = Level::xOnGrid(i * 8);
            int16_t drawJ = Level::yOnGrid(j * 8);
            uint8_t tile = Level::getTileAtTile(drawI, drawJ);
            Sprites::drawSelfMasked((drawI-xStart)*LEVEL_TILE_WIDTH-xOffset, drawJ*LEVEL_TILE_HEIGHT, Images::level, tile);
            if (tile != 0 && tile != 1) {
                Sprites::drawSelfMasked((drawI-xStart)*LEVEL_TILE_WIDTH-xOffset, drawJ*LEVEL_TILE_HEIGHT, Images::level, 2);
            }
        }
    }
}






/// End of file
