//
//  MultiTileBuilding.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/24/12.
//
//

#ifndef __ike_tdgame__MultiTileBuilding__
#define __ike_tdgame__MultiTileBuilding__

#include <iostream>
#include "Building.h"

class MultiTileBuilding : public Building
{
public:
    MultiTileBuilding(int id, bool rotatable, int tileWidth, int tileLength);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void draw();
    
    virtual void showUpgradeMenu();
    
    virtual CCSize getTileSize();

protected:
    MultiTileBuilding();
    
    virtual void initBuilding();
    virtual CCPoint getFireOffset();
    
};


#endif /* defined(__ike_tdgame__MultiTileBuilding__) */
