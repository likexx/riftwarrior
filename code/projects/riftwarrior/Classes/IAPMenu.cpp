//
//  IAPMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 4/22/13.
//
//

#include "IAPMenu.h"
#include "GameScene.h"
#include "KKIAPWrapper.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "SaleStoreMenu.h"
#include "constant.h"

#define IAP_ITEM_TAG_START 1000
#define IAP_WAITNG_RESULT_TAG 2000

#define ITEM099 "KKRiftLordGems099"
#define ITEM199 "KKRiftLordGems199"

IAPMenu* IAPMenu::create(int priority)
{
    IAPMenu* menu = new IAPMenu();
    
    if (menu && menu->init(priority))
    {
        menu->autorelease();
        return menu;
    }
    
    CC_SAFE_DELETE(menu);
    
    return NULL;
}

bool IAPMenu::init(int priority)
{
    if (!KKMenu::init(priority))
    {
        return false;
    }
    
    setup();
    
    return true;
}

void IAPMenu::setup()
{
    CCSprite* background = CCSprite::create("UI/iap/store_background.png");
    
    const map<string, IAPItem>& iapItems = KKIAPWrapper::getIAPItems();
    map<string, IAPItem>::const_iterator it = iapItems.begin();
    
    float itemX = background->getContentSize().width * 0.2;
    float itemY = background->getContentSize().height * 0.79;
    float yDistance = background->getContentSize().height * 0.20;
    
    int i = 0;
    while(it != iapItems.end())
    {
        const IAPItem* pItem = &it->second;
        
        char buffer[128] = {0};
        sprintf(buffer, "UI/iap/gem%d.png", i);
        
        CCLabelTTF* nameLabel = CCLabelTTF::create(pItem->title.c_str(), STANDARD_NUMBER_FONT_NAME, 22);
        nameLabel->setScale(0.75);
        nameLabel->setColor(ccc3(0, 0, 0));
        nameLabel->setPosition(ccp(itemX , itemY - nameLabel->getContentSize().height));
        background->addChild(nameLabel);
        
        CCSprite* itemIcon = CCSprite::create(buffer);
        itemIcon->setPosition(ccp(itemX, itemY + itemIcon->getContentSize().height * 0.15));
        background->addChild(itemIcon);
        
        CCLabelTTF* introduction = CCLabelTTF::create(pItem->description.c_str(),STANDARD_NUMBER_FONT_NAME, 22);
        introduction->setColor(ccc3(0, 0, 0));
        introduction->setScale(0.75);
        introduction->setPosition(ccp(itemX * 2.25, itemY));
        background->addChild(introduction);

        
        CCMenuItemImage* btnImg = CCMenuItemImage::create("UI/iap/buybutton_normal.png", "UI/iap/buybutton_pressed.png");
        //btnImg->setAnchorPoint(CCPointZero);
        CCPoint pos = ccp(itemX * 3.75, itemY);
        btnImg->setPosition(pos);
        addChild(btnImg, 0, IAP_ITEM_TAG_START + i);
        
        CCLabelTTF* label = CCLabelTTF::create(pItem->price.c_str(), STANDARD_NUMBER_FONT_NAME, 20);
        // label->setColor(ccc3(0, 0, 0));
        label->setPosition(ccp(btnImg->getContentSize().width/2, btnImg->getContentSize().height/2));
        btnImg->addChild(label);
        
        btnImg->setTarget(this, menu_selector(IAPMenu::buyItem));
        
        itemY -= yDistance * (1 + 0.1*i);
        ++it;
        ++i;
    }
    
    do {
        CCMenuItemImage* coinsStore = CCMenuItemImage::create("UI/skilltree/reset_button.png", "UI/skilltree/reset_button_pressed.png");
        CC_BREAK_IF(!coinsStore);
        coinsStore->setScale(0.85f);
        coinsStore->setAnchorPoint(ccp(1.0f, 1.0f));
        coinsStore->setPosition(ccp(387.0f,535.0f));
        coinsStore->setTarget(this, menu_selector(IAPMenu::switchToCoinStore));
        addChild(coinsStore);
        CCLabelTTF* coinsLab = CCLabelTTF::create(GameData::getText("store_coins"), "Arial Bold", 20);
        coinsLab->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCScaleTo::create(0.5f, 1.1f),CCScaleTo::create(0.5f, 1.0f),NULL)));
        coinsLab->setAnchorPoint(ccp(0.5f, 0.5f));
        coinsLab->setPosition(ccp(coinsStore->getContentSize().width/2, coinsStore->getContentSize().height/2));
        m_CoinStore = coinsStore;
        coinsStore->addChild(coinsLab);
    } while (false);

    CCLabelTTF* gemsTitle = CCLabelTTF::create(GameData::getText("store_GemsName"), STANDARD_NUMBER_FONT_NAME, 30);
    gemsTitle->setAnchorPoint(ccp(0.0f, 1.0f));
    gemsTitle->setPosition(ccp(21.0f, 530.0f));
    background->addChild(gemsTitle);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    background->setPosition(ccp(winSize.width/2, winSize.height));
    background->setAnchorPoint(ccp(0.5,1));
    
    this->setAnchorPoint(CCPointZero);
    this->setPosition(CCPointZero);
    background->addChild(this, 0, IAP_MENU_TAG);
    
    CCMenuItemImage* closeBtn = CCMenuItemImage::create("UI/iap/closebutton_normal.png", "UI/iap/closebutton_pressed.png");
    closeBtn->setPosition(ccp(background->getContentSize().width, background->getContentSize().height));
    closeBtn->setAnchorPoint(ccp(0,1));
    addChild(closeBtn);
    closeBtn->setTarget(this, menu_selector(IAPMenu::close));
    
    m_CloseBtn = closeBtn;
    
   /* m_CurrentGemsLabel = CCLabelTTF::create("", STANDARD_NUMBER_FONT_NAME, 24);
    m_CurrentGemsLabel->setPosition(ccp(background->getContentSize().width * 0.8, background->getContentSize().height * 0.125));
    m_CurrentGemsLabel->setAnchorPoint(ccp(1,0.5));
    background->addChild(m_CurrentGemsLabel);
    */
    
    GameScene::getInstance()->sharedMainUI->addChild(background, 0, IAP_MENU_BACKGROUND_TAG);
    
    float scale = winSize.height/640;
    background->setScale(scale);
    
    background->setPosition(ccp(winSize.width/2, winSize.height + background->getContentSize().height * scale));
    background->runAction(CCMoveTo::create(0.2f, ccp(winSize.width/2, winSize.height)));

    m_pContainer = background;
    m_WaitingForTransactionResult = false;
    
    scheduleUpdate();
}

