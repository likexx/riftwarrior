//
//  BaseAttackComponent.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/25/12.
//
//

#ifndef __ike_tdgame__AttackComponent__
#define __ike_tdgame__AttackComponent__

#include <iostream>
#include "AttackableObject.h"
#include "cocos2d.h"
using namespace cocos2d;

class AttackComponent : public CCNode
{
public:
    virtual AttackableObject* findTarget() = 0;
    virtual void setupAnimation(int buildingId) = 0;

    static bool isWithinAttackRange(const CCPoint& mapPos1, const CCPoint& mapPos2, int range, int rangePowered);
    static float getMapDistance(const CCPoint& mapPos1, const CCPoint& mapPos2);

protected:
    virtual CCPoint getFirePosition();
    
    

};

#endif /* defined(__ike_tdgame__AttackComponent__) */
