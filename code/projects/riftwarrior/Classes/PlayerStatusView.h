//
//  PlayerStatusView.h
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#ifndef __tdgame__PlayerStatusView__
#define __tdgame__PlayerStatusView__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class PlayerStatusView : public CCNode
{
public:
    virtual void update(float dt);
    
    virtual bool init();
    
    CREATE_FUNC(PlayerStatusView);
    
    CCPoint getBottomPosition();
    
private:
    CCLabelBMFont* attachStatusLabel(CCNode* background, const char* initialValue);

    CCLabelBMFont* m_LvLabel;
    CCLabelBMFont* m_HpLabel;
    CCLabelBMFont* m_MpLabel;
    CCLabelBMFont* m_ExpLabel;
    CCLabelBMFont* m_MoneyLabel;
    CCLabelBMFont* m_GemLabel;
    
    CCSprite* m_HpBar;
    CCSprite* m_MpBar;
    CCSprite* m_ExpBar;
    
    float m_MaxExpBarWidth;
};


#endif /* defined(__tdgame__PlayerStatusView__) */
