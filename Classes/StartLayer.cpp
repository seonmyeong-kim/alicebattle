//
//  StartLayer.cpp
//  cocos-x-test
//
//  Created by rin_horie_2 on 2014/01/24.
//
//

#include "StartLayer.h"
#include "GameScene.h"
#include "AppValues.h"

USING_NS_CC;
using cocos2d::network::HttpRequest;
using cocos2d::network::HttpClient;
using cocos2d::network::HttpResponse;

StartLayer::StartLayer() {
    
}
StartLayer::~StartLayer() {
    
}

bool StartLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    Size screenSize = Director::getInstance()->getVisibleSize();

    SCALE_DENSITY = screenSize.width / 640;
    this->setScale(SCALE_DENSITY);
    
    Sprite *bgSprite = Sprite::create("loading.png");
    bgSprite->setPosition(Point(screenSize.width/2, screenSize.height/2));
    this->addChild(bgSprite);

    Sprite* card = Sprite::create();
    Animation* animation = Animation::create();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("card.plist");

    int cardheight;
    for(int i=0;i<13;i++){
        char framename[64];
        sprintf(framename, "card%d.png",i+1);

        SpriteFrame *sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
        Rect rect = sf->getRect();
        cardheight = rect.size.height;
        animation->addSpriteFrame(sf);
    }
    animation->setDelayPerUnit(0.1f);

    Animate* cardAni = Animate::create(animation);
    RepeatForever* rf = RepeatForever::create(cardAni);
    card->setPosition(Point(screenSize.width/2,screenSize.height/2 + cardheight));
    this->addChild(card,1);
    card->runAction(rf);

    
    // リクエスト生成
    HttpRequest* request = new HttpRequest();
    request->setUrl("http://httpbin.org/post");
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(StartLayer::onHttpRequestCompleted));
    request->setTag("POST test1");

    // POSTデータ生成
    string postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
    request->setRequestData(postData.c_str(), postData.length());

    // リクエスト送信
    HttpClient::getInstance()->send(request);
    request->release();

    return true;
}

void StartLayer::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response) {
    if (!response)
        return;

    // タグ設定時は、タグをログに出力
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        log("%s completed", response->getHttpRequest()->getTag());
    }

    // ステータスコード取得
    int statusCode = response->getResponseCode();
    log("response code: %d", statusCode);

    // 画面表示文字列生成
    String* statusString = String::createWithFormat("HTTP Status Code: %d\ntag = %s",
                                                    statusCode,
                                                    response->getHttpRequest()->getTag());

    // レスポンスが不正な場合は、エラーを出力して終了
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        return;
    }

    // 取得データの出力
    vector<char> *buffer = response->getResponseData();
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        printf("%c", (*buffer)[i]);
    }
    printf("\n");

    // ステータスコード表示
    Size size = Director::getInstance()->getVisibleSize();
    LabelTTF* label = LabelTTF::create(statusString->getCString(), "Arial", 24);
    label->setPosition(Point(size.width / 2, size.height / 2));
    this->addChild(label, 1);

    DelayTime* delay = DelayTime::create(3);
    CallFunc* callback = CallFunc::create(CC_CALLBACK_0(StartLayer::loadingFinished, this));

    Sequence* loadingSeq = Sequence::create(delay, callback, NULL);

    this->runAction(loadingSeq);
}

void StartLayer::loadingFinished()
{
    Scene* gamescene = GameScene::scene();
    Director::getInstance()->replaceScene(gamescene);
    
}