//
//  StageMap.cpp
//  tdgame
//
//  Created by Like Zhang on 3/7/13.
//
//

#include "StageMap.h"
#include "Function.h"
#include "MapHelper.h"
#include "Environment/FishingRaft.h"
#include "Environment/SeaWave.h"
#include "Environment/Sunshine.h"
#include "Environment/CaveLight.h"
#include "AnimationManager.h"

StageMap* StageMap::create(const char *tmxFile)
{
    StageMap *pMap = NULL;
    pMap = new StageMap();


    if (pMap->initWithTMXFile(tmxFile))
    {
        pMap->autorelease();
        pMap->initPaths();
        return pMap;
    }
    CC_SAFE_DELETE(pMap);
    return NULL;
}

void StageMap::addToBackground(cocos2d::CCNode *child)
{
    CCTMXTiledMap::addChild(child, m_pBackgroundLayer->getZOrder());
}

void StageMap::addChild(cocos2d::CCNode *child)
{
    if (m_pGroundLayer)
    {
        CCTMXTiledMap::addChild(child, m_pGroundLayer->getZOrder());
    }
    else
    {
        CCTMXTiledMap::CCNode::addChild(child);
    }
}

void StageMap::addChild(cocos2d::CCNode *child, int zOrder)
{
    if (m_pGroundLayer)
    {
        CCTMXTiledMap::addChild(child, m_pGroundLayer->getZOrder() + zOrder);
    }
    else
    {
        CCTMXTiledMap::addChild(child, zOrder);
    }
}

void StageMap::addChild(cocos2d::CCNode *child, int zOrder, int tag)
{
    if (m_pGroundLayer)
    {
        CCTMXTiledMap::addChild(child, m_pGroundLayer->getZOrder() + zOrder, tag);
    }
    else
    {
        CCTMXTiledMap::addChild(child, zOrder, tag);
    }
}

