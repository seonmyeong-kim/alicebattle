#include "GameScene.h"

USING_NS_CC;

#define GRID_WIDTH 8
#define GRID_HEIGHT 7
#define MAX_TIME 230

#define DIRECTION_UP 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3
#define DIRECTION_RIGHT 4
#define DIRECTION_SELF 5

#define STONE_ANIMATION_FRAME 5
#define STONE_ANIMATION_TIME 0.1

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));


    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 5);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    //auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    //label->setPosition(Point(origin.x + visibleSize.width/2,
    //                        origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    //this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    //sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);
    SCALE_DENSITY = visibleSize.width / 640;
    float scale_density_height = visibleSize.height / 960;

    auto stoneSprite = Sprite::create("icon1.png");
    GRID_BORDER = (stoneSprite->getContentSize().width + 15) * SCALE_DENSITY;

    log("visibleSize width = %f height = %f GRID_BORDER = %d", visibleSize.width, visibleSize.height, GRID_BORDER);

    auto sprite = Sprite::create("background.png");
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->setScale(SCALE_DENSITY);

    this->addChild(sprite, 0);

    auto gridBackground = Sprite::create("grid.png");
    gridBackground->setPosition(gridBackground->getContentSize().width/2, (gridBackground->getContentSize().height/2 + GRID_BORDER*0.3));
    sprite->addChild(gridBackground);

    //
    GRID_OFFSET.x = visibleSize.width/2 - (((gridBackground->getContentSize().width*SCALE_DENSITY)/2) - (GRID_BORDER*0.5));
    GRID_OFFSET.y = GRID_BORDER*scale_density_height;

    for(int i=0;i<GRID_WIDTH; i++){
        for(int j=0;j<GRID_HEIGHT;j++){
            Stone *s = new Stone();
            s->initWithGame(this);
            grid[i][j] = s;
            s->release();
        }
    }
    this->placeStones();

    this->allowTouch = true;

    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(GameScene::touchesBegan, this);
    touchListener->onTouchesMoved = CC_CALLBACK_2(GameScene::touchesMoved, this);
    touchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::touchesEnded, this);

    EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void GameScene::initAnimation() {
    stoneDiamondAni = Animation::create();
    stoneDiamondAni->setDelayPerUnit(STONE_ANIMATION_TIME);

    stoneCloverAni = Animation::create();
    stoneCloverAni->setDelayPerUnit(STONE_ANIMATION_TIME);

    stoneHeartAni = Animation::create();
    stoneHeartAni->setDelayPerUnit(STONE_ANIMATION_TIME);

    stoneSpaceAni = Animation::create();
    stoneSpaceAni->setDelayPerUnit(STONE_ANIMATION_TIME);

    for(int i=0;i<STONE_ANIMATION_FRAME;i++) {
        char filename[128];
        sprintf(filename, "icon1_ani%d.png", i+1);
        stoneDiamondAni->addSpriteFrameWithFile(filename);

        sprintf(filename, "icon2_ani%d.png", i+1);
        stoneCloverAni->addSpriteFrameWithFile(filename);

        sprintf(filename, "icon3_ani%d.png", i+1);
        stoneHeartAni->addSpriteFrameWithFile(filename);

        sprintf(filename, "icon4_ani%d.png", i+1);
        stoneSpaceAni->addSpriteFrameWithFile(filename);
    }
}

void GameScene::touchesBegan(const std::vector<Touch*>& pTouches, Event* pEvent) {
    if(!allowTouch) return;

    Touch* pTouch = (Touch*)pTouches.back();
    Point p = pTouch->getLocation();

    grappointX = p.x;
    grappointY = p.y;

    grapidxX = (p.x - (GRID_OFFSET.x - GRID_BORDER/2)) / GRID_BORDER;
    grapidxY = (p.y - (GRID_OFFSET.y - GRID_BORDER/2)) / GRID_BORDER;
    log("idxX = %d idxY = %d", grapidxX, grapidxY);
    if(grapidxX >= GRID_WIDTH || grapidxX < 0 || grapidxY < 0 || grapidxY >= GRID_HEIGHT
       || grapStone != NULL) return;

    grapStone = grid[grapidxX][grapidxY];

    grapStone->mySprite->stopAllActions();
    grapStone->mySprite->setScale(SCALE_DENSITY*2);
    grapStone->setZOrder(2);


}

