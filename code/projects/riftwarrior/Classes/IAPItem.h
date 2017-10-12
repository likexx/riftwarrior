//
//  IAPItem.h
//  tdgame
//
//  Created by Like Zhang on 4/22/13.
//
//

#ifndef __tdgame__IAPItem__
#define __tdgame__IAPItem__

#include <iostream>
#include <string>
using namespace std;

struct IAPItem
{
    string productIdentifer;
    string title;
    string description;
    string price;
    
    int gems;
    int coins;
};

#endif /* defined(__tdgame__IAPItem__) */
