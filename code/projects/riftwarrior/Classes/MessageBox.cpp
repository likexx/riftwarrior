//
//  MessageBox.cpp
//  tdgame
//
//  Created by Like Zhang on 3/17/13.
//
//

#include "MessageBox.h"
#include "GameData.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "EnemyAnimation.h"
#include "constant.h"

#define MESSAGE_ICON_TAG 1001

bool MessageBox::init()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* background = CCSprite::create("UI/box.png");
    background->setScale(winSize.width*0.75/background->getContentSize().width);
    
    addChild(background);
    
    m_pImageIcon = CCSprite::create();
    m_pImageIcon->setPosition(ccp(background->getContentSize().width * 0.2, background->getContentSize().height*0.6));
    m_pImageIcon->setAnchorPoint(ccp(0,0.5));
    background->addChild(m_pImageIcon);
    
    m_pTextLabel = CCLabelTTF::create("", STANDARD_NUMBER_FONT_NAME, 32);
    m_pTextLabel->setAnchorPoint(ccp(0,0.5));
    m_pTextLabel->setPosition(ccp(background->getContentSize().width * 0.4, background->getContentSize().height * 0.6));
    m_pTextLabel->setColor(ccc3(255,255,255));
    m_pTextLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    background->addChild(m_pTextLabel);
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    pMenu->setContentSize(background->getContentSize());
    
    CCMenuItemImage* button = CCMenuItemImage::create("UI/button.png", "UI/button_pressed.png");
    button->setScale(background->getContentSize().width * 0.3 / button->getContentSize().width);
    CCLabelTTF* label = CCLabelTTF::create(GameData::getText("close"), STANDARD_NUMBER_FONT_NAME, 28);
    label->setPosition(ccp(button->getContentSize().width/2, button->getContentSize().height * 0.5));
    button->addChild(label);
    button->setAnchorPoint(ccp(0.5, 0.1));
    button->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.1));
    button->setTarget(this, menu_selector(MessageBox::close));
    
    pMenu->addChild(button);
    
    background->addChild(pMenu);
    
    setPosition(ccp(winSize.width/2, winSize.height/2));
    

    setVisible(false);
    
    m_Callback = NULL;
    
    return true;
}

bool MessageBox::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}


void MessageBox::setCallback(CCCallFunc* callback)
{
    CC_SAFE_RETAIN(callback);
    CC_SAFE_RELEASE(m_Callback);
    m_Callback = callback;

   // m_Callback->retain();
}

void MessageBox::showMessage(const char *pImagePath, const char *message, bool pauseGame)
{
    if (isVisible())
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    }

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    setVisible(true);

    CCNode* pBackground = m_pImageIcon->getParent();

    if (pImagePath)
    {
        m_pImageIcon->initWithFile(pImagePath);
        m_pImageIcon->setScale(pBackground->getContentSize().width * 0.2/m_pImageIcon->getContentSize().width);
        
        m_pTextLabel->setAnchorPoint(ccp(0, 0.5));
        m_pTextLabel->setPosition(ccp(pBackground->getContentSize().width * 0.4, pBackground->getContentSize().height * 0.6));
        m_pImageIcon->setVisible(true);
    }
    else
    {
        m_pTextLabel->setAnchorPoint(ccp(0.5, 0.5));
        m_pTextLabel->setPosition(ccp(pBackground->getContentSize().width * 0.5, pBackground->getContentSize().height * 0.6));
        m_pImageIcon->setVisible(false);
    }
    
    m_pTextLabel->setString(message);
    
    this->setScale(0.1f);
    
    if (!pauseGame)
    {
        this->runAction(CCScaleTo::create(0.2f, 1.0f));
    }
    else
    {
        this->runAction(CCSequence::create(CCScaleTo::create(0.2f, 1.0f),
                                           CCCallFunc::create(CCDirector::sharedDirector(), callfunc_selector(CCDirector::pause)),
                                           NULL));
    }
}


