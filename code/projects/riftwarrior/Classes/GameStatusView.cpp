//
//  GameStatusView.cpp
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#include "GameStatusView.h"
#include "GameData.h"
#include "Player.h"
#include "GameScene.h"

bool GameStatusView::init()
{
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    winSize.height -= 5;

    float bgWidth = 0;
    float bgHeight = 0;
    // load player icon background

    

    
    m_waveborad = CCSprite::create("UI/wave_background.png");
    m_waveborad->setAnchorPoint(ccp(0.5,1));
    m_waveborad->setPosition(ccp(winSize.width/2, winSize.height));
    addChild(m_waveborad);
    
    m_WaveLabel = CCLabelBMFont::create("WAVE 0/0", "font/wave.fnt", 200);
    m_WaveLabel->setScale(0.75);
    m_WaveLabel->setAnchorPoint(ccp(0.55, 1));
    m_WaveLabel->setPosition(ccp(m_waveborad->getContentSize().width/2, m_waveborad->getContentSize().height));
    m_waveborad->addChild(m_WaveLabel);
    
    bgWidth = m_WaveLabel->getContentSize().width;
    
    float x = m_waveborad->getContentSize().width/2;
    float y = m_waveborad->getContentSize().height;
    
    float space = 0.0f;
    m_TotalKilledCountLabel = CCLabelBMFont::create("0", "font/game_status.fnt");   
    CCLabelBMFont* label = CCLabelBMFont::create(GameData::getText("total_killed_label"), "font/game_status.fnt");
    space = (m_waveborad->getContentSize().width - label->getContentSize().width - m_TotalKilledCountLabel->getContentSize().width) / 2;
    label->setAnchorPoint(ccp(0, 1));
    label->setPosition(space, m_waveborad->getContentSize().height - m_WaveLabel->getContentSize().height);
    m_waveborad->addChild(label);
    
    m_TotalKilledCountLabel->setAnchorPoint(ccp(0, 1));
    m_TotalKilledCountLabel->setPosition(ccp(label->getContentSize().width + space, m_waveborad->getContentSize().height - m_WaveLabel->getContentSize().height));
    m_waveborad->addChild(m_TotalKilledCountLabel);

    y = m_TotalKilledCountLabel->getPosition().y - m_TotalKilledCountLabel->getContentSize().height/2;

    int maxTime = GameScene::getInstance()->sharedGameStage->getWinningCondition()->maxTime;
    if (maxTime > 0)
    {
        label = CCLabelBMFont::create(GameData::getText("countdown_timer_label"), "font/game_status.fnt");
        label->setAnchorPoint(ccp(0, 0.5));
        label->setPosition(x, y - label->getContentSize().height/2);
        addChild(label);

        char data[64] = {0};
        sprintf(data, "%d", maxTime);
        m_TimerLabel = CCLabelBMFont::create("100", "font/game_status.fnt");
        m_TimerLabel->setAnchorPoint(ccp(0, 0.5));
        m_TimerLabel->setPosition(ccp(label->getPosition().x + label->getContentSize().width + 5, y - m_TimerLabel->getContentSize().height/2));
        addChild(m_TimerLabel);
        
        y -= m_TimerLabel->getContentSize().height/2;
    }
    else
    {
        m_TimerLabel = NULL;
    }
    
    int maxAllowedPassingEnemies = GameScene::getInstance()->sharedGameStage->getWinningCondition()->maxPassEnemies;
    if (maxAllowedPassingEnemies > 0)
    {
        label = CCLabelBMFont::create(GameData::getText("passing_enemies_label"), "font/game_status.fnt");
        label->setAnchorPoint(ccp(0, 0.5));
        label->setPosition(x, y - label->getContentSize().height/2);
        addChild(label);
        
        char data[64] = {0};
        sprintf(data, "%d", maxTime);
        m_PassingEnemiesLabel = CCLabelBMFont::create("0", "font/game_status.fnt");
        m_PassingEnemiesLabel->setAnchorPoint(ccp(0, 0.5));
        m_PassingEnemiesLabel->setPosition(ccp(label->getPosition().x + label->getContentSize().width + 5, y - m_PassingEnemiesLabel->getContentSize().height/2));
        addChild(m_PassingEnemiesLabel);
        
        y -= m_PassingEnemiesLabel->getContentSize().height/2;
    }
    
    bgWidth = m_waveborad->getContentSize().width;
    bgHeight = m_waveborad->getContentSize().height;
    
    CCLayerColor* background = CCLayerColor::create(ccc4(1,1,1,196), bgWidth * 1.5, bgHeight * 1.2);
    background->setAnchorPoint(ccp(0, 1));
    background->setPosition(ccp(m_waveborad->getPositionX(),m_waveborad->getPositionY()));
    //addChild(background, -1);
    
    setAnchorPoint(CCPointZero);
    
    scheduleUpdate();
    
    return true;
    
}

void GameStatusView::update(float dt)
{
    char data[128] = {0};
    
    if (GameScene::getInstance()->sharedGameStage->getWinningCondition()->minKilledEnemies <= 0)
    {
        sprintf(data, "%d", Player::getInstance()->getKillCount());
    }
    else
    {
        sprintf(data, "%d/%d", Player::getInstance()->getKillCount(),GameScene::getInstance()->sharedGameStage->getWinningCondition()->minKilledEnemies);
    }
    m_TotalKilledCountLabel->setString(data);
    
    if (m_TimerLabel)
    {
        float maxTime = GameScene::getInstance()->sharedGameStage->getWinningCondition()->maxTime;
        sprintf(data, "%d", (int)(maxTime - GameScene::getInstance()->sharedGameStage->getStageRunningTime()));
        m_TimerLabel->setString(data);
    }
    
    if (m_PassingEnemiesLabel)
    {
        sprintf(data, "%d/%d", GameScene::getInstance()->sharedGameStage->getPassedEnemies(), GameScene::getInstance()->sharedGameStage->getWinningCondition()->maxPassEnemies);
        m_PassingEnemiesLabel->setString(data);
    }
}

void GameStatusView::updateWave(int currentWave, int maxWave)
{
    char data[64] = {0};
    sprintf(data, "WAVE %d/%d", currentWave, maxWave);
    m_WaveLabel->setString(data);
}

void GameStatusView::updateWave(int currentWave)
{
    char data[64] = {0};
    sprintf(data, "WAVE %d", currentWave);
    m_WaveLabel->setString(data);
}