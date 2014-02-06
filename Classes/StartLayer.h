//
//  StartLayer.h
//  cocos-x-test
//
//  Created by rin_horie_2 on 2014/01/24.
//
//

#ifndef __cocos_x_test__StartLayer__
#define __cocos_x_test__StartLayer__

#include "cocos2d.h"

using namespace cocos2d;

#include <network/HttpRequest.h>
#include <network/HttpResponse.h>
#include <network/HttpClient.h>


#define SCENE_FUNC(__TYPE__) \
    static cocos2d::Scene * scene() { \
        __TYPE__ *sl = __TYPE__::create(); \
        cocos2d::Scene *s = cocos2d::Scene::create(); \
        s->addChild(sl); \
        return s; \
    }

class StartLayer : public cocos2d::Layer {
    
public:
    
    StartLayer();
    virtual ~StartLayer();
    
    bool init();
    CREATE_FUNC(StartLayer);
    SCENE_FUNC(StartLayer);

    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void loadingFinished();
};

#endif /* defined(__cocos_x_test__StartLayer__) */
