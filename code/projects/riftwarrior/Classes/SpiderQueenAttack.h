//
//  SpiderQueenAttack.h
//  tdgame
//
//  Created by Like Zhang on 5/1/13.
//
//

#ifndef __tdgame__SpiderQueenAttack__
#define __tdgame__SpiderQueenAttack__

#include <iostream>
#include <vector>
#include "EnemyAttackComponent.h"
using namespace std;

class SpiderQueenAttack : public EnemyAttackComponent
{
public:
    virtual bool init();
    virtual void update(float dt);
    virtual void hurtTarget(CCNode* pBullet);
    
    CREATE_FUNC(SpiderQueenAttack);
    
private:
    
    float m_LastActionTime;
    bool m_AttackStarted;
    vector<CCPoint> m_AttackingPositions;
    
    
    virtual void startAttack();
    virtual void finishAttack();
    
    void hurtPlayer(CCNode* pNode);
    
    
    
};
#endif /* defined(__tdgame__SpiderQueenAttack__) */
