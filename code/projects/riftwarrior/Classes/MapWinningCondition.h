//
//  MapWinningCondition.h
//  tdgame
//
//  Created by Like Zhang on 4/25/13.
//
//

#ifndef __tdgame__MapWinningCondition__
#define __tdgame__MapWinningCondition__

#include <iostream>
#include <string>
#include <set>
using namespace std;

struct MapWinningCondition
{
    MapWinningCondition();
    
    int mapId;
    bool killPlayer;
    int killNpcId;
    int maxPassEnemies;
    int minKilledEnemies;
    set<int> restrictedWeapons;
    int maxTime; // in seconds
    bool destroyBase;
    
    void init(char** row);
    MapWinningCondition& operator=(const MapWinningCondition& condition);
};

#endif /* defined(__tdgame__MapWinningCondition__) */
