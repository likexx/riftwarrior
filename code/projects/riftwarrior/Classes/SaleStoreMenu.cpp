//
//  SuperStoreMenu.cpp
//  tdgame
//
//  Created by Wang Nero on 13-7-11.
//
//

#include "SaleStoreMenu.h"
#include "GameData.h"
#include "IAPMenu.h"
#include "Player.h"
#include "GameScene.h"
#include "constant.h"

#define TABPUBLICX 21
#define TABPUBLICY 480
#define TABYDICETANCE 115  
#define TABMAXWIDTH 357
#define TABMAXHEIGHT 92
#define PUBLIC_ZORDER 0
#define TAG_STORE_BACKGROUND 1000
#define TAG_STORE_MAINMENU 1001

void SaleStore::createAndShow()
{
    if (GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG))
    {
        return;
    }
    
    SaleStore* store = SaleStore::create();
    store->switchToCoinStore();
    GameScene::getInstance()->sharedMainUI->addChild(store, 0, SALE_STORE_TAG);
    
    if (GameScene::getInstance()->sharedGameStage && !GameScene::getInstance()->sharedGameStage->isPaused())
    {
        GameScene::getInstance()->sharedGameStage->pause();
    }
}

bool SaleStore::init()
{
    bool result = true;

    do {
        result &= this->createBackground() & this->createSwitchButton();;
    } while (false);
    
    
    if (GameScene::getInstance()->sharedGameStage && !GameScene::getInstance()->sharedGameStage->isPaused())
    {
        GameScene::getInstance()->sharedGameStage->pause();
    }

    return result;
}

void SaleStore::setActionBeforeClose(CCActionInterval *const action)
{
    CC_SAFE_RETAIN(action);
    this->actionBeforeClose = action;
}

void SaleStore::close()
{
    do {
        CCCallFunc* close = CCCallFunc::create(this, callfunc_selector(SaleStore::closeAction));
        if (this->actionBeforeClose != NULL) {
            this->runAction(CCSequence::create(this->actionBeforeClose,close,NULL));
            CC_SAFE_RELEASE(this->actionBeforeClose);
        }else{
            this->runAction(close);
        }
    } while (false);
    
    if (GameScene::getInstance()->sharedGameStage)
    {
        GameScene::getInstance()->sharedGameStage->resume();
    }

}

void SaleStore::closeAction()
{
    do {
        CCNode* parent = this->getParent();
        CC_BREAK_IF(!parent);
        parent->removeChild(this, true);
    } while (false);
}

bool SaleStore::createBackground()
{
    bool result = false;
    do {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCSprite* background = CCSprite::create("UI/iap/store_background.png");
        CC_BREAK_IF(!background);
        background->setAnchorPoint(ccp(0.5f, 1.0f));
        background->setPosition(ccp(winSize.width/2, winSize.height));
        
        this->addChild(background, PUBLIC_ZORDER, TAG_STORE_BACKGROUND);
        
        m_pBackground = background;
        
        Function::scaleNodeForScreen(this, ccp(0.5,1), ccp(winSize.width/2, winSize.height));
        
        CCMenu* menu = CCMenu::create();
        menu->setPosition(ccp(0.0f, 0.0f));
        background->addChild(menu,PUBLIC_ZORDER,TAG_STORE_MAINMENU);
        
        CCMenuItemImage* close = CCMenuItemImage::create("UI/iap/closebutton_normal.png", "UI/iap/closebutton_pressed.png");
        close->setAnchorPoint(ccp(0.0f, 1.0f));
        close->setPosition(ccp(background->getContentSize().width - 5, background->getContentSize().height - 1));
        close->setTarget(this, menu_selector(SaleStore::close));
        menu->addChild(close);
        
        float x = getPositionX();
        float y = getPositionY();
        this->setPosition(ccp(getPosition().x, getPosition().y + winSize.height));
        this->runAction(CCMoveTo::create(0.2f, ccp(x, y)));
        this->setActionBeforeClose(CCMoveTo::create(0.2f, ccp(x, y + winSize.height)));
        result = true;
        
    } while (false);
    return result;
}

