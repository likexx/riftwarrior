//
//  NpcAnimation.h
//  tdgame
//
//  Created by Like Zhang on 4/24/13.
//
//

#ifndef __tdgame__NpcAnimation__
#define __tdgame__NpcAnimation__

#include <iostream>
#include "cocos2d.h"
#include "enums.h"
#include <map>
using namespace cocos2d;
using namespace std;

class NpcAnimation : public CCNode
{
public:
    static NpcAnimation* create(int id);
    virtual bool init(int id);
    
    inline CCSprite* getCurrentSprite()
    {
        return m_pCurrentSprite;
    }
    
    
    void setAnimation(ENUM_NPC_ANIMATION animType);
    void setAnimationWithCallback(ENUM_NPC_ANIMATION animType, CCCallFunc* callback, float delay = 0);
    
    inline ENUM_NPC_ANIMATION getCurrentAnimationType()
    {
        return m_CurrentAnimationType;
    }
    
    inline float getAttackAnimationTime()
    {
        return m_AttackAnimationInterval;
    }
    
    void clear();
    
protected:
    virtual void updateSprite();
    virtual void addShadow();
    
    void initAnimations();
    
    map< ENUM_NPC_ANIMATION, CCSprite* > m_Sprites;
    map< ENUM_NPC_ANIMATION, CCFiniteTimeAction* > m_AnimationActions;
    
    CCSprite* m_pCurrentSprite;
    float m_AttackAnimationInterval;
    ENUM_NPC_ANIMATION m_CurrentAnimationType;
    
    string m_NpcName;
};
#endif /* defined(__tdgame__NpcAnimation__) */
