//
//  ActiveSkillSetting.h
//  tdgame
//
//  Created by Like Zhang on 4/23/13.
//
//

#ifndef __tdgame__ActiveSkillSetting__
#define __tdgame__ActiveSkillSetting__

#include <iostream>
#include <string>
#include <set>
#include <vector>
using namespace std;

struct ActiveSkillSetting
{
    int id;
    string name;
    int mpCost;
    float damageValue;
    int requireLevel;
    
    void init(char** row);
    
private:
    int parseInt(char* pValue);
};

#endif /* defined(__tdgame__ActiveSkillSetting__) */
