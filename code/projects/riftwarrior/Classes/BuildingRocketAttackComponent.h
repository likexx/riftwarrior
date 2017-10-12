//
//  BuildingRocketAttackComponent.h
//  tdgame
//
//  Created by Like Zhang on 4/28/13.
//
//

#ifndef __tdgame__BuildingRocketAttackComponent__
#define __tdgame__BuildingRocketAttackComponent__

#include <iostream>
#include "enums.h"
#include "BuildingAttackComponent.h"
using namespace std;

class Enemy;

class BuildingRocketAttackComponent : public BuildingAttackComponent
{
public:
    virtual ~BuildingRocketAttackComponent();
    
    virtual bool init();
    virtual void startAttack();
    
    CREATE_FUNC(BuildingRocketAttackComponent);
    
protected:
    
    virtual void processBullets(float dt);
    virtual CCPoint getFirePosition();
    virtual CCSprite* createBullet(float degree);
    virtual void playAttackAnimation();
    
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
    
    void playerFireSound();
    
    vector<Enemy*> m_Targets;
    
    CCPoint getTargetPosition(CCPoint currentMapPos, float degree);
    
    CCPoint m_TargetPosition;
    
};


#endif /* defined(__tdgame__BuildingRocketAttackComponent__) */
