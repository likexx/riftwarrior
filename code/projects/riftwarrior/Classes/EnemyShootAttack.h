//
//  EnemyShootAttack.h
//  tdgame
//
//  Created by Like Zhang on 4/29/13.
//
//

#ifndef __tdgame__EnemyShootAttack__
#define __tdgame__EnemyShootAttack__

#include <iostream>
#include "EnemyAttackComponent.h"

class SkeletonArrowAttack : public EnemyAttackComponent
{
public:
    virtual bool init();
    
    CREATE_FUNC(SkeletonArrowAttack);
    
protected:
    virtual CCActionInterval* createAction(float duration, const CCPoint& targetPos);
    virtual CCPoint getBulletPositon();
    
};


class TurtleThrowStoneAttack : public EnemyAttackComponent
{
public:
    virtual bool init();
    
    CREATE_FUNC(TurtleThrowStoneAttack);
    
protected:
    virtual CCActionInterval* createAction(float duration, const CCPoint& targetPos);
    virtual CCPoint getBulletPositon();
    
};



class SnakeVenomAttack : public EnemyAttackComponent
{
public:
    virtual bool init();
    
    CREATE_FUNC(SnakeVenomAttack);
    
protected:
    virtual CCActionInterval* createAction(float duration, const CCPoint& targetPos);
    virtual CCPoint getBulletPositon();
    
    virtual void showExplosion(CCNode* pBullet);
    
};


class SpiderWebAttack : public EnemyAttackComponent
{
public:
    virtual bool init();
    
    CREATE_FUNC(SpiderWebAttack);
    
protected:
    virtual CCActionInterval* createAction(float duration, const CCPoint& targetPos);
    virtual CCPoint getBulletPositon();
    
    virtual void showExplosion(CCNode* pBullet);
    
};

#endif /* defined(__tdgame__EnemyShootAttack__) */