void GameScene::touchesMoved(const std::vector<Touch*>& pTouches, Event* pEvent) {
    if(!allowTouch || grapStone == NULL) return;
    Touch* pTouch = (Touch*)pTouches.back();
    Point p = pTouch->getLocation();

    grapStone->mySprite->setPosition(Point(p.x, p.y));

    if(p.x > (grappointX + (GRID_BORDER*0.5))){
        swapStone(DIRECTION_RIGHT, true);
    } else if(p.x < (grappointX - (GRID_BORDER*0.5))) {
        swapStone(DIRECTION_LEFT, true);
    } else if(p.y < (grappointY - (GRID_BORDER*0.5))){
        swapStone(DIRECTION_DOWN, true);
    } else if(p.y > (grappointY + (GRID_BORDER*0.5))) {
        swapStone(DIRECTION_UP, true);
    }
}

void GameScene::touchesEnded(const std::vector<Touch*>& pTouches, Event* pEvent) {
    if(!allowTouch || grapStone == NULL) return;
    swapStone(DIRECTION_SELF, false);
}

void GameScene::swapStone(int dir, bool jackpotcheck)
{
    allowTouch = false;
    int swapstoneX, swapstoneY;

    swapstoneX = grapidxX;
    swapstoneY = grapidxY;
    switch(dir){
        case DIRECTION_UP:
            if(swapstoneY+1 >= GRID_HEIGHT) {
                allowTouch = true;
                return;
            }
            swapstoneY++;
            break;
        case DIRECTION_DOWN:
            if(swapstoneY-1 < 0) {
                allowTouch = true;
                return;
            }
            swapstoneY--;
            break;
        case DIRECTION_LEFT:
            if(swapstoneX-1 < 0) {
                allowTouch = true;
                return;
            }            swapstoneX--;
            break;
        case DIRECTION_RIGHT:
            if(swapstoneX+1 >= GRID_WIDTH) {
                allowTouch = true;
                return;
            }
            swapstoneX++;
            break;
    }

    Stone *swapstone = grid[swapstoneX][swapstoneY];

    grapStone->mySprite->setScale(SCALE_DENSITY);
    grapStone->setZOrder(1);

    int putX, putY;
    putX = GRID_OFFSET.x + (GRID_BORDER * swapstoneX);
    putY = GRID_OFFSET.y + (GRID_BORDER * swapstoneY);

    int swappointX, swappointY;
    swappointX = GRID_OFFSET.x + (GRID_BORDER * grapidxX);
    swappointY = GRID_OFFSET.y + (GRID_BORDER * grapidxY);

    ActionInterval* action = MoveTo::create(0.1f, Point(putX, putY));

    grapStone->mySprite->runAction(EaseExponentialInOut::create(action));

    if(dir != DIRECTION_SELF)
    {
        ActionInterval* actionswap = MoveTo::create(0.1f, Point(swappointX, swappointY));

        CallFuncN* callFunctionSwap = CallFuncN::create(CC_CALLBACK_0(GameScene::swapActionFinished, this, Point(grapidxX, grapidxY), Point(swapstoneX, swapstoneY), dir, jackpotcheck));

        Sequence* callActionSwap = Sequence::create(EaseExponentialInOut::create(actionswap), callFunctionSwap, NULL);

        swapstone->mySprite->stopAllActions();
        swapstone->mySprite->runAction(callActionSwap);

        Stone *temp = grid[swapstoneX][swapstoneY];
        grid[swapstoneX][swapstoneY] = grid[grapidxX][grapidxY];
        grid[grapidxX][grapidxY] = temp;
    } else {
        allowTouch = true;
        grapStone = NULL;
    }
}

