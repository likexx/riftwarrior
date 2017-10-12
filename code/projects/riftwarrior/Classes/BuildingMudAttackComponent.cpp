//
//  BuildingMudAttackComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 4/4/13.
//
//

#include "BuildingMudAttackComponent.h"
#include "Enemy.h"
#include "Building.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Explosion.h"
#include "Player.h"

BuildingMudAttackComponent::~BuildingMudAttackComponent()
{

}

bool BuildingMudAttackComponent::init()
{
    
    bool result = BuildingAttackComponent::init();
    
    return result;
}

CCActionInterval* BuildingMudAttackComponent::createBulletAction(cocos2d::CCPoint currentMapPos, cocos2d::CCPoint targetMapPos, cocos2d::CCPoint targetPos, float mapSpeed)
{
    return NULL;
}


void BuildingMudAttackComponent::update(float dt)
{
    if (GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    // for rotating attack
    Building* building = static_cast<Building*>(this->getParent());
    
    CCArray* enemies = GameScene::getInstance()->sharedGameStage->enemies;
    
    CCObject* pTarget = NULL;
    
    CCPoint currentBuildingTilePos = building->getOriginTilePosition();
    
    CCARRAY_FOREACH(enemies, pTarget)
    {
        Enemy* enemy = static_cast<Enemy*>(pTarget);
/*
        if (rand()%100 > (10 + building->getLevel() * 10))
        {
            continue;
        }
*/        
        if (!enemy->isActive() || enemy->isDead())
        {
            continue;
        }
        
        CCPoint targetTilePos = enemy->getCurrentTilePosition();
        

        if ((int)targetTilePos.x < currentBuildingTilePos.x - 1 ||
            (int)targetTilePos.x > currentBuildingTilePos.x + 1 ||
            (int)targetTilePos.y < currentBuildingTilePos.y - 1 ||
            (int)targetTilePos.y > currentBuildingTilePos.y + 1)
        {
            continue;
        }
        
        bool isWithinRange = false;
        for(int i=currentBuildingTilePos.y-1;i<=currentBuildingTilePos.y+1;++i)
        {
            for(int j=currentBuildingTilePos.x-1;j<=currentBuildingTilePos.x+1;++j)
            {
                if (building->getLevel()<2)
                {
                    if (i!=currentBuildingTilePos.y &&
                        j!=currentBuildingTilePos.x)
                    {
                        continue;
                    }
                }
                
                if ((int)targetTilePos.x == j && (int)targetTilePos.y == i)
                {
                    isWithinRange = true;
                    break;
                }
            }
        }
        
        if (!isWithinRange)
        {
            continue;
        }
        
        float value = 0.5 + (building->getLevel() - 1) * 0.1;
            
        enemy->setStatus(ENUM_BUFF_STATUS_SLOWDOWN, value, 0.2f);
   
    }

}

