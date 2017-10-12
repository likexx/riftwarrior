//
//  NPC.h
//  tdgame
//
//  Created by Like Zhang on 4/24/13.
//
//

#ifndef __tdgame__NPC__
#define __tdgame__NPC__

#include <iostream>
#include "cocos2d.h"
#include "AttackableObject.h"
#include "NpcAnimation.h"
#include "NpcSetting.h"
using namespace cocos2d;


class NPC : public AttackableObject
{
public:
    virtual ~NPC();
    
    NPC(int id);
    
    static NPC* create(int id);
    
    virtual bool init(int id);
    
    void showOnMap(const CCPoint& tilePosition);

    
    virtual CCPoint getShootPosition();
    virtual CCPoint getAttackablePosition();
    virtual void reduceHp(float damage, bool ignoreDefense = false);
    virtual void actWhenAttacked(AttackableObject* pTarget);
    virtual bool isDead();
    virtual void remove();
    virtual bool isActive();
    virtual const CCPoint& getAttackableMapPosition();
    virtual void destroy();
    
    virtual bool ableToAttack();
    virtual void finishAttack();
    
    virtual float getDamage();
    virtual float getAttackRange();
    virtual float getAttackRate();

    virtual float getBaseSpeed();
    virtual void setSpeed(float speed);
    
    virtual CCNode* getAnimationPart();
    
    virtual int getId();
    
    inline float getMaxHp()
    {
        return m_MaxHp;
    }
    
    inline float getHp()
    {
        return m_Hp;
    }
    
protected:
    
    NpcAnimation* m_pAnimation;
    CCPoint m_CurrentMapPosition;
    CCPoint m_CurrentTilePosition;
    CCPoint m_GroundMapPosition;
    
    int m_NpcId;
    
    float m_Hp;
    float m_MaxHp;
    
    bool m_Active;
    
    const NpcSetting* m_pSetting;

};


#endif /* defined(__tdgame__NPC__) */
