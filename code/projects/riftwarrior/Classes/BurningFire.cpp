//
//  BurningFire.cpp
//  tdgame
//
//  Created by Like Zhang on 4/6/13.
//
//

#include "BurningFire.h"
#include "Building.h"
#include "GameData.h"
#include "GameScene.h"
#include "MapHelper.h"
#include "Player.h"

BurningFire* BurningFire::create(int buildingId, int level)
{
    BurningFire* fire = new BurningFire();
    if (fire && fire->init(buildingId, level))
    {
        fire->autorelease();
        return fire;
    }
    
    CC_SAFE_DELETE(fire);
    return NULL;
}

bool BurningFire::init(int buildingId, int level)
{
    //create animation
    m_pSprite = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("effect_burning_fire_1.png"));
    
    CCArray* frames = CCArray::createWithCapacity(4);
    
    char name[128] = {0};
    for(int i=0;i<16;++i)
    {
        sprintf(name, "effect_burning_fire_%d.png", i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.1f);
    CCAnimate* animate = CCAnimate::create(animation);
    CCActionInterval* action = CCRepeatForever::create(animate);
    
    m_Duration = 1.2 * level; // last 1.2 second for level 1 ...
    m_TimeElapsed = 0;
    
    
    // burning fire dps is constant: 10 * level
    m_Damage = 10 + 5 * (level-1);

    float targetSize = 1;
    m_pSprite->setScale(0.2f);
    m_pSprite->setColor(ccc3(255,255 - 80 * (level-1), 255 - 80 * (level-1)));
    m_pSprite->runAction(action);
    m_pSprite->runAction(CCSequence::create(CCFadeIn::create(0.5),
                                            CCDelayTime::create(m_Duration),
                                            CCFadeOut::create(0.5),
                                            NULL));
    m_pSprite->runAction(CCSequence::create(CCScaleTo::create(0.5, targetSize),
                                            CCDelayTime::create(m_Duration),
                                            CCScaleTo::create(0.5f, 0.2f),
                                            CCCallFunc::create(this, callfunc_selector(BurningFire::remove)),
                                            NULL));
    
    addChild(m_pSprite);
    m_pSprite->setAnchorPoint(ccp(0.5,0.4));
    
    scheduleUpdate();
    
    return true;
}


void BurningFire::remove()
{
    this->stopAllActions();
    this->removeFromParentAndCleanup(true);
}

void BurningFire::update(float dt)
{
    m_TimeElapsed+=dt;
    
    if (m_TimeElapsed < 0.5 || m_TimeElapsed > (m_Duration + 0.5))
    {
        return;
    }
    
    CCObject* pObj;
    CCArray* pEnemies = GameScene::getInstance()->sharedGameStage->enemies;
    
    CCARRAY_FOREACH(pEnemies, pObj)
    {
        Enemy* pEnemy = static_cast<Enemy*>(pObj);
        if (!pEnemy->isActive() || pEnemy->isDead())
        {
            continue;
        }
        
        CCPoint enemyMapPos = pEnemy->getCurrentMapPosition();
        
        float dx = m_CurrentMapPos.x - enemyMapPos.x;
        float dy = m_CurrentMapPos.y - enemyMapPos.y;
        
        dx = dx > 0 ? dx : -dx;
        dy = dy > 0 ? dy : -dy;
        
        float distance = MapHelper::MAP_TILE_LENGTH;
        
        if (dx < distance && dy < distance)
        {
            pEnemy->reduceHp(m_Damage * dt);  // burning enemy donesn't know who to attack.
        }
    }
    
}