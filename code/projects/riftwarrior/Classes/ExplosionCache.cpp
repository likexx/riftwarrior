//
//  ExplosionCache.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/23/12.
//
//

#include "ExplosionCache.h"

typedef map<string, queue<Explosion*>*>::iterator CacheIterator;

ExplosionCache* ExplosionCache::getInstance()
{
    static ExplosionCache* cache;
    
    if (!cache) {
        cache = new ExplosionCache();
    }
    
    return cache;
}

void ExplosionCache::init()
{
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();


    frameCache->addSpriteFramesWithFile("effects/hit.plist", "effects/hit.pvr.ccz");

    for(int i=0;i<50;++i)
    {
        Explosion* explosion = new Explosion();
        explosion->initWithAnimationName("building_explosion", 1, 1, false);
        explosion->getSprite()->setAnchorPoint(ccp(0.5,0.1));
        addExplosion(explosion);
    }

    for(int i=0;i<50;++i)
    {
        Explosion* explosion = new Explosion();
        explosion->initWithAnimationName("hit_effect_explosion1", 1, 1, false);
        explosion->getSprite()->setAnchorPoint(ccp(0.5,0.3));
        addExplosion(explosion);
    }

    for(int i=0;i<50;++i)
    {
        Explosion* explosion = new Explosion();
        explosion->initWithAnimationName("hit_effect_dusty", 1, 1, false);
        explosion->getSprite()->setAnchorPoint(ccp(0.5,0.3));
        addExplosion(explosion);
    }


    for(int i=0;i<50;++i)
    {
        Explosion* explosion = new Explosion();
        explosion->initWithAnimationName("hit_effect_venom", 1, 1,false);
        explosion->getSprite()->setAnchorPoint(ccp(0.5,0.5));
        addExplosion(explosion);
    }


    for(int i=0;i<50;++i)
    {
        Explosion* explosion = new Explosion();
        explosion->initWithAnimationName("hit_effect_rocket_explosion", 1, 1,false);
        explosion->getSprite()->setAnchorPoint(ccp(0.5,0.5));
        addExplosion(explosion);
    }

}

Explosion* ExplosionCache::getExplosion(const char *name)
{
    CacheIterator it = cache.find(name);
    if (it==cache.end())
    {
        return NULL;
    }
    
    queue<Explosion*>* queue = it->second;
    Explosion* explosion = NULL;
    
    if (queue->size() > 0)
    {
        explosion = queue->front();
        queue->pop();
    }
    
    return explosion;
}

void ExplosionCache::addExplosion(const char *animFileName, int rows, int cols, int width, int height, float scale)
{
    Explosion* explosion = new Explosion();
    explosion->initWithAnimation(animFileName, rows, cols, width, height, scale);
    addExplosion(explosion);
}

void ExplosionCache::addExplosion(Explosion *explosion)
{
    CacheIterator it = cache.find(explosion->getName());
    queue<Explosion*>* pool = NULL;
    
    if (it==cache.end())
    {
        pool = new queue<Explosion*>();
        cache.insert(pair<string, queue<Explosion*>* >(explosion->getName(), pool));
    }
    else
    {
        pool = it->second;
    }

    pool->push(explosion);
    
}

