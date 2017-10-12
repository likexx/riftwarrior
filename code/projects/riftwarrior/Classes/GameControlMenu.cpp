//
//  GameControlMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#include "GameControlMenu.h"
#include "GameScene.h"
#include "SaleStoreMenu.h"
#include "SkillTreeMenu.h"
#include "IAPMenu.h"
#include "Player.h"
//#include "SnsHelper.h"
#include "NpcMessageBox.h"
#include "SimpleAudioEngine.h"

#define PAUSE_TAG 4444

bool GameControlMenu::init()
{
    if (!CCMenu::init())
    {
        return false;
    }
    
    m_pGameSpeedButton = NULL;
    m_isPaused = false;
    
    this->removeAllChildrenWithCleanup(true);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    winSize.height -=5;
    
    m_pSettingIcon = CCMenuItemImage::create("UI/button/setting_normal.png", "UI/button/setting_pressed.png", this, menu_selector(GameControlMenu::onSettingMenu));
    m_pGameSpeedButton = CCMenuItemImage::create("UI/button/pause_normal.png", "UI/button/pause_normal.png", this, menu_selector(GameControlMenu::onSpeedButton));
  
    m_IapIcon = CCMenuItemImage::create("UI/button/iap_normal.png", "UI/button/iap_pressed.png", this, menu_selector(GameControlMenu::onInGamePurchase));
//    m_pHelpButton = CCMenuItemImage::create("UI/button/help_normal.png", "UI/button/help_pressed.png",this,menu_selector(GameControlMenu::onHelpButton));
    
    float hx = winSize.width - m_pSettingIcon->getContentSize().width/2 - 10;
    float hy = winSize.height - m_pSettingIcon->getContentSize().height / 2;
        
    m_pSettingIcon->setPosition(ccp(hx, hy));
    addChild(m_pSettingIcon);

    hx -= (m_pSettingIcon->getContentSize().width + 10);
    m_pGameSpeedButton->setPosition(ccp(hx, hy));

    this->addChild(m_pGameSpeedButton);

    hx -= (m_pGameSpeedButton->getContentSize().width + 10);
    m_IapIcon->setPosition(ccp(hx, hy));
    addChild(m_IapIcon);
    
//    hx -= (m_IapIcon->getContentSize().width + 10);
//    m_pHelpButton->setPosition(ccp(hx,hy));
//    addChild(m_pHelpButton);
//    m_pHelpButton->setVisible(false);

    this->setPosition(0,0);
    scheduleUpdate();
    return true;
}

void GameControlMenu::setResumeButton()
{
    const char* imageName = "UI/button/resume_normal.png";
    CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(imageName);

    m_pGameSpeedButton->setNormalImage(CCSprite::createWithTexture(pTexture));
    m_pGameSpeedButton->setSelectedImage(CCSprite::createWithTexture(pTexture));
}

void GameControlMenu::setPauseButton()
{
    const char* imageName = "UI/button/pause_normal.png";

    CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->addImage(imageName);
    
    m_pGameSpeedButton->setNormalImage(CCSprite::createWithTexture(pTexture));
    m_pGameSpeedButton->setSelectedImage(CCSprite::createWithTexture(pTexture));
}

void GameControlMenu::onSpeedButton(cocos2d::CCObject *pSender)
{
    if (!GameScene::getInstance()->sharedGameStage->isReady() ||
        GameScene::getInstance()->sharedGameStage->isPaused() ||
        GameScene::getInstance()->sharedMainUI->getStageUI()->isInSkillTreeMenu())
    {
        return;
    }


    if (GameScene::getInstance()->sharedMainUI->getStageUI()->hasMessageBox())
    {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    if (m_isPaused)
    {
        GameScene::getInstance()->sharedScene->removeChildByTag(PAUSE_TAG, true);
        setPauseButton();
        CCDirector::sharedDirector()->resume();
        m_isPaused = false;
    }
    else
    {
        setResumeButton();
        CCDirector::sharedDirector()->pause();
        m_isPaused = true;
    
        CCLabelBMFont* label = CCLabelBMFont::create("PAUSE", "font/levelup.fnt");
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        label->setPosition(ccp(winSize.width/2, winSize.height/2));
        label->setScale(winSize.height/640);
        
        GameScene::getInstance()->sharedScene->addChild(label, 0, PAUSE_TAG);
    }
}

//void GameControlMenu::promptHelpButton()
//{
//    if(this->isHelpButtonExist()){
//        m_pHelpButton->setVisible(true);
//        this->m_pHelpButton->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCScaleTo::create(0.3f, 1.3f),CCScaleTo::create(0.3f, 1.1f),NULL)));
//    }
//}

void GameControlMenu::onSettingMenu(cocos2d::CCObject *pSender)
{
    if (!GameScene::getInstance()->sharedGameStage->isReady() ||
        GameScene::getInstance()->sharedGameStage->isPaused() ||
        GameScene::getInstance()->sharedMainUI->getStageUI()->isInSkillTreeMenu())
    {
        return;
    }

    if (GameScene::getInstance()->sharedMainUI->getStageUI()->hasMessageBox())
    {
        return;
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    
    if(!m_isPaused){
        GameScene::getInstance()->sharedMainUI->getStageUI()->showGameSettingMenu();
    }
}

//void GameControlMenu::onHelpButton(CCObject* pSender)
//{
//    if(GameScene::getInstance()->sharedGameStage->isCompleted() || Player::getInstance()->isDead()){
//        return;
//    }
//    this->m_pHelpButton->stopAllActions();
//    this->m_pHelpButton->setScale(1.0f);
//    Player* player = Player::getInstance();
//    player->showMessage(GameData::getText("game_help_words"));
//    if(!GameScene::getInstance()->sharedGameStage->isPaused()){
//        GameScene::getInstance()->sharedGameStage->pause();
//    }
//    //this->weiboShare();
//    SnsHelper::shareScreenShot(HELPSHARE);
//    GameScene::getInstance()->sharedMainUI->getStageUI()->getNpcMessageBox()->setCallback(CCCallFunc::create(this, callfunc_selector(GameControlMenu::recoverAfterLogin)));
//}
//
//void GameControlMenu::recoverAfterLogin()
//{
//    if(SnsHelper::checkLoginState()){
//        Player* player = Player::getInstance();
//        if(!player->isDead() && GameScene::getInstance()->sharedGameStage->isReady()){
//            player->updateHp(player->getMaxHp());
//            player->updateMp(player->getMaxMp());
//            int getMoney = GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId())->bonusMoney;
//            player->updateMoney(getMoney);
//        }
//        this->removeChild(m_pHelpButton, true);
//        m_pHelpButton = NULL;
//    }
//}

/*
void GameControlMenu::weiboShare()
{
    SnsHelper::shareScreenShot(WEIBOSHARE_HELPSHARE);
}
*/

void GameControlMenu::onInGamePurchase(cocos2d::CCObject *pSender)
{
    if (!GameScene::getInstance()->sharedGameStage->isReady() ||
        GameScene::getInstance()->sharedGameStage->isPaused() ||
        GameScene::getInstance()->sharedMainUI->getStageUI()->isInSkillTreeMenu())
    {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    
    if(!m_isPaused && !GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG)){
        
        SaleStore* store = SaleStore::create();
        store->switchToCoinStore();
        GameScene::getInstance()->sharedMainUI->addChild(store, 0, SALE_STORE_TAG);
        
        if (GameScene::getInstance()->sharedGameStage)
        {
            GameScene::getInstance()->sharedGameStage->pause();
        }

        
    }
}
