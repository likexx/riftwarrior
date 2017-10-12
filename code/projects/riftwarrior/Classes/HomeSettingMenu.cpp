//
//  HomeSettingMenu.cpp
//  tdgame
//
//  Created by Wang Nero on 13-7-10.
//
//

#include "SimpleAudioEngine.h"
#include "HomeSettingMenu.h"
#include "GameData.h"
#include "SystemHelper.h"
#include "StartScreen.h"
#include "constant.h"

#define PUBLIC_ZORDER 0
#define TAG_ROOT_BACKGROUND 1000
#define TAG_ROOT_MAINMENU 2000
#define TAG_BACK_BACKLAYER 1001
#define TAG_BACK_SETTINGLAYER 1002
#define TAG_BACK_ABOUTLAYER 1003
#define TAG_MENU_CLOSEBUTTON 2001
#define TAG_MENU_SETTINGLABEL 2002
#define TAG_MENU_ABOUTLABEL 2003
#define GETLAYERCOLORBLACK_CC4 ccc4(0, 0, 0, 100)
#define GETLAYERCOLORWHITE_CC4 ccc4(255, 255, 255, 100)

//HomeSettingMenu
bool HomeSettingMenu::init()
{
    bool const result = this->restart();
    if(false == result){
        this->removeAllChildrenWithCleanup(true);
    }
    return result;
}

bool HomeSettingMenu::restart()
{
    return (this->createBackground() &&
            this->createMainMenu());
}

void HomeSettingMenu::setCloseCallAction(CCCallFunc *const closeCall)
{
    do {
        CC_BREAK_IF(!closeCall);
        closeCall->retain();
        this->closeCall = closeCall;
    } while (false);
}

bool HomeSettingMenu::createMainMenu()
{
    bool result = false;
    do {
        CC_BREAK_IF(this->getChildByTag(TAG_ROOT_MAINMENU));
        CCSprite* background = (CCSprite*)this->getChildByTag(TAG_ROOT_BACKGROUND);
        CC_BREAK_IF(!background);
        CCMenu* mainMenu = CCMenu::create();
        mainMenu->setPosition(background->getPosition());
        mainMenu->setAnchorPoint(ccp(0.5f, 0.5f));
        mainMenu->setContentSize(background->getContentSize());
        CCPoint a = mainMenu->getPosition();
        CCPoint b = background->getPosition();
        this->addChild(mainMenu,PUBLIC_ZORDER,TAG_ROOT_MAINMENU);
        CCSize menuSize = mainMenu->getContentSize();
        CCMenuItemImage* closeButton = CCMenuItemImage::create("UI/close_normal.png", "UI/close_pressed.png");
        closeButton->setAnchorPoint(ccp(0.0f,1.0f));
        closeButton->setPosition(ccp(menuSize.width/2 - 17, menuSize.height/2 - 12));
        closeButton->setTarget(this, menu_selector(HomeSettingMenu::closeMyself));
        mainMenu->addChild(closeButton,PUBLIC_ZORDER,TAG_MENU_CLOSEBUTTON);
        CCMenuItemLabel* settingLabel = CCMenuItemLabel::create(CCLabelTTF::create(GameData::getText("home_setting"), STANDARD_NUMBER_FONT_NAME, 25));
        CCNode* settingLayer = background->getChildByTag(TAG_BACK_SETTINGLAYER);
        CC_BREAK_IF(!settingLayer);
        float labelX = settingLayer->getPosition().x + settingLayer->getContentSize().width /2 - menuSize.width/2;
        float labelY = menuSize.height / 2 - menuSize.height + settingLayer->getPosition().y + settingLayer->getContentSize().height/2;
        settingLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        settingLabel->setPosition(ccp(labelX,labelY));
        settingLabel->setTarget(this, menu_selector(HomeSettingMenu::settingsDown));
        mainMenu->addChild(settingLabel, PUBLIC_ZORDER, TAG_MENU_SETTINGLABEL);
        
        CCMenuItemLabel* aboutLabel = CCMenuItemLabel::create(CCLabelTTF::create(GameData::getText("credits"), STANDARD_NUMBER_FONT_NAME, 25));
        CCNode* aboutLayer = background->getChildByTag(TAG_BACK_ABOUTLAYER);
        CC_BREAK_IF(!aboutLayer);
        labelX += aboutLayer->getPosition().x - settingLayer->getPosition().x;
        aboutLabel->setAnchorPoint(ccp(0.5f, 0.5f));
        aboutLabel->setPosition(ccp(labelX,labelY));
        aboutLabel->setTarget(this, menu_selector(HomeSettingMenu::creditsDown));
        mainMenu->addChild(aboutLabel, PUBLIC_ZORDER, TAG_MENU_ABOUTLABEL);
        result = true;
        this->settingsDown();
    } while (false);
    return result;
}

