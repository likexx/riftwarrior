//
//  MapWinningCondition.cpp
//  tdgame
//
//  Created by Like Zhang on 4/25/13.
//
//

#include "MapWinningCondition.h"
#include "Function.h"
#include "SystemHelper.h"

MapWinningCondition::MapWinningCondition()
{
    killPlayer = true;
    killNpcId = 0;
    minKilledEnemies = 0;
    maxPassEnemies = 0;
    maxTime = 0;
}

void MapWinningCondition::init(char **row)
{
    int i = 0;
    mapId = atoi(row[i]);

    killPlayer = false;
    const char* value = row[i+1];
    if (value)
    {
        killPlayer = atoi(value) > 0 ? true : false;
    }
    
    killNpcId = 0;
    value = row[i+2];
    if (value)
    {
        killNpcId = atoi(value);
    }
    
    maxPassEnemies = 0;
    value = row[i+3];
    if (value)
    {
        maxPassEnemies = atoi(value);
    }
    
    minKilledEnemies = 0;
    value = row[i+4];
    if (value)
    {
        minKilledEnemies = atoi(value);
    }

    value=row[i+5];
    if (value)
    {
		for (const int& value : Function::splitStringToIntegers(value, ","))
        {
            restrictedWeapons.insert(value);
        }
    }
    
    maxTime = 0;
    value = row[i+6];
    if(value)
    {
        maxTime = atoi(value);
    }
    
    destroyBase = false;
    value = row[i+7];
    if (value)
    {
        destroyBase = atoi(value) > 0;
    }
    
}

MapWinningCondition& MapWinningCondition::operator=(const MapWinningCondition &condition)
{
    if (this == &condition)
    {
        return *this;
    }
    
    killPlayer = condition.killPlayer;
    killNpcId = condition.killNpcId;
    maxPassEnemies = condition.maxPassEnemies;
    minKilledEnemies = condition.minKilledEnemies;
    restrictedWeapons = condition.restrictedWeapons;
    maxTime = condition.maxTime;
    
    return *this;
}
