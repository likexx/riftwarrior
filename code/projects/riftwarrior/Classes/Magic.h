//
//  Magic.h
//  tdgame
//
//  Created by Like Zhang on 3/5/13.
//
//

#ifndef __tdgame__Magic__
#define __tdgame__Magic__

#include <iostream>
#include <string>
using namespace std;

struct Magic
{
    int id;
    string name;
    string performEffect;
    string explosionEffect;
    float offsetY;
    int requireLevel;
    int require;
    
    void init(char** row);
    
};
#endif /* defined(__tdgame__Magic__) */
