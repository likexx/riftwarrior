//
//  PlayerMoveMenu.h
//  tdgame
//
//  Created by Like Zhang on 3/29/13.
//
//

#ifndef __tdgame__PlayerMoveMenu__
#define __tdgame__PlayerMoveMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class PlayerMoveMenu : public CCMenu
{
public:
    virtual bool init();
    CREATE_FUNC(PlayerMoveMenu);
  
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    
    void updateButtonFunction();
    
private:
    void moveForward(CCObject* pSender);
    
    void moveBack(CCObject* pSender);
    
    CCMenuItem* m_LeftButton;
    CCMenuItem* m_RightButton;
};
#endif /* defined(__tdgame__PlayerMoveMenu__) */
