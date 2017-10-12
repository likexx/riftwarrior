//
//  ActionMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#include "ActionMenu.h"
#include "Player.h"
#include "GameData.h"
#include "ActiveSkillSetting.h"
#include "Unit.h"
#include "GameScene.h"
#include "Function.h"

#define ICON_SIZE 80

#define START_TAG 1000

bool ActionMenu::init()
{
    if (!CCMenu::init())
    {
        return false;
    }
    
    reset();
    
    scheduleUpdate();
    
    return true;
}

bool ActionMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    if (m_eState != kCCMenuStateWaiting || ! isVisible() || !isEnabled())
    {
        return false;
    }
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    m_pSelectedItem = this->itemForTouch(touch);
    if (m_pSelectedItem)
    {
        m_eState = kCCMenuStateTrackingTouch;
        m_pSelectedItem->selected();
        
        if (m_pWhirlWindBtn == m_pSelectedItem)
        {
            // for player whirlwind attack
            Player::getInstance()->startWhirlWind();
        }
        
        return true;
    }
    return false;
}

void ActionMenu::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    if (m_pSelectedItem)
    {
        m_pSelectedItem->unselected();
        
        if (m_pWhirlWindBtn != m_pSelectedItem)
        {
            // if not whirlwind attack
            m_pSelectedItem->activate();
        }
        else
        {
            // stop whirl wind
            Player::getInstance()->stopWhirlWind();
        }
    }
    m_eState = kCCMenuStateWaiting;
}





void ActionMenu::reset()
{
    removeAllChildrenWithCleanup(true);
    m_pWhirlWindBtn = NULL;
    m_pEarthSlashBtn = NULL;
    m_pRecoverBtn = NULL;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //check if player can use whirlwind
    map<int, ActiveSkillSetting>::const_iterator skillIt = GameData::getActiveSkills().begin();
    
    while (skillIt != GameData::getActiveSkills().end())
    {
        const ActiveSkillSetting& skill = skillIt->second;
//        if (Player::getInstance()->canUseSkill(skill.id))
        {
            CCMenuItemSprite* item = createSkillIcon(skill.id);
            item->setPosition(ccp(winSize.width - 20 - item->getContentSize().width * ((float)(skill.id-1)+0.5), item->getContentSize().height/2 + 10));
            
            item->setVisible(false);
            
            addChild(item, 0, START_TAG + skill.id);
            
            switch (skill.id)
            {
                case ENUM_PLAYER_PERFORM_SKILL_WHIRLWIND:
                    m_pWhirlWindBtn = item;
                    break;
                case ENUM_PLAYER_PERFORM_SKILL_EARTH_SLASH:
                    m_pEarthSlashBtn = item;
                    break;
                case ENUM_PLAYER_PERFORM_SKILL_RECOVER:
                    m_pRecoverBtn = item;
                    break;
            }
        }
        skillIt++;
    }
    
    
    /*
    if (GameScene::getInstance()->sharedGameStage->getPlayerBase())
    {
        // infantry button
        CCSprite* sprite = CCSprite::create("UI/building/building9_normal.png");
        
        CCSprite* select = CCSprite::create("UI/building/building9_pressed.png");
        
        m_InfantryBtn = CCMenuItemSprite::create(sprite, select, this, menu_selector(ActionMenu::buildInfantry));
        
        m_InfantryBtn->setPosition(ccp(winSize.width - 20 - m_InfantryBtn->getContentSize().width * ((float)count+0.5), m_InfantryBtn->getContentSize().height/2 + 10));
        
        addChild(m_InfantryBtn);
        
        count++;
        
        
        // Tank icon
        sprite = CCSprite::create("UI/building/building11_normal.png");
        
        select = CCSprite::create("UI/building/building11_pressed.png");
        
        m_TankBtn = CCMenuItemSprite::create(sprite, select, this, menu_selector(ActionMenu::buildTank));
        
        m_TankBtn->setPosition(ccp(winSize.width - 20 - m_TankBtn->getContentSize().width * ((float)count+0.5), m_TankBtn->getContentSize().height/2 + 10));
        
        addChild(m_TankBtn);
        
        count++;
        
        // Gather Point
        sprite = CCSprite::create("UI/building/upgrade_normal.png");
        
        select = CCSprite::create("UI/building/upgrade_pressed.png");
        
        CCMenuItemSprite* btn = CCMenuItemSprite::create(sprite, select, this, menu_selector(ActionMenu::setUnitDestination));
        
        btn->setPosition(ccp(winSize.width - 20 - btn->getContentSize().width * ((float)count+0.5), btn->getContentSize().height/2 + 10));
        
        addChild(btn);
        
        count++;

    }
    */
    
    // show basic magic skills from 1~5
    /*
    vector<Magic*> allMagic = GameData::getAllMagic();

    for(int i=0;i<allMagic.size();++i, ++count)
    {
        Magic* magic = allMagic[i];
        if (!Player::getInstance()->canUseMagic(magic->id))
        {
            continue;
        }
        
        CCMenuItemSprite* icon = createSkillIcon(1);
        icon->setPosition(ccp(winSize.width - 20 - icon->getContentSize().width * ((float)count+0.5), icon->getContentSize().height/2 + 10));
        addChild(icon,0,START_TAG + magic->id);
    }
     */
    
    this->setPosition(0,0);


    
}

CCMenuItemSprite* ActionMenu::createSkillIcon(int skillId)
{
    char buffer[128] = {0};
    sprintf(buffer, "UI/skill/skill_%d_normal.png", skillId);
    CCSprite* sprite = CCSprite::create(buffer);
    
    sprintf(buffer, "UI/skill/skill_%d_pressed.png", skillId);
    CCSprite* select = CCSprite::create(buffer);
    
    CCMenuItemSprite* icon = CCMenuItemSprite::create(sprite, select, this, menu_selector(ActionMenu::setPerformSkill));
    
    return icon;
}

void ActionMenu::setPerformSkill(cocos2d::CCObject *pSender)
{
    if (GameScene::getInstance()->sharedMainUI->getStageUI()->hasMessageBox() ||
        GameScene::getInstance()->sharedGameStage->isPaused() ||
        !GameScene::getInstance()->sharedGameStage->isReady())
    {
        return;
    }
    
    CCNode* pItem = static_cast<CCNode*>(pSender);
    Player::getInstance()->performSkill(pItem->getTag() - START_TAG);
    
}


void ActionMenu::update(float dt)
{
    if (GameScene::getInstance()->sharedGameStage->isPaused() || !GameScene::getInstance()->sharedGameStage->isReady())
    {
        return;
    }
    
    map<int, ActiveSkillSetting>::const_iterator skillIt = GameData::getActiveSkills().begin();
    
    while (skillIt != GameData::getActiveSkills().end())
    {
        const ActiveSkillSetting& skill = skillIt->second;
        CCMenuItemSprite* item = (CCMenuItemSprite*)getChildByTag(START_TAG+skill.id);
        
        if (item)
        {
            if (!Player::getInstance()->canUseSkill(skill.id))
            {
                item->setVisible(false);
            }
            else
            {
                if (!item->isVisible())
                {
                    item->setVisible(true);
                }
 
                if (Player::getInstance()->canPerformSkill(skill.id))
                {
                    item->setColor(ccc3(255,255,255));
                }
                else
                {
                    item->setColor(ccc3(128,128,128));
                }
                
            }
        }
                
        skillIt++;
    }
    
     
}



