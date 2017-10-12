//
//  NpcMessageBox.cpp
//  tdgame
//
//  Created by Like Zhang on 3/27/13.
//
//

#include "NpcMessageBox.h"
#include "GameData.h"
#include "GameScene.h"
#include "PlayerMoveMenu.h"
#include "constant.h"

bool NpcMessageBox::init()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLayerColor* background = CCLayerColor::create(ccc4(1,1,1,196), winSize.width, winSize.height*0.2);
    addChild(background);
    
    float fontSize = 28; // designed for 960x640;
    
    fontSize *= winSize.height/640;
    
    

    background->setAnchorPoint(ccp(0.5,0));
    background->setPosition(ccp(0, 0));
    
    m_pImageIcon = CCSprite::create();
    m_pImageIcon->setPosition(ccp(background->getContentSize().width * 0.15, background->getContentSize().height*0.6));
    m_pImageIcon->setAnchorPoint(ccp(0,0));
    background->addChild(m_pImageIcon);
    
    m_pTextLabel = CCLabelTTF::create("", STANDARD_NUMBER_FONT_NAME, fontSize);
    m_pTextLabel->setAnchorPoint(ccp(0,0.5));
    m_pTextLabel->setPosition(ccp(background->getContentSize().width * 0.25, background->getContentSize().height * 0.5));
    m_pTextLabel->setColor(ccc3(255,255,255));
    m_pTextLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    background->addChild(m_pTextLabel);
    
    /*
    CCMenu* pMenu = CCMenu::create();
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setContentSize(background->getContentSize());
    
    CCMenuItemImage* button = CCMenuItemImage::create("UI/button.png", "UI/button_pressed.png");
    button->setScale(background->getContentSize().width * 0.25 / button->getContentSize().width);
    CCLabelTTF* label = CCLabelTTF::create(GameData::getText("continue"), STANDARD_NUMBER_FONT_NAME, 28);
    label->setPosition(ccp(button->getContentSize().width/2, button->getContentSize().height * 0.5));
    button->addChild(label);

    button->setAnchorPoint(ccp(1, 0));
    button->setPosition(ccp(background->getContentSize().width * 0.95, background->getContentSize().height * 0.05));
    button->setTarget(this, menu_selector(NpcMessageBox::close));
    pMenu->addChild(button);
    
    background->addChild(pMenu);
    */

    setVisible(false);
    
    m_Callback = NULL;
    
    return true;
}

bool NpcMessageBox::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // touch any place to close
    close(NULL);
    return true;
}

void NpcMessageBox::showMessage(const char *pImagePath, const char *message)
{
    if (isVisible())
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    }
    
    GameScene::getInstance()->sharedMainUI->getStageUI()->getPlayerMoveMenu()->setVisible(false);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    setVisible(true);
    
    CCNode* pBackground = m_pImageIcon->getParent();
    CCSize size = pBackground->getContentSize();
    
    
    m_pImageIcon->initWithFile(pImagePath);
    float imageScale = pBackground->getContentSize().width * 0.3/m_pImageIcon->getContentSize().width;

    m_pImageIcon->setScale(imageScale);
    m_pImageIcon->setAnchorPoint(ccp(0,0));
    m_pImageIcon->setPosition(ccp(0, 5));

    m_pTextLabel->setAnchorPoint(ccp(0, 0.5));
    m_pTextLabel->setPosition(ccp(m_pImageIcon->getContentSize().width * imageScale, size.height * 0.5));
    m_pImageIcon->setVisible(true);
    
    m_pTextLabel->setString(message);
    
    this->setScale(0.1f);
    
    this->runAction(CCScaleTo::create(0.2f, 1.0f));

}

void NpcMessageBox::close(cocos2d::CCObject *pSender)
{
    GameScene::getInstance()->sharedMainUI->getStageUI()->getPlayerMoveMenu()->setVisible(true);
    
    MessageBox::close(pSender);
}