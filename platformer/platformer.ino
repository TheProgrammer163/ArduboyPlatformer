// Sam Sibbens
// 2018 DECEMBER 10
// Arduboy Platformer

#include <Arduino.h>
#include <Arduboy2.h>
#include <FixedPoints.h>
#include "InputManager.h"
#include "View.h"
#include "Player.h"
#include "Images.h"
Arduboy2 arduboy;
InputManager input;
View view;

Player player;

constexpr uint8_t ONE_SECOND = 60;
void setup() {
    arduboy.begin();
    arduboy.clear();
    arduboy.setFrameRate(ONE_SECOND);
}

void loop() {
    //Prevent the Arduboy from running too fast
    if(!arduboy.nextFrame()) {return;}
    input.update();

    arduboy.clear();

    player.update();
    if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(DOWN_BUTTON) && !(arduboy.pressed(RIGHT_BUTTON) || arduboy.pressed(LEFT_BUTTON))) {
        view.update(3000);
    } else {
        view.update(player.getX());
    }
    view.draw();
    player.draw();
    
    arduboy.display();
}






/// End of file
