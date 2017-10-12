//
//  ike_tdgameAppDelegate.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/5/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

#include <cocos2d.h>
#include <cocos-ext.h>

#include "AppDelegate.h"
#include "GameScene.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "GameData.h"

using namespace cocos2d::extension;
using namespace CocosDenshion;

#define DESIGN_SCREEN_WIDTH 960
#define DESIGN_SCREEN_HEIGHT 640

USING_NS_CC;

class LogoScene : public CCScene
{
public:
    CREATE_FUNC(LogoScene);
    
    bool init()
    {
        if (!CCScene::init())
        {
            return false;
        }
        
        CCSprite* pLogo1 = CCSprite::create("UI/kkgame_logo.png");

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        pLogo1->setScale(winSize.width / pLogo1->getContentSize().width);

        pLogo1->setPosition(ccp(winSize.width/2, winSize.height/2));

        addChild(pLogo1);

        pLogo1->runAction(CCSequence::create(CCDelayTime::create(1.5),
                                             CCFadeOut::create(0.5),
                                             CCRemoveSelf::create(),
                                             CCCallFunc::create(this, callfunc_selector(LogoScene::startGame)),
                                             NULL));
        
        return true;
    }
    
    void startGame()
    {
        CCDirector *pDirector = CCDirector::sharedDirector();
        
        GameData::initLanguageText();

        StartScreen* startScreen = StartScreen::createNewScreen();
        
        CCSprite* background = static_cast<CCSprite*>(startScreen->background);
        
        CCSize screenSize = startScreen->getContentSize();
        
        background->setScaleX(screenSize.width / background->getContentSize().width);
        background->setScaleY(screenSize.height / background->getContentSize().height);
        
        CCScene* scene = CCScene::create();
        scene->addChild(startScreen);

        // FIXME
        // Check %RootPath%\CocosDenshion\wp8\Audio.cpp:502, no mp3 support on WP8 yet. We have to convert this to wav.
        //SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/op.mp3", true);

        pDirector->replaceScene(scene);
    }
};

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
//    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCDirector::sharedDirector()->setDepthTest(true);
//    pDirector->setProjection(kCCDirectorProjection2D);
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    
    
    // turn on display FPS
//    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    
    
    CCScene* pLogoScene = LogoScene::create();
    
    pDirector->pushScene(pLogoScene);


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();
    // FIXME
    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    // FIXME
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