void GameScene::swapActionFinished(Point grapIdx, Point swapIdx, int dir, bool jackpotcheck)
{
    log("swapActionFinished");
    if(jackpotcheck) {
        checkJackpot(grapIdx, swapIdx, dir);
    } else {
        allowTouch = true;
        grapStone = NULL;
    }
}

Array* GameScene::getJackpotList(int sType, Point idx){
    int idxX, idxY;
    idxX = idx.x;
    idxY = idx.y;

    auto jackpotarr = Array::create();
    auto jackpotXarr = Array::create();
    auto jackpotYarr = Array::create();

    int smallX = idxX - 1;
    while(smallX>=0){
        if(sType == grid[smallX][idxY]->stoneType) {
            jackpotXarr->addObject(grid[smallX][idxY]);
            smallX--;
        } else {
            break;
        }
    }

    int largeX = idxX + 1;
    while(largeX<GRID_WIDTH){
        if(sType == grid[largeX][idxY]->stoneType) {
            jackpotXarr->addObject(grid[largeX][idxY]);
            largeX++;
        } else {
            break;
        }
    }

    int smallY = idxY - 1;
    while(smallY>=0){
        if(sType == grid[idxX][smallY]->stoneType){
            jackpotYarr->addObject(grid[idxX][smallY]);
            smallY--;
        } else {
            break;
        }
    }

    int largeY = idxY + 1;
    while(largeY<GRID_HEIGHT){
        if(sType == grid[idxX][largeY]->stoneType){
            jackpotYarr->addObject(grid[idxX][largeY]);
            largeY++;
        } else {
            break;
        }
    }

    if(jackpotXarr->count() > 1) {
        for(int i=0;i<jackpotXarr->count();i++){
            jackpotarr->addObject(jackpotXarr->getObjectAtIndex(i));
        }
    }
    if(jackpotYarr->count() > 1) {
        for(int i=0;i<jackpotYarr->count();i++){
            jackpotarr->addObject(jackpotYarr->getObjectAtIndex(i));
        }
    }
    return jackpotarr;
}

