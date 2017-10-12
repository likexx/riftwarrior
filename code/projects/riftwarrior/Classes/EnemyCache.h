//
//  EnemyCache.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/8/13.
//
//

#ifndef __ike_tdgame__EnemyCache__
#define __ike_tdgame__EnemyCache__

#include <iostream>
#include "Enemy.h"
#include "EnemySetting.h"
#include "GameData.h"
#include <map>
#include <queue>
using namespace std;

class EnemyCache
{
public:
    static void init();
    static Enemy* get(int id);
    static void put(Enemy* pEnemy);
    static void clear();
    
private:
};

#endif /* defined(__ike_tdgame__EnemyCache__) */
