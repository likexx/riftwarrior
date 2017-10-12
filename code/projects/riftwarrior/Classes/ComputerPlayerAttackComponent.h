//
//  ComputerPlayerAttackComponent.h
//  tdgame
//
//  Created by Like Zhang on 5/6/13.
//
//

#ifndef __tdgame__ComputerPlayerAttackComponent__
#define __tdgame__ComputerPlayerAttackComponent__

#include <iostream>
#include "EnemyAttackComponent.h"

class ComputerPlayerAttackComponent : public EnemyAttackComponent
{
public:
    virtual bool init();
    virtual void update(float dt);
    
    virtual AttackableObject* findTarget();
    
    CREATE_FUNC(ComputerPlayerAttackComponent);
    
private:
};

#endif /* defined(__tdgame__ComputerPlayerAttackComponent__) */
