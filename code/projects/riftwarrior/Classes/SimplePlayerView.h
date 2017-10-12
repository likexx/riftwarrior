//
//  SimplePlayerView.h
//  tdgame
//
//  Created by Like Zhang on 7/11/13.
//
//

#ifndef __tdgame__SimplePlayerView__
#define __tdgame__SimplePlayerView__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class SimplePlayerView : public CCNode
{
public:
    virtual void update(float dt);
    
    virtual bool init();
    
    CREATE_FUNC(SimplePlayerView);
    
    
private:
    CCLabelBMFont* attachStatusLabel(CCNode* background, const char* initialValue);
    
    CCLabelBMFont* m_MoneyLabel;
    CCLabelBMFont* m_GemLabel;
};
#endif /* defined(__tdgame__SimplePlayerView__) */
