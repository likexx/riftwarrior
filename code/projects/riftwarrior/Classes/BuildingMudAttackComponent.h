//
//  BuildingMudAttackComponent.h
//  tdgame
//
//  Created by Like Zhang on 4/4/13.
//
//

#ifndef __tdgame__BuildingMudAttackComponent__
#define __tdgame__BuildingMudAttackComponent__

#include <iostream>
#include "enums.h"
#include "BuildingAttackComponent.h"
using namespace std;

class Enemy;

class BuildingMudAttackComponent : public BuildingAttackComponent
{
public:
    virtual ~BuildingMudAttackComponent();
    
    virtual bool init();
    virtual void update(float dt);
    
    CREATE_FUNC(BuildingMudAttackComponent);
    
    
protected:
    
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
    
    
};
#endif /* defined(__tdgame__BuildingMudAttackComponent__) */
