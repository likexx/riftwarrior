//
//  CCSmartSprite.cpp
//  tdgame
//
//  Created by Like Zhang on 3/11/13.
//
//

#include "CCSmartSprite.h"
#include "GameScene.h"

CCSmartSprite* CCSmartSprite::create(const char* pFilename)
{
    CCSmartSprite* sprite = new CCSmartSprite();
    if (sprite && sprite->initWithFile(pFilename))
    {
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}


CCSmartSprite* CCSmartSprite::createWithTexture(cocos2d::CCTexture2D *pTexture)
{
    CCSmartSprite* sprite = new CCSmartSprite();
    if (sprite && sprite->initWithTexture(pTexture))
    {
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

CCSmartSprite* CCSmartSprite::createWithSpriteFrameName(const char *pSpriteFrameName)
{
    CCSmartSprite* sprite = new CCSmartSprite();
    if (sprite && sprite->initWithSpriteFrameName(pSpriteFrameName))
    {
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

CCSmartSprite* CCSmartSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    CCSmartSprite* sprite = new CCSmartSprite();
    if (sprite && sprite->initWithSpriteFrame(pSpriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void CCSmartSprite::addToGround()
{
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(this);
}

void CCSmartSprite::makeVisible()
{
    setVisible(true);
}

void CCSmartSprite::removeSelf()
{
    removeFromParentAndCleanup(true);
}