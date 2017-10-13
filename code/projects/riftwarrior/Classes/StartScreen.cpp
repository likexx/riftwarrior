//
//  StartMenu.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/1/12.
//
//

#include "StartScreen.h"
#include "StartScreenBuilderLoader.h"
#include "GameScene.h"
#include "Player.h"
#include "Function.h"
#include "LoadingScreen.h"
#include "SystemHelper.h"
#include "KKMenu.h"
#include "SimpleAudioEngine.h"
#include "HomeSettingMenu.h"

#define CREDITS_TAG 23598
#define CREDITS_MENU_TAG 34592352109

StartScreen* StartScreen::createNewScreen()
{
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCNodeLoaderLibrary *ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("StartScreen", StartScreenBuilderLoader::loader());
    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    
    StartScreen * node = static_cast<StartScreen*>(ccbReader->readNodeGraphFromFile("ccb/StartScreen.ccbi"));

    if (!Function::hasPlayerData())
    {
        // player data doesn't exist 
        CCLabelTTF* label = static_cast<CCLabelTTF*>(node->firstButton->getTitleLabel());
        label->setString(GameData::getText("start_screen_new_game"));
        node->firstButton->addTargetWithActionForControlEvents(node, cccontrol_selector(StartScreen::startNewGame), CCControlEventTouchDown);
    }
    else
    {
        CCLabelTTF* label = static_cast<CCLabelTTF*>(node->firstButton->getTitleLabel());
        label->setString(GameData::getText("start_screen_continue"));
        node->firstButton->addTargetWithActionForControlEvents(node, cccontrol_selector(StartScreen::continueGame), CCControlEventTouchDown);
    }
    
    CCLabelTTF* label = static_cast<CCLabelTTF*>(node->settingButton->getTitleLabel());
    label->setString(GameData::getText("home_setting"));
    node->settingButton->addTargetWithActionForControlEvents(node, cccontrol_selector(StartScreen::openMenu), CCControlEventTouchDown);
    
    string copyright = "MIT Licensed by Like Zhang, 2017\n";
    node->m_pCopyrightLabel->setString(( copyright + SystemHelper::getVersionInfo() ).c_str());
    node->m_pCopyrightLabel->setFontSize(16);
    node->m_pCopyrightLabel->setHorizontalAlignment(kCCTextAlignmentCenter);

    node->scheduleUpdate();
    return node;
}


void StartScreen::openMenu(
	CCObject * /* dummy parameter to pass macro cccontrol_selector */,
	CCControlEvent /* dummy parameter to pass macro cccontrol_selector */)
{
    do {
        CC_BREAK_IF(this->isShowSettingMenu);
        HomeSettingMenu* settingMenu = HomeSettingMenu::create();
        CC_BREAK_IF(!settingMenu);
        settingMenu->setPosition(ccp(0, 0));
        settingMenu->setScale(0.1f);
        this->screenLock();
        settingMenu->runAction(CCScaleTo::create(0.2f, 1.0f));
        settingMenu->setCloseCallAction(CCCallFunc::create(this,callfunc_selector(StartScreen::screenUnlock)));
        this->addChild(settingMenu);
    } while (false);
    
}


void StartScreen::startNewGame(
	CCObject * /* dummy parameter to pass macro cccontrol_selector */,
	CCControlEvent /* dummy parameter to pass macro cccontrol_selector */)
{
    if (isShowSettingMenu)
	{
        return;
    }
    
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    
    Player* pPlayer = Player::getInstance(); // initialize data

    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    pUserDefault->purgeSharedUserDefault();
    pUserDefault->flush();
    
    pPlayer->save();
    
    GameScene::getInstance()->sharedMainUI->showWorldMap();
    CCScene *pScene = GameScene::getInstance()->sharedScene;
    CCDirector::sharedDirector()->pushScene(pScene);

    LoadingScreen* pLoadingScreen = LoadingScreen::create(101);
    
    CCDirector::sharedDirector()->pushScene(pLoadingScreen);
}

void StartScreen::createNewGame()
{
    
}

void StartScreen::continueGame(
	CCObject * /* dummy parameter to pass macro cccontrol_selector */,
	CCControlEvent /* dummy parameter to pass macro cccontrol_selector */)
{

    
    if(isShowSettingMenu){
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    
    LoadingScreen* pLoadingScreen = LoadingScreen::create(0);
    
    CCDirector::sharedDirector()->pushScene(pLoadingScreen);


    
}



SEL_MenuHandler StartScreen::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    
    return NULL;
}

extension::SEL_CCControlHandler StartScreen::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{

    
    return NULL;
}

bool StartScreen::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "background", CCSprite *, this->background);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "leftImage", CCSprite *, this->leftImage);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "rightImage", CCSprite *, this->rightImage);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "logoImage", CCSprite *, this->logoImage);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "firstButton", CCControlButton *, this->firstButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "settingBtn", CCControlButton *, this->settingButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "copyrightLabel", CCLabelTTF*, this->m_pCopyrightLabel);

    return false;
}

void StartScreen::onNodeLoaded(cocos2d::CCNode *pNode, cocos2d::extension::CCNodeLoader *pNodeLoader)
{
    totalTime = 0;
    isShowSettingMenu = false;
    
    initButtonAction();
    resetButtonLabels();
}

void StartScreen::onEnter()
{
    CCLayer::onEnter();
    resetButtonLabels();
}

void StartScreen::initButtonAction()
{
    /*
    if(forumButton != NULL){
        forumButton->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScreen::openForumUrl),
                                                         CCControlEventTouchDown);
    }
    if (creditsButton != NULL) {
        creditsButton->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScreen::showCredits),
                                                           CCControlEventTouchDown);
    }
     */
}

void StartScreen::resetButtonLabels()
{
    
    if (!Function::hasPlayerData())
    {
        // player data doesn't exist
        CCLabelTTF* label = static_cast<CCLabelTTF*>(firstButton->getTitleLabel());
        label->setString(GameData::getText("start_screen_new_game"));
        
    }
    else
    {
        CCLabelTTF* label = static_cast<CCLabelTTF*>(firstButton->getTitleLabel());
        label->setString(GameData::getText("start_screen_continue"));
        
    }

    CCLabelTTF* label = static_cast<CCLabelTTF*>(settingButton->getTitleLabel());
    label->setString(GameData::getText("home_setting"));
    

}

void StartScreen::update(float dt)
{
    totalTime += dt;
    /*
    background->getShaderProgram()->use();
    glUniform1f(timeUniformLocation, totalTime);
    */
}

void StartScreen::setupBackgroundEffect()
{
    
    CCLog("%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    CCGLProgram* shaderProgram = new CCGLProgram();
    shaderProgram->initWithVertexShaderFilename("PositionColor.vsh", "StartScreen.fsh");
    
    //    shaderProgram->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, fragmentSource);
    background->setShaderProgram(shaderProgram);
    
    shaderProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    shaderProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    shaderProgram->link();
    shaderProgram->updateUniforms();
    
    timeUniformLocation = glGetUniformLocation(background->getShaderProgram()->getProgram(), "u_time");
    background->getShaderProgram()->use();

}

void StartScreen::screenLock()
{
    this->isShowSettingMenu = true;
}

void StartScreen::screenUnlock()
{
    this->isShowSettingMenu = false;
}

