//
//  ActionMenu.h
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#ifndef __tdgame__ActionMenu__
#define __tdgame__ActionMenu__

#include <iostream>
#include "enums.h"
#include "cocos2d.h"
using namespace cocos2d;

class ActionMenu : public CCMenu
{
public:
    virtual bool init();
    CREATE_FUNC(ActionMenu);
    
    virtual void update(float dt);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

    
    void reset();
    


private:
    
    CCMenuItemSprite* createSkillIcon(int skillId);
    void setPerformSkill(CCObject* pSender);
    
    
    CCMenuItemSprite* m_pWhirlWindBtn;  // for whirlwind attack, keep pressing until released
    CCMenuItemSprite* m_pEarthSlashBtn; // build infantry
    CCMenuItemSprite* m_pRecoverBtn;    
    

    
};

class ActionIndication : public CCNode
{
public:
    virtual bool init();
    CREATE_FUNC(ActionIndication);
    
    virtual void draw();
    
    inline void setActionMenu(ActionMenu* menu)
    {
        m_ActionMenu = menu;
    }
    
private:
    ActionMenu* m_ActionMenu;
    
};

#endif /* defined(__tdgame__ActionMenu__) */
