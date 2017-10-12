//
//  SmartAnimation.h
//  tdgame
//
//  Created by Like Zhang on 3/27/13.
//
//

#ifndef __tdgame__SmartAnimation__
#define __tdgame__SmartAnimation__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class SmartAnimation : public CCNode
{
public:
    static SmartAnimation* create(float duration, bool fadeOut);
    virtual bool init(float duration, bool fadeOut);
    
    void removeSelf();

private:
    float m_Duration;
    bool m_FadeOut;

    
};


#endif /* defined(__tdgame__SmartAnimation__) */
