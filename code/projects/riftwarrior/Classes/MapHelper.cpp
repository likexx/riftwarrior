//
//  MapHelper.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/5/12.
//
//

#include "MapHelper.h"
#include "GameScene.h"
#include "GameStage.h"
#include "Function.h"

// Note: we should define the map tile size by ourself (just imaging how big the
//       tile should be in real world. When calculating mapping "map position", it's
//       whatever u define. TMX map "Tile Width/Height" is the image sprite size, not
//       waht u want in the square map tile.
/*
CCPoint MapHelper::groundPosToTilePos(const CCPoint& screenPos, CCTMXTiledMap* pTileMap, bool usingFloat)
{
    
    CCPoint pos = ccpSub(screenPos, pTileMap->getPosition());
    
    CCSize mapSize = pTileMap->getMapSize();
    CCSize tileSize = pTileMap->getTileSize();
    
    float halfMapWidth = mapSize.width * 0.5f;
    float mapHeight = mapSize.height;
    float tileWidth = tileSize.width / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = tileSize.height / CC_CONTENT_SCALE_FACTOR();
    
    CCPoint tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileHeight);
    float mapHeightDiff = mapHeight - tilePosDiv.y;
    
    float tileX = mapHeightDiff + tilePosDiv.x - halfMapWidth;
    float tileY = mapHeightDiff - tilePosDiv.x + halfMapWidth;
    
    tileX = MAX(0, tileX);
    tileX = MIN(mapSize.width - 1, tileX);
    tileY = MAX(0, tileY);
    tileY = MIN(mapSize.height - 1, tileY);
    
    return usingFloat ? CCPointMake(tileX, tileY) : ccp((int)tileX, (int)tileY);
}



CCPoint MapHelper::tilePosToGroundPos(float x, float y, CCTMXTiledMap *pTileMap)
{
    
    CCSize mapSize = pTileMap->getMapSize();
    CCSize tileSize = pTileMap->getTileSize();
    float halfMapWidth = mapSize.width * 0.5f;
    float mapHeight = mapSize.height;
    float tileWidth = tileSize.width / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = tileSize.height / CC_CONTENT_SCALE_FACTOR();
    
    float posX = tileWidth * (x - y + 2 * halfMapWidth) / 2;
    float posY = tileHeight * (2 * mapHeight - x - y) / 2;
    
    CCPoint temp = CCPointMake(posX, posY);
    
    return ccpAdd(temp, pTileMap->getPosition());
    
}
*/

CCPoint MapHelper::groundMapPosToTilePos(const CCPoint& pos, CCTMXTiledMap* pTileMap, bool usingFloat)
{
    
    CCSize mapSize = pTileMap->getMapSize();
    CCSize tileSize = pTileMap->getTileSize();
    
    float halfMapWidth = mapSize.width * 0.5f;
    float mapHeight = mapSize.height;
    float tileWidth = tileSize.width / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = tileSize.height / CC_CONTENT_SCALE_FACTOR();
    
    CCPoint tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileHeight);
    float mapHeightDiff = mapHeight - tilePosDiv.y;
    
    float tileX = mapHeightDiff + tilePosDiv.x - halfMapWidth;
    float tileY = mapHeightDiff - tilePosDiv.x + halfMapWidth;
    /*
    tileX = MAX(0, tileX);
    tileX = MIN(mapSize.width - 1, tileX);
    tileY = MAX(0, tileY);
    tileY = MIN(mapSize.height - 1, tileY);
    */
    return usingFloat ? CCPointMake(tileX, tileY) : ccp((int)tileX, (int)tileY);
}

CCPoint MapHelper::tilePosToGroundMapPos(const cocos2d::CCPoint &tilePos)
{
    CCTMXTiledMap* pTileMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
 
    float x = tilePos.x;
    float y = tilePos.y;
    
    CCSize mapSize = pTileMap->getMapSize();
    CCSize tileSize = pTileMap->getTileSize();
    float halfMapWidth = mapSize.width * 0.5f;
    float mapHeight = mapSize.height;
    float tileWidth = tileSize.width / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = tileSize.height / CC_CONTENT_SCALE_FACTOR();
    
    float posX = tileWidth * (x - y + 2 * halfMapWidth) / 2;
    float posY = tileHeight * (2 * mapHeight - x - y) / 2;

    return ccp (posX, posY);
    
}

CCPoint MapHelper::tilePosToGroundMapPos(const cocos2d::CCPoint &tilePos, CCTMXTiledMap* pTileMap)
{
    
    float x = tilePos.x;
    float y = tilePos.y;
    
    CCSize mapSize = pTileMap->getMapSize();
    CCSize tileSize = pTileMap->getTileSize();
    float halfMapWidth = mapSize.width * 0.5f;
    float mapHeight = mapSize.height;
    float tileWidth = tileSize.width / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = tileSize.height / CC_CONTENT_SCALE_FACTOR();
    
    float posX = tileWidth * (x - y + 2 * halfMapWidth) / 2;
    float posY = tileHeight * (2 * mapHeight - x - y) / 2;
    
    return ccp (posX, posY);
    
}

CCPoint MapHelper::tilePosToStagePos(const cocos2d::CCPoint &tilePos)
{
    CCTMXTiledMap* pTileMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    CCPoint groundMapPos = tilePosToGroundMapPos(tilePos);
    CCPoint worldPos = pTileMap->convertToWorldSpace(groundMapPos);
    
    return GameScene::getInstance()->sharedGameStage->convertToNodeSpace(worldPos);
    
//    return ccpAdd(pTileMap->getPosition(), tilePosToGroundMapPos(tilePos));
}

