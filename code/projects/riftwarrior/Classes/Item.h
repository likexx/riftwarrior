//
//  Item.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/4/13.
//
//

#ifndef __ike_tdgame__Item__
#define __ike_tdgame__Item__

#include <iostream>
#include "enums.h"
#include <string>
#include <map>
using namespace std;



struct Item
{
    int id;
    ENUM_ELEMENT_CLASS_TYPE elementClass;
    string name;
    string image;
    string animationKey;
    
    map<ENUM_ELEMENT_CLASS_TYPE, int> attackBonus;
    map<ENUM_ELEMENT_CLASS_TYPE, int> defenseBonus;
    
    int getAttackBonus(ENUM_ELEMENT_CLASS_TYPE type);
    int getDefenseBonus(ENUM_ELEMENT_CLASS_TYPE type);
};

#endif /* defined(__ike_tdgame__Item__) */
