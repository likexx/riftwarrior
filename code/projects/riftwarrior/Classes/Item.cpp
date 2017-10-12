//
//  Item.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/4/13.
//
//

#include "Item.h"

int Item::getAttackBonus(ENUM_ELEMENT_CLASS_TYPE type)
{
    map<ENUM_ELEMENT_CLASS_TYPE, int>::iterator it = attackBonus.find(type);
    
    if (it != attackBonus.end())
    {
        return it->second;
    }
    
    return 0;
    
}

int Item::getDefenseBonus(ENUM_ELEMENT_CLASS_TYPE type)
{
    map<ENUM_ELEMENT_CLASS_TYPE, int>::iterator it = defenseBonus.find(type);
    
    if (it != defenseBonus.end())
    {
        return it->second;
    }
    
    return 0;
    
}