//
//  SeaWave.cpp
//  tdgame
//
//  Created by Like Zhang on 3/22/13.
//
//

#include "SeaWave.h"
#include "MapHelper.h"

bool SeaWave::init()
{
    if (!CCSprite::init())
    {
        return false;
    }
    
    return true;
}


void SeaWave::startAnimation(int type, const cocos2d::CCPoint &startTilePos, CCTMXTiledMap* pMap)
{
    char buffer[128] = {0};
    sprintf(buffer, "environment/seawave%d.png", type);
    
    initWithFile(buffer);
    
    float distance = 0.3 + (rand()%20)/100.0f;

    CCPoint startPos = MapHelper::tilePosToGroundMapPos(ccp(startTilePos.x+distance, startTilePos.y+distance), pMap);
    
    m_StartPosition = startPos;
    
    
    this->setPosition(startPos);
    pMap->addChild(this);
    
    float duration = 2*(1.25 + (rand()%100)/200.0f);
    float idleTime = 0.1 + (rand()%10)/50;
    
    
    CCPoint targetPos = MapHelper::tilePosToGroundMapPos(ccp(startTilePos.x, startTilePos.y), pMap);
    
    
    CCActionInterval* moveAction = CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveTo::create(duration, targetPos),
                                                                                                 CCCallFunc::create(this, callfunc_selector(SeaWave::reset)),
                                                                                                 CCDelayTime::create(idleTime),
                                                                                                   NULL));

    CCActionInterval* fadeOutAction = CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeIn::create(duration/2),
                                                                                                    CCFadeOut::create(duration/2),
                                                                                                    CCDelayTime::create(idleTime),
                                                                                                    NULL));

    
    
    runAction(moveAction);
    runAction(fadeOutAction);
}

void SeaWave::reset()
{
    setPosition(m_StartPosition);
    setOpacity(0);
}