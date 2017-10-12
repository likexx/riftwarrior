//
//  UnitAttackComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 5/9/13.
//
//

#include "UnitAttackComponent.h"
#include "Player.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool UnitAttackComponent::init()
{
    if (!EnemyAttackComponent::init())
    {
        return false;
    }
    
    return true;
}


void UnitAttackComponent::update(float dt)
{
    EnemyAttackComponent::update(dt);
}


AttackableObject* UnitAttackComponent::findTarget()
{
    CCArray* enemies = GameScene::getInstance()->sharedGameStage->enemies;
    
    
    if (AttackableObject* pComputerPlayer = GameScene::getInstance()->sharedGameStage->getComputerPlayer())
    {
        if (!pComputerPlayer->isDead() &&
            isWithinAttackRange(pComputerPlayer->getAttackableMapPosition()))
        {
            return pComputerPlayer;
        }
    }
    
    if (Building* pBase = GameScene::getInstance()->sharedGameStage->getComputerBase())
    {
        if (isWithinAttackRange(pBase->getAttackableMapPosition()))
        {
            return pBase;
        }
    }
    
    CCObject* pTarget = NULL;
    CCARRAY_FOREACH(enemies, pTarget)
    {
        Enemy* enemy = static_cast<Enemy*>(pTarget);
        
        if (!enemy->isActive() || enemy->isDead())
        {
            continue;
        }
        
        CCPoint targetMapPos = enemy->getCurrentMapPosition();
        
        if (isWithinAttackRange(targetMapPos))
        {
            return enemy;
        }
        
    }
    
    return NULL;

}
