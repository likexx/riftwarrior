//
//  MultiTileBuilding.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/24/12.
//
//

#include "MultiTileBuilding.h"
#include "MapHelper.h"
#include "GameScene.h"
#include "GameData.h"
#include "BuildingUpgradeMenu.h"

MultiTileBuilding::MultiTileBuilding(int id, bool rotatable, int tileWidth, int tileLength)
:Building(id, rotatable)
{
    this->tileWidth = tileWidth;
    this->tileLength = tileLength;
}


void MultiTileBuilding::initBuilding()
{
    char buffer[128];
    sprintf(buffer, "building_image/building_defense_%d.png", id);
    m_pBaseSprite = CCSprite::create(buffer);

    const BuildingSetting& setting = GameData::getBuildingSetting(id);
    m_pBaseSprite->setAnchorPoint(ccp(setting.anchorX, setting.anchorY));
    m_pBaseSprite->setPosition(CCPointZero);
    
    CCTMXTiledMap* pTileMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint mapLeftMost = MapHelper::tilePosToGroundMapPos(ccp(0, tileLength));
    CCPoint mapRightMost = MapHelper::tilePosToGroundMapPos(ccp(tileWidth, 0));
    /*
    CCPoint groundLeftMost = MapHelper::tilePosToGroundPos(0, tileLength, pTileMap);
    CCPoint groundRightMost = MapHelper::tilePosToGroundPos(tileWidth, 0, pTileMap);
    */
    int diffX = abs((int)mapRightMost.x - (int)mapLeftMost.x);
    
    float scale = diffX / m_pBaseSprite->getContentSize().width;
    
    m_pBaseSprite->setScale(scale);

    this->addChild(m_pBaseSprite);
}

CCSize MultiTileBuilding::getTileSize()
{
    return CCSizeMake(tileWidth, tileLength);
}

CCPoint MultiTileBuilding::getFireOffset()
{
    return centerGroundOffset;
}

void MultiTileBuilding::showUpgradeMenu()
{
    if (m_pBuildingMenu)
    {
        return;
    }

    m_pBuildingMenu = BuildingUpgradeMenu::create(this);
    
    float y=this->getContentSize().height * this->getAnchorPoint().y;
    
    m_pBuildingMenu->setPosition(ccp(30, y+5));
    this->addChild(m_pBuildingMenu);
    
}


bool MultiTileBuilding::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint touchPos = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    
    CCPoint mapPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->convertToNodeSpace(touchPos);
    
//    CCPoint groundPos = MapHelper::screenPosToGroundPos(touchPos);
    
    CCTMXTiledMap* pTileMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint touchMapPos = MapHelper::isoMapPosToOrthMapPos(mapPos);
    CCPoint buildingMapPos = MapHelper::isoMapPosToOrthMapPos(this->getPosition());
    CCPoint pos = ccpSub(touchMapPos, buildingMapPos);
  //  pos.x /= getScale();
  //  pos.y /= getScale();

    CCRect rect = CCRectMake(2, 2, tileWidth * MapHelper::MAP_TILE_LENGTH - 4, tileLength * MapHelper::MAP_TILE_LENGTH - 4);
    
    if (rect.containsPoint(pos))
    {
        BuildingManager* pBuildingManager = GameScene::getInstance()->sharedGameStage->getBuildingManager();
        pBuildingManager->unselectAllBuildings();

        selected = true;
        
        if (isActive())
        {
            showUpgradeMenu();
        }
    }
    else
    {
        selected = false;
        
    }
        
    return selected;
}

void MultiTileBuilding::draw()
{
    CCNode::draw();

    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    CCTMXTiledMap* pTileMap = pStage->getGroundMap();

    CCSize tileSize = pTileMap->getTileSize();
    CCPoint startPos = ccp(0,0);
    
    CCPoint points[4];

    if (selected || !isActive())
    {
        float originX=this->getContentSize().width * this->getAnchorPoint().x;
        float originY=this->getContentSize().height * this->getAnchorPoint().y;

        glLineWidth(2.0f);

        if (isActive())
        {
            ccDrawColor4B(128, 255, 128, 255);
        }
        else
        {
            ccDrawColor4B(128, 128, 128, 255);
        }
    
        for(int i = 0; i < tileLength; ++i)
        {
            if (i > 0)
            {
            originX -= tileSize.width/2;
            originY -= tileSize.height/2;
            }
        
            for (int j = 0; j < tileWidth; ++j)
            {
                float x = originX + j * tileSize.width / 2;
                float y = originY - j * tileSize.height / 2;
            
                points[0] = ccp(x,y);
                points[1] = ccp(x-tileSize.width/2, y-tileSize.height/2);
                points[2] = ccp(x, y - tileSize.height);
                points[3] = ccp(x+tileSize.width/2, y-tileSize.height/2);
                ccDrawPoly(points, 4, true);
            
            }
        }
        showAttackRange();

    }
/*
    ccDrawColor4B(255, 0, 0, 255);
    CCSize size = getContentSize();
//    CCPoint points[4];
    points[0] = ccp(0,0);
    points[1] = ccp(0, size.height);
    points[2] = ccp(size.width, size.height);
    points[3] = ccp(size.width, 0);
    ccDrawPoly(points, 4, true);
*/
    
    
}