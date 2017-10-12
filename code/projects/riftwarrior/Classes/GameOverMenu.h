//
//  GameOverMenu.h
//  tdgame
//
//  Created by Like Zhang on 3/1/13.
//
//

#ifndef __tdgame__GameOverMenu__
#define __tdgame__GameOverMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class GameOverMenu : public CCLayerColor
{
public:
    virtual bool init();
    
    CREATE_FUNC(GameOverMenu);
    
private:
    void onReplay(CCObject* pSender);
    void onExit(CCObject* pSender);
};

#endif /* defined(__tdgame__GameOverMenu__) */
