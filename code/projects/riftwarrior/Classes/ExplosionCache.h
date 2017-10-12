//
//  ExplosionCache.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/23/12.
//
//

#ifndef __ike_tdgame__ExplosionCache__
#define __ike_tdgame__ExplosionCache__

#include <iostream>
#include "Explosion.h"
#include <queue>
#include <map>
#include <string>
using namespace std;

class ExplosionCache
{
public:
    static ExplosionCache* getInstance();
    Explosion* getExplosion(const char* name);
    void init();
    void addExplosion(const char* animFileName, int rows, int cols, int width, int height, float scale);
    void addExplosion(Explosion* explosion);

private:
    map< string, queue<Explosion*>* > cache;

};

#endif /* defined(__ike_tdgame__ExplosionCache__) */
