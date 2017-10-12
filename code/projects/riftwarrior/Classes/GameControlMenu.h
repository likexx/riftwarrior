//
//  GameControlMenu.h
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#ifndef __tdgame__GameControlMenu__
#define __tdgame__GameControlMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class GameControlMenu : public CCMenu
{
public:
    virtual bool init();
    CREATE_FUNC(GameControlMenu);

//    inline bool isHelpButtonExist(){
//        if(m_pHelpButton){
//            return true;
//        }else{
//            return false;
//        }
//    }
//    void promptHelpButton();
    CCPoint getSkillTreeLoc();
    CCSize getSkillTreeSize();
    
private:

    void setPauseButton();
    void setResumeButton();
    
    void onSpeedButton(CCObject* pSender);
    void onSettingMenu(CCObject* pSender);
//    void onHelpButton(CCObject* pSender);    
    void onInGamePurchase(CCObject* pSender);

    void recoverAfterLogin();
    bool m_isPaused;
    
    CCMenuItemImage* m_pGameSpeedButton;
    CCMenuItemImage* m_pSettingIcon;
//    CCMenuItemImage* m_pHelpButton;
    CCMenuItem* m_IapIcon;
};

#endif /* defined(__tdgame__GameControlMenu__) */
