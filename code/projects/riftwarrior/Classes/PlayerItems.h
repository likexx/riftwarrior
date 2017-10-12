//
//  PlayerItems.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/4/13.
//
//

#ifndef __ike_tdgame__PlayerItems__
#define __ike_tdgame__PlayerItems__

#include <iostream>
#include "Item.h"

class PlayerItems
{
public:

    inline map<int, int> getItems() const
    {
        return items;
    }
    
    inline int getItemCount(int id)
    {
        map<int, int>::iterator current = items.find(id);
        if (current == items.end())
        {
            return 0;
        }
        
        return current->second;
        
    }
    
    inline void addItem(int id, int number)
    {
        int oldValue = items[id];
        items[id] = oldValue + number;
    }
    
    void removeItem(int id, int number);

private:
    map<int, int> items;
    
    
};

#endif /* defined(__ike_tdgame__PlayerItems__) */
