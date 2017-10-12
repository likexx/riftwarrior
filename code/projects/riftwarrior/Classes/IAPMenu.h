//
//  IAPMenu.h
//  tdgame
//
//  Created by Like Zhang on 4/22/13.
//
//

#ifndef __tdgame__IAPMenu__
#define __tdgame__IAPMenu__

#include <iostream>
#include "cocos2d.h"
#include "KKMenu.h"
#include "IAPItem.h"
#include <string>
using namespace cocos2d;
using namespace std;


#define IAP_MENU_BACKGROUND_TAG 90000
#define IAP_MENU_TAG 90001

enum ENUM_TRANSACTION_RESULT
{
    ENUM_TRANSACTION_COMPLETED,
    ENUM_TRANSACTION_FAILED,
};

class IAPMenu : public KKMenu
{
public:
    static IAPMenu* create(int priority);
    
    virtual void update(float dt);
    
    bool virtual init(int priority);
    bool init(int priority,CCSprite* background);
    
    void setup();

    void setTransactionCompleted(const char* identifier);
    void setTransactionFailed(const char* identifier);
    
private:
    
    void close(CCObject* pSender);
    void buyItem(CCObject* pSender);
	void switchToCoinStore(CCObject* /* dummy parameter */);
    void closeMenu();
    
    CCNode* m_pContainer;
    
    string m_PendingProductIdentifier;
    bool m_WaitingForTransactionResult;

    ENUM_TRANSACTION_RESULT m_TransactionResult;

    CCMenuItem* m_CloseBtn;
    CCMenuItemImage* m_CoinStore;
    CCLabelTTF* m_CurrentGemsLabel;

};


#endif /* defined(__tdgame__IAPMenu__) */
