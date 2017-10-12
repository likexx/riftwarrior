//
//  FishingRaft.h
//  tdgame
//
//  Created by Like Zhang on 3/22/13.
//
//

#ifndef __tdgame__FishingRaft__
#define __tdgame__FishingRaft__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class FishingRaft : public CCSprite
{
public:
    virtual bool init();
    CREATE_FUNC(FishingRaft);
    
    void startAnimation(const CCPoint& startTilePos, CCTMXTiledMap* pMap);
};

#endif /* defined(__tdgame__FishingRaft__) */
