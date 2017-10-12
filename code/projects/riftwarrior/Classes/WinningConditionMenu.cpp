//
//  WinningConditionMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 4/26/13.
//
//

#include "WinningConditionMenu.h"
#include "GameData.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "constant.h"

bool WinningConditionMenu::init()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* background = CCSprite::create("UI/winning_condition/condition_background.png");
    
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    background->setAnchorPoint(ccp(0.5, 0.5));
    m_pBackground = background;
        
    m_TitleLabel = CCLabelTTF::create(GameData::getText("winning_condition"), STANDARD_NUMBER_FONT_NAME, 28);
    m_TitleLabel->setAnchorPoint(ccp(0.5, 0));
    m_TitleLabel->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.9));
    m_TitleLabel->setColor(ccc3(255, 255 ,255));
    background->addChild(m_TitleLabel);
    
    char data[512] = {0};

    
    const MapWinningCondition* pCondition = GameScene::getInstance()->sharedGameStage->getWinningCondition();
    
    float x = 10;
    float y = background->getContentSize().height * 0.78;
    float yDiff = background->getContentSize().height * 0.1;
    
    if (pCondition->killPlayer)
    {
        CCLabelTTF* label = CCLabelTTF::create(GameData::getText("player_must_be_alive"), STANDARD_NUMBER_FONT_NAME, 28);
        label->setScale(0.8f);
        label->setPosition(ccp(x, y));
        label->setAnchorPoint(ccp(0.0f, 0.5f));
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setColor(ccc3(1,1,1));
        background->addChild(label);
        y-=yDiff;
    }
    
    if (pCondition->killNpcId > 0)
    {
        const NpcSetting* setting = GameData::getNpcSetting(pCondition->killNpcId);
        
        sprintf(data, GameData::getText("protect_npc"), setting->name.c_str());

        CCLabelTTF* label = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 28);
        label->setScale(0.8f);
        label->setPosition(ccp(x, y));
        label->setAnchorPoint(ccp(0.0f, 0.5f));
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setColor(ccc3(1,1,1));

        background->addChild(label);
        y-=yDiff;
        
    }
    
    if (pCondition->maxPassEnemies > 0)
    {
        sprintf(data, GameData::getText("max_allowed_passing_enemies"), pCondition->maxPassEnemies);

        CCLabelTTF* label = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 28);
        label->setScale(0.8f);
        label->setPosition(ccp(x, y));
        label->setAnchorPoint(ccp(0.0f, 0.5f));
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setColor(ccc3(1, 1, 1));

        background->addChild(label);
        y-=yDiff;
        
    }

    if (pCondition->minKilledEnemies > 0)
    {
        sprintf(data, GameData::getText("min_killed_enemies"), pCondition->minKilledEnemies);
        CCLabelTTF* label = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 28);
        label->setScale(0.8f);
        label->setPosition(ccp(x, y));
        label->setAnchorPoint(ccp(0.0f, 0.5f));
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setColor(ccc3(1,1,1));

        background->addChild(label);
        y-=yDiff;

    }
    
    if (pCondition->maxTime)
    {
        sprintf(data, GameData::getText("max_time_to_finish"), pCondition->maxTime);
        CCLabelTTF* label = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 28);
        label->setScale(0.8f);
        label->setPosition(ccp(x, y));
        label->setAnchorPoint(ccp(0.0f, 0.5f));
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setColor(ccc3(1,1,1));

        background->addChild(label);
        y-=yDiff;
    }

    if (pCondition->restrictedWeapons.size() > 0)
    {
        set<int>::iterator it = pCondition->restrictedWeapons.begin();
        string value = "";
        
        value += GameData::getText("cannot_use_weapons");
        
        while(it != pCondition->restrictedWeapons.end())
        {
            int weaponId = *it;
            
            const BuildingSetting& setting = GameData::getBuildingSetting(weaponId);
            value += setting.name;
            ++it;
            if (it!=pCondition->restrictedWeapons.end())
            {
                value += ",";
            }
        }
        
        CCLabelTTF* label = CCLabelTTF::create(value.c_str(), STANDARD_NUMBER_FONT_NAME, 28);
        label->setScale(0.8f);
        label->setPosition(ccp(x, y));
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setAnchorPoint(ccp(0.0f, 0.5f));
        label->setColor(ccc3(1,1,1));

        background->addChild(label);
        y-=yDiff;

    }
    
    if (pCondition->destroyBase)
    {
        CCLabelTTF* label = CCLabelTTF::create(GameData::getText("destroy_base"), STANDARD_NUMBER_FONT_NAME, 28);
        label->setScale(0.8f);
        label->setPosition(ccp(x, y));
        label->setAnchorPoint(ccp(0.0f, 0.5f));
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        label->setColor(ccc3(1,1,1));
        
        background->addChild(label);
        y-=yDiff;
    }
    
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    menu->setContentSize(background->getContentSize());
    
    CCMenuItemImage* closeBtn = CCMenuItemImage::create("UI/greenbutton_nomal.png", "UI/greenbutton_pressed.png");
    closeBtn->setTarget(this, menu_selector(WinningConditionMenu::close));
    closeBtn->setPosition(ccp(menu->getContentSize().width/2, menu->getContentSize().height * 0.15));
    
    CCLabelTTF* closeLabel = CCLabelTTF::create(GameData::getText("close"), STANDARD_NUMBER_FONT_NAME, 24);
    closeLabel->setPosition(ccp(closeBtn->getContentSize().width/2, closeBtn->getContentSize().height * 0.5));
    closeBtn->addChild(closeLabel);
    
    menu->addChild(closeBtn);
    
    background->addChild(menu);
    
    this->setAnchorPoint(CCPointZero);
    this->setPosition(CCPointZero);
    addChild(background);
    
    return true;

}


void WinningConditionMenu::close(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    removeFromParentAndCleanup(true);
    
    GameScene::getInstance()->sharedGameStage->start();
}