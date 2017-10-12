//
//  WorldMap.h
//  tdgame
//
//  Created by Like Zhang on 3/2/13.
//
//

#ifndef __tdgame__WorldMap__
#define __tdgame__WorldMap__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class SkillTreeMenu;
class WorldmapGameMenu;

class WorldMap : public CCNode
{
public:
    virtual bool init();
    virtual void update(float dt);
    
    CREATE_FUNC(WorldMap);
    
    void showServerMessage(const char* value);
    inline WorldmapGameMenu* getWorldmapGameMenu(){
        return this->m_pWorldmapGameMenu;
    }
private:

    CCMenuItem* createStageMapMark(const char* name, const CCPoint& pos, int mapId);
    void showStageInformation(CCObject* pSender);
    void removeStageInformation(CCObject* pSender);
    void enterStage(CCObject* pSender);
    void enterBossStage(CCObject* pSender);
    void showMainUI();
    
    void purchaseMap(CCObject* pSender);
    
    CCSprite* m_pBackground;
    CCMenu* m_worldMenu;
 
    CCNode* m_pStageIntroductionNode;

    
    //SkillTreeMenu* m_pSkillTreeMenu;
    
    WorldmapGameMenu* m_pWorldmapGameMenu;
    
    bool m_LoadNormalStage;
    
};

#endif /* defined(__tdgame__WorldMap__) */