bool HomeSettingMenu::createBackground()
{
    bool result = false;
    do {
        CC_BREAK_IF(this->getChildByTag(TAG_ROOT_BACKGROUND));
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite* backgroundImage = CCSprite::create("UI/setting_box.png");
        CC_BREAK_IF(!backgroundImage);
        backgroundImage->setAnchorPoint(ccp(0.5f, 0.5f));
        backgroundImage->setPosition(ccp(winSize.width/2,winSize.height/2));
        addChild(backgroundImage, PUBLIC_ZORDER, TAG_ROOT_BACKGROUND);
        CCSize backgroundSize = backgroundImage->getContentSize();
        CCLayerColor *backgroundLayer = CCLayerColor::create(GETLAYERCOLORWHITE_CC4);
        CC_BREAK_IF(!backgroundLayer);
        backgroundLayer->setContentSize(CCSize(backgroundSize.width * 0.85, backgroundSize.height * 0.80));
        float layerX = (backgroundSize.width - backgroundLayer->getContentSize().width) / 2;
        backgroundLayer->setPosition(ccp(layerX, backgroundSize.height * 0.05));
        backgroundImage->addChild(backgroundLayer,PUBLIC_ZORDER,TAG_BACK_BACKLAYER);
        CCLayerColor* settings = CCLayerColor::create(GETLAYERCOLORBLACK_CC4);
        CC_BREAK_IF(!settings);
        float layerY = backgroundLayer->getContentSize().height + backgroundLayer->getPosition().y;
        settings->setContentSize(CCSize(backgroundSize.width/3.5, backgroundSize.height * 0.10f));
        settings->setPosition(layerX,layerY);
        backgroundImage->addChild(settings, PUBLIC_ZORDER, TAG_BACK_SETTINGLAYER);
        CCLayerColor* aboutLayer = CCLayerColor::create(GETLAYERCOLORBLACK_CC4);
        CC_BREAK_IF(!aboutLayer);
        aboutLayer->setContentSize(CCSize(backgroundSize.width/3.5, backgroundSize.height * 0.10f));
        aboutLayer->setPosition(layerX + aboutLayer->getContentSize().width * 1.1,layerY);
        backgroundImage->addChild(aboutLayer, PUBLIC_ZORDER, TAG_BACK_ABOUTLAYER);
        this->resetSelectLayerColor(TAG_BACK_SETTINGLAYER, TAG_BACK_ABOUTLAYER);
        result = true;
    } while (false);
    return result;
}

void HomeSettingMenu::resetSelectLayerColor(int layerTagToW,int layerTagToB)
{
    do {
        CCNode* parent = this->getChildByTag(TAG_ROOT_BACKGROUND);
        CC_BREAK_IF(!parent);
        CCLayerColor* const toWhite = (CCLayerColor*)parent->getChildByTag(layerTagToW);
        CCLayerColor* const toBlack = (CCLayerColor*)parent->getChildByTag(layerTagToB);
        if(NULL != toWhite){
            toWhite->setColor(ccc3(255, 255, 255));
        }
        if(NULL != toBlack){
            toBlack->setColor(ccc3(0, 0, 0));
        }
    } while (false);
}

void HomeSettingMenu::closeMyself()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    CCCallFunc* closeAction = CCCallFunc::create(this, callfunc_selector(HomeSettingMenu::closeActonFunc));
    if(NULL != this->closeCall){
        this->runAction(CCSequence::create(this->closeCall,closeAction,NULL));
    }else{
        this->runAction(closeAction);
    }
}

void HomeSettingMenu::closeActonFunc()
{
    StartScreen* pStartScreen = static_cast<StartScreen*>(getParent());
    this->getParent()->removeChild(this, true);
    pStartScreen->resetButtonLabels();
}

