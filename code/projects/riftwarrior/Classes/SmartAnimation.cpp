//
//  SmartAnimation.cpp
//  tdgame
//
//  Created by Like Zhang on 3/27/13.
//
//

#include "SmartAnimation.h"

SmartAnimation* SmartAnimation::create(float duration, bool fadeOut)
{
    SmartAnimation* pNode = new SmartAnimation();
    if (pNode && pNode->init(duration, fadeOut))
    {
        pNode->autorelease();
        return pNode;
    }
    
    CC_SAFE_DELETE(pNode);
    return NULL;
}

bool SmartAnimation::init(float duration, bool fadeOut)
{
    m_Duration = duration;
    m_FadeOut = fadeOut;
    
    return true;
}

void SmartAnimation::removeSelf()
{
    removeFromParentAndCleanup(true);
}
