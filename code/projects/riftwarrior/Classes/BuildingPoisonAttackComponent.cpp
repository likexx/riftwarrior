//
//  BuildingPoisonAttackComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 4/18/13.
//
//

#include "BuildingPoisonAttackComponent.h"
#include "Building.h"
#include "GameScene.h"

BuildingPoisonAttackComponent::~BuildingPoisonAttackComponent()
{
    
}

bool BuildingPoisonAttackComponent::init()
{
    
    bool result = BuildingMudAttackComponent::init();
    
    return result;
}

CCActionInterval* BuildingPoisonAttackComponent::createBulletAction(cocos2d::CCPoint currentMapPos, cocos2d::CCPoint targetMapPos, cocos2d::CCPoint targetPos, float mapSpeed)
{
    return NULL;
}


void BuildingPoisonAttackComponent::update(float dt)
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
        
        float value = 4 + (building->getLevel()-1)*2;
        
        enemy->setStatus(ENUM_BUFF_STATUS_POISON, value, building->getLevel());
        
    }
    
}

