//
//  NewInfoMenu.cpp
//  tdgame
//
//  Created by Wang Nero on 13-7-4.
//
//

#include "NewInfoMenu.h"
#include "MessageBox.h"
#include "GameScene.h"
#include "SkillTreeMenu.h"
#include "SimpleAudioEngine.h"

#define MAXCOUNT 3

bool NewInfoMenu::init()
{
    if (!CCMenu::init())
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    infoX = 0;
    infoY = winSize.height * 0.5;
    this->flush();
    return true;
}

void NewInfoMenu::createNewInfo(const int messageId)
{
    CCMenuItemImage* newInfo = CCMenuItemImage::create("UI/button/new_normal.png", "UI/button/new_pressed.png");
    newInfo->setTarget(this, menu_selector(NewInfoMenu::showInfoMessage));
    this->addToList(messageId, newInfo);
    this->flush();
}

void NewInfoMenu::setLocation(const float x,const float y)
{
    this->infoX = x;
    this->infoY = y;
}

void NewInfoMenu::flush()
{
    this->removeAllChildrenWithCleanup(false);
    showSkillTreeIcon();
    if(infoBtnList.size() > 0){
        auto p = this->infoBtnList.begin();
        for (int count = 0; count < MAXCOUNT; ++count) {
            if(p == this->infoBtnList.end()){
                break;
            }
            CCMenuItemImage *buffer = p->second;
            
            this->addAction(buffer);
            this->setLocation(buffer, count);
            this->addChild(buffer);
            p++;
        }
    }
}

void NewInfoMenu::addToList(const int message,CCMenuItemImage* const button)
{
    CC_SAFE_RETAIN(button);
    this->infoBtnList.push_back(std::pair<int, CCMenuItemImage*>(message, button));
}

bool NewInfoMenu::addAction(CCMenuItemImage* const infoButton)
{
    if(infoButton != NULL){
        infoButton->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(1.5f, 100),
                                                                                            CCFadeTo::create(1.5f, 255),
                                                                                            NULL)));
        return true;
    }else{
        return false;
    }
}

bool NewInfoMenu::setLocation(CCMenuItemImage* const infoButton,int index)
{
    if(infoButton != NULL && index <= 3){
        int distance = infoY - infoButton->getContentSize().height * 1.1 * index;
        Function::scaleNodeForScreen(infoButton, ccp(0.5f,0.5f));
        infoButton->setAnchorPoint(ccp(0.0f,0.5f));
        infoButton->setPosition(ccp(infoX,distance));
        return true;
    }else{
        return false;
    }
}

int NewInfoMenu::foundMessageAndRemove(CCMenuItemImage* button)
{
    int result = -1;
    for(auto p = this->infoBtnList.begin();p != this->infoBtnList.end();p++){
        if(p->second == button){
            result = p->first;
            CC_SAFE_RELEASE(p->second);
            this->infoBtnList.remove(*p);
            break;
        }
    }
    return result;
}

void NewInfoMenu::showInfoMessage(CCObject* pSender)
{
    if(!CCDirector::sharedDirector()->isPaused()){
        MessageBox* pMessageBox = GameScene::getInstance()->sharedMainUI->getStageUI()->getMessageBox();
        pMessageBox->showEnemyIntroduction(this->foundMessageAndRemove((CCMenuItemImage*)pSender));
        this->flush();
    }
}

void NewInfoMenu::showSkillTreeIcon()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    m_pSkillTreeIcon = CCMenuItemImage::create("UI/button/book_normal.png", "UI/button/book_pressed.png", this, menu_selector(NewInfoMenu::onSkillTree));
    float vx = 0;
    float vy = winSize.height * 0.60;
    Function::scaleNodeForScreen(m_pSkillTreeIcon, ccp(0.5f,0.5f));
    m_pSkillTreeIcon->setAnchorPoint(ccp(0,0));
    m_pSkillTreeIcon->setPosition(vx,vy);
    addChild(m_pSkillTreeIcon);
}

void NewInfoMenu::onSkillTree(cocos2d::CCObject *pSender)
{
    
    if (!GameScene::getInstance()->sharedGameStage->isReady() ||
        GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    
    if (GameScene::getInstance()->sharedMainUI->getStageUI()->hasMessageBox())
    {
        return;
    }
    
    
    if (!Player::getInstance()->hasCompletedTutorial())
    {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    
    GameScene::getInstance()->sharedMainUI->getStageUI()->showSkillTreeMenu();
   
}
