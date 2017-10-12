//
//  AttackComponent.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/25/12.
//
//

#include "AttackComponent.h"

bool AttackComponent::isWithinAttackRange(const cocos2d::CCPoint &mapPos1, const cocos2d::CCPoint &mapPos2, int range, int rangePowerd)
{
    CCPoint diff = ccpSub(mapPos1, mapPos2);
    
    if ( (diff.x > 0 && diff.x > range) ||
        (diff.x < 0 && diff.x < -range) ||
        (diff.y > 0 && diff.y > range) ||
        (diff.y < 0 && diff.y < -range) )
    {
        return false;
    }
    
    return pow(diff.x, 2) + pow(diff.y, 2) < rangePowerd;
    
}

float AttackComponent::getMapDistance(const cocos2d::CCPoint &mapPos1, const cocos2d::CCPoint &mapPos2)
{
    CCPoint diff = ccpSub(mapPos1, mapPos2);
    
    return sqrtf(pow(diff.x, 2) + pow(diff.y, 2));
}


CCPoint AttackComponent::getFirePosition()
{
    AttackableObject* owner = static_cast<AttackableObject*>(getParent());
    return owner->getShootPosition();
    
}

