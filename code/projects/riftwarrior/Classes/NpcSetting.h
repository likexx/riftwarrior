//
//  NpcSetting.h
//  tdgame
//
//  Created by Like Zhang on 4/25/13.
//
//

#ifndef __tdgame__NpcSetting__
#define __tdgame__NpcSetting__

#include <iostream>
#include <string>
using namespace std;

struct NpcSetting
{
    int id;
    string name;
    int maxHp;
  
    void init(char** row);
    
};

#endif /* defined(__tdgame__NpcSetting__) */
