//
//  EnemySetting.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/8/13.
//
//

#include "EnemySetting.h"
#include "Function.h"
#include "SystemHelper.h"

void EnemySetting::init(char **row)
{
    int i = 0;
    
    id = atoi(row[i]);
    vector<string> names = Function::splitString(row[i+1], ";");
    
    name = names[SystemHelper::getCurrentLanguage()];

    type = ENUM_ENEMY_TYPE_NORMAL;
    const char* value = row[i+2];
    if (value)
    {
        if (strcmp(value, "air") == 0)
        {
            type = ENUM_ENEMY_TYPE_AIR;
        }
        else if (strcmp(value, "boss") == 0)
        {
            type = ENUM_ENEMY_TYPE_BOSS;
        }
        else if (strcmp(value, "child") == 0)
        {
            type = ENUM_ENEMY_TYPE_CHILD;
        }
        else if (strcmp(value, "special") == 0)
        {
            type = ENUM_ENEMY_TYPE_SPECIAL;
        }
    }
    
    hp = atoi(row[i+3]);
    attack = atoi(row[i+4]);
    attackRange = atoi(row[i+5]);
    requireBaseLevel = row[i+6] == NULL ? 0 : atoi(row[i+6]);
    
    
    baseExp = atoi(row[i+7]);

    vector<int> values = Function::splitStringToIntegers(row[i+8], ",");
    dropMoney = values[0];
    dropGem = values.size()>0 ? values[1] : 0;

    if (row[i+9] && strlen(row[i+9])>0)
    {
        vector<string> values = Function::splitString(row[i+9], ";");
        introduction = values[SystemHelper::getCurrentLanguage()];
    }
    
    moveSpeed = atoi(row[i+10]);
    
    value = (row[i+11]==NULL || strlen(row[i+11])<1) ? NULL : row[i+11];
    if (!value)
    {
        attackType = ENUM_ENEMY_ATTACK_MELEE;
    }
    else
    {
        if (strcmp(value, "melee")==0)
        {
            attackType= ENUM_ENEMY_ATTACK_MELEE;
        }
        else if (strcmp(value, "throw_stone")==0)
        {
            attackType = ENUM_ENEMY_ATTACK_THROW_STONE;
        }
        else if (strcmp(value, "throw_spear")==0)
        {
            attackType = ENUM_ENEMY_ATTACK_THROW_SPEAR;
        }
        else if (strcmp(value, "arrow")==0)
        {
            attackType = ENUM_ENEMY_ATTACK_ARROW;
        }
        else if (strcmp(value, "half_circle_melee")==0)
        {
            attackType = ENUM_ENEMY_ATTACK_HALF_CIRCLE_MELEE;
        }
        else if (strcmp(value, "round_area") == 0)
        {
            attackType = ENUM_ENEMY_ATTACK_ROUND_AREA;
        }
        else if (strcmp(value, "snake_venom") == 0)
        {
            attackType = ENUM_ENEMY_ATTACK_SNAKE_VENOM;
        }
        else if (strcmp(value, "spider_web") == 0)
        {
            attackType = ENUM_ENEMY_ATTACK_SPIDER_WEB;
        }
        else if (strcmp(value, "shock_wave") == 0)
        {
            attackType = ENUM_ENEMY_ATTACK_SHOCK_WAVE;
        }
        
        attackSpeed = (row[i+12]==NULL || strlen(row[i+12])<1) ? 0 : atoi(row[i+12]);
    }
    
    anchorX = (row[i+13] == NULL || strlen(row[i+13])<1) ? 0.5 : atof(row[i+13]);
    anchorY = (row[i+14] == NULL || strlen(row[i+14])<1) ? 0.5 : atof(row[i+14]);
    scale = (row[i+15] == NULL || strlen(row[i+15])<1) ? 0.5 : atof(row[i+15]);
    
    value = (row[i+16]==NULL || strlen(row[i+16])<1) ? NULL : row[i+16];

    attackDelay = value ? atof(value) : 0;
    
    int magicTypeValue = (row[i+17]==NULL || strlen(row[i+17])<1) ? 0 : atoi(row[i+17]);
    magicType = (ENUM_MAGIC_TYPE)(ENUM_MAGIC_TYPE_1 + (magicTypeValue - 1));
    
    childId = (row[i+18]==NULL || strlen(row[i+18])<1) ? 0 : atoi(row[i+18]);

}