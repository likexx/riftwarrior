//
//  StageShadowLayer.h
//  tdgame
//
//  Created by Like Zhang on 3/5/13.
//
//

#ifndef __tdgame__StageShadowLayer__
#define __tdgame__StageShadowLayer__

#include <iostream>
#include <vector>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class StageShadowLayer : public CCNode
{
public:
    
    virtual bool init();
    virtual void update(float dt);
    virtual void draw();
    
    CREATE_FUNC(StageShadowLayer);


private:
    CCTMXLayer* m_pNightMaskLayer;
    
    void setTileOpacity(int x, int y, int value);

    void drawTileShadow(int x, int y);
    
    vector<int> m_TileOpacities;
    
    CCSize m_MapSize;
    
    vector<CCPoint> m_TilePositions;
};

#endif /* defined(__tdgame__StageShadowLayer__) */
