//
//  FishingRaft.cpp
//  tdgame
//
//  Created by Like Zhang on 3/22/13.
//
//

#include "FishingRaft.h"
#include "MapHelper.h"

bool FishingRaft::init()
{
    if (!CCSprite::init())
    {
        return false;
    }
    
    initWithFile("environment/fishing_raft.png");

    return true;
}


void FishingRaft::startAnimation(const cocos2d::CCPoint &startTilePos, CCTMXTiledMap* pMap)
{
    CCPoint startPos = MapHelper::tilePosToGroundMapPos(startTilePos, pMap);
    this->setPosition(startPos);
    pMap->addChild(this);
    
    float distance = 0.1f;
    float duration = 1.25f + (rand()%100)/200.0f;
    float idleTime = 0.1f + (rand()%10)/50;
    
    
    CCPoint targetPos = MapHelper::tilePosToGroundMapPos(ccp(startTilePos.x + distance, startTilePos.y + distance), pMap);
    
    
    CCActionInterval* repeatAction = CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCDelayTime::create(idleTime),
                                                                                                   CCMoveTo::create(duration, targetPos),
                                                                                                   CCDelayTime::create(idleTime),
                                                                                                   CCMoveTo::create(duration, startPos),
                                                                                                   NULL));
    
    runAction(repeatAction);

}