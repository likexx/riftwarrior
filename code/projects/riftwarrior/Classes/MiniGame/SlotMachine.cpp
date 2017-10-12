//
//  SlotMachine.cpp
//  tdgame
//
//  Created by Like Zhang on 4/11/13.
//
//

#include "SlotMachine.h"

#include "GameScene.h"
#include "Player.h"
#include "LoadingScreen.h"
#include "SimpleAudioEngine.h"
#include "constant.h"

bool SlotMachine::init()
{
    if (!CCLayerColor::initWithColor(ccc4(1,1,1,128)))
    {
        return false;
    }

    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* background = CCSprite::create("UI/winning_menu/background.png");
    Function::scaleNodeForScreen(background, ccp(0.5f,0.5f));    
    background->setAnchorPoint(ccp(0.5, 0.5));
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    m_pBackground = background;
    
    const MapConfig* pConfig = GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId());

    m_MaxValue = pConfig->bonusMoney;
    
    CCPoint pos = ccp(134, 190);
    
    m_Thousands = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
    m_Thousands->setPosition(pos);
    m_Thousands->setScale(2);
    background->addChild(m_Thousands);
    
    if (m_MaxValue > 99)
    {
        pos = ccp(300, 190);
        m_Hundreds = CCLabelBMFont::create("", "font/winning_menu_number.fnt");
        m_Hundreds->setPosition(pos);
        m_Hundreds->setScale(2);
        background->addChild(m_Hundreds);
    }
    

    if (m_MaxValue > 9)
    {
        pos = ccp(456,190);
        m_Tens = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
        m_Tens->setPosition(pos);
        m_Tens->setScale(2);
        background->addChild(m_Tens);
        
    }
    
    pos = ccp(612, 190);
    m_Digits = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
    m_Digits->setPosition(pos);
    m_Digits->setScale(2);
    background->addChild(m_Digits);
    
    /*
    const char* pszTitle = GameData::getText("slot_machine_title");
    
    CCLabelBMFont* title = CCLabelBMFont::create(pszTitle, "font/slot_machine_label.fnt", m_pBackground->getContentSize().width * 0.9);
    title->setPosition(ccp(m_pBackground->getContentSize().width * 0.5, m_pBackground->getContentSize().height * 0.8));
    background->addChild(title);
    */
    
    CCLabelBMFont* hint = CCLabelBMFont::create(GameData::getText("slot_machine_hint"), "font/slot_machine_label.fnt");
    hint->setScale(0.8f);
    hint->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height*0.85));
    hint->setAnchorPoint(ccp(0.5, 1));
    background->addChild(hint);
    m_pHint = hint;
    
    CCMenu* menu = CCMenu::create();
    
    CCSprite* button = CCSprite::create("UI/gameover/button.png");
    CCSprite* pressed = CCSprite::create("UI/gameover/button_pressed.png");
    CCMenuItemSprite* exitButton = CCMenuItemSprite::create(button, pressed, this, menu_selector(SlotMachine::stop));
    m_BtnTitle = CCLabelTTF::create(GameData::getText("stop_slot_machine"), STANDARD_NUMBER_FONT_NAME, 32);
    m_BtnTitle->setPosition(ccp(exitButton->getContentSize().width/2, exitButton->getContentSize().height/2));
    exitButton->addChild(m_BtnTitle);
    
    exitButton->setPosition(ccp(background->getContentSize().width/2, background->getContentSize().height * 0.2));
    menu->addChild(exitButton);
    
    m_pCloseBtn = exitButton;
    
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    background->addChild(menu);
    this->addChild(background);
    
    m_MachineStopped = false;
    
    // FIXME
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/slotmachine.mp3", true);
    
    scheduleUpdate();
    
    return true;
}


void SlotMachine::update(float dt)
{
    if (m_MachineStopped)
    {
        return;
    }
    
    const MapConfig* pConfig = GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId());
    
    char buffer[8] = {0};


    if (m_MaxValue > 99)
    {
        sprintf(buffer, "%d", rand()%(1 + m_MaxValue/100));
        m_Hundreds->setString(buffer);
    }
    
    if (m_MaxValue>9)
    {
        sprintf(buffer, "%d", rand()%(1 + (m_MaxValue%100)/10));
        m_Tens->setString(buffer);
    }

    sprintf(buffer, "%d", rand()%10);
    m_Digits->setString(buffer);
    

}

void SlotMachine::gotoWorldMap(cocos2d::CCObject *pSender)
{
    
    CCDirector::sharedDirector()->resume();
    this->removeFromParentAndCleanup(true);
    
    GameScene::getInstance()->exitStageMap();
}


void SlotMachine::stop(cocos2d::CCObject *pSender)
{
    const MapConfig* pConfig = GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId());
    
    int award = rand()%(pConfig->bonusMoney);
    
    char buffer[8] = {0};
    
    if (m_MaxValue > 99)
    {
        sprintf(buffer, "%d", award/100);
        m_Hundreds->setString(buffer);
    }
    
    if (m_MaxValue>9)
    {
        sprintf(buffer, "%d", (award%100)/10);
        m_Tens->setString(buffer);
    }
    
    sprintf(buffer, "%d", award%10);
    m_Digits->setString(buffer);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/coin1.wav", false);
    m_MachineStopped = true;
    Player::getInstance()->updateMoney(award);
    Player::getInstance()->save();
    
    char value[256] = {0};
    sprintf(value, "%s %d", GameData::getText("slot_machine_result"), award);
    m_pHint->setString(value);
    
    
    m_BtnTitle->setString(GameData::getText("go_back_to_worldmap"));
    m_pCloseBtn->setTarget(this, menu_selector(SlotMachine::gotoWorldMap));

    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
}