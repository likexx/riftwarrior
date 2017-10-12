//
//  PlayerSkills.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/17/13.
//
//

#ifndef __ike_tdgame__PlayerSkills__
#define __ike_tdgame__PlayerSkills__

#include <iostream>
#include <map>
using namespace std;

class PlayerSkills
{
public:
    
    inline map<int, int> getBuildingSkills() const
    {
        return m_BuildingSkills;
    }
    
    inline map<int, int> getMagicSkills() const
    {
        return m_MagicSkills;
    }
    
    
    inline int getBuildingSkillLevel(int id)
    {
        map<int, int>::iterator current = m_BuildingSkills.find(id);
        if (current == m_BuildingSkills.end())
        {
            return 0;
        }
        
        return current->second;
    }

    inline int getMagicSkillLevel(int id)
    {
        map<int, int>::iterator current = m_MagicSkills.find(id);
        if (current == m_MagicSkills.end())
        {
            return 0;
        }
        
        return current->second;
    }

    void increaseBuildingSkillLevel(int id);
    
    void increaseMagicSkillLevel(int id);
        
    
private:
    map<int, int> m_BuildingSkills;
    map<int, int> m_MagicSkills;
};

#endif /* defined(__ike_tdgame__PlayerSkills__) */
