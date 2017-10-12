//
//  BuildingPoisonAttackComponent.h
//  tdgame
//
//  Created by Like Zhang on 4/18/13.
//
//

#ifndef __tdgame__BuildingPoisonAttackComponent__
#define __tdgame__BuildingPoisonAttackComponent__

#include <iostream>
#include "BuildingMudAttackComponent.h"

class BuildingPoisonAttackComponent : public BuildingMudAttackComponent
{
public:
    virtual ~BuildingPoisonAttackComponent();
    
    virtual bool init();
    virtual void update(float dt);
    
    CREATE_FUNC(BuildingPoisonAttackComponent);
    
protected:
    
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
    
};

#endif /* defined(__tdgame__BuildingPoisonAttackComponent__) */
