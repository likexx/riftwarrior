//
//  BurningFire.h
//  tdgame
//
//  Created by Like Zhang on 4/6/13.
//
//

#ifndef __tdgame__BurningFire__
#define __tdgame__BurningFire__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class Building;

class BurningFire : public CCNode
{
public:
    static BurningFire* create(int buildingId, int level);
    virtual bool init(int buildingId, int level);
    
    virtual void update(float dt);
    
    inline void setMapPos(CCPoint mapPos)
    {
        m_CurrentMapPos= mapPos;
    }
    
private:
    
    void remove();
    
    CCSprite* m_pSprite;
    
    float m_Duration;
    float m_Damage;
    CCPoint m_CurrentMapPos;
    
    float m_TimeElapsed;
};


#endif /* defined(__tdgame__BurningFire__) */
