//
//  WorldmapGameMenu.h
//  tdgame
//
//  Created by Like Zhang on 6/28/13.
//
//

#ifndef __tdgame__WorldmapGameMenu__
#define __tdgame__WorldmapGameMenu__

#include <iostream>

#include <iostream>
#include "Magic.h"
#include "BuildingSetting.h"
#include <list>
#include <map>
#include <vector>
#include "cocos2d.h"
#include "SkillTreeMenu.h"

using namespace cocos2d;

class WorldmapGameMenu : public CCNode
{
public:
    virtual bool init();
    
    
    CREATE_FUNC(WorldmapGameMenu);
    
    inline bool isInUse()
    {
        return skillBookMenu != NULL ? skillBookMenu->isInUse() : false;
    }
    
    void updateSkillTree();
    
    void showServerMessage(const char* pValue);
    
private:

    
    void build();
    
    void backToHome(CCObject* pSender);
    void showSkillBook(CCObject* pSender);    
    void showLeaderboard(CCObject* pSender);
    void openIapMenu(CCObject* pSender);
    
    void closeSkillBook();
    
    //    void addBuildingPoint(CCObject* pSender);
    
    CCMenu* m_pMenu;
    CCSprite* m_pBackground;
    SkillTreeMenu* skillBookMenu;
};

#endif /* defined(__tdgame__WorldmapGameMenu__) */
