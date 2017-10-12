//
//  KKMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 4/19/13.
//
//

#include "KKMenu.h"

KKMenu* KKMenu::createWithPriority(int priority)
{
    KKMenu* pMenu = new KKMenu();
    if (pMenu && pMenu->init(priority))
    {
        pMenu->autorelease();
        return pMenu;
    }
    
    CC_SAFE_DELETE(pMenu);
    return NULL;
}


bool KKMenu::init(int priority)
{
    m_TouchPriority = priority;
    
    if (!CCMenu::init())
    {
        return false;
    }
    
    return true;
}

void KKMenu::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, m_TouchPriority, true);

}

bool KKMenu::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    bool result = CCMenu::ccTouchBegan(touch, event);
    
    if (result)
    {
        return result;
    }
    
    m_eState = kCCMenuStateTrackingTouch;
    
    return true;
}


