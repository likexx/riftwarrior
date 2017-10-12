//
//  GameSettingMenu.h
//  tdgame
//
//  Created by Like Zhang on 3/27/13.
//
//

#ifndef __tdgame__GameSettingMenu__
#define __tdgame__GameSettingMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class GameSettingMenu : public CCLayerColor
{
public:
    virtual bool init();
    
    CREATE_FUNC(GameSettingMenu);
private:
    void onResume(CCObject* pSender);
    void onExit(CCObject* pSender);
//    void onShare(CCObject* pSender);
    void exitStage();
    
};

#endif /* defined(__tdgame__GameSettingMenu__) */
