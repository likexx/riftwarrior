//
//  KKIAPWrapper.cpp
//  tdgame
//
//  Created by Like Zhang on 4/22/13.
//
//

#include "KKIAPWrapper.h"
#include <StoreKit/StoreKit.h>
#include "KKIAPHelper.h"
#include "MainUI.h"
#include "GameScene.h"
#include "IAPMenu.h"
#include "Player.h"

static map<string, IAPItem> _allItems;

void KKIAPWrapper::addItem(const IAPItem &item)
{
    _allItems[item.productIdentifer] = item;
}

const map<string, IAPItem>& KKIAPWrapper::getIAPItems()
{
    return _allItems;
}

bool KKIAPWrapper::buyItem(const char *productIdentifier)
{
    NSString* identifier = [NSString stringWithUTF8String:productIdentifier];
    [[KKIAPHelper sharedInstance] buyProductByIdentifier:identifier];
    
    return true;
}


void KKIAPWrapper::notifyTransactionCompleted(const char* identifier)
{

    map<string, IAPItem>::iterator it = _allItems.find(identifier);
    if (it != _allItems.end())
    {
        IAPItem& item = it->second;
        Player::getInstance()->updateGems(item.gems);
        Player::getInstance()->updateMoney(item.coins);
//        Player::getInstance()->saveGems();
        Player::getInstance()->saveMoney();
    }
    
    if (GameScene::getInstance()->sharedMainUI &&
        GameScene::getInstance()->sharedMainUI->getChildByTag(IAP_MENU_BACKGROUND_TAG) &&
        GameScene::getInstance()->sharedMainUI->getChildByTag(IAP_MENU_BACKGROUND_TAG)->getChildByTag(IAP_MENU_TAG))
    {
        IAPMenu* pMenu = static_cast<IAPMenu*>(GameScene::getInstance()->sharedMainUI->getChildByTag(IAP_MENU_BACKGROUND_TAG)->getChildByTag(IAP_MENU_TAG));
        
        if (!pMenu)
        {
            return;
        }
        
        pMenu->setTransactionCompleted(identifier);
        
    }
    
}


void KKIAPWrapper::notifyTransactionFailed(const char* identifier)
{
    
    if (GameScene::getInstance()->sharedMainUI &&
        GameScene::getInstance()->sharedMainUI->getChildByTag(IAP_MENU_BACKGROUND_TAG) &&
        GameScene::getInstance()->sharedMainUI->getChildByTag(IAP_MENU_BACKGROUND_TAG)->getChildByTag(IAP_MENU_TAG))
    {
        IAPMenu* pMenu = static_cast<IAPMenu*>(GameScene::getInstance()->sharedMainUI->getChildByTag(IAP_MENU_BACKGROUND_TAG)->getChildByTag(IAP_MENU_TAG));
        
        if (!pMenu)
        {
            return;
        }
        
        pMenu->setTransactionFailed(identifier);
    }
    
        
}