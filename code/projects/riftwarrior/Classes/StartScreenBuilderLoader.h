//
//  StartScreenBuilderLoader.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/3/12.
//
//

#ifndef __ike_tdgame__StartScreenBuilderLoader__
#define __ike_tdgame__StartScreenBuilderLoader__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "StartScreen.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class StartScreenBuilderLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StartScreenBuilderLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StartScreen);
};

#endif /* defined(__ike_tdgame__StartScreenBuilderLoader__) */
