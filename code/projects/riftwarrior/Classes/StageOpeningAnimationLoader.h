//
//  StageOpeningAnimationLoader.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/27/12.
//
//

#ifndef __ike_tdgame__StageOpeningAnimationLoader__
#define __ike_tdgame__StageOpeningAnimationLoader__

#include "StageOpeningAnimation.h"

class CCBReader;

class StageOpeningAnimationLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StageOpeningAnimationLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StageOpeningAnimation);
};

#endif /* defined(__ike_tdgame__StageOpeningAnimationLoader__) */
