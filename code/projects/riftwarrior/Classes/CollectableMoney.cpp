//
//  CollectableMoney.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/14/12.
//
//

#include "CollectableMoney.h"
#include "Player.h"
#include "MapHelper.h"
#include "GameScene.h"

bool CollectableMoney::init()
{
    
    sprite = CCSprite::create("peach.png");
    
    this->setContentSize(sprite->getContentSize());
    
    this->addChild(sprite);


    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

//    scheduleUpdate();
    
    return true;
}

bool CollectableMoney::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

    CCPoint screenLocation = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    CCPoint touchGroundPos = this->convertToNodeSpace(screenLocation);

    float dx = touchGroundPos.x - sprite->getPosition().x;
    float dy = touchGroundPos.y - sprite->getPosition().y;
    float d = sqrt(dx*dx + dy*dy);
    
    CCLog("touch d = %f", d);
    if (d < sprite->getContentSize().width / 2)
    {
        onClick(NULL);
        return true;
    }
    
    return false;
}

void CollectableMoney::onClick(cocos2d::CCObject *pSender)
{
  
    if (collected)
    {
        return;
    }
    
//    Player::getInstance()->updateMoney(10);
    
    if (sprite->numberOfRunningActions() > 0)
    {
        sprite->stopAllActions();
    }

    /*
    CCPoint targetPos = GameScene::getInstance()->sharedStatusMenu->getPosition();
    CCPoint targetRelativePos = ccpSub(targetPos, getPosition());
    
    collected = true;
    
    sprite->runAction((CCActionInterval*)(CCSequence::create(CCMoveTo::create(0.5, targetRelativePos), CCCallFunc::create(this, callfunc_selector(CollectableMoney::remove)), NULL)));

    Player::getInstance()->gotoNextTutorialStep(NULL);
    */
}

void CollectableMoney::remove()
{
    this->unscheduleUpdate();
    sprite->stopAllActions();
    this->removeFromParentAndCleanup(true);
    
}

void CollectableMoney::update(float dt)
{
    /*
    currentLifeTime += dt;
    
    if (currentLifeTime > maxLifeSeconds)
    {
        remove();
        return;
    }
    
    if (!collected)
    {
        if (GameScene::getInstance()->sharedGameStage->moved)
        {
            // update position
            Building* building = GameScene::getInstance()->sharedGameStage->getBuildingManager()->getBuilding(buildingTileIndex);
            
            CCPoint newPos = ccpAdd(building->getPosition(), offsetToBuilding);
            
            this->setPosition(newPos);
        }

        if (maxLifeSeconds - currentLifeTime < 5)
        {
            if (sprite->numberOfRunningActions() == 0)
            {
                CCFadeOut* fadeOut = CCFadeOut::create(0.4);
                CCFadeIn* fadeIn = CCFadeIn::create(0.4);
            
                sprite->runAction(CCRepeatForever::create((CCActionInterval*)(CCSequence::create(fadeOut, fadeIn, NULL))));
            }
        }
    }
    */
}