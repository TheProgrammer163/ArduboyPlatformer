



#include <Arduboy2.h>
#include "View.h"
#include "Level.h"
#include "Images.h"


int16_t View::getX() {
    return x;
}
int16_t View::getY() {
    return y;
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
    this->timer++;
    this->timer%=20;
}

// draw the world
void View::draw() {
    int8_t xOffset = (this->x % LEVEL_TILE_WIDTH);
    int8_t yOffset = LEVEL_TILE_WIDTH/2;
    int16_t xStart = this->x/LEVEL_TILE_WIDTH;
    for(uint8_t i = xStart; i <= viewWidthInTiles+xStart; i++) {
        for(uint8_t j = 0; j <= viewHeightInTiles; j++) {
            uint8_t tile = Level::getTileAtTile(i, j);
            if (tile == 4 && this->timer < 10) {
                tile++;
            }
            Sprites::drawSelfMasked((i-xStart)*LEVEL_TILE_WIDTH-xOffset, j*LEVEL_TILE_HEIGHT-yOffset, Images::level, tile);
        }
    }
}






/// End of file
