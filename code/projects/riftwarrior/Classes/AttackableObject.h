//
//  AttackableObject.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/17/12.
//
//

#ifndef __ike_tdgame__IAttackableObject__
#define __ike_tdgame__IAttackableObject__

#include <iostream>
#include <map>
#include "enums.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

#define DIZZY_ANIMATION_TAG 1001
#define SPIDERWEB_ANIMATION_TAG 1002

class AttackableObject : public CCNode
{
public:
    virtual CCPoint getShootPosition() = 0;
    virtual CCPoint getAttackablePosition() = 0;
    virtual void reduceHp(float damage, bool ignoreDefense = false) = 0;
    virtual void actWhenAttacked(AttackableObject* pTarget) = 0;
    virtual bool isDead() = 0;
    virtual void remove() = 0;
    virtual bool isActive() = 0;
    virtual const CCPoint& getAttackableMapPosition() = 0;
    virtual void destroy() = 0;
    
    virtual bool ableToAttack() = 0;
    virtual void finishAttack() = 0;
    
    virtual float getDamage() = 0;
    virtual float getAttackRange() = 0;
    virtual float getAttackRate() = 0;

    virtual float getBaseSpeed() = 0;
    virtual void setSpeed(float speed) = 0;

    virtual CCNode* getAnimationPart() = 0;

    virtual int getId();
    
    virtual void setStatus(ENUM_BUFF_STATUS status, float value, float duration);

    virtual void updateStatusByTap();
    
    inline bool failToAct()
    {
        return hasStatus(ENUM_BUFF_STATUS_DIZZY) || hasStatus(ENUM_BUFF_STATUS_SPIDER_WEB);
    }

protected:
    void createDestructionAnimation(CCSprite* pSprite);
    void createDestructionClipAnimation(cocos2d::CCNode *pClip, float dx, float dy);
    void removeClip(cocos2d::CCNode *pClip);
    
    
    virtual void updateStatus(float dt);
    
    void resetStatus();
    bool hasStatus(ENUM_BUFF_STATUS);
    void removeStatus(ENUM_BUFF_STATUS);

    map<ENUM_BUFF_STATUS, float> m_StatusEffect;
    map<ENUM_BUFF_STATUS, float> m_StatusDuration;
    map<ENUM_BUFF_STATUS, float> m_StatusMaxDuration;
    
    int m_TapCount;

};

#endif /* defined(__ike_tdgame__IAttackableObject__) */
