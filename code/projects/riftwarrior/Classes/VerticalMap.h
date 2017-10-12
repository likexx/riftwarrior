//
//  VerticalMap.h
//  tdgame
//
//  Created by Like Zhang on 2/18/13.
//
//

#ifndef __tdgame__VerticalMap__
#define __tdgame__VerticalMap__

#include <iostream>
#include "RandomMap.h"

class VerticalMap : public RandomMap
{
protected:
    virtual queue<CCPoint> createRandomWayPoints(const CCPoint& startTilePos, const CCPoint& endTilePos);
    
};
#endif /* defined(__tdgame__VerticalMap__) */
