//
//  NpcMessageBox.h
//  tdgame
//
//  Created by Like Zhang on 3/27/13.
//
//

#ifndef __tdgame__NpcMessageBox__
#define __tdgame__NpcMessageBox__

#include <iostream>
#include "MessageBox.h"
using namespace cocos2d;

class NpcMessageBox : public MessageBox
{
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void showMessage(const char* pImagePath, const char* message);
    virtual void close(CCObject* pSender);

    CREATE_FUNC(NpcMessageBox);
    
protected:
};


#endif /* defined(__tdgame__NpcMessageBox__) */
