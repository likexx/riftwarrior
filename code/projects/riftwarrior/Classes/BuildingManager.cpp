//
//  BuildingManager.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/8/12.
//
//

#include "BuildingManager.h"
#include "MapHelper.h"
#include "GameScene.h"
#include "Building.h"
#include "SimpleAudioEngine.h"
#include "Player.h"

BuildingManager::BuildingManager(StageMap* pMap)
:pGroundMap(pMap),pBuildingToPlace(NULL)
{
    CCSize size = pGroundMap->getMapSize();
 
    m_TotalBuildingDestroyed = 0;
    m_TotalBuildingCreated = 0;
    
//    buildingMap = new int[(int)(size.width * size.height)];
    buildingMap.resize(size.width * size.height, NULL);
//    memset(buildingMap, 0, sizeof(int)* size.width * size.height);
    
    buildingTable = new CCDictionary();
    buildingTable->retain();
    buildingArray = new CCArray();
    buildingArray->retain();

//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

    // building layer is updated after ground layer manually since it's touch priority is higher
//    this->scheduleUpdate();
}

BuildingManager::~BuildingManager()
{
    removeAllBuildings();
    
    if (buildingTable)
    {
        buildingTable->release();
    }
    
    if (buildingArray)
    {
        buildingArray->release();
    }
}


void BuildingManager::removeAllBuildings()
{
    CCObject* pObj = nullptr;
    CCARRAY_FOREACH(buildingArray, pObj)
    {
        Building* pBuilding = static_cast<Building*>(pObj);
		pBuilding->remove();
    }
    buildingArray->removeAllObjects();
}

bool BuildingManager::hasBuilding(int x, int y)
{
    CCSize size = pGroundMap->getMapSize();
    int index = Building::getBuildingTileIndex(x, y);
  
    if (index < 0)
    {
        return false;
    }
    
    return buildingMap[index] != NULL;
//    CCObject* currentBuilding = this->buildingTable->objectForKey(index);
    
//    return currentBuilding != NULL;
    
}

bool BuildingManager::removeBuilding(int x, int y)
{
    int index = Building::getBuildingTileIndex(x, y);

    Building* pBuilding = static_cast<Building*>(buildingTable->objectForKey(index));
    
    if (!pBuilding)
    {
        return false;
    }
    
    buildingTable->removeObjectForKey(index);
    buildingArray->removeObject(pBuilding);

    CCSize buildingSize = pBuilding->getTileSize();
    
    for(int m = x; m< x + buildingSize.width; ++m)
        for(int n = y; n < y + buildingSize.height; ++n)
        {
            // fill occupied tiles
            int index = Building::getBuildingTileIndex(m, n);
            buildingMap[index] = NULL;
        }

    pBuilding->removeFromParentAndCleanup(true);
//    pGroundMap->removeChild(pBuilding, true);
//    this->removeChild(building, true);
    
    m_TotalBuildingDestroyed++;
    
    return true;

}

bool BuildingManager::isPlaceable(int x, int y, int id)
{
    if (!pGroundMap->isInsidePlaceableArea(ccp(x, y)) ||
        hasBuilding(x, y))
    {
        return false;
    }
    
    if (0 == (Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON))
    {
        // if still in tutorial
        // place to target position
        CCPoint expectTilePos = GameScene::getInstance()->sharedGameStage->getGroundMap()->getFirstTimeLightWeaponPlaceTile();
        if (x != expectTilePos.x || y != expectTilePos.y)
        {
            return false;
        }
    }
    
    
    int cost = GameData::getBuildingSetting(id).baseCost;
    if (Player::getInstance()->getMoney() < cost)
    {
        return false;
    }

    return true;
}

bool BuildingManager::placeBuilding(int x, int y, int id, bool forceToPlace, bool isPlayerBuilding)
{
   if(!forceToPlace && !isPlaceable(x, y, id))
   {
       return false;
   }
    
    int index = Building::getBuildingTileIndex(x, y);

    Building* pBuilding = Building::create(id, x, y, false);

    buildingTable->setObject(pBuilding, index);
    buildingMap[index] = pBuilding;
    
    pGroundMap->addChild(pBuilding);
    pGroundMap->reorderChild(pBuilding, pBuilding->getOrderZ());
//    this->addChild(building);
//    this->reorderChild(building, building->getOrderZ());
    
    CCSize buildingSize = pBuilding->getTileSize();
    
    for(int m = x; m < x + buildingSize.width; ++m)
        for(int n = y; n < y + buildingSize.height; ++n)
        {
            // fill occupied tiles
            int index = Building::getBuildingTileIndex(m, n);
            buildingMap[index] = pBuilding;
        }
    
    if (pBuilding->isBaseVisible())
    {
        pBuilding->getBaseSprite()->setOpacity(0);
        pBuilding->getBaseSprite()->runAction(CCFadeTo::create(0.9f, 255));
    }
    pBuilding->getRotatingPart()->setOpacity(0);
    pBuilding->getRotatingPart()->runAction(CCFadeTo::create(0.9f, 255));
    pBuilding->runAction(CCSequence::create(CCDelayTime::create(1.0f), CCCallFunc::create(pBuilding, callfunc_selector(Building::activate)), NULL));
//    pBuilding->activate();
    pBuilding->showConstructionAnimation();
    
    buildingArray->addObject(pBuilding);
    
    pBuilding->setPlayerBuilding(isPlayerBuilding);
    
    m_TotalBuildingCreated++;

    return true;
}