void IAPMenu::buyItem(cocos2d::CCObject *pSender)
{
    if (m_WaitingForTransactionResult)
    {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    
    CCMenuItem* pItem = static_cast<CCMenuItem*>(pSender);
    int index = pItem->getTag() - IAP_ITEM_TAG_START;
    
    const map<string, IAPItem>& iapItems = KKIAPWrapper::getIAPItems();
    map<string, IAPItem>::const_iterator it = iapItems.begin();
    
    int i = 0;
    
    while (i != index)
    {
        it++;
        i++;
    }
    
    const IAPItem& iapItem = it->second;
    
    if (KKIAPWrapper::buyItem(iapItem.productIdentifer.c_str()))
    {
        m_PendingProductIdentifier = iapItem.productIdentifer;
        m_WaitingForTransactionResult = true;
        
        CCSize winSize = m_pContainer->getContentSize();
        CCLabelTTF* waitingLabel = CCLabelTTF::create("Waiting...", STANDARD_BOLD_FONT_NAME, 48);
        CCLayerColor* waitingLayer = CCLayerColor::create(ccc4(0,0,0,100));
        waitingLayer->setContentSize(waitingLabel->getContentSize());
        waitingLayer->setAnchorPoint(ccp(0.5f, 0.5f));
        waitingLayer->addChild(waitingLabel);
        waitingLabel->setPosition(ccp(waitingLayer->getContentSize().width/2, waitingLayer->getContentSize().height/2));
        waitingLayer->setPosition(ccp(winSize.width / 4,winSize.height / 2));
        
        m_pContainer->addChild(waitingLayer, 0, IAP_WAITNG_RESULT_TAG);
    }
}

void IAPMenu::close(cocos2d::CCObject *pSender)
{
    if (m_WaitingForTransactionResult)
    {
        return;
    }
  
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_pContainer->runAction(CCSequence::create(CCMoveTo::create(0.2f, ccp(winSize.width/2, winSize.height + m_pContainer->getContentSize().height)),
                                               CCCallFunc::create(this, callfunc_selector(IAPMenu::closeMenu)),
                                               NULL));
}

void IAPMenu::closeMenu()
{
    m_pContainer->removeFromParentAndCleanup(true);
    
    if (CCDirector::sharedDirector()->isPaused())
    {
        CCDirector::sharedDirector()->resume();
    }
    
    if (!GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG) &&
        GameScene::getInstance()->sharedGameStage &&
        GameScene::getInstance()->sharedGameStage->isPaused())
    {
        GameScene::getInstance()->sharedGameStage->resume();
    }
}

void IAPMenu::setTransactionCompleted(const char* identifier)
{
    m_pContainer->removeChildByTag(IAP_WAITNG_RESULT_TAG, true);
    m_WaitingForTransactionResult = false;
    
}

void IAPMenu::setTransactionFailed(const char* identifier)
{
    m_pContainer->removeChildByTag(IAP_WAITNG_RESULT_TAG, true);
    m_WaitingForTransactionResult = false;
}


void IAPMenu::update(float dt)
{
    if (m_WaitingForTransactionResult && m_CloseBtn->isVisible())
    {
        m_CoinStore->setVisible(false);
        m_CloseBtn->setVisible(false);
    }
    else if (!m_WaitingForTransactionResult && !m_CloseBtn->isVisible())
    {
        m_CloseBtn->setVisible(true);
        m_CoinStore->setVisible(true);
    }
    
    char data[256] = {0};
    sprintf(data, "%d", Player::getInstance()->getGems());
    //m_CurrentGemsLabel->setString(data);
}

void IAPMenu::switchToCoinStore(CCObject* pObj = nullptr)
{
    if (GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG))
    {
        return;
    }
    
    
    SaleStore* store = SaleStore::create();
    store->switchToCoinStore();
    GameScene::getInstance()->sharedMainUI->addChild(store, 0, SALE_STORE_TAG);

    this->close(NULL);

    if (GameScene::getInstance()->sharedGameStage && !GameScene::getInstance()->sharedGameStage->isPaused())
    {
        GameScene::getInstance()->sharedGameStage->pause();
    }

}

