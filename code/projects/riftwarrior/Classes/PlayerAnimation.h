//
//  PlayerAnimation.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/1/13.
//
//

#ifndef __ike_tdgame__PlayerAnimation__
#define __ike_tdgame__PlayerAnimation__

#include <iostream>
#include "enums.h"
#include "NpcAnimation.h"
#include <map>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class PlayerAnimation : public NpcAnimation
{
public:
    virtual bool init();
    
    CREATE_FUNC(PlayerAnimation);
    
protected:
    virtual void addShadow();
    virtual void updateSprite();
    
    

    
};

#endif /* defined(__ike_tdgame__PlayerAnimation__) */
