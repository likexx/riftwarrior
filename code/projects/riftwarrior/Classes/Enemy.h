//
//  Enemy.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/13/12.
//
//

#ifndef __ike_tdgame__Enemy__
#define __ike_tdgame__Enemy__

#include <iostream>
#include "AttackableObject.h"
#include "EnemyAttackComponent.h"
#include "EnemyAnimation.h"
#include "EnemySetting.h"
#include "Building.h"
#include "cocos2d.h"
#include "enums.h"
#include <string>
using namespace cocos2d;
using namespace std;



class Enemy : public AttackableObject
{
public:
	Enemy()
		: pAnimation(nullptr), active(false), m_CurrentMoveDirection(ENUM_MOVE_DIRECTION_NONE), m_pAttackComponent(nullptr) {};

    static Enemy* create(int id, CCPoint startTilePos);
    static Enemy* create(int id, int parentId, CCPoint startTilePos);
    static void initCache();
    
    virtual ~Enemy();
    
    virtual void update(float dt);
    virtual void draw();
    
    virtual const CCPoint& getSpritePosition();
    virtual CCPoint getShootPosition();
    virtual CCPoint getAttackablePosition();
    virtual bool isActive();
    virtual const CCPoint& getAttackableMapPosition();
    virtual void destroy();
    virtual void actWhenAttacked(AttackableObject* pTarget);
    virtual float getDamage();
    virtual float getAttackRange();
    virtual float getAttackRate();
    virtual float getBaseSpeed();
    virtual void setSpeed(float speed);

    virtual void finishAttack();
    virtual bool ableToAttack();
    
    virtual CCNode* getAnimationPart();

    virtual void setStatus(ENUM_BUFF_STATUS status, float value, float duration);
    
    void activate();
    void setToTile(int x, int y);
    
    bool tryAttack(int x, int y);
    
    inline void trySetAttackTarget(AttackableObject* pTarget)
    {
        m_pAttackComponent->trySetTarget(pTarget);
    }
    
    bool isDead();
    
    virtual void remove();
    
    void arriveDestination();

    inline int getLevel()
    {
        return m_Level;
    }
    
    void showLootAnimation(int money, int gems);

    inline void setAnimation(ENUM_NPC_ANIMATION animationType)
    {
        pAnimation->setAnimation(animationType);
    }

    inline CCSprite* getAnimationSprite()
    {
        return pAnimation->getCurrentSprite();
    }
    
    inline EnemyAnimation* getAnimation()
    {
        return pAnimation;
    }

    inline const CCPoint& getCurrentTilePosition()
    {
        return m_CurrentTilePosition;
    }
    
    inline const CCPoint& getCurrentMapPosition()
    {
        return m_CurrentMapPosition;
    }
    
    inline const int getCurrentHp()
    {
        return currentHp;
    }
    
    inline const int getMaxHp()
    {
        return maxHp;
    }
    
    inline const EnemySetting* getEnemySetting()
    {
        return m_pEnemySetting;
    }
    
    int getDropMoney();
    
    int getDropGems();
    
    inline void setKilledByPlayer()
    {
        m_KilledByPlayer = true;
    }
    
    inline void setKilledByExplosion(bool value)
    {
        m_KilledInExplosion = value;
    }
    
    inline ENUM_MOVE_DIRECTION getMoveDirection()
    {
        return m_CurrentMoveDirection;
    }
    
    inline void setBoss(bool value)
    {
        m_IsBoss = value;
    }
    
    inline bool isBoss()
    {
        return m_IsBoss;
    }
    
    inline ENUM_TEAM getCurrentTeam()
    {
        return m_Team;
    }
    
    int getExperience();
    
    void init(int level);
    void reduceHp(float damage, bool ignoreDefense = false);
    
    inline int getId()
    {
        return id;
    }
    
    void resetIdleAnimation();


protected:
    Enemy(int id);
    Enemy(int id, int parentId);
    
    virtual void createAttackComponent();
    
    void initialize(int id, int parentId);
    
    virtual bool updateDirection();
    virtual void updateDestination();
    
    bool reachDestination();
    
    void showBodyDestruction();
    
    
    
//    CCPoint getLastTile();
//    bool isAroundTileCenter();
    
//    void initWithAnimation(const char* spriteName, const char* spriteAnimName);

    EnemyAnimation* pAnimation;
    
    
    int id;
    int maxHp;
    float attack;
    float attackRange;
    int moveSpeed;
    int m_Level;
    
    int m_RandSeed;
    const EnemySetting* m_pEnemySetting;
    
    bool active;
    bool m_KilledByPlayer;
    bool m_KilledInExplosion;  // body should be thrown up and explode

    
    // boss in completed map stage
    bool m_IsBoss;
    
    ENUM_TEAM m_Team;
    
    float currentHp;
    float attackRate;
    
    ENUM_MOVE_DIRECTION m_CurrentMoveDirection;

    bool m_MoveToEnd;
    
    // position on orthogonal map
    CCPoint m_CurrentMapPosition;
    CCPoint m_CurrentTilePosition;
    
    CCPoint m_DestinationTile;

    EnemyAttackComponent* m_pAttackComponent;
    
    // attackable tiles
    
};
#endif /* defined(__ike_tdgame__Enemy__) */
