//
//  WinningConditionMenu.h
//  tdgame
//
//  Created by Like Zhang on 4/26/13.
//
//

#ifndef __tdgame__WinningConditionMenu__
#define __tdgame__WinningConditionMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class WinningConditionMenu : public CCNode
{
public:
    virtual bool init();
    
    CREATE_FUNC(WinningConditionMenu);
    
    
private:
    void close(CCObject* pSender);
    
    CCNode* m_pBackground;
    CCMenuItem* m_pCloseBtn;
    CCLabelTTF* m_TitleLabel;
    
};

#endif /* defined(__tdgame__WinningConditionMenu__) */
