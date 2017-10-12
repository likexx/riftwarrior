//
//  SimplePlayerView.cpp
//  tdgame
//
//  Created by Like Zhang on 7/11/13.
//
//

#include "SimplePlayerView.h"
#include "Player.h"

#define UI_FONT_NAME "font/marker_felt_24.fnt"
#define UI_PLAYER_STATUS_FONT_NAME "font/user_status.fnt"
#define UI_PLAYER_LEVEL_FONT_NAME "font/arial_bolder_24.fnt"

#define STATUS_BAR_LENGTH 116

#define BACKGROUND_TAG 1001

bool SimplePlayerView::init()
{
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    winSize.height -= 5;
    
    // load player icon background
    CCSprite* playerBackground = CCSprite::create("UI/user_box/simple_userbox.png");
    playerBackground->setPosition(ccp(5 + playerBackground->getContentSize().width/2, winSize.height - playerBackground->getContentSize().height/2));
    this->addChild(playerBackground, 0, BACKGROUND_TAG);

    float x = 51;
    float y = 6 ;
    m_MoneyLabel = CCLabelBMFont::create("0", "font/arial.fnt");
    m_MoneyLabel->setPosition(ccp(x, y));
    m_MoneyLabel->setAnchorPoint(ccp(0, 0));
    m_MoneyLabel->setScale(0.5);
    playerBackground->addChild(m_MoneyLabel);
    
    x = 145;
    
    m_GemLabel = CCLabelBMFont::create("0", "font/arial.fnt");
    m_GemLabel->setAnchorPoint(ccp(0, 0));
    m_GemLabel->setPosition(ccp(x, y));
    m_GemLabel->setScale(0.5);
    playerBackground->addChild(m_GemLabel);
    
    scheduleUpdate();
    
    return true;
    
}

CCLabelBMFont* SimplePlayerView::attachStatusLabel(cocos2d::CCNode *background, const char *initialValue)
{
    CCLabelBMFont* label = CCLabelBMFont::create(initialValue, UI_PLAYER_STATUS_FONT_NAME);
    label->setPosition(ccp(background->getContentSize().width*0.6,background->getContentSize().height/2));
    label->setScale(background->getContentSize().height/label->getContentSize().height);
    background->addChild(label);
    
    return label;
}


void SimplePlayerView::update(float dt)
{
    Player* player = Player::getInstance();
    
    char data[128] = {0};
    
    sprintf(data, "%d", player->getMoney());
    m_MoneyLabel->setString(data);
    
    sprintf(data, "%d", player->getGems());
    m_GemLabel->setString(data);
}