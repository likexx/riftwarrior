//
//  PlayerAnimation.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/1/13.
//
//

#include "PlayerAnimation.h"
#include "Function.h"

bool PlayerAnimation::init()
{
    m_CurrentAnimationType = ENUM_NPC_ANIMATION_NONE;
    m_pCurrentSprite = NULL;
    
    m_NpcName = "player";
    
    initAnimations();
    
    return true;
}

void PlayerAnimation::updateSprite()
{
    m_pCurrentSprite->setAnchorPoint(ccp(0.5, 0.5));
    
    m_pCurrentSprite->setVisible(true);
    m_pCurrentSprite->setColor(ccc3(255, 255,255));
}


void PlayerAnimation::addShadow()
{
    CCSprite* shadow = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("npc_shadow.png"));
    addChild(shadow);
}