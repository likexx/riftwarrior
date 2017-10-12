//
//  MessageBox.h
//  tdgame
//
//  Created by Like Zhang on 3/17/13.
//
//

#ifndef __tdgame__MessageBox__
#define __tdgame__MessageBox__

#include <iostream>
#include "cocos2d.h"
#include <string>
#include <queue>

using namespace cocos2d;
using namespace std;

struct MessageIcon
{
    string name;
    string description;
    string imagePath;
};


class MessageBox : public CCNode, public CCTouchDelegate
{
public:
    virtual bool init();
    CREATE_FUNC(MessageBox);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    virtual void showMessage(const char* pImagePath, const char* message, bool pauseGame);
    virtual void close();
	virtual void close(CCObject * /* dummy parameter to get compile macro pass. */);

    void showTextMessage(const char* message, bool pauseGame);
    
    void showMessageWithIcons(const char* message, const vector<MessageIcon>& icons, bool pauseGame);
    
    void showMessageIcons();
    
    void showEnemyIntroduction(int id);
    
    void setCallback(CCCallFunc* callback);
    
    void resetPosition();
    
    inline void setMessage(const char* pMessage)
    {
        m_pMessageToShow = pMessage;
    }
    
    inline void addMessageIcon(const MessageIcon& icon)
    {
        m_MessageIcons.push(icon);
    }
    
protected:
    void resetCallback();
    void removeAllIcons();

    CCSprite* m_pImageIcon;
    CCLabelTTF* m_pTextLabel;
    CCSprite* m_pBackground;
    
    const char* m_pMessageToShow;
    queue<MessageIcon> m_MessageIcons;
    
    CCCallFunc* m_Callback;
};


#endif /* defined(__tdgame__MessageBox__) */
