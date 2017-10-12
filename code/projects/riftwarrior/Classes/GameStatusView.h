//
//  GameStatusView.h
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#ifndef __tdgame__GameStatusView__
#define __tdgame__GameStatusView__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class GameStatusView : public CCNode
{
public:
    virtual void update(float dt);
    
    virtual bool init();
    
    CREATE_FUNC(GameStatusView);
    void updateWave(int currentwave);
    void updateWave(int currentWave, int maxWave);
private:
    CCSprite* m_waveborad;
    CCLabelBMFont* m_WaveLabel;
    CCLabelBMFont* m_TotalKilledCountLabel;
    CCLabelBMFont* m_TimerLabel;
    CCLabelBMFont* m_PassingEnemiesLabel;
    
};
#endif /* defined(__tdgame__GameStatusView__) */