void StageMap::initPaths()
{
    CCSize mapSize = getMapSize();
    
    CCTMXLayer* pLayer = this->layerNamed("property");
    CCString* pEnvironmentValue = pLayer->propertyNamed("environment");
    
    CCSize mapTileSize = this->getTileSize();
    
    m_TileDirectionMask.resize(mapTileSize.height);
    for(int i=0;i<mapTileSize.height;++i)
    {
        m_TileDirectionMask[i].resize(mapTileSize.width, TileNone);
    }
    
    initProperties();
    
    vector<CCPoint> startTiles = getStartTilePositions();
    
    m_EndTilePosition = createEndTilePosition(startTiles);
    
    m_TotalTileCount = 0;

	for (const CCPoint& startTile : startTiles)
    {
        createPath(startTile, m_EndTilePosition);
    }
    
    // remove property layer (not needed anymore)
    removeChild(m_pPropertyLayer, true);
    m_pPropertyLayer = NULL;

    const char* layersToRemove[] = {"road", "building_area"};
    size_t size = sizeof(layersToRemove)/sizeof(layersToRemove[0]);
    for(size_t i=0;i<size;++i)
    {
        CCNode* pLayer = layerNamed(layersToRemove[i]);
        if (pLayer)
        {
            removeChild(pLayer, true);
        }
        
    }

    
    // add environment animation
    if (pEnvironmentValue)
    {
        //vector<int> values = Function::splitStringToIntegers(pEnvironmentValue->getCString(), ",");
        vector<string> values = Function::splitString(pEnvironmentValue->getCString(), ",");
        
        for(size_t i=0;i<values.size();i+=3)
        {
            int animationId = atoi(values[i].c_str());
            CCPoint startTilePos = ccp(atof(values[i+1].c_str()), atof(values[i+2].c_str()));
            
            switch(animationId)
            {
                case 1:
                {
                    FishingRaft* pAnimation = FishingRaft::create();
                    pAnimation->startAnimation(startTilePos, this);
                }
                    break;
                case 2:
                {
                    SeaWave* pAnimation = SeaWave::create();
                    pAnimation->startAnimation(1, startTilePos, this);
                }
                    break;
                case 3:
                {
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_MAP101_SEAWAVE1);
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                case 4:
                {
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_MAP101_SEAWAVE2);
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                case 5:
                {
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_MAP101_SEAWAVE3);
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                case 6:
                {
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_MAP101_SEAWAVE4);
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                case 7:
                {
                    Sunshine* pAnimation = Sunshine::create();
                    pAnimation->startAnimation(1, startTilePos, this);
                }
                    break;

                case 10201:
                case 10202:
                {
                    int offset = animationId - 10201;
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation((ENUM_ENVIRONMENT_ANIMATION_TYPE)((int)ENUM_ENVIRONMENT_ANIMATION_MAP102_RABBIT + offset));
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                case 10203:
                {
                    CaveLight* pAnimation = CaveLight::create();
                    pAnimation->startAnimation(startTilePos, this, 102);
                }
                    break;

                case 10501:
                {
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_MAP105_WATERFALL1);
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;

                case 10502:
                {
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_MAP105_WATERFALL2);
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;

                case 10503:
                {
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_MAP105_WATERFALL3);
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                case 10504:
                case 10505:
                case 10506:
                case 10507:
                case 10508:
                {
                    int offset = animationId - 10504;
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation((ENUM_ENVIRONMENT_ANIMATION_TYPE)((int)ENUM_ENVIRONMENT_ANIMATION_MAP105_WAVE1 + offset));
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                case 10509:
                {
                    CaveLight* pAnimation = CaveLight::create();
                    pAnimation->startAnimation(startTilePos, this, 105);
                }
                    break;
                    
                case 10601:
                case 10602:
                case 10603:
                {
                    int offset = animationId - 10601;
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation((ENUM_ENVIRONMENT_ANIMATION_TYPE)((int)ENUM_ENVIRONMENT_ANIMATION_MAP106_FOUNTAIN + offset));
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;

                case 10801:
                case 10802:
                case 10803:
                case 10804:
                case 10805:
                {
                    int offset = animationId - 10801;
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation((ENUM_ENVIRONMENT_ANIMATION_TYPE)((int)ENUM_ENVIRONMENT_ANIMATION_MAP108_WATERFALL1 + offset));
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;

                case 11001:
                {
                    int offset = animationId - 11001;
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation((ENUM_ENVIRONMENT_ANIMATION_TYPE)((int)ENUM_ENVIRONMENT_ANIMATION_MAP110_LIGHTENING + offset));
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                
                case 10901:
                case 10902:
                case 10903:
                case 10904:
                case 10905:
                {
                    int offset = animationId - 10901;
                    CCSprite* pAnimation = AnimationManager::getInstance()->createEnvironmentAnimation((ENUM_ENVIRONMENT_ANIMATION_TYPE)((int)ENUM_ENVIRONMENT_ANIMATION_MAP109_SEAWEED1 + offset));
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(startTilePos, this);
                    pAnimation->setPosition(pos);
                    this->addChild(pAnimation);
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    
    return;
    

}

void StageMap::initProperties()
{
    m_pPropertyLayer = this->layerNamed("property");
    m_pBackgroundLayer = this->layerNamed("background");
    m_pGroundLayer = this->layerNamed("ground");
    m_pEnvironmentBuildingLayer = layerNamed("environment_building");
    m_pEnvironmentLayer = layerNamed("environment");
    m_pShadowLayer = layerNamed("shadow");
    
    m_MinMapTilePosition = getPositionPropertyValue("map_min_tile");
    m_MaxMapTilePosition = getPositionPropertyValue("map_max_tile");

    
    CCTMXLayer* pBuildingAreaLayer = layerNamed("building_area");
    
    CCSize mapSize = this->getMapSize();

    
    
    CCString* value = m_pPropertyLayer->propertyNamed("start_tiles");
    vector<pair<float, float> > valuePairs = Function::splitStringToValuePairs(value->getCString(), ",");
    
    // the following 4 properties only show up in the 1st map for tutorial purpose
    if (m_pPropertyLayer->propertyNamed("light_weapon_placement"))
    {
        m_FirstTimeLightWeaponPlaceTile = getPositionPropertyValue("light_weapon_placement");
    }

    
    if (m_pPropertyLayer->propertyNamed("missle_weapon_placement"))
    {
        m_FirstTimeMissleWeaponPlaceTile = getPositionPropertyValue("missle_weapon_placement");
    }

    
    if (m_pPropertyLayer->propertyNamed("first_time_player_move_target"))
    {
        m_FirstTimeMoveTarget = getPositionPropertyValue("first_time_player_move_target");
    }

    if (m_pPropertyLayer->propertyNamed("second_time_player_move_target"))
    {
        m_SecondTimeMoveTarget = getPositionPropertyValue("second_time_player_move_target");
    }


	for (const auto& valuePair : valuePairs)
    {
		m_StartTiles.push_back(ccp(valuePair.first, valuePair.second));
    }
    
    if (m_pShadowLayer)
    {
        
        for(int y=0;y<mapSize.height;++y)
        {
            for (int x=0;x<mapSize.width;++x)
            {
                CCPoint pos = ccp(x,y);
                
                if (m_pShadowLayer->tileAt(pos))
                {
                    m_ShadowTilePositions.push_back(pos);
                }
            }
        }
    }

    for(int i=0;i< mapSize.height; ++i)
    {
        int gid = m_pPropertyLayer->tileGIDAt(ccp(0,i));
        if (gid < 1)
        {
            break;
        }
        m_RoadTileGids.insert(gid);
    }

    // get placeable building area
    for(int i=0;i< mapSize.height; ++i)
    {
        for(int j=0;j<mapSize.width;++j)
        {
            int gid = pBuildingAreaLayer->tileGIDAt(ccp(j,i));
            if (gid<1)
            {
                continue;
            }
            
            int index = i*mapSize.width + j;
            m_PlaceableTiles.insert(index);
        }
    }

}

void StageMap::setTileMask(const cocos2d::CCPoint &tilePos, ENUM_TILE_DIRECTION direction)
{
    //    m_pGroundLayer->setTileGID(m_RoadTileGid, tilePos);
    m_TileDirectionMask[tilePos.y][tilePos.x] = direction;
    
    CCSize mapSize = getMapSize();
    int index = tilePos.y * mapSize.width + tilePos.x;
    m_RoadTileIndexes[index] = ++m_TotalTileCount;
}

int StageMap::getSequenceByTile(const cocos2d::CCPoint &tilePos)
{
    int index = (int)tilePos.y * getMapSize().width + (int)tilePos.x;
    
    map<int, int>::iterator it = m_RoadTileIndexes.find(index);
    
    if (it==m_RoadTileIndexes.end())
    {
        return 0;
    }
    
    return it->second;
    
}

vector<CCPoint> StageMap::getNeighborBuildingTiles(const cocos2d::CCPoint &tile, int range)
{
    vector<CCPoint> result;
    
    for(int i=-range;i<=range;++i)
    {
        for (int j=-range; j<=range; ++j)
        {
            if (abs(i) + abs(j) > range)
            {
                continue;
            }
            
            CCPoint current = ccp((int)tile.x+j, (int)tile.y+i);
            
            if (isTileValid(current) && isInsidePlaceableArea(current))
            {
                result.push_back(current);
            }
            
        }
    }
    
    return result;
}


CCPoint StageMap::getTileBySequence(int sequence)
{
    map<int, int>::iterator it = m_RoadTileIndexes.begin();
    
    bool found = false;
    while (it!=m_RoadTileIndexes.end())
    {
        if (it->second == sequence)
        {
            found = true;
            break;
        }
        ++it;
    }
    
    if (!found)
    {
        return CCPointZero;
    }
    
    return ccp((it->first)%(int)getMapSize().width, (it->first)/(int)getMapSize().width);
}

vector<CCPoint> StageMap::getStartTilePositions()
{

    return m_StartTiles;
}

CCPoint StageMap::createEndTilePosition(const vector<CCPoint>& startTiles)
{
    CCTMXLayer* pLayer = this->layerNamed("property");
    CCString* value = pLayer->propertyNamed("end_tile");
    vector<pair<float, float> > valuePairs = Function::splitStringToValuePairs(value->getCString(), ",");
    pair<int, int> data = valuePairs[0];
    return ccp(data.first, data.second);
    
}

void StageMap::createPath(const cocos2d::CCPoint &startTilePos, const cocos2d::CCPoint &endTilePos)
{
    const int goRight = 1;
    const int goLeft = 2;
    const int goUp = 3;
    const int goDown = 4;
    
    CCTMXLayer* pLayer = layerNamed("road");
    if (!pLayer)
    {
        pLayer = m_pBackgroundLayer;
    }
    
    CCPoint current = startTilePos;
    int prevDirection = 0;
    

    if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x-1, current.y))))
    {
        setTileMask(current, TileHorizontal);
        prevDirection = goLeft;
    }
    else if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x+1, current.y))))
    {
        setTileMask(current, TileHorizontal);
        prevDirection = goRight;
    }
    if (isRoadTileGid( pLayer->tileGIDAt(ccp(current.x, current.y+1)) ))
    {
        setTileMask(current, TileVertical);
        prevDirection = goDown;
    }
    else if (isRoadTileGid( pLayer->tileGIDAt(ccp(current.x, current.y-1)) ))
    {
        setTileMask(current, TileVertical);
        prevDirection = goUp;
    }
    
    while (current.x != endTilePos.x || current.y != endTilePos.y)
    {

        CCPoint prevPos = current;

        switch (prevDirection)
        {
            case goUp:
                if (isRoadTileGid( pLayer->tileGIDAt(ccp(current.x, current.y-1)) ))
                {
                    setTileMask(current, TileVertical);
                    prevDirection = goUp;
                    current.y--;
                }
                else if (isRoadTileGid( pLayer->tileGIDAt(ccp(current.x - 1, current.y)) ))
                {
                    setTileMask(current, TileRightTop);
                    prevDirection = goLeft;
                    current.x--;
                }
                else if (isRoadTileGid( pLayer->tileGIDAt(ccp(current.x + 1, current.y)) ))
                {
                    setTileMask(current, TileLeftTop);
                    prevDirection = goRight;
                    current.x++;
                }
                break;
            case goDown:
                if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x, current.y+1))))
                {
                    setTileMask(current, TileVertical);
                    prevDirection = goDown;
                    current.y++;
                }
                else if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x - 1, current.y))))
                {
                    setTileMask(current, TileRightBottom);
                    prevDirection = goLeft;
                    current.x--;
                }
                else if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x + 1, current.y))))
                {
                    setTileMask(current, TileLeftBottom);
                    prevDirection = goRight;
                    current.x++;
                }
                break;
            case goLeft:
                if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x-1, current.y))))
                {
                    setTileMask(current, TileHorizontal);
                    prevDirection = goLeft;
                    current.x--;
                }
                else if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x, current.y+1))))
                {
                    setTileMask(current, TileLeftTop);
                    prevDirection = goDown;
                    current.y++;
                }
                else if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x, current.y-1))))
                {
                    setTileMask(current, TileLeftBottom);
                    prevDirection = goUp;
                    current.y--;
                }
                break;
            case goRight:
                if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x+1, current.y))))
                {
                    setTileMask(current, TileHorizontal);
                    prevDirection = goRight;
                    current.x++;
                }
                else if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x, current.y+1))))
                {
                    setTileMask(current, TileRightTop);
                    prevDirection = goDown;
                    current.y++;
                }
                else if (isRoadTileGid(pLayer->tileGIDAt(ccp(current.x, current.y-1))))
                {
                    setTileMask(current, TileRightBottom);
                    prevDirection = goUp;
                    current.y--;
                }
                break;
        }
        
        if (prevPos.x == current.x && prevPos.y == current.y)
        {
            break;
        }
    }
    
    if (current.x == endTilePos.x && current.y == endTilePos.y)
    {
        if (prevDirection == goDown || prevDirection == goUp)
        {
            setTileMask(current, TileVertical);
        }
        else
        {
            setTileMask(current, TileHorizontal);
        }
    }
}

