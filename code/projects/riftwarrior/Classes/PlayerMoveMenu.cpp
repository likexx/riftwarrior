//
//  PlayerMoveMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 3/29/13.
//
//

#include "PlayerMoveMenu.h"
#include "Player.h"
#include "StageMap.h"
#include "MapHelper.h"
#include "GameScene.h"
#include "SpecialActions.h"

bool PlayerMoveMenu::init()
{
    if (!CCMenu::init())
    {
        return false;
    }
    
    
    return true;
}

void PlayerMoveMenu::moveForward(cocos2d::CCObject *pSender)
{
    if (GameScene::getInstance()->sharedGameStage->isPaused() ||
        !GameScene::getInstance()->sharedGameStage->isReady())
    {
        return;
    }
    
    Player* pPlayer = Player::getInstance();
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint destTile = pMap->getTileBySequence(4);
    CCPoint target = MapHelper::tilePosToGroundMapPos(ccp(destTile.x + 0.5, destTile.y + 0.5));
    pPlayer->setMoveDestination(target, false);
    pPlayer->setForceToMove(true);
    GameScene::getInstance()->sharedGameStage->scrollToPlayer();
    
}

void PlayerMoveMenu::moveBack(cocos2d::CCObject *pSender)
{
    if (GameScene::getInstance()->sharedGameStage->isPaused() ||
        !GameScene::getInstance()->sharedGameStage->isReady())
    {
        return;
    }
    
    Player* pPlayer = Player::getInstance();
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint destTile = pMap->getEndTilePosition();
    CCPoint target = MapHelper::tilePosToGroundMapPos(ccp(destTile.x + 0.5, destTile.y + 0.5));
    pPlayer->setMoveDestination(target, false);
    pPlayer->setForceToMove(true);
    GameScene::getInstance()->sharedGameStage->scrollToPlayer();

}


bool PlayerMoveMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
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
        
        m_pSelectedItem->activate();
        
        return true;
    }
    return false;
}

void PlayerMoveMenu::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    if (m_pSelectedItem)
    {
        m_pSelectedItem->unselected();
        Player::getInstance()->stopMove();
//        m_pSelectedItem->activate();
    }
    m_eState = kCCMenuStateWaiting;
}

void PlayerMoveMenu::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
    if (m_pSelectedItem)
    {
        m_pSelectedItem->unselected();
        
    }
    
    Player::getInstance()->stopMove();
    
    m_eState = kCCMenuStateWaiting;
}

void PlayerMoveMenu::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    CCMenuItem *currentItem = this->itemForTouch(touch);
    if (currentItem != m_pSelectedItem)
    {
        if (m_pSelectedItem)
        {
            m_pSelectedItem->unselected();
            Player::getInstance()->stopMove();
        }
        m_pSelectedItem = currentItem;
        if (m_pSelectedItem)
        {
            m_pSelectedItem->selected();
            m_pSelectedItem->activate();
        }
    }
}

void PlayerMoveMenu::updateButtonFunction()
{
    bool entranceOnLeft = false;
    
    ENUM_NPC_ANIMATION standingDirection = GameScene::getInstance()->sharedGameStage->getGroundMap()->getPlayerStandingDirection();
    
    if (standingDirection == ENUM_NPC_ANIMATION_STAND_DOWN || standingDirection == ENUM_NPC_ANIMATION_STAND_LEFT)
    {
        entranceOnLeft = false;
    }
    else
    {
        entranceOnLeft = true;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    
    CCMenuItemImage* pMoveForward = CCMenuItemImage::create("UI/forward_normal.png", "UI/forward_pressed.png");
    pMoveForward->setPosition(ccp(20 + pMoveForward->getContentSize().width/2, 10 + pMoveForward->getContentSize().height/2));
    m_LeftButton = pMoveForward;
    
    CCMenuItemImage* pMoveBack = CCMenuItemImage::create("UI/backward_normal.png", "UI/backward_pressed.png");
    pMoveBack->setPosition(ccp(pMoveForward->getPosition().x + pMoveForward->getContentSize().width/2 + pMoveBack->getContentSize().width/2, 10 + pMoveBack->getContentSize().height/2));
    m_RightButton = pMoveBack;

    m_RightButton->setTarget(this, entranceOnLeft ? menu_selector(PlayerMoveMenu::moveForward) : menu_selector(PlayerMoveMenu::moveBack));
    m_LeftButton->setTarget(this, entranceOnLeft ? menu_selector(PlayerMoveMenu::moveBack) : menu_selector(PlayerMoveMenu::moveForward));

    addChild(m_LeftButton);
    addChild(m_RightButton);
    
    //    updateButtonFunction();
    
    setPosition(ccp(0,0));
    setAnchorPoint(ccp(0,0));
    
    
    CCSize size = CCSizeMake(10 + pMoveForward->getContentSize().width + 10 + pMoveBack->getContentSize().width, pMoveBack->getContentSize().height < pMoveForward->getContentSize().height ? pMoveForward->getContentSize().height : pMoveBack->getContentSize().height);
    setContentSize(size);

    

}
