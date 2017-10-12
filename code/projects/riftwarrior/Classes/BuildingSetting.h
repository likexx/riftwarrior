//
//  BuildingSetting.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/24/12.
//
//

#ifndef __ike_tdgame__BuildingSetting__
#define __ike_tdgame__BuildingSetting__

#include <iostream>
#include <string>
#include "enums.h"
using namespace std;



struct BuildingSetting
{
    int id;
    string name;
    ENUM_BUILDING_TYPE type;
    string introduction;
    int tileWidth;
    int tileLength;
    float anchorX;
    float anchorY;
    
    int baseCost;
    int baseAttack;
    int baseRange;
    
    ENUM_BUILDING_ATTACK_TYPE attackType;
    ENUM_BULLET_ANIMATION attackBullet;
    
    ENUM_DAMAGE_TYPE damageType;
    float attackAnimationTime;
    float attackInterval;
    string baseSprite;
    int baseHp;
    
    int require;
    int requireLevel;
    
    int bulletSpeed;
    
    int maxLevel;
    
    float baseAnchorX;
    float baseAnchorY;
    
    float defaultScale;
    
    bool airCapable;
    
    int gemCost;
    
    void init(char** row);
};


#endif /* defined(__ike_tdgame__BuildingSetting__) */