void GameScene::checkJackpot(Point grapIdx, Point swapIdx, int dir){
    int grapX, grapY;
    grapX = grapIdx.x;
    grapY = grapIdx.y;

    int swapX, swapY;
    swapX = swapIdx.x;
    swapY = swapIdx.y;

    int sType = grid[grapX][grapY]->stoneType;
    int swapSType = grid[swapX][swapY]->stoneType;

    auto jackpotarr = this->getJackpotList(sType, grapIdx);
    auto jackpotswaparr = this->getJackpotList(swapSType, swapIdx);

    bool isGrapjackpot = false;
    bool isSwapjackpot = false;
    if(jackpotarr->count() > 0){
        isGrapjackpot = true;
    }
    if(jackpotswaparr->count() > 0){
        isSwapjackpot = true;
    }

    for(int i=0;i<jackpotswaparr->count();i++) {
        if(jackpotarr->getIndexOfObject(jackpotswaparr->getObjectAtIndex(i)) >= 0){
            jackpotswaparr->removeObject(jackpotswaparr->getObjectAtIndex(i));
        }
    }

    initAnimation();

    if(isSwapjackpot){
        Animate* animate;
        jackpotswaparr->addObject(grid[swapX][swapY]);
        for(int i=0;i<jackpotswaparr->count();i++){
            Stone* jpstone = (Stone*)jackpotswaparr->getObjectAtIndex(i);
            switch(swapSType){
                case STONE_TYPE_DIAMOND:
                    animate = Animate::create(stoneDiamondAni);
                    break;
                case STONE_TYPE_CLOVER:
                    animate = Animate::create(stoneCloverAni);
                    break;
                case STONE_TYPE_HEART:
                    animate = Animate::create(stoneHeartAni);
                    break;
                case STONE_TYPE_SPACE:
                    animate = Animate::create(stoneSpaceAni);
                    break;
            }

            CallFuncN* callFunctionSwap = CallFuncN::create(CC_CALLBACK_0(GameScene::stoneAniActionFinished, this, jpstone));

            Sequence* callActionSwap;
            if(jpstone == grid[swapX][swapY] && !isGrapjackpot) {
                CallFuncN* callFunctionSwapFinish = CallFuncN::create(CC_CALLBACK_0(GameScene::stoneAniActionAllFinished, this));
                DelayTime* delay = DelayTime::create(0.1f);
                callActionSwap = Sequence::create(animate, callFunctionSwap, delay, callFunctionSwapFinish, NULL);

            } else {
                callActionSwap = Sequence::create(animate, callFunctionSwap, NULL);
            }

            jpstone->mySprite->stopAllActions();
            jpstone->mySprite->runAction(callActionSwap);
        }
    }

    if(isGrapjackpot){
        Animate* animate;
        jackpotarr->addObject(grid[grapX][grapY]);
        for(int i=0;i<jackpotarr->count();i++){
            Stone* jpstone = (Stone*)jackpotarr->getObjectAtIndex(i);
            switch(sType){
                case STONE_TYPE_DIAMOND:
                    animate = Animate::create(stoneDiamondAni);
                    break;
                case STONE_TYPE_CLOVER:
                    animate = Animate::create(stoneCloverAni);
                    break;
                case STONE_TYPE_HEART:
                    animate = Animate::create(stoneHeartAni);
                    break;
                case STONE_TYPE_SPACE:
                    animate = Animate::create(stoneSpaceAni);
                    break;
            }

            CallFuncN* callFunctionSwap = CallFuncN::create(CC_CALLBACK_0(GameScene::stoneAniActionFinished, this, jpstone));

            Sequence* callActionSwap;
            if(jpstone == grid[grapX][grapY]) {
                CallFuncN* callFunctionSwapFinish = CallFuncN::create(CC_CALLBACK_0(GameScene::stoneAniActionAllFinished, this));
                DelayTime* delay = DelayTime::create(0.1f);
                callActionSwap = Sequence::create(animate, callFunctionSwap, delay, callFunctionSwapFinish, NULL);

            } else {
                callActionSwap = Sequence::create(animate, callFunctionSwap, NULL);
            }

            jpstone->mySprite->stopAllActions();
            jpstone->mySprite->runAction(callActionSwap);
        }
    }

    if(!isGrapjackpot && !isSwapjackpot) {
        int backdir;
        switch (dir) {
            case DIRECTION_UP:
                backdir = DIRECTION_DOWN;
                break;
            case DIRECTION_DOWN:
                backdir = DIRECTION_UP;
                break;
            case DIRECTION_LEFT:
                backdir = DIRECTION_RIGHT;
                break;
            case DIRECTION_RIGHT:
                backdir = DIRECTION_LEFT;
                break;
        }
        grapStone = grid[swapX][swapY];
        grapidxX = swapX;
        grapidxY = swapY;
        
        swapStone(backdir, false);
    }
}

void GameScene::stoneAniActionFinished(Stone* stone){
    stone->mySprite->setVisible(false);
    Point indxpt = stone->mySprite->getPosition();
    int idxX, idxY;
    idxX = (indxpt.x - (GRID_OFFSET.x - GRID_BORDER/2)) / GRID_BORDER;
    idxY = (indxpt.y - (GRID_OFFSET.y - GRID_BORDER/2)) / GRID_BORDER;
    log("setVisible(false) X = %d Y = %d",idxX, idxY);
}

