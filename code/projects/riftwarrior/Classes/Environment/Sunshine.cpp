//
//  Sunshine.cpp
//  tdgame
//
//  Created by Like Zhang on 5/9/13.
//
//

#include "Sunshine.h"
#include "MapHelper.h"

bool Sunshine::init()
{
    if (!CCSprite::init())
    {
        return false;
    }
    
    return true;
}


void Sunshine::startAnimation(int type, const cocos2d::CCPoint &startTilePos, CCTMXTiledMap* pMap)
{
    
    initWithSpriteFrameName("environment_map101_sunshine_1.png");

    setAnchorPoint(ccp(0,0.5));
    
    CCPoint startPos = MapHelper::tilePosToGroundMapPos(startTilePos, pMap);
    setPosition(startPos);
    
    pMap->addChild(this);
    
    setScale(0.75);
    setOpacity(32);

    runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(5, 255),
                                                                            CCDelayTime::create(5),
                                                                            CCFadeTo::create(5, 32),
                                                                            NULL)));

    runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCScaleTo::create(5.0f, 1.2f),
                                                                            CCDelayTime::create(1),
                                                                            CCScaleTo::create(1.0f, 1.1f),
                                                                            CCScaleTo::create(0.5f, 1.15f),
                                                                            CCDelayTime::create(0.5),
                                                                            CCScaleTo::create(1, 1.25),
                                                                            CCDelayTime::create(1),
                                                                            CCScaleTo::create(5, 0.75),
                                                                            NULL)));

    runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCDelayTime::create(5),
                                                                            CCRotateBy::create(5, 5),
                                                                            CCDelayTime::create(10),
                                                                            CCRotateBy::create(5, -5),
                                                                            CCDelayTime::create(5),
                                                                            NULL)));

}

void Sunshine::reset()
{
    setPosition(m_StartPosition);
    setOpacity(0);
}