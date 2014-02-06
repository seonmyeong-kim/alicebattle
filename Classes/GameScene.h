#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "AppValues.h"
#include "Stone.h"
#include "StartLayer.h"

class Stone;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    void swapActionFinished(cocos2d::Point grapIdx, cocos2d::Point swapIdx, int dir, bool jackpotcheck);
    void stoneAniActionFinished(Stone* stone);
    void stoneAniActionAllFinished();
    cocos2d::Array* getJackpotList(int sType, cocos2d::Point Idx);
    void checkJackpot(cocos2d::Point grapIdx, cocos2d::Point swapIdx, int dir);
    void jackpotAniActionAllFinished();

    virtual void placeStones();
    //virtual void checkGroup(bool firstTime);
    //virtual void moveStoneDown(float dt);
    //virtual void swapStones(Stone *stone, int dir);
    virtual void swapStone(int dir, bool jackpotcheck);

    Stone *grid[8][7];
    bool allowTouch;
    int score;
    Stone *grapStone;
    float grappointX, grappointY;
    int grapidxX, grapidxY;

    // Animation
    cocos2d::Animation *stoneDiamondAni;
    cocos2d::Animation *stoneCloverAni;
    cocos2d::Animation *stoneHeartAni;
    cocos2d::Animation *stoneSpaceAni;

    void initAnimation();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    SCENE_FUNC(GameScene);

    void touchesBegan(const std::vector<cocos2d::Touch*>& pTouches, cocos2d::Event* pEvent);
    void touchesMoved(const std::vector<cocos2d::Touch*>& pTouches, cocos2d::Event* pEvent);
    void touchesEnded(const std::vector<cocos2d::Touch*>& pTouches, cocos2d::Event* pEvent);
};

#endif // __HELLOWORLD_SCENE_H__
