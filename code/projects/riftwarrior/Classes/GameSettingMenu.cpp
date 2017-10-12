//
//  GameSettingMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 3/27/13.
//
//

#include "GameSettingMenu.h"
#include "GameScene.h"
#include "Player.h"
#include "SkillTreeMenu.h"
#include "SystemHelper.h"
//#include "SnsHelper.h"
#include "StageUI.h"
#include "SimpleAudioEngine.h"
#include "constant.h"

#define STAGE_UI_TAG 1001

bool GameSettingMenu::init()
{
    if (!CCLayerColor::initWithColor(ccc4(1,1,1,128)))
    {
        return false;
    }
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* background = CCSprite::create("UI/setting_background.png");
    
    background->setAnchorPoint(ccp(0.5, 0.5));
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    
    CCMenu* menu = CCMenu::create();

    
//    CCSprite* button = CCSprite::create("UI/redbutton_nomal.png");
//    CCSprite* pressed = CCSprite::create("UI/redbutton_pressed.png");
//    
//    CCMenuItemSprite* shareBtn = CCMenuItemSprite::create(button, pressed, this, menu_selector(GameSettingMenu::onShare));
//    CCLabelTTF*  message = CCLabelTTF::create(GameData::getText("share"), STANDARD_NUMBER_FONT_NAME, 28);
//    message->setPosition(ccp(shareBtn->getContentSize().width/2, shareBtn->getContentSize().height/2));
//    shareBtn->addChild(message);
//    shareBtn->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.7));
//    
//    menu->addChild(shareBtn);
    
    
    CCSprite* button = CCSprite::create("UI/greenbutton_nomal.png");
    CCSprite* pressed = CCSprite::create("UI/greenbutton_pressed.png");

    CCMenuItemSprite* resumeButton = CCMenuItemSprite::create(button, pressed, this, menu_selector(GameSettingMenu::onResume));
    CCLabelTTF* message = CCLabelTTF::create(GameData::getText("resume_game"), STANDARD_NUMBER_FONT_NAME, 28);
    message->setPosition(ccp(resumeButton->getContentSize().width/2, resumeButton->getContentSize().height/2));
    resumeButton->addChild(message);
    resumeButton->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.6));
    menu->addChild(resumeButton);

    button = CCSprite::create("UI/redbutton_nomal.png");
    pressed = CCSprite::create("UI/redbutton_pressed.png");
    CCMenuItemSprite* exitButton = CCMenuItemSprite::create(button, pressed, this, menu_selector(GameSettingMenu::onExit));
    message = CCLabelTTF::create(GameData::getText("go_back_to_worldmap"), STANDARD_NUMBER_FONT_NAME, 28);
    message->setPosition(ccp(exitButton->getContentSize().width/2, exitButton->getContentSize().height/2));
    exitButton->addChild(message);
    exitButton->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.3));
    menu->addChild(exitButton);
    
    
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    background->addChild(menu);
    this->addChild(background);
    
    scheduleUpdate();
    return true;
}

//void GameSettingMenu::onShare(cocos2d::CCObject *pSender)
//{
////    SnsHelper::weiboLogin();
//    CCNode* menu = GameScene::getInstance()->sharedMainUI->getChildByTag(STAGE_UI_TAG)->getChildByTag(OPENINGWINDOW_TAG);
//    menu->setVisible(false);
//    SnsHelper::shareScreenShot(GAMESHARE);
//    menu->setVisible(true);
//}

void GameSettingMenu::onResume(cocos2d::CCObject *pSender)
{
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    GameScene::getInstance()->sharedGameStage->resume();
    this->removeFromParentAndCleanup(true);
}

void GameSettingMenu::onExit(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    
    if (Player::getInstance()->hasCompletedMap(GameScene::getInstance()->sharedGameStage->getMapId()))
    {
        Player::getInstance()->save();
    }
    else
    {
        // if player hasn't completed the map, dont' save anything
        
    }
    
    CCDictionary* params = CCDictionary::create();
    char eventName[128] = {0};
    sprintf(eventName, "play_map_%d", GameScene::getInstance()->sharedGameStage->getMapId());
    
    params = CCDictionary::create();
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getLevel()), "player_level");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getMoney()), "player_money");
    params->setObject(CCString::create("exit"), "result");
    SystemHelper::endEvent(eventName, params);
    
    GameScene::getInstance()->sharedGameStage->resume();
    this->removeFromParentAndCleanup(true);
    /*
     CCDirector::sharedDirector()->popScene();
     */
    GameScene::getInstance()->exitStageMap();
    //    scheduleOnce(schedule_selector(GameOverMenu::exitStage), 1);
}

void GameSettingMenu::exitStage()
{
    GameScene::getInstance()->exitStageMap();
}

