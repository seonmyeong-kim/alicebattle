//
//  Stone.h
//  SampleGame
//
//  Created by seonmyeong_kim on 2014/01/29.
//
//

#ifndef SampleGame_Stone_h
#define SampleGame_Stone_h

#include "cocos2d.h"
#include "GameScene.h"

USING_NS_CC;

#define STONE_TYPE_DIAMOND 1
#define STONE_TYPE_CLOVER 2
#define STONE_TYPE_HEART 3
#define STONE_TYPE_SPACE 4

using namespace std;

class GameScene;

typedef enum tagState {
    kStateGrabbed,
    kStateUngrabbed
} touchState;

class Stone : public Node
{
public:
    virtual bool initWithGame(GameScene* game);
    virtual void placeInGride(cocos2d::Point place, int pt, int pl);
    virtual string* setStoneColor(int sType);

    Sprite *mySprite;
    GameScene *theGame;
    int stoneType;
    int curVGroup;
    int curHGroup;
    touchState state;
    bool disappearing;
    cocos2d::Point initdir;
};


#endif
