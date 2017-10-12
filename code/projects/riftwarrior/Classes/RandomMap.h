//
//  RandomMap.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/23/13.
//
//

#ifndef __ike_tdgame__RandomMap__
#define __ike_tdgame__RandomMap__

#include <iostream>
#include "cocos2d.h"
#include "StageMap.h"
#include <vector>
#include <queue>
#include "enums.h"
using namespace cocos2d;
using namespace std;



class RandomMap : public StageMap
{
public:
    static RandomMap* create(const char* tmxFile);

    virtual void initPaths();

    virtual void addChild(CCNode* child);
    virtual void addChild(cocos2d::CCNode *child, int zOrder);
    virtual void addChild(cocos2d::CCNode *child, int zOrder, int tag);

    virtual bool isInsidePlaceableArea(const CCPoint& tilePos);
    
protected:

    virtual queue<CCPoint> createRandomWayPoints(const CCPoint& startTilePos, const CCPoint& endTilePos) = 0;

    virtual void initProperties();
    virtual void createPath(const CCPoint& startTilePos, const CCPoint& endTilePos);
    
    void connectWayPoints(const CCPoint& pos1, const CCPoint& pos2);

    void fillHorizontalPath(CCPoint p1, CCPoint p2);
    void fillVerticalPath(CCPoint p1, CCPoint p2);

    bool tileExists(const CCPoint& tilePos);
    
    void createPathTiles();
    void createPropertyTiles();
    void createTileAnimation(const char* pAnimationName, CCArray* frames, CCPoint tilePos);
    void createStageAnimation(const char* pAnimationName, CCArray* frames, CCPoint tilePos);

    CCPoint m_MinRoadTilePosition;
    CCPoint m_MaxRoadTilePosition;
    
    vector<int> m_RoadGids;
    vector<int> m_EnvironmentPropertyGids;
};

#endif /* defined(__ike_tdgame__RandomMap__) */
