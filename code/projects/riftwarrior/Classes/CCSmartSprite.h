//
//  CCSmartSprite.h
//  tdgame
//
//  Created by Like Zhang on 3/11/13.
//
//

#ifndef __tdgame__CCSmartSprite__
#define __tdgame__CCSmartSprite__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class CCSmartSprite : public CCSprite
{
public:
    
    static CCSmartSprite* create(const char* pFilename);
    static CCSmartSprite* createWithTexture(CCTexture2D* pTexture);
    static CCSmartSprite* createWithSpriteFrameName(const char* pSpriteFrameName);
    static CCSmartSprite* createWithSpriteFrame(CCSpriteFrame* pFrame);
    
    void addToGround();
    void makeVisible();
    void removeSelf();
    
};

#endif /* defined(__tdgame__CCSmartSprite__) */
