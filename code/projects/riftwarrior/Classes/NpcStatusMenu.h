//
//  NpcStatusMenu.h
//  tdgame
//
//  Created by Like Zhang on 4/25/13.
//
//

#ifndef __tdgame__NpcStatusMenu__
#define __tdgame__NpcStatusMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;


class NpcStatusMenu : public CCNode
{
public:
    static NpcStatusMenu* create(int id);
    
    virtual bool init(int id);
    virtual void draw();

private:
    int m_NpcId;
};

#endif /* defined(__tdgame__NpcStatusMenu__) */
