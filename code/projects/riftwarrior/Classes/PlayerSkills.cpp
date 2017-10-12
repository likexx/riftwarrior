//
//  PlayerSkills.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/17/13.
//
//

#include "PlayerSkills.h"


void PlayerSkills::increaseBuildingSkillLevel(int id)
{
    map<int, int>::iterator current = m_BuildingSkills.find(id);
    if (current == m_BuildingSkills.end())
    {
        m_BuildingSkills[id] = 1;
        return;
    }
    
    current->second++;
}


void PlayerSkills::increaseMagicSkillLevel(int id)
{
    map<int, int>::iterator current = m_MagicSkills.find(id);
    if (current == m_MagicSkills.end())
    {
        m_MagicSkills[id] = 1;
        return;
    }
    
    current->second++;
}