void SaleStore::createTitle()
{
    CCNode* background = this->getChildByTag(TAG_STORE_BACKGROUND);
    CCLabelTTF* coinsTitle = CCLabelTTF::create(GameData::getText("store_CoinsName"), STANDARD_NUMBER_FONT_NAME, 30);
    coinsTitle->setAnchorPoint(ccp(0.0f, 1.0f));
    coinsTitle->setPosition(ccp(21.0f, 530.0f));
    background->addChild(coinsTitle);
}

bool SaleStore::createSwitchButton()
{
    bool result = false;
    do {
        CCMenu* menu = (CCMenu*)m_pBackground->getChildByTag(TAG_STORE_MAINMENU);
        CC_BREAK_IF(!menu);
        CCMenuItemImage* gemsStore = CCMenuItemImage::create("UI/skilltree/reset_button.png", "UI/skilltree/reset_button_pressed.png");
        CC_BREAK_IF(!gemsStore);
        CCLabelTTF* gemsLab = CCLabelTTF::create(GameData::getText("store_gems"), "Arial Bold", 20);
        gemsLab->setAnchorPoint(ccp(0.5f, 0.5f));
        gemsLab->setPosition(ccp(gemsStore->getContentSize().width/2, gemsStore->getContentSize().height/2));
        gemsLab->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCScaleTo::create(0.5f, 1.1f), CCScaleTo::create(0.5f, 1.0f))));
        gemsStore->addChild(gemsLab);
        gemsStore->setScale(0.85f);
        gemsStore->setAnchorPoint(ccp(1.0f, 1.0f));
        gemsStore->setPosition(ccp(387.0f,535.0f));
        gemsStore->setTarget(this, menu_selector(SaleStore::switchToGemstore));
        menu->addChild(gemsStore);
        result = true;
    } while (false);
    return result;
}

void SaleStore::switchToGemstore()
{
    this->clearALLTab();
    this->close();
    
    if (GameScene::getInstance()->sharedGameStage && !GameScene::getInstance()->sharedGameStage->isPaused())
    {
        GameScene::getInstance()->sharedGameStage->pause();
    }
    
    IAPMenu::create(-256);    
}

void SaleStore::switchToCoinStore()
{
        
    //this->clearALLTab();
    char cost[256] = {0};
    sprintf(cost, "%s%d",GameData::getText("store_Gems"),5);
    this->createStoreTab(1, GameData::getText("store_smallcoinsbag"), "x 1000", "UI/iap/coin1.png", cost, this, menu_selector(SaleStore::buySmallCoin));
    
    sprintf(cost, "%s%d",GameData::getText("store_Gems"),10);
    this->createStoreTab(2, GameData::getText("store_normalcoinsbag"), "x 3000", "UI/iap/coin2.png", cost, this, menu_selector(SaleStore::buyNormalCoin));

    sprintf(cost, "%s%d",GameData::getText("store_Gems"),20);
    this->createStoreTab(3, GameData::getText("store_fullcoinsbag"), "x 9000", "UI/iap/coin3.png", cost, this, menu_selector(SaleStore::buyFullCoin));
    
    sprintf(cost, "%s%d",GameData::getText("store_Gems"),40);
    this->createStoreTab(4, GameData::getText("store_hugecoinsbag"), "x 20000", "UI/iap/coin4.png", cost, this, menu_selector(SaleStore::buyHugeCoin));
    
    this->createTitle();
}

void SaleStore::clearALLTab()
{
    do {
        CCNode *background = this->getChildByTag(TAG_STORE_BACKGROUND);
        CC_BREAK_IF(!background);
        background->removeAllChildrenWithCleanup(true);
    }while (false);
}

bool SaleStore::turnGemToCoin(int numberOfGems,int numberOfCoins)
{
    bool result = false;
    do {
        int playerGems = Player::getInstance()->getGems();
        CC_BREAK_IF(playerGems < numberOfGems);
        Player::getInstance()->updateGems(-numberOfGems);
        Player::getInstance()->updateMoney(numberOfCoins);
        Player::getInstance()->save();
        
        char msg[256] = {0};
        sprintf(msg, "buy_coins_%d", numberOfCoins);
        SystemHelper::logEvent(msg);
        
        result = true;
    } while (false);
    if(result){
        this->showWarningMessage(GameData::getText("store_waring_suc"));
    }else{
        this->showWarningMessage(GameData::getText("store_waring_fai"));
    }
    return result;
}

