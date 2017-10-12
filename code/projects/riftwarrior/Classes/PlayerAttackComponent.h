//
//  PlayerAttackComponent.h
//  tdgame
//
//  Created by Like Zhang on 2/7/13.
//
//

#ifndef __tdgame__PlayerAttackComponent__
#define __tdgame__PlayerAttackComponent__

#include <iostream>
#include "EnemyAttackComponent.h"

class PlayerAttackComponent : public EnemyAttackComponent
{
public:
    virtual bool init();
    virtual void update(float dt);

    virtual AttackableObject* findTarget();
    
    CREATE_FUNC(PlayerAttackComponent);
    
private:
    virtual bool isWithinAttackRange(const cocos2d::CCPoint &targetMapPos);
    virtual void startAttack();
    virtual void hurtTarget(CCNode* pBullet);
};

#endif /* defined(__tdgame__PlayerAttackComponent__) */
