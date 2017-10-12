//
//  GameOverMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 3/1/13.
//
//

#include "GameOverMenu.h"
#include "GameScene.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "constant.h"

bool GameOverMenu::init()
{
    if (!CCLayerColor::initWithColor(ccc4(1,1,1,128)))
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* background = CCSprite::create("UI/winning_menu/lost.png");
    background->setAnchorPoint(ccp(0.5f, 0.5f));
    background->setPosition(ccp(winSize.width/2, winSize.height * 1.5f));
    
    string value = "";
    const MapWinningCondition* pCondition = GameScene::getInstance()->sharedGameStage->getWinningCondition();
    Player* pPlayer = Player::getInstance();
    
    char reason[512] = {0};
    
    GameStage* pStage =  GameScene::getInstance()->sharedGameStage;
    
    if (pCondition->killPlayer && pPlayer->isDead())
    {
        sprintf(reason, GameData::getText("player_must_be_alive"));
    }
    else if (pCondition->killNpcId>0 && pStage->getNpc(pCondition->killNpcId)->isDead())
    {
        const NpcSetting* setting = GameData::getNpcSetting(pCondition->killNpcId);
        sprintf(reason, GameData::getText("protect_npc"), setting->name.c_str(), setting->name.c_str());
    }
    else if (pCondition->maxPassEnemies > 0 && pStage->getPassedEnemies()>pCondition->maxPassEnemies)
    {
        sprintf(reason, GameData::getText("max_allowed_passing_enemies"), pStage->getPassedEnemies(), pCondition->maxPassEnemies);
    }
    else if (pCondition->minKilledEnemies > 0 && pPlayer->getKillCount() < pCondition->minKilledEnemies)
    {
        sprintf(reason, GameData::getText("not_enough_killed_enemies"), pPlayer->getKillCount(), pCondition->minKilledEnemies);
    }
    else if (pCondition->maxTime > 0)
    {
        sprintf(reason, GameData::getText("time_out"));
    }
    
    CCLabelTTF* label = CCLabelTTF::create(reason, STANDARD_NUMBER_FONT_NAME, 26);
    label->setAnchorPoint(ccp(0.5, 1));
    label->setColor(ccc3(0, 0, 0));
    label->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.7));
    background->addChild(label);
    
    CCMenu* menu = CCMenu::create();

    CCSprite* button = CCSprite::create("UI/greenbutton_nomal.png");
    CCSprite* pressed = CCSprite::create("UI/greenbutton_pressed.png");
    CCMenuItemSprite* replayButton = CCMenuItemSprite::create(button, pressed, this, menu_selector(GameOverMenu::onReplay));
    CCLabelTTF* message = CCLabelTTF::create(GameData::getText("retry"), STANDARD_NUMBER_FONT_NAME, 28);
    message->setPosition(ccp(replayButton->getContentSize().width/2, replayButton->getContentSize().height/2));
    replayButton->addChild(message);
    replayButton->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.4));
    menu->addChild(replayButton);

    if (Player::getInstance()->hasCompletedTutorial())
    {
        button = CCSprite::create("UI/redbutton_nomal.png");
        pressed = CCSprite::create("UI/redbutton_pressed.png");
        CCMenuItemSprite* exitButton = CCMenuItemSprite::create(button, pressed, this, menu_selector(GameOverMenu::onExit));
        message = CCLabelTTF::create(GameData::getText("go_back_to_worldmap"), STANDARD_NUMBER_FONT_NAME, 28);
        message->setPosition(ccp(exitButton->getContentSize().width/2, exitButton->getContentSize().height/2));
        exitButton->addChild(message);
        exitButton->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.2));
        menu->addChild(exitButton);
    }
    
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    background->addChild(menu);
    this->addChild(background);
    
    
    CCSmartSprite* icon = CCSmartSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/winning_menu/lost_icon.png"));
    icon->setAnchorPoint(ccp(0.5f, 0.5f));
    icon->setPosition(ccp(winSize.width/2, winSize.height/2));
    
    addChild(icon);
    
    background->setPosition(ccp(background->getPosition().x, background->getPosition().y + background->getContentSize().height));
    
    icon->setScale(0.2f);
    icon->runAction(CCSequence::create(CCScaleTo::create(0.3f, 1.0f),
                                       CCScaleTo::create(0.1f, 0.9f),
                                       CCScaleTo::create(0.1f, 1.0f),
                                       CCDelayTime::create(1),
                                       CCCallFunc::create(icon, callfunc_selector(CCSmartSprite::removeSelf)),
                                       NULL));
    
    background->runAction(CCSequence::create(CCDelayTime::create(1.5),
                                             CCMoveTo::create(0.2f, ccp(background->getPosition().x, winSize.height/2)),
                                             NULL));

    this->setAnchorPoint(CCPointZero);
    this->setPosition(CCPointZero);
    
    return true;
}

void GameOverMenu::onReplay(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    int exp = Player::getInstance()->getExperience();
    int level = Player::getInstance()->getLevel();
    
    Player::getInstance()->load();

    
    Player::getInstance()->setExperience(exp);
    Player::getInstance()->setLevel(level);
    
    Player::getInstance()->save();

    
    CCDirector::sharedDirector()->resume();
    this->removeFromParentAndCleanup(true);
    GameScene::getInstance()->sharedMainUI->getStageUI()->removeBuildingMenu();
    GameScene::getInstance()->sharedGameStage->reset();
}

void GameOverMenu::onExit(cocos2d::CCObject *pSender)
{
 
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    int exp = Player::getInstance()->getExperience();
    int level = Player::getInstance()->getLevel();
    
    Player::getInstance()->load();

    
    Player::getInstance()->setExperience(exp);
    Player::getInstance()->setLevel(level);
    
    Player::getInstance()->save();

    CCDirector::sharedDirector()->resume();
    this->removeFromParentAndCleanup(true);
    /*
    CCDirector::sharedDirector()->popScene();
    */
    GameScene::getInstance()->exitStageMap();
//    scheduleOnce(schedule_selector(GameOverMenu::exitStage), 1);
}



