//
//  BuildingSetting.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/24/12.
//
//

#include "BuildingSetting.h"
#include "SystemHelper.h"
#include "Function.h"
#include <vector>
using namespace std;

void BuildingSetting::init(char **row)
{
    int j = 0;
    id = atoi(row[j]);
    
    const char* value = row[j+1];
    
    vector<string> values = Function::splitString(value, ";");
    name = values[SystemHelper::getCurrentLanguage()];
    
    value = row[j+2];
    if (strcmp(value, "missle")==0)
    {
        type = ENUM_WEAPON_MISSLE;
    }
    else if (strcmp(value, "snipe")==0)
    {
        type = ENUM_WEAPON_SNIPE;
    }
    else if (strcmp(value, "shot")==0)
    {
        type = ENUM_WEAPON_SHOT;
    }
    else if (strcmp(value, "assist")==0)
    {
        type = ENUM_BUILDING_ASSIST;
    }
    else if (strcmp(value, "special")==0)
    {
        type = ENUM_BUILDING_SPECIAL;
    }
    
    if (row[j+3])
    {
        vector<string> values = Function::splitString(row[j+3], ";");
        introduction = values[SystemHelper::getCurrentLanguage()];
    }

    tileWidth = atoi(row[j+4]);
    tileLength = atoi(row[j+5]);
    anchorX = atof(row[j+6]);
    anchorY = atof(row[j+7]);
    baseCost = atoi(row[j+8]);
    
    if (row[j+9])
    {
        baseAttack = atoi(row[j+9]);
    }
    else
    {
        baseAttack = 0;
    }
    
    if (row[j+10])
    {
        baseRange = atoi(row[j+10]);
    }
    else
    {
        baseRange = 0;
    }
    
    attackType = ENUM_BUILDING_ATTACK_NONE;
    value = row[j+11];
    if (value && strlen(value)>0)
    {
        if (strcmp(value, "throw_stone")==0)
        {
            attackType = ENUM_BUILDING_ATTACK_THROW_STONE;
        }
        else if (strcmp(value, "cannon") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_CANNON;
        }
        else if (strcmp(value, "single_arrow")==0)
        {
            attackType = ENUM_BUILDING_ATTACK_SINGLE_ARROW;
        }
        else if (strcmp(value, "round") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_ROUND;
        }
        else if (strcmp(value, "multi_arrow") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_MULTI_ARROW;
        }
        else if (strcmp(value, "shotgun") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_SHOTGUN;
        }
        else if (strcmp(value, "mud") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_MUD;
        }
        else if (strcmp(value, "firegun") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_FIREGUN;
        }
        else if (strcmp(value, "poison") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_POISON;
        }
        else if (strcmp(value, "sniper") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_SNIPER;
        }
        else if (strcmp(value, "machinegun") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_MACHINEGUN;
        }
        else if (strcmp(value, "rocket") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_ROCKET;
        }
        else if (strcmp(value, "base") == 0)
        {
            attackType = ENUM_BUILDING_ATTACK_BASE;
        }

        value = row[j+12];
        if (value)
        {
            if (strcmp(value, "stone")==0)
            {
                attackBullet = ENUM_BULLET_ANIMATION_STONE;
            }
            else if (strcmp(value, "arrow") == 0)
            {
                attackBullet = ENUM_BULLET_ANIMATION_ARROW;
            }
            else if (strcmp(value, "fire_arrow") == 0)
            {
                attackBullet = ENUM_BULLET_ANIMATION_FIRE_ARROW;
            }
            else if (strcmp(value, "fire_stone") == 0)
            {
                attackBullet = ENUM_BULLET_ANIMATION_FIRE_STONE;
            }
            else if (strcmp(value, "firegun") == 0)
            {
                attackBullet = ENUM_BULLET_ANIMATION_FIREGUN_BULLET;
            }
            else if (strcmp(value, "rocket") == 0)
            {
                attackBullet = ENUM_BULLET_ANIMATION_ROCKET;
            }
            else if (strcmp(value, "sniper_fire") == 0)
            {
                attackBullet = ENUM_BULLET_ANIMATION_SNIPER_FIRE;
            }
        }
        
        
        value = row[j+13];
        if (strcmp(value, "single_tile") == 0)
        {
            damageType = ENUM_DAMAGE_SINGLE_TILE;
        }
        else if (strcmp(value, "single_enemy") == 0)
        {
            damageType = ENUM_DAMAGE_SINGLE_ENEMY;
        }
        else if (strcmp(value, "square_tiles") == 0)
        {
            damageType = ENUM_DAMAGE_SQUARE_TILES;
        }
        else if (strcmp(value, "round_area") == 0)
        {
            damageType = ENUM_DAMAGE_ROUND_AREA;
        }
        else if (strcmp(value, "burning_fire") == 0)
        {
            damageType = ENUM_DAMAGE_BURNING_FIRE;
        }
        
        attackAnimationTime = atof(row[j+14]);
        
        if (row[j+15] == NULL || strcmp(row[j+15], "")==0)
        {
            attackInterval = 0;
        }
        else
        {
            attackInterval = atof(row[j+15]);
        }
        
    }

    value = row[j+16];
    if (!value || strcmp(value,"")==0)
    {
        baseSprite = "";
    }
    else
    {
        baseSprite = value;
    }

    value = row[j+17];
    
    baseHp = atoi(value);
    
    value = row[j+18];
    require = (!value || strlen(value)<1) ? 0 : atoi(value);

    value = row[j+19];
    requireLevel = (!value || strlen(value)<1) ? 0 : atoi(value);
    
    value = row[j+20];
    bulletSpeed = (!value || strlen(value)<1) ? 0 : atoi(value);
 
    value = row[j+21];
    maxLevel = (!value || strlen(value)<1) ? 0 : atoi(value);
    
    value = row[j+22];
    baseAnchorX = (!value || strlen(value)<1) ? 0.5 : atof(value);

    value = row[j+23];
    baseAnchorY = (!value || strlen(value)<1) ? 0.5 : atof(value);
    
    value = row[j+24];
    defaultScale = (!value || strlen(value)<1) ? 0.5 : atof(value);
    
    value = row[j+25];
    airCapable = (!value || strlen(value)<1) ? false : (atoi(value) == 1);
    
    value = row[j+26];
    gemCost = (!value || strlen(value)<1) ? 0 : atoi(value);
    
}
