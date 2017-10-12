//
//  EnemyCache.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/8/13.
//
//

#include "EnemyCache.h"
#include <queue>
using namespace std;

static map< int, queue<Enemy*>* > _enemyCache;


void EnemyCache::init()
{
    
}

void EnemyCache::clear()
{
    map< int, queue<Enemy*>* >::iterator it = _enemyCache.begin();
    
    while (it != _enemyCache.end())
    {
        queue<Enemy*>* enemies = it->second;
        while ( !enemies->empty() )
        {
            Enemy* pEnemy = enemies->front();
            enemies->pop();
            delete pEnemy;
        }
        
        delete enemies;
        it++;
    }
    
    _enemyCache.clear();
}

void EnemyCache::put(Enemy *pEnemy)
{
    // try find queue
    queue<Enemy*>* enemyQueue = NULL;
    
    map<int, queue<Enemy*>* >::iterator it = _enemyCache.find(pEnemy->getId());
    if (it == _enemyCache.end())
    {
        // no queue
        enemyQueue = new queue<Enemy*>();
        _enemyCache[pEnemy->getId()] = enemyQueue;
    }
    else
    {
        enemyQueue = it->second;
    }
    
    enemyQueue->push(pEnemy);
}

Enemy* EnemyCache::get(int id)
{
    queue<Enemy*>* enemyQueue = NULL;

    map<int, queue<Enemy*>* >::iterator it = _enemyCache.find(id);

    if (it == _enemyCache.end())
    {
        // create queue for the enemy and return NULL
        enemyQueue = new queue<Enemy*>();
        _enemyCache[id] = enemyQueue;
        return NULL;
    }
    
    enemyQueue = it->second;
    
    if (enemyQueue->empty())
    {
        return NULL;
    }
    
    Enemy* pEnemy = enemyQueue->front();
    enemyQueue->pop();
    return pEnemy;
}
