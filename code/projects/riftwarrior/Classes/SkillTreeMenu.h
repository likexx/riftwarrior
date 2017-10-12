//
//  SkillTreeMenu.h
//  tdgame
//
//  Created by Like Zhang on 3/4/13.
//
//

#ifndef __tdgame__SkillTreeMenu__
#define __tdgame__SkillTreeMenu__

#include <iostream>
#include "Magic.h"
#include "BuildingSetting.h"
#include <list>
#include <map>
#include <vector>
#include "cocos2d.h"
using namespace cocos2d;


struct SkillProgressMask
{
    CCPoint Position;
    float Size;
    float Percentage;

};

class SkillTreeMaskLayer : public CCNode
{
public:
    
    virtual bool init();
    virtual void update(float dt);
    
    CREATE_FUNC(SkillTreeMaskLayer);
    
    virtual void draw();
    
    void setDrawProgress(int id, const CCPoint pos, float size, float completionPercentage);
        
private:
    bool m_DrawInProgressSkill;
    
    map<int, SkillProgressMask> m_SkillMasks;
};


class SkillTreeMenu : public CCNode
{
public:
    virtual bool init();

    virtual void update(float dt);
    
    CREATE_FUNC(SkillTreeMenu);

    inline bool isInUse()
    {
        return m_IsInUse;
    }
    
    void setRemoveOnClose(bool value);
    void show(CCObject* pSender);
    void close(CCObject* pSender);
    void updateSkillTreeInfo();
private:
    void showMsgWindow(int useFor);
    void closeMsgWindow();
    CCSprite* createMsgWindow();
    void learnSkillByGems();    
    void openIAPMenu();
    
    void resetSkillTree();
    void showSkillTree(CCMenu* pMenu);    
    
    void updateSkillInfo();
    void showSkillInfo(CCObject* pSender);
    void showCompleteSkillMsg();
    void completeSkill();
    void trainSkill(CCObject* pSender);
    void showSaleStore();

    
    CCNode* m_pBackground;
    CCMenu* m_pMenu;
    
    CCLabelBMFont* m_countDown;
    CCLabelTTF* m_SkillTitle;
    CCLabelTTF* m_SkillTrainTime;
    CCLabelTTF* m_SkillSummary;
    CCMenuItemImage* m_AddSkillPointButton;
    CCMenuItemImage* m_ResetAllButton;
    CCMenuItemImage* m_closeSkillTreeButton;
    
    CCNode* m_pSkillIconSelectedBackground;
    SkillTreeMaskLayer* m_pMaskLayer;
    
    int m_CurrentSelectedSkillId;

    bool m_IsInUse;
    
    bool m_RemoveOnClose;
};


#endif /* defined(__tdgame__SkillTreeMenu__) */