void MessageBox::showEnemyIntroduction(int id)
{
    
    if (isVisible())
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    }
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    setVisible(true);

    removeAllIcons();
    
    CCNode* pBackground = m_pImageIcon->getParent();
    //CCSprite* pBackground = CCSprite::create("UI/enrmy_show.png");

    char name[128]={0};
    sprintf(name, "enemy_%d_attack_1_1.png", id);
    
    CCArray* frames = CCArray::createWithCapacity(2);
    CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);

    const EnemySetting* pSetting = GameData::getEnemySetting(id);

   
    if (frame1)
    {
        CCSprite* pEnemySprite = NULL;
        
        pEnemySprite = CCSprite::createWithSpriteFrameName(name);
        
        for (int i=1;i<=8;++i)
        {
            sprintf(name, "enemy_%d_attack_1_%d.png", id, i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
            if (!frame)
            {
                break;
            }
            frames->addObject(frame);
        }
        
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.125f);
        CCAnimate* animate = CCAnimate::create(animation);
        CCActionInterval* pAnimation = CCRepeatForever::create(animate);
        
        pEnemySprite->runAction(pAnimation);
        
        float spriteIconWidth = pBackground->getContentSize().width * 0.2;
        
        if (spriteIconWidth < pEnemySprite->getTextureRect().size.width)
        {
            pEnemySprite->setScale(spriteIconWidth/pEnemySprite->getTextureRect().size.width);
        }

        float x = pBackground->getContentSize().width * 0.2;
        float y = pBackground->getContentSize().height * 0.6;
        
        pEnemySprite->setPosition(ccp(x, y));
        
        pBackground->addChild(pEnemySprite, 0, MESSAGE_ICON_TAG);
        
        x = pBackground->getContentSize().width * 0.4;

        m_pTextLabel->setAnchorPoint(ccp(0, 0.5));
        m_pTextLabel->setPosition(ccp(x, pBackground->getContentSize().height * 0.6));
        
        m_pTextLabel->setString(GameData::getText("new_enemy"));
        CCSize size = m_pTextLabel->getContentSize();

        char buffer[512] = {0};
        sprintf(buffer, GameData::getText("new_enemy"), pSetting->name.c_str() );
        m_pTextLabel->setString(buffer);
        
        sprintf(buffer, "   \n%s", pSetting->introduction.c_str());
        CCLog("%s", buffer);
        CCLabelTTF* pTextData = CCLabelTTF::create(buffer, m_pTextLabel->getFontName(), m_pTextLabel->getFontSize());
        
        pTextData->setPosition(ccp(m_pTextLabel->getPosition().x + size.width + pTextData->getContentSize().width/2, m_pTextLabel->getPosition().y));
        pTextData->setColor(ccc3(0,255,0));
        
        pBackground->addChild(pTextData, 0, MESSAGE_ICON_TAG);
        
    }
    
    
    this->setScale(0.1f);
    
    this->runAction(CCSequence::create(
                                       CCCallFunc::create(GameScene::getInstance()->sharedGameStage, callfunc_selector(GameStage::pause)),
                                       CCScaleTo::create(0.2f, 1.0f),
//                                       CCCallFunc::create(CCDirector::sharedDirector(), callfunc_selector(CCDirector::pause)),
                                       NULL));

    setCallback(CCCallFunc::create(GameScene::getInstance()->sharedGameStage, callfunc_selector(GameStage::resume)));
}


void MessageBox::showMessageIcons()
{
    if (isVisible())
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    }
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    setVisible(true);
    
    CCNode* pBackground = m_pImageIcon->getParent();
    
    m_pTextLabel->setAnchorPoint(ccp(0.5, 1));
    m_pTextLabel->setPosition(ccp(pBackground->getContentSize().width * 0.5, pBackground->getContentSize().height * 0.8));
    m_pTextLabel->setString(m_pMessageToShow);
    
    removeAllIcons();
    
    if (!m_MessageIcons.empty())
    {
        MessageIcon iconData = m_MessageIcons.front();
        
        CCSprite* icon = CCSprite::create(iconData.imagePath.c_str());
        CCLabelTTF* label = CCLabelTTF::create(iconData.name.c_str(), STANDARD_NUMBER_FONT_NAME, 24);
        label->setPosition(ccp(icon->getContentSize().width/2, -label->getContentSize().height/2));
        icon->addChild(label);
        
        CCLabelTTF* introduction = CCLabelTTF::create(iconData.description.c_str(), STANDARD_NUMBER_FONT_NAME, 24);
        introduction->setAnchorPoint(ccp(0,0.5));
        introduction->setPosition(ccp(icon->getContentSize().width * 1.5, icon->getContentSize().height * 0.5));
        icon->addChild(introduction);
        
        
        
        //    icon->setScale(iconSize/icon->getContentSize().width);
        
        float x = pBackground->getContentSize().width * 0.3;
        float y = pBackground->getContentSize().height * 0.6;
        
        icon->setPosition(ccp(x, y));

        pBackground->addChild(icon, 0, MESSAGE_ICON_TAG);
        
        m_MessageIcons.pop();
        
    }
    
    if (m_MessageIcons.empty())
    {
        setCallback(CCCallFunc::create(this, callfunc_selector(MessageBox::close)));
    }
    else
    {
        setCallback(CCCallFunc::create(this, callfunc_selector(MessageBox::showMessageIcons)));
    }
    
    
    GameScene::getInstance()->sharedGameStage->pause();
/*
    this->runAction(CCSequence::create(CCScaleTo::create(0.2, 1.0),
                                       CCCallFunc::create(CCDirector::sharedDirector(), callfunc_selector(CCDirector::pause)),
                                       NULL));
*/    
}

void MessageBox::showTextMessage(const char *message, bool pauseGame)
{
    showMessage(NULL, message, pauseGame);
}

void MessageBox::close(CCObject * /* dummy parameter to get compile some weird macro pass. */)
{
	this->close();
}

void MessageBox::close()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    if (numberOfRunningActions() > 0)
    {
        stopAllActions();
    }

    removeAllIcons();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    setVisible(false); // cannot remove it, otherwise callback won't work

    
    if (CCDirector::sharedDirector()->isPaused())
    {
        CCDirector::sharedDirector()->resume();
    }
    
    if (GameScene::getInstance()->sharedGameStage->isPaused())
    {
        GameScene::getInstance()->sharedGameStage->resume();
    }
    
    if (m_Callback)
    {
        CCCallFunc* pCallback = m_Callback;
        this->runAction(CCSequence::create(CCCallFunc::create(this, callfunc_selector(MessageBox::resetCallback)), pCallback, NULL));
    }
}

void MessageBox::resetCallback()
{
    m_Callback->release();
    m_Callback = NULL;
}

void MessageBox::removeAllIcons()
{
    CCNode* pBackground = m_pImageIcon->getParent();
    
    while (CCNode* pIcon = pBackground->getChildByTag(MESSAGE_ICON_TAG))
    {
        pIcon->removeFromParentAndCleanup(true);
    }

    if (m_pImageIcon)
    {
        m_pImageIcon->setVisible(false);
    }
}

void MessageBox::resetPosition()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    setPosition(ccp(winSize.width/2, winSize.height/2));
   
}
