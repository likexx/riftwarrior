//
//  PlayerItems.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/4/13.
//
//

#include "PlayerItems.h"


void PlayerItems::removeItem(int id, int number)
{
    map<int, int>::iterator current = items.find(id);
    if (current == items.end())
    {
        return;
    }
    
    int newValue = current->second - number;
    if (newValue <= 0)
    {
        items.erase(current);
    }
    else
    {
        items[id] = newValue;
    }
    
    return;
    
}