Building* BuildingManager::getBuilding(int x, int y)
{
    int index = Building::getBuildingTileIndex(x, y);
    
    if (index < 0)
    {
        return NULL;
    }
    
    Building* building = static_cast<Building*>(buildingTable->objectForKey(index));
    return building;
    /*
    CCArray* keys = buildingTable->allKeys();
    if (!keys)
    {
        return NULL;
    }
    
    for(int i=0; i < keys->count(); ++i)
    {
        CCInteger* key = (CCInteger*)keys->objectAtIndex(i);
        int index = key->getValue();
        if (index == buildingTileIndex)
        {
            Building* building = static_cast<Building*>(buildingTable->objectForKey(index));
            return building;
        }
    }

    return NULL;
    */
}

CCDictionary* BuildingManager::getBuildingTable()
{
    return buildingTable;
}

void BuildingManager::unselectAllBuildings()
{
    CCArray* keys = buildingTable->allKeys();
    if (!keys)
    {
        return;
    }
    
    for (size_t i=0; i < keys->count(); ++i)
    {
        CCInteger* key = (CCInteger*)keys->objectAtIndex(i);
        int index = key->getValue();
        Building* building = static_cast<Building*>(buildingTable->objectForKey(index));
        building->unselect();
    }
}

/*
void BuildingManager::updateBuildingPositions(CCArray* buildings)
{
    
    if (GameScene::getInstance()->sharedGameStage->moved)
    {
        CCSize mapSize = pTileMap->getMapSize();

        for(int i=0; i < buildings->count(); ++i)
        {
            CCInteger* key = (CCInteger*)buildings->objectAtIndex(i);
            int index = key->getValue();
            Building* building = static_cast<Building*>(buildingTable->objectForKey(index));
            
            int x = (int)(index/(int)mapSize.height);
            int y = index - x * mapSize.height;
            
            float vertexZ = MapHelper::getVertexZ(ccp(x,y), pTileMap);
            CCPoint oldPos = building->getPosition();
            CCLog("pos: %f, %f", oldPos.x, oldPos.y);
            CCPoint pos = MapHelper::tilePosToGroundPos(x, y, pTileMap);
            building->setPosition(ccp(pos.x, pos.y));
            this->reorderChild(building, vertexZ);
            
        }
        
    }
}
*/

void BuildingManager::showBuildingToPlace(int buildingId, CCPoint tilePos)
{
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
//    CCPoint groundCenter = MapHelper::screenPosToGroundPos(ccp(screenSize.width/2, screenSize.height/2));
//    CCPoint centerTile = MapHelper::groundPosToTilePos(groundCenter, pGroundMap, false);
    
//    CCPoint mapPos = pGroundMap->convertToNodeSpace(screenPos);
  
//    CCPoint centerTile = MapHelper::groundMapPosToTilePos(mapPos, pGroundMap, false);
    
    pBuildingToPlace = Building::create(buildingId, tilePos.x, tilePos.y, true);

    pBuildingToPlace->select();
    
    pGroundMap->addChild(pBuildingToPlace, 128);
    
//    this->addChild(pBuildingToPlace);
    

}

void BuildingManager::removeAnyBuildingMenu()
{
    CCObject* pObj;
    CCARRAY_FOREACH(buildingArray, pObj)
    {
        Building* pBuilding = static_cast<Building*>(pObj);
        pBuilding->removeBuildingMenu();
    }
}

void BuildingManager::moveBuildingToPlace(const cocos2d::CCPoint &pos)
{
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(pos, pGroundMap, false);
    pBuildingToPlace->moveToTile(tilePos);
}

bool BuildingManager::hasUnplacedBuilding()
{
    return pBuildingToPlace != NULL;
}

void BuildingManager::removeBuildingToPlace()
{
    if (!pBuildingToPlace)
    {
        return;
    }
    
  
    pGroundMap->removeChild(pBuildingToPlace, true);
//    this->removeChild(pBuildingToPlace, true);
    pBuildingToPlace = NULL;

}

void BuildingManager::updateBuildingToPlace()
{
    if (!pBuildingToPlace)
    {
        return;
    }
    
    CCPoint tilePos = pBuildingToPlace->getOriginTilePosition();
    
    
    int cost = pBuildingToPlace->getBaseCost();
    if (pGroundMap->isInsidePlaceableArea(tilePos) &&
        !hasBuilding(tilePos.x, tilePos.y) &&
        Player::getInstance()->getMoney() >= cost)
    {
        if (pBuildingToPlace->isUnplaceable())
        {
            pBuildingToPlace->setColor(ccc3(128,128,196));
            pBuildingToPlace->setUnplaceable(false);
        }
    }
    else
    {
        if (!pBuildingToPlace->isUnplaceable())
        {
//            pBuildingToPlace->setColor(ccc3(255, 0, 0));
            pBuildingToPlace->setUnplaceable(true);
        }
    }

}


