//
//  BuildingAttackComponent.h
//  tdgame
//
//  Created by Like Zhang on 2/27/13.
//
//

#ifndef __tdgame__BuildingAttackComponent__
#define __tdgame__BuildingAttackComponent__

#include <iostream>
#include "enums.h"
#include "AttackComponent.h"
#include "AttackableObject.h"
#include "BuildingSetting.h"
#include "enums.h"
#include <map>
using namespace std;


class BuildingAttackComponent : public AttackComponent
{
public:
	BuildingAttackComponent()
		: bullets(nullptr), selectedTarget(nullptr), targetType(ATTACK_TARGET_NONE) {}
    virtual ~BuildingAttackComponent();
    
    virtual bool init();
    virtual void update(float dt);
    virtual AttackableObject* findTarget();

    virtual void startAttack();
    
    void setupAnimation(int buildingId);
    
    void damageEnemy(CCNode* pBullet);
    void removeBullet(CCNode* pBullet);
    
    inline void setTargetType(ENUM_TARGET_TYPE type)
    {
        targetType = type;
    }
    
    void removeTarget();
    
    bool isWithinAttackRange(const CCPoint& targetMapPos);
    bool bulletHitTarget(const CCPoint& bulletPos, const CCPoint& targetPos);
    void aimAtTarget(const CCPoint& target);
    
protected:
    
    CCArray* bullets;
    
    AttackableObject* selectedTarget;
    
    ENUM_TARGET_TYPE targetType;
    
    map<int, CCSpriteFrame*> m_AnimationFrames;
    map<int, CCFiniteTimeAction*> m_AnimationActions;

    map<int, CCSpriteFrame*> m_IdleAnimationFrames;
    map<int, CCFiniteTimeAction*> m_IdleAnimationActions;

    
    const BuildingSetting* m_pSetting;
    
    int m_Quadrant;
    float m_Degree;
    bool m_waitingForAttack;
    CCPoint m_AimedPosition;
    
    float m_IdleTime;
    
    CCSprite* createBulletAnimation(int buildingId, int level);
    void shootAtEnemy();
    void showHitEffect(AttackableObject* pTarget, CCPoint targetPos);
    
    //    void switchAnimation(int sequence);
    virtual void playAttackAnimation();
    void resetAnimation();
    
    CCPoint calculateFirePosition(float* xOffsets, float* yOffsets);
    virtual void processBullets(float dt);
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed) = 0;
    
    void makeSingleEnemyDamage(CCNode* pBullet);
    void makeSingleTileDamage(CCNode* pBullet);
    void makeBurningFire(CCNode* pBullet);
};


#endif /* defined(__tdgame__BuildingAttackComponent__) */
