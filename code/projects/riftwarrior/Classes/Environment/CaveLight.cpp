//
//  CaveLight.cpp
//  tdgame
//
//  Created by Like Zhang on 5/28/13.
//
//

#include "CaveLight.h"
#include "MapHelper.h"
bool CaveLight::init()
{
    if (!CCSprite::init())
    {
        return false;
    }
    
    return true;
}


void CaveLight::startAnimation(const cocos2d::CCPoint &startTilePos, CCTMXTiledMap* pMap, int mapId)
{
    
    char buffer[128] = {0};
    sprintf(buffer, "environment_map%d_cavelight_1.png", mapId);
    
    initWithSpriteFrameName(buffer);
    
    setAnchorPoint(ccp(0,0.5));
    
    CCPoint startPos = MapHelper::tilePosToGroundMapPos(startTilePos, pMap);
    setPosition(startPos);
    
    pMap->addChild(this);
    
    runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(2, 128),
                                                                            CCFadeTo::create(2, 255),
                                                                            NULL)));
    
}

