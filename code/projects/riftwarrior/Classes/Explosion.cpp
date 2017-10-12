//
//  Explosion.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/23/12.
//
//

#include "Explosion.h"
#include "ExplosionCache.h"
#include "MapHelper.h"
#include "GameScene.h"
#include "Function.h"


Explosion* Explosion::create(const char* animName, const CCPoint& mapPos)
{
    Explosion* pExplosion = ExplosionCache::getInstance()->getExplosion(animName);
    
    if (!pExplosion)
    {
        return NULL;
    }
    
    pExplosion->sprite->setPosition(mapPos);
    pExplosion->setAnchorPoint(CCPointZero);
    pExplosion->setPosition(CCPointZero);
    
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(mapPos, pMap, true);
    float orderZ = MapHelper::calculateOrderZ(ccp(tilePos.x+1, tilePos.y+1), pMap);
    
    pMap->addChild(pExplosion, orderZ);

    pExplosion->sprite->runAction(pExplosion->m_pAction);
    pExplosion->scheduleUpdate();

    return pExplosion;
}

void Explosion::initWithAnimationName(const char *animName, int level, float scale, bool repeat)
{
    char name[128] = {0};
    sprintf(name, "%s_%d_1.png", animName, level);
    bool hasSpecificLevel = true;
    
    CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
    if (!pFrame)
    {
        sprintf(name, "%s_1.png", animName);
        pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        hasSpecificLevel = false;
    }
    
    if (!pFrame)
    {
        return;
    }
    
    this->name = animName;
    
    sprite = CCSprite::create();
    sprite->retain();
    
    CCArray* frames = CCArray::createWithCapacity(16);

    for(int i=1;i<=24;++i)
    {
        if (hasSpecificLevel)
        {
            sprintf(name, "%s_%d_%d.png", animName, level, i);
        }
        else
        {
            sprintf(name, "%s_%d.png", animName, i);
        }
        pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        
        if (!pFrame)
        {
            break;
        }
        
        frames->addObject(pFrame);
    }
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.1f);
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    if (repeat)
    {
        m_pAction = CCRepeatForever::create(animate);
    }
    else
    {
        m_pAction = animate;
    }

    m_pAction->retain();
    
    sprite->setScale(scale);
    this->addChild(sprite);
}

void Explosion::initWithAnimation(const char *animFileName, int rows, int cols, int width, int height, float scale)
{
    CCTexture2D* pTexture = Function::tryGetCachedTexture(animFileName);
    if (!pTexture)
    {
        return;
    }

    name = animFileName;
    
    CCRect rect = CCRectMake(0, 0, width, height);
    
    CCArray* frames = CCArray::createWithCapacity(rows * cols);
    sprite = CCSprite::create();
    sprite->retain();
    
    if (sprite->initWithTexture(pTexture, rect))
    {
        for (int row = 0; row < rows; row++)
            for (int col = 0; col < cols; col++)
            {
                rect = CCRectMake(width * col, height * row, width-1, height-1);
                CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(pTexture, rect);
                frames->addObject(frame);
            }
        
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.1f);
        
        m_pAction = CCAnimate::create(animation);
        m_pAction->retain();
//        CCRepeatForever* repeat = CCRepeatForever::create(animate);
    }
    sprite->setScale(scale);
    this->addChild(sprite);
}

void Explosion::update(float dt)
{
    CCNode* parent = this->getParent();
    if (!parent)
    {
        ExplosionCache::getInstance()->addExplosion(this);
        return;
    }

    if (sprite->numberOfRunningActions() == 0)
    {
        parent->removeChild(this, true);
        ExplosionCache::getInstance()->addExplosion(this);
    }
    
    return;
}