//
//  KKMenu.h
//  tdgame
//
//  Created by Like Zhang on 4/19/13.
//
//

#ifndef __tdgame__KKMenu__
#define __tdgame__KKMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class KKMenu : public CCMenu
{
public:
    static KKMenu* createWithPriority(int priority);
    
    bool init(int priority);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void registerWithTouchDispatcher();
    
protected:
    int m_TouchPriority;
};

#endif /* defined(__tdgame__KKMenu__) */
