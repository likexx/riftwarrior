//
//  PlayerStatusView.cpp
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#include "PlayerStatusView.h"
#include "Player.h"

#define UI_FONT_NAME "font/marker_felt_24.fnt"
#define UI_PLAYER_STATUS_FONT_NAME "font/user_status.fnt"
#define UI_PLAYER_LEVEL_FONT_NAME "font/arial_bolder_24.fnt"

#define STATUS_BAR_LENGTH 116

#define BACKGROUND_TAG 1001

bool PlayerStatusView::init()
{

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    winSize.height -= 5;
    
    // load player icon background
    CCSprite* playerBackground = CCSprite::create("UI/user_box/userbox_background.png");
    playerBackground->setPosition(ccp(5 + playerBackground->getContentSize().width/2, winSize.height - playerBackground->getContentSize().height/2));
    this->addChild(playerBackground, 0, BACKGROUND_TAG);

    float x = 20;
    float y = 41;
    
    m_LvLabel = CCLabelBMFont::create("", UI_PLAYER_LEVEL_FONT_NAME);
    m_LvLabel->setAnchorPoint(ccp(0.5,0));
    m_LvLabel->setScale(0.55f);
    m_LvLabel->setPosition(ccp(x, y));
    playerBackground->addChild(m_LvLabel);

    x = 75;
    y = 98;

    /*
    CCSprite* hpBackground = CCSprite::create("UI/user_box/hp_background.png");
    hpBackground->setPosition(ccp(x + hpBackground->getContentSize().width/2, y - hpBackground->getContentSize().height/2));
    this->addChild(hpBackground);
    */
    m_HpBar = CCSprite::create("UI/user_box/hp.png");
    m_HpBar->setAnchorPoint(ccp(0,0.5));
    m_HpBar->setPosition(ccp(x, y));
    playerBackground->addChild(m_HpBar);
    
    m_HpLabel = CCLabelBMFont::create("", UI_PLAYER_STATUS_FONT_NAME);
    m_HpLabel->setPosition(ccp(x + STATUS_BAR_LENGTH/2, y));
    playerBackground->addChild(m_HpLabel);

    
    y = 77;
    /*
    CCSprite* mpBackground = CCSprite::create("UI/user_box/mp_background.png");
    mpBackground->setPosition(ccp(x + mpBackground->getContentSize().width/2, y - mpBackground->getContentSize().height/2));
    this->addChild(mpBackground);
    */
    m_MpBar = CCSprite::create("UI/user_box/mp.png");
    m_MpBar->setAnchorPoint(ccp(0,0.5));
    m_MpBar->setPosition(ccp(x, y));
    playerBackground->addChild(m_MpBar);
    m_MpLabel = CCLabelBMFont::create("", UI_PLAYER_STATUS_FONT_NAME);
    m_MpLabel->setPosition(ccp(x + STATUS_BAR_LENGTH/2, y));
    playerBackground->addChild(m_MpLabel);

    y = 59;
    m_ExpBar = CCSprite::create("UI/user_box/exp.png");
    m_ExpBar->setAnchorPoint(ccp(0,0.5));
    m_ExpBar->setPosition(ccp(x, y));
    playerBackground->addChild(m_ExpBar);
//    m_ExpLabel = attachStatusLabel(playerBackground, "0/0");

    x = 51;
    y = playerBackground->getContentSize().height - 114;
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

CCLabelBMFont* PlayerStatusView::attachStatusLabel(cocos2d::CCNode *background, const char *initialValue)
{
    CCLabelBMFont* label = CCLabelBMFont::create(initialValue, UI_PLAYER_STATUS_FONT_NAME);
    label->setPosition(ccp(background->getContentSize().width*0.6,background->getContentSize().height/2));
    label->setScale(background->getContentSize().height/label->getContentSize().height);
    background->addChild(label);
    
    return label;
}

CCPoint PlayerStatusView::getBottomPosition()
{
    CCSprite* background = static_cast<CCSprite*>(getChildByTag(BACKGROUND_TAG));
    
    CCPoint pos = background->getPosition();
    
    return ccp(pos.x - background->getContentSize().width/2, pos.y - background->getContentSize().height/2);
}

void PlayerStatusView::update(float dt)
{
    Player* player = Player::getInstance();
    
    char data[128] = {0};
    
    sprintf(data, "%d", player->getLevel());
    m_LvLabel->setString(data);

    float hp = player->getHp();
    if (hp<=0)
    {
        hp = 0;
    }
    else if (hp < 1)
    {
        hp = 1;
    }
    
    sprintf(data, "%d / %d", (int)hp, player->getMaxHp());
    m_HpLabel->setString(data);
    float ratio = (float)player->getHp()/player->getMaxHp();
    float scale = (STATUS_BAR_LENGTH / m_HpBar->getContentSize().width) * ratio;
    m_HpBar->setScaleX(scale);

    sprintf(data, "%d / %d", player->getMp(), player->getMaxMp());
    m_MpLabel->setString(data);
    ratio = (float)player->getMp()/player->getMaxMp();
    m_MpBar->setScaleX((STATUS_BAR_LENGTH / m_MpBar->getContentSize().width) * ratio);

    /*
    sprintf(data, "%d / %d", player->getExperience(), player->getNextLevelExperience());
    m_ExpLabel->setString(data);
    */
    ratio = (float)player->getExperience()/player->getNextLevelExperience();
    m_ExpBar->setScaleX((STATUS_BAR_LENGTH / m_ExpBar->getContentSize().width) * ratio);

    sprintf(data, "%d", player->getMoney());
    m_MoneyLabel->setString(data);
    
    sprintf(data, "%d", player->getGems());
    m_GemLabel->setString(data);
}