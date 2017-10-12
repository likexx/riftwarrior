//
//  Skill.h
//  tdgame
//
//  Created by Like Zhang on 3/15/13.
//
//

#ifndef __tdgame__Skill__
#define __tdgame__Skill__

#include <iostream>
#include <string>
#include <set>
#include <vector>
using namespace std;

struct Skill
{
    int id;
    string name;
    string introduction;
    int cost;
    int requireLevel;
    int maxLevel;
    
    int trainingBaseTime;
    int trainingTimePerLevel;
    
    void init(char** row);
    
private:
    int parseInt(char* pValue);
};

#endif /* defined(__tdgame__Skill__) */
