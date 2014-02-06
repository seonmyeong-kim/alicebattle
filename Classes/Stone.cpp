//
//  Stone.cpp
//  SampleGame
//
//  Created by seonmyeong_kim on 2014/01/29.
//
//

#include "Stone.h"
#include <iostream>

using namespace std;

bool Stone::initWithGame(GameScene *game) {
    theGame = game;
    game->addChild(this, 1);
    this->state = kStateUngrabbed;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    SCALE_DENSITY = visibleSize.width / 640;
    
    return true;
}

void Stone::placeInGride(Point place, int pt, int pl) {
    int sType = (rand() % 4) + 1;
    if(sType == pt || sType == pl) {
        this->placeInGride(place, pt, pl);
        return;
    } else {
        string *color = setStoneColor(sType);
        mySprite = Sprite::create((char *) color);
        mySprite->setScale(SCALE_DENSITY);
        this->addChild(mySprite, 1);

        this->stoneType = sType;
        this->mySprite->setPosition(place);
    }
}

string* Stone::setStoneColor(int sType) {
    this->stoneType = sType;
    string* color;
    switch(this->stoneType) {
        case STONE_TYPE_DIAMOND:
            color = (string*) "icon1.png";
            break;
        case STONE_TYPE_CLOVER:
            color = (string*) "icon2.png";
            break;
        case STONE_TYPE_HEART:
            color = (string*) "icon3.png";
            break;
        case STONE_TYPE_SPACE:
            color = (string*) "icon4.png";
            break;
    }
    return color;
}