CCPoint MapHelper::tilePosToMapPos(float x, float y, cocos2d::CCTMXTiledMap *pTileMap)
{
    CCSize tileSize = pTileMap->getTileSize();
    float tileWidth = MAP_TILE_LENGTH / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = MAP_TILE_LENGTH / CC_CONTENT_SCALE_FACTOR();

    return CCPointMake(x * tileWidth, y * tileHeight);
}

CCPoint MapHelper::isoMapPosToOrthMapPos(const cocos2d::CCPoint &pos)
{
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    CCPoint tilePos = groundMapPosToTilePos(pos, pMap, true);
    
    return tilePosToMapPos(tilePos.x, tilePos.y, pMap);
    
}

CCPoint MapHelper::orthMapPosToIsoMapPos(const cocos2d::CCPoint &pos)
{
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint tilePos = mapPosToTilePos(pos, pMap);
    
    return tilePosToGroundMapPos(tilePos);
    
}

CCPoint MapHelper::mapPosToTilePos(const cocos2d::CCPoint &mapPos, cocos2d::CCTMXTiledMap *pTileMap, bool usingFloat)
{
    CCSize tileSize = pTileMap->getTileSize();
    float tileWidth =  MAP_TILE_LENGTH / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = MAP_TILE_LENGTH / CC_CONTENT_SCALE_FACTOR();
    
    return usingFloat ? CCPointMake(mapPos.x / tileWidth, mapPos.y / tileHeight) : ccp((int)(mapPos.x / tileWidth), (int)(mapPos.y / tileHeight));
    
}

/*
CCPoint MapHelper::groundPosToMapPos(const cocos2d::CCPoint &screenPos, cocos2d::CCTMXTiledMap *pTileMap)
{
    /*
    CCPoint pos = ccpSub(screenPos, pTileMap->getPosition());
    
    CCSize mapSize = pTileMap->getMapSize();
    CCSize tileSize = pTileMap->getTileSize();
    
    float halfMapWidth = mapSize.width * 0.5f;
    float mapHeight = mapSize.height;
    float tileWidth = mapTileLength / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = mapTileLength / CC_CONTENT_SCALE_FACTOR();
    
    CCPoint tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileHeight);
    float mapHeightDiff = mapHeight - tilePosDiv.y;
    
    float tileX = mapHeightDiff + tilePosDiv.x - halfMapWidth;
    float tileY = mapHeightDiff - tilePosDiv.x + halfMapWidth;
    
    return CCPointMake(tileX*tileWidth, tileY*tileHeight);
     */
    /*
    CCPoint tilePos = groundPosToTilePos(screenPos, pTileMap);
    return tilePosToMapPos(tilePos.x, tilePos.y, pTileMap);
}
*/

/*
CCPoint MapHelper::mapPosToGroundPos(float x, float y, cocos2d::CCTMXTiledMap *pTileMap)
{
    /*
    CCSize tileSize = pTileMap->getTileSize();
    float tileWidth = mapt / CC_CONTENT_SCALE_FACTOR();
    float tileHeight = tileSize.height / CC_CONTENT_SCALE_FACTOR();

    float tileX = x / tileWidth;
    float tileY = y / tileHeight;
    
    CCPoint pos = tilePosToScreenPos(tileX, tileY, pTileMap);
    return pos;
     */
/*
    CCPoint tilePos = mapPosToTilePos(ccp(x,y), pTileMap);
    return tilePosToGroundPos(tilePos.x, tilePos.y, pTileMap);
}
*/

CCPoint MapHelper::mapPosToGroundMapPos(const cocos2d::CCPoint &mapPos)
{
    CCTMXTiledMap* pTileMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint tilePos = mapPosToTilePos(mapPos, pTileMap);
    return tilePosToGroundMapPos(tilePos);
}

/*
CCPoint MapHelper::screenPosToGroundPos(const cocos2d::CCPoint &screenPos)
{
    // ground layer never move, only zoom in/out
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    // 1. get screen center
    CCPoint stagePos = CCPointZero;
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint screenCenter = CCPointMake(screenSize.width/2, screenSize.height/2);

    // left up corner
    float dx = screenPos.x - screenCenter.x;
    float dy = screenPos.y - screenCenter.y;
        
    stagePos.x = screenCenter.x + dx/pStage->getScale();
    stagePos.y = screenCenter.y + dy/pStage->getScale();
    
    return stagePos;
}
*/

float MapHelper::calculateOrderZ(const cocos2d::CCPoint &tilePos, cocos2d::CCTMXTiledMap *pTileMap)
{
//    float lowestZ = -(pTileMap->getMapSize().width + pTileMap->getMapSize().height);
    float currentZ = tilePos.x + tilePos.y;
    
    return currentZ - 1.5f;
}

/*
CCPoint MapHelper::getMapPosFromGround(const cocos2d::CCPoint &groundPos)
{
    return ccpSub(groundPos, GameScene::getInstance()->sharedGameStage->getGroudMap()->getPosition());
}
*/
/*
const char* MapHelper::getRoadLayerProperty(const char *key)
{
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCTMXLayer* road = pMap->layerNamed("property");
    
    CCDictionary* props = road->getProperties();
    
    CCString* data = static_cast<CCString*>(props->objectForKey(key));

    return data->getCString();

}
*/

float const MapHelper::MAP_TILE_LENGTH = 30.0f;

