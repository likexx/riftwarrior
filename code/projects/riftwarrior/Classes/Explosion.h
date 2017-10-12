//
//  Explosion.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/23/12.
//
//

#ifndef __ike_tdgame__Explosion__
#define __ike_tdgame__Explosion__

#include <iostream>
#include "cocos2d.h"
#include <queue>
using namespace cocos2d;

class Explosion : public CCNode
{
public:
    
    static Explosion* create(const char* animName, const CCPoint& mapPos);
    
    virtual void update(float dt);
    
    void initWithAnimation(const char* imageFileName, int rows, int cols, int width, int height, float scale);
    
    void initWithAnimationName(const char* animName, int level, float scale, bool repeat);
    
    inline const char* getName()
    {
        return name;
    }
    
    inline CCSprite* getSprite()
    {
        return sprite;
    }
    
	Explosion() : 
		name(nullptr), sprite(nullptr), m_pAction(nullptr), duration(1.0f) {};

private:
    const char* name;
    CCSprite* sprite;
    CCActionInterval* m_pAction;
    float duration;
};


#endif /* defined(__ike_tdgame__Explosion__) */
