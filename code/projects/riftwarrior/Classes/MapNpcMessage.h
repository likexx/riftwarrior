//
//  LevelNpcMessage.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/27/12.
//
//

#ifndef __ike_tdgame__LevelNpcMessage__
#define __ike_tdgame__LevelNpcMessage__

#include <iostream>
#include <string>
using namespace std;

struct MapNpcMessage
{
    int mapId;
    int sequence;
    int wave;
    string npcImage;
    string npcName;
    string message;
    
    int tileX;
    int tileY;
    
};

#endif /* defined(__ike_tdgame__LevelNpcMessage__) */
