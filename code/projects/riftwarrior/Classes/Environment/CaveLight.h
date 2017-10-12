//
//  CaveLight.h
//  tdgame
//
//  Created by Like Zhang on 5/28/13.
//
//

#ifndef __tdgame__CaveLight__
#define __tdgame__CaveLight__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class CaveLight : public CCSprite
{
public:
    virtual bool init();
    CREATE_FUNC(CaveLight);
    
    void startAnimation(const CCPoint& startTilePos, CCTMXTiledMap* pMap, int mapId);
    
private:
    CCPoint m_StartPosition;
};

#endif /* defined(__tdgame__CaveLight__) */