bool StageMap::isTargetAfterCurrentPostion(const cocos2d::CCPoint &currentMapPos, const cocos2d::CCPoint &targetMapPos)
{
    CCPoint currentTilePos = MapHelper::mapPosToTilePos(currentMapPos, this);
    CCPoint targetTilePos = MapHelper::mapPosToTilePos(targetMapPos, this);
    int currentIndex = getSequenceByTile(currentTilePos);
    int targetIndex = getSequenceByTile(targetTilePos);
    
    if (currentIndex != targetIndex)
    {
        return currentIndex < targetIndex;
    }
    
    return false;
}


CCPoint StageMap::getPositionPropertyValue(const char *name)
{
    CCTMXLayer* pLayer = this->layerNamed("property");
    CCString* value = pLayer->propertyNamed(name);
    
    vector<pair<float, float> > valuePairs = Function::splitStringToValuePairs(value->getCString(), ",");
    pair<float, float> data = valuePairs[0];
    return ccp(data.first, data.second);
    
}

bool StageMap::isInsidePlaceableArea(const CCPoint& tilePos)
{

    CCSize mapSize = getMapSize();
    int index = tilePos.y * mapSize.width + tilePos.x;
    
    return m_PlaceableTiles.find(index) != m_PlaceableTiles.end();
}

