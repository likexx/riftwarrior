//
//  SuperStoreMenu.h
//  tdgame
//
//  Created by Wang Nero on 13-7-11.
//
//

#ifndef __tdgame__SuperStoreMenu__
#define __tdgame__SuperStoreMenu__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

#define SALE_STORE_TAG 20001

class SaleStore : public CCNode
{
public:
    virtual bool init();
    CREATE_FUNC(SaleStore);
    static void createAndShow();
    void clearALLTab();
    void setActionBeforeClose(CCActionInterval *const action);
    void switchToGemstore();
    void switchToCoinStore();
private:
    void close();
    void closeAction();
    bool createBackground();
    bool createSwitchButton();
    void createTitle();

    void showWarningMessage(const char *msg);
    bool turnGemToCoin(int numberOfGems,int numberOfCoins);   
    bool createStoreTab(int i, const char *tabName,const char *tabIntro,const char *tabIcon,const char *buttonLab,CCObject *handler,SEL_MenuHandler delegte);
    void buySmallCoin();
    void buyNormalCoin();
    void buyFullCoin();
    void buyHugeCoin();
    CCActionInterval* actionBeforeClose;
    
    
    CCSprite* m_pBackground;
};


#endif /* defined(__tdgame__SuperStoreMenu__) */
