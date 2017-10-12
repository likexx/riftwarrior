//
//  StageMap.h
//  tdgame
//
//  Created by Like Zhang on 3/7/13.
//
//

#ifndef __tdgame__StageMap__
#define __tdgame__StageMap__

#include <iostream>
#include <cocos2d.h>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>

#include "enums.h"

using namespace cocos2d;
using namespace std;

class StageMap : public CCTMXTiledMap
{
public:
	StageMap()
		: m_pPropertyLayer(nullptr),
		m_pBackgroundLayer(nullptr),
		m_pGroundLayer(nullptr),
		m_pEnvironmentLayer(nullptr),
		m_pEnvironmentBuildingLayer(nullptr),
		m_pShadowLayer(nullptr) {}

    static StageMap* create(const char* tmxFile);
    
    virtual void initPaths();
    
    virtual void addChild(CCNode* child);
    virtual void addChild(cocos2d::CCNode *child, int zOrder);
    virtual void addChild(cocos2d::CCNode *child, int zOrder, int tag);
    
    void addToBackground(CCNode* child);
    
    virtual bool isInsidePlaceableArea(const CCPoint& tilePos);
    
    vector<CCPoint> getStartTilePositions();
    CCPoint getPositionPropertyValue(const char* name);
    
    bool isTargetAfterCurrentPostion(const cocos2d::CCPoint &currentTilePos, const cocos2d::CCPoint &targetTilePos);

    inline const CCPoint& getEndTilePosition()
    {
        return m_EndTilePosition;
    }
    
    inline const ENUM_TILE_DIRECTION getTileDirection(const CCPoint& tilePos)
    {
        return m_TileDirectionMask[(int)tilePos.y][(int)tilePos.x];
    }
    
    inline CCTMXLayer* getGroundLayer()
    {
        return m_pGroundLayer;
    }
    
    inline CCTMXLayer* getBackgroundLayer()
    {
        return m_pBackgroundLayer;
    }
    
    inline CCTMXLayer* getShadowLayer()
    {
        return m_pShadowLayer;
    }
    
    inline CCSprite* getMapSprite(const CCPoint& pos)
    {
        CCSprite* pSprite = m_pBackgroundLayer->tileAt(pos);
        if (!pSprite)
        {
            pSprite = m_pEnvironmentBuildingLayer->tileAt(pos);
        }
        if (!pSprite)
        {
            pSprite = m_pEnvironmentLayer->tileAt(pos);
        }
        
        return pSprite;
    }
    
    inline CCPoint getMapMinTilePosition()
    {
        return m_MinMapTilePosition;
    }
    
    inline CCPoint getMapMaxTilePosition()
    {
        return m_MaxMapTilePosition;
    }
    
    inline const vector<CCPoint>& getShadowTiles()
    {
        return m_ShadowTilePositions;
    }
    
    inline const int getMapIndex(const CCPoint& tilePos)
    {
        return  (int)tilePos.y * getMapSize().width + (int)tilePos.x;

    }
    
    inline bool isRoadTile(const CCPoint& tilePos)
    {
        if ((int)tilePos.x == m_EndTilePosition.x && (int)tilePos.y == m_EndTilePosition.y)
        {
            return true;
        }
        
        int index = getMapIndex(tilePos);
        
        return m_RoadTileIndexes.find(index) != m_RoadTileIndexes.end();
    }
    
    vector<CCPoint> getNeighborBuildingTiles(const CCPoint& tile, int range);
    
    int getSequenceByTile(const CCPoint& tilePos);
    
    CCPoint getTileBySequence(int sequence);
    
    inline CCPoint getFirstTimeLightWeaponPlaceTile()
    {
        return m_FirstTimeLightWeaponPlaceTile;
    }

    inline CCPoint getFirstTimeMissleWeaponPlaceTile()
    {
        return m_FirstTimeMissleWeaponPlaceTile;
    }

    inline CCPoint getFirstTimeMoveTarget()
    {
        return m_FirstTimeMoveTarget;
    }
    
    inline CCPoint getSecondTimeMoveTarget()
    {
        return m_SecondTimeMoveTarget;
    }
    
    
    inline unordered_set<int>& getPlaceableTiles()
    {
        return m_PlaceableTiles;
    }
    
    inline bool isTileValid(const CCPoint& pos)
    {
        CCSize mapSize = getMapSize();
        return pos.x >= 0 && pos.x <= mapSize.width && pos.y >= 0 && pos.y <= mapSize.height;
    }
    
    ENUM_NPC_ANIMATION getPlayerStandingDirection();
    
protected:
    
    
    virtual void initProperties();
    virtual void createPath(const CCPoint& startTilePos, const CCPoint& endTilePos);
    
    CCPoint createEndTilePosition(const vector<CCPoint>& startTiles);
    
    
    void setTileMask(const cocos2d::CCPoint &tilePos, ENUM_TILE_DIRECTION direction);
    
    inline bool isRoadTileGid(int gid)
    {
        return gid > 0;
//        return m_RoadTileGids.find(gid) != m_RoadTileGids.end();
    }
    
    CCTMXLayer* m_pPropertyLayer;
    CCTMXLayer* m_pBackgroundLayer;
    CCTMXLayer* m_pGroundLayer;
    CCTMXLayer* m_pEnvironmentLayer;
    CCTMXLayer* m_pEnvironmentBuildingLayer;
    CCTMXLayer* m_pShadowLayer;
    
    CCPoint m_EndTilePosition;
    CCPoint m_MinMapTilePosition;
    CCPoint m_MaxMapTilePosition;
    
    CCPoint m_FirstTimeLightWeaponPlaceTile; // for 1st time building placement
    CCPoint m_FirstTimeMissleWeaponPlaceTile;
    CCPoint m_FirstTimeMoveTarget; // for 1st time player move target
    CCPoint m_SecondTimeMoveTarget; // for 1st time player move target
    
    vector<CCPoint> m_StartTiles;
    vector<CCPoint> m_ShadowTilePositions;

    unordered_set<int> m_PlaceableTiles;
    
    map<int, int> m_RoadTileIndexes;
    size_t m_TotalTileCount;
    
    vector< vector<ENUM_TILE_DIRECTION> > m_TileDirectionMask;
    
    unordered_set<int> m_RoadTileGids;
    
    
};
#endif /* defined(__tdgame__StageMap__) */
