//
//  PerformEffectAnimation.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/15/13.
//
//

#ifndef __ike_tdgame__PerformEffectAnimation_
#define __ike_tdgame__PerformEffectAnimation__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class PerformEffectAnimation : public CCNode
{
public:
    virtual bool init();
    
	PerformEffectAnimation() : m_pSprite(nullptr), totalTimeElapsed(0.0f) {};
    
	CREATE_FUNC(PerformEffectAnimation);
    
    virtual void update(float dt);
    
    static void loadEffects();
    
    void perform(int performSkillId, const CCPoint& pos);
    
    inline CCSprite* getCurrentSprite()
    {
        return m_pSprite;
    }
    
private:
    CCSprite* m_pSprite;
    
    float totalTimeElapsed;
  
    void attackEnemies();
};

#endif /* defined(__ike_tdgame__PerformEffectAnimation__) */
