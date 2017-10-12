//
//  Unit.h
//  tdgame
//
//  Created by Like Zhang on 5/9/13.
//
//

#ifndef __tdgame__Unit__
#define __tdgame__Unit__

#include <iostream>
#include "cocos2d.h"
#include "Player.h"

using namespace cocos2d;

class Unit : public Enemy
{
public:
    
    static Unit* create(int id, CCPoint startTilePos);
    
    Unit(int id);
    
    virtual bool init();
    
    virtual void update(float dt);
    virtual void remove();

    
private:
    virtual void createAttackComponent();

    virtual void updateDestination();
    
};
#endif /* defined(__tdgame__Unit__) */
