//
//  EnemySetting.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/8/13.
//
//

#ifndef __ike_tdgame__EnemySetting__
#define __ike_tdgame__EnemySetting__

#include <iostream>
#include <string>
#include "enums.h"
using namespace std;

struct EnemySetting
{
    int id;
    string name;
    ENUM_ENEMY_TYPE type;
    int hp;
    float attack;
    float attackRange;
    int requireBaseLevel;
    
    int baseExp;
    int dropMoney;
    int dropGem;

    string introduction;
    
    int moveSpeed;
    
    ENUM_ENEMY_ATTACK_TYPE attackType;
    int attackSpeed;
    
    float anchorX;
    float anchorY;
    float scale;
    
    float attackDelay;
    
    ENUM_MAGIC_TYPE magicType;
    
    int childId;
    
    void init(char** row);

};

#endif /* defined(__ike_tdgame__EnemySetting__) */
