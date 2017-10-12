//
//  EnemyShootAttack.cpp
//  tdgame
//
//  Created by Like Zhang on 4/29/13.
//
//

#include "EnemyShootAttack.h"
#include "Enemy.h"
#include "Function.h"
#include "MapHelper.h"
#include "Explosion.h"

bool SkeletonArrowAttack::init()
{
    return EnemyAttackComponent::init();
}


CCActionInterval* SkeletonArrowAttack::createAction(float duration, const CCPoint& targetPos)
{
    CCActionInterval* action = CCMoveTo::create(duration, targetPos);
    
    return action;
}

CCPoint SkeletonArrowAttack::getBulletPositon()
{
    
    Enemy* pEnemy = static_cast<Enemy*>(getParent());
    
    float xOffsets[] = { 247, 250, 206, 79, 131};
    float yOffsets[] = { 119, 80,  58, 120, 49};
    
    return Function::calculateFirePosition(pEnemy->getAnimationSprite(), m_Quadrant, xOffsets, yOffsets);
}



bool TurtleThrowStoneAttack::init()
{
    return EnemyAttackComponent::init();
}


CCActionInterval* TurtleThrowStoneAttack::createAction(float duration, const CCPoint& targetPos)
{
    CCActionInterval* action = CCJumpTo::create(duration, targetPos, MapHelper::MAP_TILE_LENGTH * 2, 1);
    
    return action;
}

CCPoint TurtleThrowStoneAttack::getBulletPositon()
{
    
    Enemy* pEnemy = static_cast<Enemy*>(getParent());
    
    float xOffsets[] = { 176, 120, 63, 45, 124};
    float yOffsets[] = { 116, 63,  48, 166, 196};
    
    return Function::calculateFirePosition(pEnemy->getAnimationSprite(), m_Quadrant, xOffsets, yOffsets);
}


bool SnakeVenomAttack::init()
{
    return EnemyAttackComponent::init();
}


CCActionInterval* SnakeVenomAttack::createAction(float duration, const CCPoint& targetPos)
{
    CCActionInterval* action = CCMoveTo::create(duration, targetPos);
    
    return action;
}

CCPoint SnakeVenomAttack::getBulletPositon()
{
    
    Enemy* pEnemy = static_cast<Enemy*>(getParent());
    
    float xOffsets[] = { 295, 272, 188, 118, 195};
    float yOffsets[] = { 135, 85,  60, 183, 216};
    
    return Function::calculateFirePosition(pEnemy->getAnimationSprite(), m_Quadrant, xOffsets, yOffsets);
}

void SnakeVenomAttack::showExplosion(cocos2d::CCNode *pBullet)
{
    Explosion::create("hit_effect_venom", pBullet->getPosition());

}

// spider

bool SpiderWebAttack::init()
{
    return EnemyAttackComponent::init();
}


CCActionInterval* SpiderWebAttack::createAction(float duration, const CCPoint& targetPos)
{
    CCActionInterval* action = CCMoveTo::create(duration, targetPos);
    
    return action;
}

CCPoint SpiderWebAttack::getBulletPositon()
{
    
    Enemy* pEnemy = static_cast<Enemy*>(getParent());
    
    float xOffsets[] = { 219, 163, 112, 41, 113};
    float yOffsets[] = { 95, 59,  18, 148, 175};
    
    return Function::calculateFirePosition(pEnemy->getAnimationSprite(), m_Quadrant, xOffsets, yOffsets);
}

void SpiderWebAttack::showExplosion(cocos2d::CCNode *pBullet)
{
//    Explosion::create("hit_effect_spider_web", pBullet->getPosition());
    
}