void HomeSettingMenu::creditsDown()
{
    do {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
        CCNode* mainMenu = this->getChildByTag(TAG_ROOT_MAINMENU);
        CCMenuItemLabel* item = (CCMenuItemLabel*)mainMenu->getChildByTag(TAG_MENU_ABOUTLABEL);
        CC_BREAK_IF(!item);
        item->setColor(ccc3(0, 0, 0));
        item = (CCMenuItemLabel*)mainMenu->getChildByTag(TAG_MENU_SETTINGLABEL);
        CC_BREAK_IF(!item);
        item->setColor(ccc3(255, 255, 255));
        this->resetSelectLayerColor(TAG_BACK_ABOUTLAYER,TAG_BACK_SETTINGLAYER);
        this->foundBackgroundLayer()->removeAllChildrenWithCleanup(true);
        homeSettingmenu::CreditsLayer::getInstance(this->foundBackgroundLayer());
    } while (false);
}
void HomeSettingMenu::settingsDown()
{
    do {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
        CCNode* mainMenu = this->getChildByTag(TAG_ROOT_MAINMENU);
        CCMenuItemLabel* item = (CCMenuItemLabel*)mainMenu->getChildByTag(TAG_MENU_SETTINGLABEL);
        CC_BREAK_IF(!item);
        item->setColor(ccc3(0, 0, 0));
        item = (CCMenuItemLabel*)mainMenu->getChildByTag(TAG_MENU_ABOUTLABEL);
        CC_BREAK_IF(!item);
        item->setColor(ccc3(255, 255, 255));
        this->resetSelectLayerColor(TAG_BACK_SETTINGLAYER, TAG_BACK_ABOUTLAYER);
        homeSettingmenu::SettingLayer::getInstance(this->foundBackgroundLayer());
    } while (false);
}

CCLayer* HomeSettingMenu::foundBackgroundLayer()
{
    CCLayer *result = NULL;
    do {
        CCNode* background = this->getChildByTag(TAG_ROOT_BACKGROUND);
        CC_BREAK_IF(!background);
        CCLayer* layer = (CCLayer*)background->getChildByTag(TAG_BACK_BACKLAYER);
        CC_BREAK_IF(!layer);
        result = layer;
    } while (false);
    return result;
}

//SettingLayer
using namespace homeSettingmenu;

#define TAG_SET_MENU 1000

SettingLayer* SettingLayer::getInstance(CCLayer* const parent)
{
    do {
        CC_BREAK_IF(!parent);
        SettingLayer* newLayer = SettingLayer::create();
        CC_BREAK_IF(!newLayer);
        newLayer->setPosition(ccp(0.0f,0.0f));
        newLayer->setContentSize(parent->getContentSize());
        CC_BREAK_IF(!newLayer->restart());
        parent->removeAllChildrenWithCleanup(true);
        parent->addChild(newLayer);
        return newLayer;
    } while (false);
    return NULL;
}

bool SettingLayer::init()
{
    return CCLayer::init();
}

bool SettingLayer::restart()
{
    return this->createMenu() & this->createButton();
}

bool SettingLayer::createMenu()
{
    bool result = false;
    do {
        CCMenu* mainMenu = CCMenu::create();
        CC_BREAK_IF(!mainMenu);
        CCSize parentSize = this->getContentSize();
        mainMenu->setPosition(ccp(parentSize.width/2, parentSize.height/2));
        mainMenu->setContentSize(this->getContentSize());
        this->addChild(mainMenu, PUBLIC_ZORDER, TAG_SET_MENU);
        result = true;
    } while (false);
    return result;
}

bool SettingLayer::createButton()
{
    bool result = false;
    do {
        CCNode* parent = this->getChildByTag(TAG_SET_MENU);
        CC_BREAK_IF(!parent);
        CCLabelTTF* label = CCLabelTTF::create(GameData::getText("switch_language"),STANDARD_NUMBER_FONT_NAME, 22);
        CCMenuItemImage* switchLan = CCMenuItemImage::create("UI/greenbutton_nomal.png", "UI/greenbutton_pressed.png");
        CC_BREAK_IF(!switchLan);
        switchLan->setPosition(ccp(0.0f, parent->getContentSize().height/8));
        label->setPosition(ccp(switchLan->getContentSize().width/2, switchLan->getContentSize().height/2));
        switchLan->setTarget(this, menu_selector(SettingLayer::switchLanDown));
        switchLan->addChild(label);
        parent->addChild(switchLan);
        if(SystemHelper::getCurrentLanguage() == ENUM_LANGUAGE_CHINESE){
            label = CCLabelTTF::create(GameData::getText("forum_baidu"),STANDARD_NUMBER_FONT_NAME, 22);
        }else{
            label = CCLabelTTF::create(GameData::getText("forum_facebook"),STANDARD_NUMBER_FONT_NAME, 22);
        }
        CCMenuItemImage* sharing = CCMenuItemImage::create("UI/greenbutton_nomal.png", "UI/greenbutton_pressed.png");
        CC_BREAK_IF(!sharing);
        sharing->setPosition(ccp(0.0f, -(parent->getContentSize().height/8)));
        label->setPosition(ccp(sharing->getContentSize().width/2, sharing->getContentSize().height/2));
        sharing->addChild(label);
        sharing->setTarget(this, menu_selector(SettingLayer::sharingDown));
        parent->addChild(sharing);
        result = true;
    } while (false);
    return result;
}

