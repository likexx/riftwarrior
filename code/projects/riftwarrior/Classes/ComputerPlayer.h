//
//  ComputerPlayer.h
//  tdgame
//
//  Created by Like Zhang on 5/6/13.
//
//

#ifndef __tdgame__ComputerPlayer__
#define __tdgame__ComputerPlayer__

#include <iostream>
#include "cocos2d.h"
#include "Player.h"

using namespace cocos2d;

class ComputerPlayer : public Enemy
{
public:
    
    static ComputerPlayer* create(int id, CCPoint startTilePos);
    
    ComputerPlayer(int id);
    
    virtual bool init();
    
    virtual void update(float dt);
    virtual void remove();
    
    
    void revive();
    
    inline unsigned long getNextLevelExperience()
    {
        if (m_Level < 4)
        {
            return (unsigned long)(100 * pow(m_Level, 2));
        }
        else
        {
            return (unsigned long)(100 * (m_Level-3) * pow(2,m_Level));
        }
    }

    void updateExperience(int delta);
    
    inline void updateMoney(int delta)
    {
        m_Money += delta;
        
        if (m_Money < 0)
        {
            m_Money = 0;
        }
    }
    
    inline void updateGems(int delta)
    {
        m_Gems += delta;
        
        if (m_Gems < 0)
        {
            m_Gems = 0;
        }
        
    }
    
    
private:
    virtual void createAttackComponent();

    virtual void updateDestination();

    int m_Experience;
    
    int m_Money;
    int m_Gems;

};

#endif /* defined(__tdgame__ComputerPlayer__) */
