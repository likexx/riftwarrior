//
//  LoadingScreen.h
//  tdgame
//
//  Created by Like Zhang on 3/28/13.
//
//

#ifndef __tdgame__LoadingScreen__
#define __tdgame__LoadingScreen__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class PlayerAnimation;
class EnemyAnimation;

class LoadingScreen : public CCScene
{
public:
    static LoadingScreen* create(int mapId);
    
    virtual bool init(int mapId);
    virtual void update(float dt);
    
    inline void setNormalStage(bool value)
    {
        m_LoadNormalStage = value;
    }
    
private:
    
    void startLoading();
    
    int m_MapId;

    float m_TotalRunningTime;
    
    bool m_LoadNormalStage;
    
    CCLabelTTF* m_HintLabel;
    CCLabelTTF* m_LoadingInformation;
    
};

#endif /* defined(__tdgame__LoadingScreen__) */
