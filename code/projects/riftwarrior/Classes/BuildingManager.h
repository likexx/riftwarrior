//
//  BuildingManager.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/8/12.
//
//

#ifndef __ike_tdgame__BuildingManager__
#define __ike_tdgame__BuildingManager__

#include <iostream>
#include "cocos2d.h"
#include "Building.h"
#include "StageMap.h"
using namespace cocos2d;

class BuildingManager
{
public:
    BuildingManager(StageMap* pMap);
    virtual ~BuildingManager();
/*
    virtual void update(float dt);
    virtual void draw();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
*/
    void showBuildingToPlace(int buildingId, CCPoint tilePos);
    void moveBuildingToPlace(const CCPoint& pos);
    bool hasUnplacedBuilding();
    void removeBuildingToPlace();

    inline Building* getUnplacedBuilding()
    {
        return pBuildingToPlace;
    }

    bool hasBuilding(int x, int y);

    bool removeBuilding(int x, int y);
    Building* getBuilding(int x, int y);
    
    bool isPlaceable(int x, int y, int id);
    bool placeBuilding(int x, int y, int id, bool forceToPlace = false, bool isPlayerBuilding = true);
    
    void removeAnyBuildingMenu();
    
    CCDictionary* getBuildingTable();
    inline CCArray* getAllBuildings()
    {
        return buildingArray;
    }
    
    inline int getTotalBuildingsCreated()
    {
        return m_TotalBuildingCreated;
    }
    
    
    inline int getTotalBuildingDestroyed()
    {
        return m_TotalBuildingDestroyed;
    }
    
    inline void adjustTotalBuildingDestroyed(int value)
    {
        // used for upgrading to new building.
        // removing the old one, and we should compensate by 1
        m_TotalBuildingDestroyed += value;
    }
    
    void updateBuildingToPlace();
    void unselectAllBuildings();
    void removeAllBuildings();

private:
    
//    void updateBuildingPositions(CCArray* buildings);
    
    
    StageMap* pGroundMap;
    CCDictionary* buildingTable;
    CCArray* buildingArray;
    
    Building* pBuildingToPlace;
    
    vector<Building*> buildingMap;
    
    
    int m_TotalBuildingCreated;
    int m_TotalBuildingDestroyed;

};

#endif /* defined(__ike_tdgame__BuildingManager__) */
