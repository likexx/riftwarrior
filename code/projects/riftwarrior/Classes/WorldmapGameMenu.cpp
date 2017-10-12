//
//  WorldmapGameMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 6/28/13.
//
//

#include "WorldmapGameMenu.h"
#include "GameData.h"
#include "Magic.h"
#include "BuildingSetting.h"
#include "Player.h"
#include "GameScene.h"
#include "KKMenu.h"
#include "IAPMenu.h"
#include "SimpleAudioEngine.h"
#include "NewInfoMenu.h"
#include "StartScreen.h"
#include "HomeSettingMenu.h"
#include "constant.h"

//test
#include "SaleStoreMenu.h"
#include "SystemHelper.h"

#define SKILL_RESET_GEM_COST 20

#define SKILL_POINT_TITLE_LABEL_TAG 1001
#define SKILL_SELECTED_ICON_TAG 1002

#define SKILL_LEVEL_LABEL_TAG 1003

#define RESET_SKILL_MENU_TAG 1004
#define SIDE_BAR_TAG 1005

#define BUILDING_ITEM_LABEL_TAG 10000
#define MAGIC_ITEM_LEVEL_LABEL_TAG 20000
#define SKILL_ITEM_TAG 30000
#define SERVER_MESSAGE_TAG 40000


bool WorldmapGameMenu::init()
{
    build();
    return true;
}

void WorldmapGameMenu::build()
{
    this->removeAllChildrenWithCleanup(true);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->setContentSize(winSize);
    
    CCSprite* background = CCSprite::create("UI/map_backrail.png");
    background->setAnchorPoint(ccp(0.5f, 0.5f));
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    float scaleX = winSize.width/background->getContentSize().width;
    float scaleY = winSize.height/background->getContentSize().height;
    background->setScaleX(scaleX);
    background->setScaleY(scaleY);
    //background->setContentSize(winSize);
    addChild(background);
    m_pBackground = background;
    
    skillBookMenu = SkillTreeMenu::create();
    skillBookMenu->setAnchorPoint(ccp(0.5f,0.0f));
    skillBookMenu->setContentSize(m_pBackground->getContentSize());
    scaleX = skillBookMenu->getContentSize().width / winSize.width;
    scaleY = skillBookMenu->getContentSize().height / winSize.height;
    skillBookMenu->setScaleX(scaleX);
    skillBookMenu->setScaleY(scaleY);
    skillBookMenu->setPosition(scaleX * m_pBackground->getContentSize().width / 2,scaleY * winSize.height);

    m_pBackground->addChild(skillBookMenu,2);
    
    m_pMenu = CCMenu::create();
    m_pMenu->setContentSize(m_pBackground->getContentSize());
    m_pMenu->setAnchorPoint(CCPointZero);
    m_pMenu->setPosition(CCPointZero);
    m_pBackground->addChild(m_pMenu,1);
    
    float x = m_pBackground->getContentSize().width;
    float y = m_pBackground->getContentSize().height * 0.005;
    float distance = m_pBackground->getContentSize().width * 0.03;
    
    x -= distance * 0.5;
    
    x-= distance * 3;
    CCMenuItemImage* skillBook = CCMenuItemImage::create("UI/button/book_normal.png", "UI/button/book_pressed.png");
    skillBook->setAnchorPoint(ccp(1.0f,0.0f));
    skillBook->setPosition(ccp(x,y));
    skillBook->setTarget(this, menu_selector(WorldmapGameMenu::showSkillBook));
    m_pMenu->addChild(skillBook);
    
    x-= distance * 29;
    CCMenuItemImage* home = CCMenuItemImage::create("UI/button/home_normal.png", "UI/button/home_pressed.png");
    home->setAnchorPoint(ccp(0.0f,0.0f));
    home->setPosition(ccp(x,y));
    home->setTarget(this, menu_selector(WorldmapGameMenu::backToHome));
    m_pMenu->addChild(home);
 
    
    x += distance * 3;
    CCMenuItemImage* iap = CCMenuItemImage::create("UI/button/iap_normal.png", "UI/button/iap_pressed.png");
    iap->setAnchorPoint(ccp(0.5f,0.5f));
    iap->setPosition(ccp(x + iap->getContentSize().width / 2,y + iap->getContentSize().height / 2));
    iap->setTarget(this, menu_selector(WorldmapGameMenu::openIapMenu));
    iap->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCRotateTo::create(0.5f, -5.0f),CCRotateTo::create(0.5f, 5.0f),NULL)));
    m_pMenu->addChild(iap);
    
    CCPoint b = m_pBackground->getPosition();
    b = m_pMenu->getPosition();
    b = this->getPosition();
    
    
    CCLabelTTF* pServerMessageLabel = CCLabelTTF::create("", STANDARD_FONT_NAME, 18);
    pServerMessageLabel->setPosition(ccp(m_pBackground->getContentSize().width/2, 26));
    pServerMessageLabel->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCScaleTo::create(0.3f, 1.2f),CCScaleTo::create(0.05f, 1.0f),CCDelayTime::create(5.0f),NULL)));
    m_pBackground->addChild(pServerMessageLabel, 0, SERVER_MESSAGE_TAG);
}

void WorldmapGameMenu::showLeaderboard(cocos2d::CCObject *pSender)
{
    if (GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG))
    {
        return;
    }
}

void WorldmapGameMenu::openIapMenu(cocos2d::CCObject *pSender)
{
    //IAPMenu::create(-256);
    
    if (GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG))
    {
        return;
    }
    
    SaleStore*store =  SaleStore::create();
    store->switchToCoinStore();
    GameScene::getInstance()->sharedMainUI->addChild(store, 0, SALE_STORE_TAG);
}

void WorldmapGameMenu::backToHome(CCObject* pSender)
{
    if (GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG))
    {
        return;
    }
    
    SystemHelper::removeGoogleBar();

    CCDirector::sharedDirector()->popScene();

}

void WorldmapGameMenu::showSkillBook(CCObject* pSender)
{
    if (GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG))
    {
        return;
    }

    if(!skillBookMenu->isInUse()){
        skillBookMenu->show(pSender);
    }
}

void WorldmapGameMenu::updateSkillTree()
{
    if(skillBookMenu != NULL){
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        skillBookMenu->updateSkillTreeInfo();
        skillBookMenu->setAnchorPoint(ccp(0.5f,0.0f));
        skillBookMenu->setContentSize(m_pBackground->getContentSize());
        float scaleX = skillBookMenu->getContentSize().width / winSize.width;
        float scaleY = skillBookMenu->getContentSize().height / winSize.height;
        skillBookMenu->setScaleX(scaleX);
        skillBookMenu->setScaleY(scaleY);
        skillBookMenu->setPosition(scaleX * m_pBackground->getContentSize().width / 2,skillBookMenu->getPosition().y);
    }
}

void WorldmapGameMenu::showServerMessage(const char *pValue)
{
    CCLabelTTF* pLabel = static_cast<CCLabelTTF*>(m_pBackground->getChildByTag(SERVER_MESSAGE_TAG));
    if (!pLabel)
    {
        return;
    }
    
    pLabel->setString(pValue);
}


