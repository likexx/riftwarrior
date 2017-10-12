//
//  MainUI.h
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#ifndef __tdgame__MainUI__
#define __tdgame__MainUI__

#include <iostream>
#include "StageUI.h"
#include "WorldMap.h"
#include "cocos2d.h"
using namespace cocos2d;

class MainUI : public CCNode
{
public:
    virtual bool init();
    
    CREATE_FUNC(MainUI);
    
    StageUI* getStageUI();
    WorldMap* getWorldMap();
    
    void initStageUI();
    void showWorldMap();
};

#endif /* defined(__tdgame__MainUI__) */
