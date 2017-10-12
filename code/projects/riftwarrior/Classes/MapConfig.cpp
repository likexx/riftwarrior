//
//  LevelConfig.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/28/12.
//
//

#include "MapConfig.h"
#include "Function.h"
#include "SystemHelper.h"

void MapConfig::init(char **row)
{
    int j = 0;
    mapId = atoi(row[j]);

    vector<string> values = Function::splitString(row[j+1], ";");
    name = values[SystemHelper::getCurrentLanguage()];

    if (row[j+2] && strlen(row[j+2])>1)
    {
        values = Function::splitString(row[j+2], ";");
        introduction = values[SystemHelper::getCurrentLanguage()];
        
    }
    
    if (row[j+3])
    {
        maxDiamondsAward = atoi(row[j+3]);
    }

    maxLevel = atoi(row[j+4]);

    enemies = Function::splitString(row[j+5], ";");
    
    requireMapId = atoi(row[j+6]);
    worldmapX = atoi(row[j+7]);
    worldmapY = atoi(row[j+8]);

    environment = ENUM_MAP_ENVIRONMENT_NORMAL;
    const char* value = row[j+9];
    if (value)
    {
        if (strcmp(value, "undersea")==0)
        {
            environment = ENUM_MAP_ENVIRONMENT_UNDERSEA;
        }
        else if (strcmp(value, "rain")==0)
        {
            environment = ENUM_MAP_ENVIRONMENT_RAIN;
        }
        else if (strcmp(value, "bloodrain")==0)
        {
            environment = ENUM_MAP_ENVIRONMENT_BLOOD_RAIN;
        }
        else if (strcmp(value, "sandywind")==0)
        {
            environment = ENUM_MAP_ENVIRONMENT_SANDY_WIND;
        }
    }
    
    bonusMoney = atoi(row[j+10]);
    bonusExperience = atoi(row[j+11]);
    
    value = row[j+12];
    
    if (value && strlen(value)>0)
    {
        char name[128]={0};
        sprintf(name, "music/%s.mp3", value);
        music = name;
    }
    else
    {
        char name[128]={0};
        sprintf(name, "music/map%d.mp3", mapId);
        music = name;
    }
    
    hideAfter = (!row[j+13] || strlen(row[j+13])<1) ? 0 : atoi(row[j+13]);
    
    price = (!row[j+14] || strlen(row[j+14])<1) ? 0 : atoi(row[j+14]);
}