//
//  Magic.cpp
//  tdgame
//
//  Created by Like Zhang on 3/5/13.
//
//

#include "Magic.h"
#include "GameData.h"

void Magic::init(char **row)
{
    int j = 0;
    id = atoi(row[j]);
    name = row[j+1];

    performEffect = row[j+2];
    explosionEffect = row[j+3];
    offsetY = atof(row[j+4]);

    requireLevel = (row[j+5] == NULL || strlen(row[j+5])<1) ? 0 : atoi(row[j+5]);
    require = (row[j+6] == NULL || strlen(row[j+6])<1) ? 0 : atoi(row[j+6]);
    
}


