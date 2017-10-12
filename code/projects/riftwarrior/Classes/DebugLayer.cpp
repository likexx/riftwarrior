//
//  DebugLayer.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/6/12.
//
//

#include "DebugLayer.h"
#include "MapHelper.h"
#include "GameScene.h"
#include "Function.h"

static int TileLocationTagStart = 0;

CCScene* DebugLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    DebugLayer *layer = DebugLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool DebugLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    
    return true;
}

DebugLayer::~DebugLayer()
{
    if (!tileLocationLabels)
    {
        delete tileLocationLabels;
        tileLocationLabels = NULL;
    }
}

void DebugLayer::displayTileLocations(CCTMXTiledMap* pTileMap)
{
    CCLabelTTF* label;
    
    this->pTileMap = pTileMap;
    CCSize tileSize = pTileMap->getTileSize();
    CCSize mapSize = pTileMap->getMapSize();

    tileLocationLabels = CCArray::create();
    int count = 0;
    for(int x=0;x<pTileMap->getMapSize().width;x++)
    {
        for(int y=0;y<pTileMap->getMapSize().height;y++)
        {
            char buffer[50];
            sprintf(buffer,"(%i, %i)\0", x, y);
            label = CCLabelTTF::create(buffer, "Helvetica", 10);
            
            CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(x,y));
            label->cocos2d::CCNode::setPosition(pos.x, pos.y );
            this->addChild(label, 1, TileLocationTagStart + count);
            count++;
        }
    }
    
    this->scheduleUpdate();
    
}

void DebugLayer::update(float dt)
{
    int count = 0;
    
    for(int x=0;x<pTileMap->getMapSize().width;x++)
    {
        for(int y=0;y<pTileMap->getMapSize().height;y++)
        {
            CCLabelTTF* label = static_cast<CCLabelTTF*>(this->getChildByTag(TileLocationTagStart + count));
            char buffer[50];
            sprintf(buffer,"(%i, %i)", x, y);
            
            CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(x,y));
            label->cocos2d::CCNode::setPosition(pos.x, pos.y );
            count++;
            
        }
    }
    
}

void DebugLayer::draw()
{
    CCLayer::draw();
}
