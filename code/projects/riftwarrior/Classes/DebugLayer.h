//
//  DebugLayer.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/6/12.
//
//

#ifndef __ike_tdgame__DebugLayer__
#define __ike_tdgame__DebugLayer__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class DebugLayer : public CCLayer
{
public:
    virtual bool init();
    virtual void update(float dt);
    virtual void draw();
    virtual ~DebugLayer();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    void displayTileLocations(CCTMXTiledMap* pTileMap);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(DebugLayer);
    
private:
    CCTMXTiledMap* pTileMap;
    CCArray* tileLocationLabels;

};

#endif /* defined(__ike_tdgame__DebugLayer__) */
