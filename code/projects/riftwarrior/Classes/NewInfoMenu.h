//
//  NewInfoMenu.h
//  tdgame
//
//  Created by Wang Nero on 13-7-4.
//
//

#ifndef __tdgame__NewInfoMenu__
#define __tdgame__NewInfoMenu__

#include <iostream>
#include <list>
#include <utility>

#include <cocos2d.h>

using namespace cocos2d;

typedef std::pair<int, CCMenuItemImage*> InfoBtnPair;
typedef std::list<InfoBtnPair> InfoBtnList;

class NewInfoMenu : public CCMenu
{
public :
    virtual bool init();
    CREATE_FUNC(NewInfoMenu);
    void createNewInfo(const int messageId);
    void setLocation(const float x,const float y);
    inline CCNode* getSkillTreeIconNode()
    {
        return m_pSkillTreeIcon;
    }
private:
    void flush();
    bool addAction(CCMenuItemImage* const infoButton);
    int foundMessageAndRemove(CCMenuItemImage* button);
    bool setLocation(CCMenuItemImage* const infoButton,int index);
    void addToList(const int message,CCMenuItemImage* const button);
    void showInfoMessage(CCObject* pSender);
    void showSkillTreeIcon();
    void onSkillTree(CCObject* pSender);

    float infoX;
    float infoY;
    InfoBtnList infoBtnList;
    CCMenuItem* m_pSkillTreeIcon;
};

#endif /* defined(__tdgame__NewInfoMenu__) */
