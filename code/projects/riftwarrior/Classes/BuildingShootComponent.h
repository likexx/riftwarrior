//
//  AttackComponent.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/17/12.
//
//

#ifndef __ike_tdgame__BuildingShootComponent__
#define __ike_tdgame__BuildingShootComponent__

#include <iostream>
#include "enums.h"
#include "BuildingAttackComponent.h"
using namespace std;


class BuildingShootComponent : public BuildingAttackComponent
{
public:
    virtual ~BuildingShootComponent();
    
    virtual bool init();
    
    CREATE_FUNC(BuildingShootComponent);

protected:
    
    virtual CCPoint getFirePosition();
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);

};

class BuildingThrowComponent : public BuildingAttackComponent
{
public:
    virtual ~BuildingThrowComponent();
    
    virtual bool init();
    
    CREATE_FUNC(BuildingThrowComponent);
    
protected:
    
    virtual CCPoint getFirePosition();
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
    
};


class BuildingFiregunComponent : public BuildingAttackComponent
{
public:
    virtual ~BuildingFiregunComponent();
    
    virtual bool init();
    
    CREATE_FUNC(BuildingFiregunComponent);
    
protected:
    
    virtual CCPoint getFirePosition();
    
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
    
};

class BuildingSniperComponent : public BuildingAttackComponent
{
public:
    virtual ~BuildingSniperComponent();
    
    virtual bool init();
    
    CREATE_FUNC(BuildingSniperComponent);
    
protected:
    
    virtual void startAttack();

    virtual CCPoint getFirePosition();
    
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
};


class BuildingCannonComponent : public BuildingAttackComponent
{
public:
    virtual ~BuildingCannonComponent();
    
    virtual bool init();
    
    CREATE_FUNC(BuildingCannonComponent);
    
protected:
    
    virtual CCPoint getFirePosition();
    
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
    
};

#endif /* defined(__ike_tdgame__BuildingShootComponent__) */