ENUM_NPC_ANIMATION StageMap::getPlayerStandingDirection()
{
    ENUM_TILE_DIRECTION upperTile = getTileDirection(ccp(m_EndTilePosition.x, m_EndTilePosition.y-1));
    ENUM_TILE_DIRECTION lowerTile = getTileDirection(ccp(m_EndTilePosition.x, m_EndTilePosition.y+1));
    ENUM_TILE_DIRECTION leftTile = getTileDirection(ccp(m_EndTilePosition.x-1, m_EndTilePosition.y));
    ENUM_TILE_DIRECTION rightTile = getTileDirection(ccp(m_EndTilePosition.x+1, m_EndTilePosition.y));
    
    
    if (lowerTile == TileVertical || lowerTile == TileLeftBottom || lowerTile == TileRightBottom)
    {
        return ENUM_NPC_ANIMATION_STAND_DOWN;
    }
    else if (leftTile == TileHorizontal || leftTile == TileLeftBottom || leftTile == TileLeftTop)
    {
        return ENUM_NPC_ANIMATION_STAND_LEFT;
    }
    else if (rightTile == TileHorizontal || rightTile == TileRightBottom || rightTile == TileRightTop)
    {
        return ENUM_NPC_ANIMATION_STAND_RIGHT;
    }
    
    return ENUM_NPC_ANIMATION_STAND_UP;
}
