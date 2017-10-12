//
//  SeaWave.h
//  tdgame
//
//  Created by Like Zhang on 3/22/13.
//
//

#ifndef __tdgame__SeaWave__
#define __tdgame__SeaWave__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class SeaWave : public CCSprite
{
public:
    virtual bool init();
    CREATE_FUNC(SeaWave);
    
    void startAnimation(int type, const CCPoint& startTilePos, CCTMXTiledMap* pMap);
    
private:
    void reset();
    CCPoint m_StartPosition;
};
#endif /* defined(__tdgame__SeaWave__) */
