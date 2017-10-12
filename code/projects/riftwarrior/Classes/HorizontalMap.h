//
//  HorizontalMap.h
//  tdgame
//
//  Created by Like Zhang on 2/18/13.
//
//

#ifndef __tdgame__HorizontalMap__
#define __tdgame__HorizontalMap__

#include <iostream>
#include "RandomMap.h"

class HorizontalMap : public RandomMap
{
protected:
    virtual queue<CCPoint> createRandomWayPoints(const CCPoint& startTilePos, const CCPoint& endTilePos);

};

#endif /* defined(__tdgame__HorizontalMap__) */
