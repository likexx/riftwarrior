//
//  BuildingMultiArrowAttackComponent.h
//  tdgame
//
//  Created by Like Zhang on 4/4/13.
//
//

#ifndef __tdgame__BuildingMultiArrowAttackComponent__
#define __tdgame__BuildingMultiArrowAttackComponent__

#include <iostream>
#include "enums.h"
#include "BuildingAttackComponent.h"
using namespace std;

class Enemy;

class BuildingMultiArrowAttackComponent : public BuildingAttackComponent
{
public:
    virtual ~BuildingMultiArrowAttackComponent();
    
    virtual bool init();
    virtual void startAttack();
    
    CREATE_FUNC(BuildingMultiArrowAttackComponent);
    
protected:
    
    virtual void processBullets(float dt);
    virtual CCPoint getFirePosition();
    virtual CCSprite* createBullet(float degree);
    
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
    
    vector<Enemy*> m_Targets;
    
    CCPoint getTargetPosition(CCPoint currentMapPos, float range, float degree);
};

#endif /* defined(__tdgame__BuildingMultiArrowAttackComponent__) */
