//
//  UnitAttackComponent.h
//  tdgame
//
//  Created by Like Zhang on 5/9/13.
//
//

#ifndef __tdgame__UnitAttackComponent__
#define __tdgame__UnitAttackComponent__

#include <iostream>
#include "EnemyAttackComponent.h"

class UnitAttackComponent : public EnemyAttackComponent
{
public:
    virtual bool init();
    virtual void update(float dt);
    
    virtual AttackableObject* findTarget();
    
    CREATE_FUNC(UnitAttackComponent);
    
private:
};

#endif /* defined(__tdgame__UnitAttackComponent__) */
