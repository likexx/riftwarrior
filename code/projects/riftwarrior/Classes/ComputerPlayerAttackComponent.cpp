//
//  ComputerPlayerAttackComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 5/6/13.
//
//

#include "ComputerPlayerAttackComponent.h"
#include "Player.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool ComputerPlayerAttackComponent::init()
{
    if (!EnemyAttackComponent::init())
    {
        return false;
    }
    
    return true;
}


void ComputerPlayerAttackComponent::update(float dt)
{
    EnemyAttackComponent::update(dt);
}


AttackableObject* ComputerPlayerAttackComponent::findTarget()
{
    AttackableObject* target = EnemyAttackComponent::findTarget();
    if (target)
    {
        return target;
    }
    
    CCArray* units = GameScene::getInstance()->sharedGameStage->units;
    
    CCObject* pTarget = NULL;
    CCARRAY_FOREACH(units, pTarget)
    {
        Unit* unit = static_cast<Unit*>(pTarget);
        
        if (!unit->isActive() || unit->isDead())
        {
            continue;
        }
        
        CCPoint targetMapPos = unit->getCurrentMapPosition();
        
        if (isWithinAttackRange(targetMapPos))
        {
            return unit;
        }
        
    }
    
    return NULL;
}
