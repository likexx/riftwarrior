//
//  StageMaskLayer.cpp
//  tdgame
//
//  Created by Like Zhang on 3/5/13.
//
//

#include "StageShadowLayer.h"
#include "GameScene.h"
#include "RandomMap.h"
#include "Building.h"
#include "MapHelper.h"

bool StageShadowLayer::init()
{
 
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    m_MapSize = pMap->getMapSize();
    
    this->setAnchorPoint(ccp(0,0));
    this->setPosition(ccp(0,0));
    
    m_TileOpacities.resize(m_MapSize.width * m_MapSize.height, 255);
    m_TilePositions.resize(m_MapSize.width * m_MapSize.height * 4);
    

    
    CCSpriteBatchNode* pBatchNode = CCSpriteBatchNode::create("tilemap/shadow.png");
    pBatchNode->setAnchorPoint(CCPointZero);
    
    pBatchNode->setContentSize(GameScene::getInstance()->sharedGameStage->getGroundMap()->getContentSize());
    
    for(int y=0;y<m_MapSize.height;++y)
    {
        for(int x=0;x<m_MapSize.width;++x)
        {
            CCPoint tilePos = ccp(x,y);
            CCPoint pos = MapHelper::tilePosToStagePos(tilePos);
            
            for(float i = 0; i < 1; i+=0.25)
            {
                for(float j=0;j<1;j+=0.25)
                {
                    CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(tilePos.x + j, tilePos.y + i));
                    CCSprite* pSprite = CCSprite::create("tilemap/shadow.png");
                    pSprite->setAnchorPoint(ccp(0.5,1));
                    pSprite->setPosition(pos);
                    pSprite->setScale(2);

                    pBatchNode->addChild(pSprite);
                }
            }
            
        }
    }

    addChild(pBatchNode);

    //scheduleUpdate();
    
    return true;
}

void StageShadowLayer::setTileOpacity(int x, int y, int value)
{
    
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCSize mapSize = pMap->getMapSize();

    for (int i = y-2; i<=y+2;++i)
    {
        for (int j=x-2;j<=x+2;++j)
        {

            if (i<0 || i>=mapSize.height || j<0 || j>=mapSize.width)
            {
                continue;
            }

            int index = i * mapSize.width + j;
            
            int opacity = m_TileOpacities[index];

            if (opacity>0)
            {
                if (i == y && j==x)
                {
                    opacity -= 96;
                }
                else if ((i==y && abs(j-x)==1) || (j==x && abs(i-y)==1))
                {
                    opacity -=80;
                }
                else if (abs(i-y)==1 && abs(j-x)==1)
                {
                    opacity -= 64;
                }
                else if ((i==y && abs(j-x)==2) || (j==x && abs(i-y)==2))
                {
                    opacity -=48;
                }
                else
                {
                    opacity -=32;
                }
            }
            
            if (opacity <0)
            {
                opacity = 0;
            }
            
            m_TileOpacities[index] = opacity;
            
        }
    }
    
}


void StageShadowLayer::update(float dt)
{
    
    std::fill(m_TileOpacities.begin(), m_TileOpacities.end(), 255);
    
    BuildingManager* pBuildingManager = GameScene::getInstance()->sharedGameStage->getBuildingManager();
    
    if (pBuildingManager->hasUnplacedBuilding())
    {
        Building* pBuilding = pBuildingManager->getUnplacedBuilding();
        CCPoint tilePos = pBuilding->getOriginTilePosition();
        setTileOpacity(tilePos.x, tilePos.y, 255);
    }
    
    
    for(int y=0;y<m_MapSize.height;++y)
    {
        for(int x=0;x<m_MapSize.width;++x)
        {
            int index = y*m_MapSize.width + x;
            CCPoint tilePos = ccp(x,y);
            m_TilePositions[index*4] = MapHelper::tilePosToStagePos(tilePos);
            
            m_TilePositions[index*4 + 1] = MapHelper::tilePosToStagePos(ccp(tilePos.x+1, tilePos.y));
            
            m_TilePositions[index*4 + 2] = MapHelper::tilePosToStagePos(ccp(tilePos.x+1, tilePos.y+1));
            
            m_TilePositions[index*4 + 3] = MapHelper::tilePosToStagePos(ccp(tilePos.x, tilePos.y+1));
            
        }
    }
    


}

void StageShadowLayer::draw()
{
    
    for (int y=0;y<m_MapSize.height;++y)
    {
        for (int x=0;x<m_MapSize.width;++x)
        {
            
            drawTileShadow(x, y);
        }
    }
}

void StageShadowLayer::drawTileShadow(int x, int y)
{
    CCPoint tilePos = ccp(x,y);
    int index = y * m_MapSize.width + x;
    CCPoint pos[4];
    
    pos[0] = m_TilePositions[index*4];
    pos[1] = m_TilePositions[index*4+1];
    pos[2] = m_TilePositions[index*4+2];
    pos[3] = m_TilePositions[index*4+3];
    
    float opacity = m_TileOpacities[index]/255.0f;
    
    ccDrawSolidPoly(pos, 4, ccc4f(0,0,0,opacity));

}
