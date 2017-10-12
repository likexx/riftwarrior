//
//  RandomMap.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/23/13.
//
//

#include "RandomMap.h"
#include "HorizontalMap.h"
#include "VerticalMap.h"
#include "MapHelper.h"
#include "Function.h"
#include <time.h>
#include <algorithm>
#include <stack>

RandomMap* RandomMap::create(const char *tmxFile)
{
    RandomMap *pMap = NULL;
    pMap = new VerticalMap();
/*
    if (rand()%2 == 0)
    {
        pMap = new HorizontalMap();
    }
    else
    {
        pMap = new VerticalMap();
    }
*/    
    if (pMap->initWithTMXFile(tmxFile))
    {
        pMap->autorelease();
        pMap->initPaths();
        return pMap;
    }
    CC_SAFE_DELETE(pMap);
    return NULL;
}

void RandomMap::addChild(cocos2d::CCNode *child)
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

void RandomMap::addChild(cocos2d::CCNode *child, int zOrder)
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

void RandomMap::addChild(cocos2d::CCNode *child, int zOrder, int tag)
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

void RandomMap::initPaths()
{

    m_MinRoadTilePosition = getPositionPropertyValue("min_tile");
    m_MaxRoadTilePosition = getPositionPropertyValue("max_tile");

    m_MinMapTilePosition = getPositionPropertyValue("map_min_tile");
    m_MaxMapTilePosition = getPositionPropertyValue("map_max_tile");

    
    CCSize mapTileSize = this->getTileSize();
    
    m_TileDirectionMask.resize(mapTileSize.height);
    for(int i=0;i<mapTileSize.height;++i)
    {
        m_TileDirectionMask[i].resize(mapTileSize.width, TileNone);
    }

    initProperties();
    
    vector<CCPoint> startTiles = getStartTilePositions();
    
    m_EndTilePosition = createEndTilePosition(startTiles);

    return;
    
    size_t size = startTiles.size();
    
    for(int i=0; i<size; ++i)
    {
        CCPoint startTile = startTiles[i];
        createPath(startTile, m_EndTilePosition);
    }
    
    createPathTiles();
    createPropertyTiles();
    
    CCArray* frames = CCArray::createWithCapacity(4);

    char name[128] = {0};
    /*
    const char* pAnimationName = "sea_wave_light";
    for (int i=1;i<=8;++i)
    {
        sprintf(name, "tile_animation_%s_%d.png", pAnimationName, i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        frames->addObject(frame);
    }
    
    createTileAnimation("sea_wave_light", frames, ccp(13,21));
    */
    
    const char* pAnimationName = "wave2";
    for (int i=1;i<=24;++i)
    {
        sprintf(name, "stage_animation_%s_%d.png", pAnimationName, i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        frames->addObject(frame);
    }
    for (int i=24;i>=1;--i)
    {
        sprintf(name, "stage_animation_%s_%d.png", pAnimationName, i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            continue;
        }
        frames->addObject(frame);
    }
    
    createStageAnimation(pAnimationName, frames, ccp(13,21));
}


void RandomMap::initProperties()
{
    m_pPropertyLayer = this->layerNamed("property");
    m_pBackgroundLayer = this->layerNamed("background");
    
    CCSize mapSize = this->getMapSize();
    
    // 1st column for road gids
    for(int i=0;i< mapSize.height; ++i)
    {
        int gid = m_pPropertyLayer->tileGIDAt(ccp(0,i));
        if (gid < 1)
        {
            break;
        }
        m_RoadGids.push_back(gid);
    }
    
    // 2nd column for environment gids
    for(int i=0;i< mapSize.height; ++i)
    {
        int gid = m_pPropertyLayer->tileGIDAt(ccp(1,i));
        if (gid < 1)
        {
            break;
        }
        m_EnvironmentPropertyGids.push_back(gid);
    }
    
}

bool RandomMap::isInsidePlaceableArea(const cocos2d::CCPoint &tilePos)
{
    return (tilePos.x >= m_MinRoadTilePosition.x &&
            tilePos.x <= m_MaxRoadTilePosition.x &&
            tilePos.y >= m_MinRoadTilePosition.y &&
            tilePos.y <= m_MaxRoadTilePosition.y);
}


void RandomMap::createPath(const cocos2d::CCPoint &startTilePos, const cocos2d::CCPoint &endTilePos)
{
    
    m_pPropertyLayer = this->layerNamed("property");
    m_pGroundLayer = this->layerNamed("ground");
    
    queue<CCPoint> wayPoints = createRandomWayPoints(startTilePos, endTilePos);
    CCPoint prev = startTilePos;

    while(!wayPoints.empty())
    {
        CCPoint current = wayPoints.front();
        wayPoints.pop();
        connectWayPoints(prev, current);
        prev = current;
        
    }
    
    connectWayPoints(prev, endTilePos);
    
//    fillVerticalPath(ccp(8,12), ccp(8,15));
//    fillHorizontalPath(ccp(8,15), ccp(13,15));
//    connectWayPoints(ccp(13,15), ccp(15,14));
    
    //    pLayer->setTileGID(1, endTilePos);
    
}

void RandomMap::connectWayPoints(const cocos2d::CCPoint &pos1, const cocos2d::CCPoint &pos2)
{
    CCPoint p1 = pos1;
    CCPoint p2 = pos2;
    
    CCLog("connect %f,%f => %f,%f", p1.x, p1.y, p2.x, p2.y);
    
    if (p1.x == p2.x && p1.y == p2.y)
    {
        return;
    }
    
    if (p1.y == p2.y)
    {
        fillHorizontalPath(p1, p2);
        return;
    }
    
    if (p1.x == p2.x)
    {
        fillVerticalPath(p1, p2);
        return;
    }
    
    if (p1.y > p2.y)
    {
        //check if it's already been filled above
        bool goHorizontal = rand() & 0x01;
        bool goRight = p1.x < p2.x;
        
        
        if (!goHorizontal && p2.y == m_EndTilePosition.y)
        {
            if ( (goRight && p2.x > m_EndTilePosition.x) ||
                 (!goRight && p2.x < m_EndTilePosition.x) )
            {
                goHorizontal = true;
            }
        }

        goHorizontal = goHorizontal && !tileExists(goRight ? ccp(p1.x+1, p1.y) : ccp(p1.x-1,p1.y));
        
        if (goHorizontal || tileExists(ccp(p1.x, p1.y-1)) || tileExists(ccp(goRight ? (p2.x-1) : (p2.x+1), p2.y)))
        {
            CCPoint corner = ccp(p2.x, p1.y);
            fillHorizontalPath(p1, corner);
            fillVerticalPath(corner, p2);
            
            return;
        }
        CCPoint corner = ccp(p1.x, p2.y);
        fillVerticalPath(p1, corner);
        fillHorizontalPath(corner, p2);
        
        
        return;
        
    }

    if (p1.y < p2.y)
    {
        //check if it's already been filled below
        bool goHorizontal = rand() & 0x01;
        bool goRight = p1.x < p2.x;
        
        if (!goHorizontal && p2.y == m_EndTilePosition.y)
        {
            if ( (goRight && p2.x > m_EndTilePosition.x) ||
                (!goRight && p2.x < m_EndTilePosition.x) )
            {
                goHorizontal = true;
            }
        }

        goHorizontal = goHorizontal && !tileExists(goRight ? ccp(p1.x+1, p1.y) : ccp(p1.x-1,p1.y));
        
        if (goHorizontal || tileExists(ccp(p1.x, p1.y + 1)) || tileExists(ccp(goRight ? (p2.x - 1) : (p2.x+1), p2.y)))
        {
            CCPoint corner = ccp(p2.x, p1.y);
            fillHorizontalPath(p1, corner);
            fillVerticalPath(corner, p2);
            
            return;
        }
        CCPoint corner = ccp(p1.x, p2.y);
        fillVerticalPath(p1, corner);
        fillHorizontalPath(corner, p2);
        
        return;
        
    }
   
}

void RandomMap::fillHorizontalPath(cocos2d::CCPoint p1, cocos2d::CCPoint p2)
{
    CCLog("[%f, %f]=>[%f, %f]", p1.x, p1.y, p2.x, p2.y);
    bool goRight = p1.x < p2.x;
    
    CCPoint start = p1.x < p2.x ? p1 : p2;
    CCPoint end = p1.x < p2.x ? p2 : p1;
    
    for(int x = start.x; x <= end.x; ++x)
    {
        setTileMask(ccp(x, start.y), TileHorizontal);
    }
    
    // check p1
    if (m_TileDirectionMask[p1.y-1][p1.x] == TileVertical ||
        m_TileDirectionMask[p1.y-1][p1.x] == TileLeftTop ||
        m_TileDirectionMask[p1.y-1][p1.x] == TileRightTop)
    {
        setTileMask(p1, goRight ? TileLeftBottom : TileRightBottom);
    }
    else if (m_TileDirectionMask[p1.y+1][p1.x] == TileVertical ||
             m_TileDirectionMask[p1.y+1][p1.x] == TileLeftBottom ||
             m_TileDirectionMask[p1.y+1][p1.x] == TileRightBottom)
    {
        setTileMask(p1, goRight ? TileLeftTop : TileRightTop);
    }
}

void RandomMap::fillVerticalPath(cocos2d::CCPoint p1, cocos2d::CCPoint p2)
{
    CCLog("[%f, %f]=>[%f, %f]", p1.x, p1.y, p2.x, p2.y);

    bool goDown = p1.y < p2.y;
    CCPoint start = p1.y < p2.y ? p1 : p2;
    CCPoint end = p1.y < p2.y ? p2 : p1;
    
    for(int y = start.y; y <= end.y; ++y)
    {
        setTileMask(ccp(start.x, y), TileVertical);
    }
    
    // check p1
    if (m_TileDirectionMask[p1.y][p1.x-1] == TileHorizontal ||
        m_TileDirectionMask[p1.y][p1.x-1] == TileLeftBottom ||
        m_TileDirectionMask[p1.y][p1.x-1] == TileLeftTop)
    {
        setTileMask(p1, goDown ? TileRightTop : TileRightBottom);
    }
    else if (m_TileDirectionMask[p1.y][p1.x+1] == TileHorizontal ||
             m_TileDirectionMask[p1.y][p1.x+1] == TileRightTop ||
             m_TileDirectionMask[p1.y][p1.x+1] == TileRightBottom)
    {
        setTileMask(p1, goDown ? TileLeftTop : TileLeftBottom);
    }
    
}


bool RandomMap::tileExists(const cocos2d::CCPoint &tilePos)
{
//    return m_pGroundLayer->tileGIDAt(tilePos) == m_RoadTileGid;
    return m_TileDirectionMask[tilePos.y][tilePos.x] != TileNone;
}

void RandomMap::createPathTiles()
{
    CCSize mapTileSize = this->getTileSize();
    
    for(int i=0;i<mapTileSize.height;++i)
    {
        if (i<m_MinRoadTilePosition.y)
        {
            continue;
        }
        
        if (i>m_MaxRoadTilePosition.y)
        {
            break;
        }
        
        for(int j=0;j<mapTileSize.width;++j)
        {

            if (j<m_MinRoadTilePosition.x || j>m_MaxRoadTilePosition.x || TileNone == m_TileDirectionMask[i][j])
            {
                continue;
            }
            
            if (m_EndTilePosition.x == j && m_EndTilePosition.y == i)
            {
                continue;
            }
            
            // road should be located around the mapter center area, no need for boundary detection
            
            
            int gid = m_RoadGids[rand()%m_RoadGids.size()];
            /*
            switch(m_TileDirectionMask[i][j])
            {
                case TileHorizontal:
                    gid = m_RoadHorizonTileGid;
                    break;
                case TileVertical:
                    gid = m_RoadVerticalTileGid;
                    break;
                case TileLeftTop:
                    gid = m_RoadLeftTopTileGid;
                    break;
                case TileLeftBottom:
                    gid = m_RoadLeftBottomTileGid;
                    break;
                case TileRightTop:
                    gid = m_RoadRightTopTileGid;
                    break;
                case TileRightBottom:
                    gid = m_RoadRightBottomTileGid;
                    break;
            }
            */
            m_pGroundLayer->setTileGID(gid, ccp(j,i));
            
        }
    }
    
}

void RandomMap::createPropertyTiles()
{
    
    CCTMXLayer* pLayer = m_pPropertyLayer;

    vector<int> properties = m_EnvironmentPropertyGids;
    bool usingTileGid = true;
    
    CCString* value = pLayer->propertyNamed("properties");
    if (value)
    {
        vector<int> properties = Function::splitStringToIntegers(value->getCString(), ",");
        usingTileGid = false;
    }
    
    int max = (m_MaxRoadTilePosition.y - m_MinRoadTilePosition.y + 1) * (m_MaxRoadTilePosition.x - m_MinRoadTilePosition.x + 1);

    // map location indexes
    vector<int> indexes;
    for(int i=0;i<max;++i)
    {
        indexes.push_back(i);
    }
    
    std::random_shuffle(indexes.begin(), indexes.end());
    
    int areaWidth = m_MaxRoadTilePosition.x - m_MinRoadTilePosition.x + 1;
    
    CCSize mapSize = getTileSize();
//    int mapOffset = (m_MinRoadTilePosition.y - 1) * mapSize.width + m_MinRoadTilePosition.x;
    
    srand(time(NULL));

    // totoal number of properties
    int propertyNumber = 10 + rand() % (indexes.size()/4);
    
    for(int i=0;i<indexes.size() && i<propertyNumber;++i)
    {
        int offset = indexes[i];
        int row = offset / areaWidth;
        int col = offset % areaWidth;
        
        int realCol = m_MinRoadTilePosition.x + col;
        int realRow = m_MinRoadTilePosition.y + row;
        
        int propId = properties[rand()%properties.size()];
        
        if (m_TileDirectionMask[realRow][realCol] == TileNone)
        {
            if (!usingTileGid)
            {
                char path[128] = {0};
                sprintf(path, "tilemap/property/%d.png", propId);
                CCSprite* sprite = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage(path));
                CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(realCol + 0.5, realRow+0.5), this);
                sprite->setPosition(pos);
                sprite->setScale(this->getTileSize().width / sprite->getContentSize().width);
                addChild(sprite);
            }
            else
            {
                m_pGroundLayer->setTileGID(propId, ccp(realCol, realRow));
            }
        }
    }
}


void RandomMap::createTileAnimation(const char* pAnimationName, CCArray* frames, CCPoint tilePos)
{
    
    char name[128] = {0};
    sprintf(name, "tile_animation_%s_1.png", pAnimationName);
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName(name);
    
    CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(tilePos.x + 0.5, tilePos.y + 0.5), this);
    pSprite->setPosition(pos);
    addChild(pSprite);
    

    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.2 + (float)(rand()%5)/10.0f);
    CCAnimate* animate = CCAnimate::create(animation);
    CCActionInterval* pAnimation = CCRepeatForever::create(animate);
    
    pSprite->runAction(pAnimation);

}

void RandomMap::createStageAnimation(const char *pAnimationName, CCArray* frames, cocos2d::CCPoint tilePos)
{
    char name[128] = {0};
    sprintf(name, "stage_animation_%s_1.png", pAnimationName);
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName(name);
    
    CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(tilePos.x, tilePos.y), this);
    pSprite->setPosition(pos);
    addChild(pSprite);
    
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.2);
    CCAnimate* animate = CCAnimate::create(animation);
    CCActionInterval* pAnimation = CCRepeatForever::create(animate);
    
    pSprite->runAction(pAnimation);

}
