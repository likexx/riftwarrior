//
//  EnemyAttackComponent.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/17/12.
//
//

#ifndef __ike_tdgame__EnemyAttackComponent__
#define __ike_tdgame__EnemyAttackComponent__

#include <iostream>
#include "enums.h"
#include "AttackComponent.h"
#include "AttackableObject.h"
#include "EnemySetting.h"

class Enemy;

class EnemyAttackComponent : public AttackComponent
{
public:
	EnemyAttackComponent() 
		: m_Bullets(nullptr), selectedTarget(nullptr), targetType(ATTACK_TARGET_NONE) {}
    virtual ~EnemyAttackComponent();
    
    virtual bool init();
    virtual void update(float dt);
    virtual void setupAnimation(int id);
    
    CREATE_FUNC(EnemyAttackComponent);
    
    virtual void hurtTarget(CCNode* pBullet);
    void removeBullet(CCNode* pBullet);
    
    inline void setTargetType(ENUM_TARGET_TYPE type)
    {
        targetType = type;
    }
    
    void removeTarget();
    

    virtual AttackableObject* findTarget();
    virtual bool isWithinAttackRange(const cocos2d::CCPoint &targetMapPos);
    
    bool trySetTarget(AttackableObject* pTarget);
    bool hasTarget();
    bool bulletHitTarget(const CCPoint& bulletPos, const CCPoint& targetPos);
    void aimAtTarget(const CCPoint& target);
    void shootAtTarget();
    
protected:
    
    CCArray* m_Bullets;
    
    AttackableObject* selectedTarget;
    
    ENUM_TARGET_TYPE targetType;
    
    bool m_attackMoveFinished;
    int m_Quadrant;
    float m_AttackAnimationInterval;
    
    CCSprite* m_pHitSprite;
    CCFiniteTimeAction* m_pHitAction;
    
    int m_RandSeed;
    
    float m_IdleTime;
    
    virtual void startAttack();
    virtual CCActionInterval* createAction(float duration, const CCPoint& targetPos);
    virtual CCPoint getBulletPositon();

    virtual void showExplosion(CCNode* pBullet);
    
    void shootBullet(void* pObejct);
    
    
    void makeSingleTargetDamage(CCNode* pBullet);
    void makeSnakeVenomDamage(CCNode* pBullet);
    void makeHalfCircleMeleeDamage();
    void makeRoundAreaDamage();

    bool isWithinRange(const CCPoint& centerMapPos, const cocos2d::CCPoint &targetMapPos);
    
    void playAttackSound();

};


#endif /* defined(__ike_tdgame__EnemyAttackComponent__) */
