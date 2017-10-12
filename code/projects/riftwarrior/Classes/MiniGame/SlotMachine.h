//
//  SlotMachine.h
//  tdgame
//
//  Created by Like Zhang on 4/11/13.
//
//

#ifndef __tdgame__SlotMachine__
#define __tdgame__SlotMachine__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class SlotMachine : public CCLayerColor
{
public:
    virtual bool init();
    
    CREATE_FUNC(SlotMachine);
    
    virtual void update(float dt);
    
private:
    void gotoWorldMap(CCObject* pSender);
    void stop(CCObject* pSender);
    
    
    
    CCNode* m_pBackground;
    CCMenuItem* m_pCloseBtn;
    CCLabelTTF* m_BtnTitle;
    
    CCLabelBMFont* m_Thousands;
    CCLabelBMFont* m_Hundreds;
    CCLabelBMFont* m_Tens;
    CCLabelBMFont* m_Digits;
    
    CCLabelBMFont* m_pHint;
    
    bool m_MachineStopped;
    int m_MaxValue;
    
    // for number animation;
};
#endif /* defined(__tdgame__SlotMachine__) */
