//
//  LevelConfig.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/28/12.
//
//

#ifndef __ike_tdgame__LevelConfig__
#define __ike_tdgame__LevelConfig__

#include <iostream>
#include <string>
#include <vector>
#include "enums.h"
using namespace std;

struct MapConfig
{
    int mapId;
    string name;
    string introduction;
    int maxDiamondsAward;
    int maxLevel;
    vector< string > enemies;
    
    int requireMapId;
    int worldmapX;
    int worldmapY;
    
    ENUM_MAP_ENVIRONMENT environment;
    
    int bonusMoney;
    int bonusExperience;
    
    string music;
    
    int hideAfter;
    
    int price;
    
    void init(char** row);
};

#endif /* defined(__ike_tdgame__LevelConfig__) */