void SettingLayer::sharingDown(CCObject* pObj = nullptr)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    if (SystemHelper::getCurrentLanguage() == ENUM_LANGUAGE_CHINESE)
    {
        SystemHelper::openUrl("http://tieba.baidu.com/f?kw=%CA%B1%BF%D5%B7%C5%C0%CB%D5%DF");
    }
    else
    {
        SystemHelper::openUrl("http://www.facebook.com/RiftWarrior");
    }
}

void SettingLayer::switchLanDown(CCObject* pObj = nullptr)
{
    if (SystemHelper::getCurrentLanguage() == ENUM_LANGUAGE_ENGLISH)
    {
        SystemHelper::setGameLanguage(ENUM_LANGUAGE_CHINESE);
    }
    else
    {
        SystemHelper::setGameLanguage(ENUM_LANGUAGE_ENGLISH);
    }
    
    // reload StartScreen
    
    this->removeFromParentAndCleanup(true);
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    
    GameData::initLanguageText();
    
    StartScreen* startScreen = StartScreen::createNewScreen();
    
    CCSprite* background = static_cast<CCSprite*>(startScreen->background);
    
    CCSize screenSize = startScreen->getContentSize();
    
    background->setScaleX(screenSize.width / background->getContentSize().width);
    background->setScaleY(screenSize.height / background->getContentSize().height);
    
    CCScene* scene = CCScene::create();
    scene->addChild(startScreen);

    pDirector->replaceScene(scene);
}

//CreditsLayer
CreditsLayer* CreditsLayer::getInstance(CCLayer* const parent)
{
    do {
        CC_BREAK_IF(!parent);
        CreditsLayer* newLayer = CreditsLayer::create();
        CC_BREAK_IF(!newLayer);
        newLayer->setPosition(ccp(0.0f,0.0f));
        newLayer->setContentSize(parent->getContentSize());
        CC_BREAK_IF(!newLayer->restart());
        parent->removeAllChildrenWithCleanup(true);
        parent->addChild(newLayer);
        return newLayer;
    } while (false);
    return NULL;
}

bool CreditsLayer::init()
{
    return CCLayer::init();
}

bool CreditsLayer::restart()
{
    return this->createCreditList();
}

bool CreditsLayer::createCreditList()
{
    do {
        CCSize size = this->getContentSize();
        
        char buffer[256] = {0};
        sprintf(buffer, "%s & %s", GameData::getText("game_design"), GameData::getText("game_programming"));
        
        CCLabelTTF* label = CCLabelTTF::create(buffer, "Helvetica", 24);
        label->setPosition(ccp(size.width/2, size.height * 0.9));
        label->setColor(ccc3(115, 115, 115));
        this->addChild(label);
        
        label = CCLabelTTF::create(GameData::getText("name_likezhang"), "Helvetica", 24);
        label->setPosition(ccp(size.width/2, size.height * 0.8));
        label->setColor(ccc3(0, 0, 0));
        this->addChild(label);
        
        
        label = CCLabelTTF::create(GameData::getText("game_art"), "Helvetica", 24);
        label->setPosition(ccp(size.width/2, size.height * 0.6));
        label->setColor(ccc3(115, 115, 115));
        this->addChild(label);
        
        label = CCLabelTTF::create(GameData::getText("name_junma"), "Helvetica", 24);
        label->setPosition(ccp(size.width/2, size.height * 0.5));
        label->setColor(ccc3(0, 0, 0));
        this->addChild(label);
        
        
        label = CCLabelTTF::create(GameData::getText("game_music"), "Helvetica", 24);
        label->setPosition(ccp(size.width/2, size.height * 0.3));
        label->setColor(ccc3(115, 115, 115));
        this->addChild(label);
        
        label = CCLabelTTF::create(GameData::getText("name_wenhaiwang"), "Helvetica", 24);
        label->setPosition(ccp(size.width/2, size.height * 0.2));
        label->setColor(ccc3(0, 0, 0));
        this->addChild(label);
        return true;
    }while (false);
    return false;
}
