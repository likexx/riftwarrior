//
//  Sunshine.h
//  tdgame
//
//  Created by Like Zhang on 5/9/13.
//
//

#ifndef __tdgame__Sunshine__
#define __tdgame__Sunshine__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class Sunshine : public CCSprite
{
public:
    virtual bool init();
    CREATE_FUNC(Sunshine);
    
    void startAnimation(int type, const CCPoint& startTilePos, CCTMXTiledMap* pMap);
    
private:
    void reset();
    CCPoint m_StartPosition;
};

#endif /* defined(__tdgame__Sunshine__) */