void SaleStore::showWarningMessage(const char *msg)
{
    do {
        CCLabelTTF* text = CCLabelTTF::create(msg, STANDARD_NUMBER_FONT_NAME, 50);
        text->setColor(ccc3(255, 255, 255));
        CCLayerColor*back = CCLayerColor::create();
        back->setContentSize(text->getContentSize());
        back->setColor(ccc3(0, 0, 0));
        back->setOpacity(170);
        text->setAnchorPoint(ccp(0.5f, 0.5f));
        text->setPosition(ccp(back->getContentSize().width/2, back->getContentSize().height/2));
        back->addChild(text);
        CCNode* background = this->getChildByTag(TAG_STORE_BACKGROUND);
        CC_BREAK_IF(!background);
        back->setPosition(ccp(background->getContentSize().width/2 - back->getContentSize().width/2, background->getContentSize().height/2));
        background->addChild(back);
        back->runAction(CCSequence::create(CCDelayTime::create(1.0f),CCScaleTo::create(0.5f, 0.0f),NULL));
    } while (false);
}

bool SaleStore::createStoreTab(int i, const char *tabName,const char *tabIntro,const char *tabIcon,const char *buttonLab,CCObject *handler,SEL_MenuHandler delegte)
{
    bool result = false;
    do {
        CCNode* parent = CCNode::create();
        CCLabelTTF* name = CCLabelTTF::create(tabName, STANDARD_NUMBER_FONT_NAME, 18);
        CC_BREAK_IF(!name);
        name->setColor(ccc3(0, 0, 0));
        parent->addChild(name);
        CCSprite* icon = CCSprite::create(tabIcon);
        CC_BREAK_IF(!icon);
        name->addChild(icon);
        CCLabelTTF* intro = CCLabelTTF::create(tabIntro, STANDARD_NUMBER_FONT_NAME, 18);
        CC_BREAK_IF(!intro);
        intro->setColor(ccc3(0, 0, 0));
        parent->addChild(intro);
        CCMenu* menu = CCMenu::create();
        CC_BREAK_IF(!menu);
        parent->addChild(menu);
        menu->setPosition(parent->getPosition());
        CCMenuItemImage* button = CCMenuItemImage::create("UI/greenbutton_nomal.png", "UI/greenbutton_pressed.png");
        CC_BREAK_IF(!button);
        menu->addChild(button);
        CCLabelTTF* cost = CCLabelTTF::create(buttonLab, STANDARD_NUMBER_FONT_NAME, 18);
        CC_BREAK_IF(!cost);
        cost->setAnchorPoint(ccp(0.5f, 0.5f));
        cost->setPosition(ccp(button->getContentSize().width/2, button->getContentSize().height/2));
        button->addChild(cost);
        //button->setScale(0.75f);
        button->setAnchorPoint(ccp(1.0f, 0.5f));
        button->setPosition(ccp(TABMAXWIDTH, TABMAXHEIGHT * 0.5f));
        button->setTarget(handler, delegte);
        name->setAnchorPoint(ccp(0.0f, 0.0f));
        name->setPosition(ccp(TABMAXWIDTH * 0.005f, TABMAXHEIGHT * 0.05f));
        float iconY = name->getContentSize().height + 0.2 * TABMAXHEIGHT;
        icon->setAnchorPoint(ccp(0.5f, 0.0f));
        icon->setPosition(ccp(name->getContentSize().width/2, iconY));
        intro->setAnchorPoint(ccp(0.5f, 0.5f));
        intro->setPosition(ccp(TABMAXWIDTH/2, TABMAXHEIGHT/2));
        
        float y = TABPUBLICY - i * TABYDICETANCE;
        parent->setPosition(ccp(TABPUBLICX, y));
        m_pBackground->addChild(parent);

        
    } while (false);
    return result;
}


void SaleStore::buySmallCoin()
{
    this->turnGemToCoin(5, 1000);
}
void SaleStore::buyNormalCoin()
{
    this->turnGemToCoin(10, 3000);
}
void SaleStore::buyFullCoin()
{
    this->turnGemToCoin(20, 9000);
}
void SaleStore::buyHugeCoin()
{
    this->turnGemToCoin(40, 20000);
}