//
//  PerformEffectAnimation.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/15/13.
//
//

#include "PerformEffectAnimation.h"
#include "GameScene.h"
#include "Function.h"
#include "Magic.h"
#include "GameData.h"
#include "MapHelper.h"
#include "AttackComponent.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d::extension;
using namespace CocosDenshion;

void PerformEffectAnimation::loadEffects()
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    /*
    frameCache->addSpriteFramesWithFile("effects/lightening.plist");
    frameCache->addSpriteFramesWithFile("effects/meteor.plist");
    frameCache->addSpriteFramesWithFile("effects/lightening2.plist");
    frameCache->addSpriteFramesWithFile("effects/hit.plist");
    frameCache->addSpriteFramesWithFile("effects/moltenball.plist", "effects/moltenball.pvr.ccz");
     */
}

bool PerformEffectAnimation::init()
{
    if (!CCNode::create())
    {
        return false;
    }
    
    return true;
}

void PerformEffectAnimation::perform(int performSkillId, const cocos2d::CCPoint &pos)
{
    const Magic* pMagic = GameData::getMagic(performSkillId);
    
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();

    char buffer[256] = {0};
    sprintf(buffer, "%s_1.png", pMagic->performEffect.c_str());

    m_pSprite = Function::createAnimationSpriteWithImageFrames(buffer, pMagic->performEffect.c_str(), 20, 0.1f, performSkillId == 3 ? true : false);
    m_pSprite->setAnchorPoint(ccp(0.5, pMagic->offsetY));
//    m_pSprite->setPosition(ccp(0, -m_pSprite->getContentSize().height/3));
    
    this->addChild(m_pSprite);
    this->setPosition(pos);
    this->scheduleUpdate();
    this->setAnchorPoint(CCPointZero);
    
    pMap->addChild(this, 128);
    
}

void PerformEffectAnimation::update(float dt)
{
    totalTimeElapsed += dt;
    
    if (totalTimeElapsed > 0.6)
    {
        attackEnemies();
        
    }
    
    if (m_pSprite->numberOfRunningActions() == 0)
    {
        CCLog("remove skill performing");
        this->unscheduleUpdate();
        this->removeFromParentAndCleanup(true);
    }
}


void PerformEffectAnimation::attackEnemies()
{
    SimpleAudioEngine::sharedEngine()->playEffect("wav/explode_1.wav");
    
    AttackableObject* target = NULL;
    CCArray* targets = GameScene::getInstance()->sharedGameStage->enemies;
    
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    CCPoint currentMapPos = MapHelper::isoMapPosToOrthMapPos(this->getPosition());
    
    float rangePowerd = pow(60, 2);
    float attackRange = 60;
    
    CCObject* pTarget = NULL;
    CCARRAY_FOREACH(targets, pTarget)
    {
        AttackableObject* pTargetToAim = static_cast<AttackableObject*>(pTarget);
        
        if (!pTargetToAim->isActive())
        {
            continue;
        }
        
        CCPoint targetPos = pTargetToAim->getAttackablePosition();
        
        const CCPoint& targetMapPos = MapHelper::isoMapPosToOrthMapPos(targetPos);
        
        //            const CCPoint& targetMapPos = pTargetToAim->getAttackableMapPosition();
        
        if (AttackComponent::isWithinAttackRange(currentMapPos, targetMapPos, attackRange, rangePowerd))
        {
            pTargetToAim->reduceHp(5);
            
            if (pTargetToAim->isDead())
            {
                pTargetToAim->destroy();
            }
        }
        
    }

}
