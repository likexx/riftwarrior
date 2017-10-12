//
//  EnemyAnimation.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/8/13.
//
//

#ifndef __ike_tdgame__EnemyAnimation__
#define __ike_tdgame__EnemyAnimation__

#include <iostream>
#include <map>
#include "enums.h"
#include "NpcAnimation.h"
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

struct EnemySetting;

class EnemyAnimation : public NpcAnimation
{
public:

    static EnemyAnimation* create(int enemyId);
    static EnemyAnimation* create(int enemyId, int parentId);

    virtual bool init(int enemyId, int parentId);
    
    void setupBossAnimation();
    
    inline CCSprite* getBossStar()
    {
        return m_pBossStar;
    }
    
    void reset();
   
    void initAnimation(int id,
                               int direction,
                               int animationType,
                               CCPoint anchorPoint,
                               ENUM_NPC_ANIMATION enumAnimValue);
    
    
    void removeEffect();
    
    
protected:
    
    void updateSprite();
    
    CCSprite* m_pShadow;
    CCSprite* m_pBossBackground;
    CCSprite* m_pBossStar;
    
    const EnemySetting* m_pSetting;
};
#endif /* defined(__ike_tdgame__EnemyAnimation__) */
