//
//  MapHelper.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/5/12.
//
//

#ifndef __ike_tdgame__MapHelper__
#define __ike_tdgame__MapHelper__

#include <iostream>
#include "cocos2d.h"
#include <vector>
using namespace cocos2d;
using namespace std;

//
// Helper to convert 3 different positions:
// 1. ground position: location on the ground layer (in pixels). (0,0) is left-bottom
// 2. map position: orthogonal relative position on map (in pixels). (0,0) is the position of tile (0,0) from top-left
// 3. tile position(index): (x,y) is the tile at column x, row y
// 4. screen position: absolute position on screen. without zoom, it's the same as ground postion (relative to ground layer)
//
class MapHelper
{
public:
    static const float MAP_TILE_LENGTH;

    ///
    /// map screen location to tile position (indexes)
    ///
//    static CCPoint groundPosToTilePos(const CCPoint& groundPos, CCTMXTiledMap* pTileMap, bool usingFloat = true);
    
    ///
    /// map tile indexes (e.g. [0,0], [0, 1]) to screen locations
    ///
//    static CCPoint tilePosToGroundPos(float x, float y, CCTMXTiledMap* pTileMap);
    
    static CCPoint tilePosToGroundMapPos(const CCPoint& tilePos);
    static CCPoint tilePosToGroundMapPos(const CCPoint& tilePos, CCTMXTiledMap* pTileMap);
    static CCPoint tilePosToStagePos(const CCPoint& tilePos);
    
    static CCPoint tilePosToMapPos(float x, float y, CCTMXTiledMap* pTileMap);
    
    static CCPoint isoMapPosToOrthMapPos(const CCPoint& pos);
    static CCPoint orthMapPosToIsoMapPos(const CCPoint& pos);
    
    ///
    /// convert map orthogonal position (relative to map) to screen position
//    static CCPoint mapPosToGroundPos(float x, float y, CCTMXTiledMap* pTileMap);
    static CCPoint mapPosToTilePos(const CCPoint& mapPos, CCTMXTiledMap* pTileMap, bool usingFloat = true);
    
    static CCPoint groundMapPosToTilePos(const CCPoint& pos, CCTMXTiledMap* pTileMap, bool usingFloat);
    static CCPoint mapPosToGroundMapPos(const cocos2d::CCPoint &mapPos);

    
    ///
    /// convert screen pos to map orthogonal position
//    static CCPoint groundPosToMapPos(const CCPoint& groundPos, CCTMXTiledMap* pTileMap);

//    static CCPoint screenPosToGroundPos(const CCPoint& screenPos);
    
//    static CCPoint getMapPosFromGround(const CCPoint& groundPos);
    
    ///
    /// update z order
    static float calculateOrderZ(const CCPoint& tilePos, CCTMXTiledMap* pTileMap);
    
    
};

#endif /* defined(__ike_tdgame__MapHelper__) */