void GameScene::stoneAniActionAllFinished(){
    log("GameScene::stoneAniActionAllFinished ENTRY");
    for(int i=0;i<GRID_WIDTH;i++) {
        for(int j=0;j<GRID_HEIGHT;j++) {
            if(!grid[i][j]->mySprite->isVisible()) {
                log("grid[%d][%d] is invisible", i, j);
                grid[i][j]->removeFromParent();
                grid[i][j] = NULL;
            }
        }
    }

    // Reset Stone
    for(int i=0;i<GRID_WIDTH;i++) {
        int lh = 0;
        for(int j=0;j<GRID_HEIGHT;j++) {
            if(grid[i][j] == NULL) {
                lh = j;
                int h = j+1;
                int emptycnt = 0;

                while(h < GRID_HEIGHT) {
                    if(grid[i][h] == NULL){
                        emptycnt++;
                        h++;
                        continue;
                    } else {
                        grid[i][lh] = grid[i][h];
                        grid[i][h] = NULL;

                        int moveX, moveY;
                        moveX = GRID_OFFSET.x + (GRID_BORDER * i);
                        moveY = GRID_OFFSET.y + (GRID_BORDER * lh);

                        ActionInterval* action = MoveTo::create(0.5f, Point(moveX, moveY+10));
                        ActionInterval* action2 = MoveTo::create(0.5f, Point(moveX, moveY));

                        Sequence* actionStone = Sequence::create(EaseExponentialIn::create(action), EaseBounceOut::create(action2), NULL);
                        grid[i][lh]->mySprite->stopAllActions();
                        grid[i][lh]->mySprite->runAction(actionStone);
                        lh = h - emptycnt;
                    }
                    h++;
                }

                int emptyinputidx = (GRID_HEIGHT-1) - emptycnt;
                int newstoneidxY = 0;
                while(emptycnt >= 0) {
                    Stone *s = new Stone();
                    s->initWithGame(this);
                    grid[i][emptyinputidx] = s;

                    grid[i][emptyinputidx]->placeInGride(Point(GRID_BORDER*i + GRID_OFFSET.x, GRID_BORDER*(GRID_HEIGHT + newstoneidxY++) + GRID_OFFSET.y), -1, -1);

                    int moveX, moveY;
                    moveX = GRID_OFFSET.x + (GRID_BORDER * i);
                    moveY = GRID_OFFSET.y + (GRID_BORDER * emptyinputidx);
                    ActionInterval* action = MoveTo::create(0.5f, Point(moveX, moveY+10));
                    ActionInterval* action2 = MoveTo::create(0.5f, Point(moveX, moveY));
                    Sequence* actionStone = Sequence::create(EaseExponentialIn::create(action), EaseBounceOut::create(action2), NULL);

                    CallFuncN* callFunctionSwap = CallFuncN::create(CC_CALLBACK_0(GameScene::jackpotAniActionAllFinished, this));

                    Sequence* callActionNewStone = Sequence::create(actionStone, callFunctionSwap, NULL);

                    grid[i][emptyinputidx]->mySprite->stopAllActions();
                    grid[i][emptyinputidx]->mySprite->runAction(callActionNewStone);

                    emptyinputidx++;
                    emptycnt--;
                }
            }
        }
    }
}

void GameScene::jackpotAniActionAllFinished(){
    allowTouch = true;
    grapStone = NULL;
}

void GameScene::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::placeStones() {
    for(int i=0;i<GRID_WIDTH;i++) {
        for(int j=0;j<GRID_HEIGHT;j++){
            Stone *lefts = NULL;
            Stone *leftmostS = NULL;
            Stone *topS = NULL;
            Stone *topmostS = NULL;
            int prohibitedLeft = -1, prohibitedTop = -1;

            if(i >= 2) {
                lefts = (Stone *) grid[i-1][j];
                leftmostS = (Stone *) grid[i-2][j];
            }
            if(j >= 2) {
                topS = (Stone *) grid[i][j-1];
                topmostS = (Stone *) grid[i][j-2];
            }
            if(lefts && leftmostS && lefts->stoneType == leftmostS->stoneType) {
                prohibitedLeft = lefts->stoneType;
            }
            if(topS && topmostS && topS->stoneType == topmostS->stoneType) {
                prohibitedTop = topS->stoneType;
            }
            grid[i][j]->placeInGride(Point(GRID_BORDER*i + GRID_OFFSET.x, GRID_BORDER*j + GRID_OFFSET.y), prohibitedTop, prohibitedLeft);
        }
    }
}
