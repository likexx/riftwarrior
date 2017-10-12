//
//  GameRoundWinMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 3/18/13.
//
//

#include "GameWinningMenu.h"

#include "GameScene.h"
#include "Player.h"
#include "LoadingScreen.h"
#include "SimpleAudioEngine.h"
#include "constant.h"

bool GameWinningMenu::init()
{
    if (!CCLayerColor::initWithColor(ccc4(1,1,1,128)))
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* background = CCSprite::create("UI/winning_menu/win.png");
    Function::scaleNodeForScreen(background, ccp(0.5f,0.5f));    
    background->setAnchorPoint(ccp(0.5, 1));
    background->setPosition(ccp(winSize.width/2, winSize.height * 1.5));
    CCMenu* menu = CCMenu::create();
    
    CCLabelBMFont* labels = CCLabelBMFont::create(GameData::getText("winning_info"), "font/winning_menu.fnt", background->getContentSize().width);
    labels->setAnchorPoint(ccp(0,1));
    labels->setColor(ccc3(0, 0, 0));
    labels->setScale(0.8f);
    labels->setPosition(ccp(background->getContentSize().width * 0.1, background->getContentSize().height * 0.72));
    background->addChild(labels);

    CCSprite* button = CCSprite::create("UI/greenbutton_nomal.png");
    CCSprite* pressed = CCSprite::create("UI/greenbutton_pressed.png");
    CCMenuItemSprite* exitButton = CCMenuItemSprite::create(button, pressed, this, menu_selector(GameWinningMenu::gotoWorldMap));
    CCLabelTTF* message = CCLabelTTF::create(GameData::getText("go_back_to_worldmap"), STANDARD_NUMBER_FONT_NAME, 28);
    message->setPosition(ccp(exitButton->getContentSize().width/2, exitButton->getContentSize().height/2));
    exitButton->addChild(message);
    exitButton->setPosition(ccp(background->getContentSize().width * 0.5, background->getContentSize().height * 0.085));
    menu->addChild(exitButton);
    
//    button = CCSprite::create("UI/greenbutton_nomal.png");
//    pressed = CCSprite::create("UI/greenbutton_pressed.png");
//    CCMenuItemSprite* shareButton = CCMenuItemSprite::create(button, pressed,this,menu_selector(GameWinningMenu::share));
//    CCLabelTTF* shareWords = CCLabelTTF::create(GameData::getText("share"),STANDARD_NUMBER_FONT_NAME, 28);
//    shareWords->setPosition(ccp(shareButton->getContentSize().width/2, shareButton->getContentSize().height/2));
//    shareButton->addChild(shareWords);
//    shareButton->setPosition(ccp(background->getContentSize().width * 0.335, background->getContentSize().height * 0.085));
//    menu->addChild(shareButton);
    
    m_pCloseBtn = exitButton;
    m_pCloseBtn->setVisible(false);
//    m_pShareBtn = shareButton;
//    m_pShareBtn->setVisible(false);
    
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    background->addChild(menu);
    this->addChild(background);
    
    m_pBackground = background;
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
    
//    showTotalEnemiesKilledByPlayer();
    
    CCSmartSprite* icon = CCSmartSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/winning_menu/win_icon.png"));
    
    icon->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(icon);

    background->setPosition(ccp(background->getPosition().x, background->getPosition().y + background->getContentSize().height));
    
    icon->setScale(0.2f);
    icon->runAction(CCSequence::create(CCScaleTo::create(0.3f, 1.0f),
                                       CCScaleTo::create(0.1f, 0.9f),
                                       CCScaleTo::create(0.1f, 1.0f),
                                       CCDelayTime::create(1),
                                       CCCallFunc::create(icon, callfunc_selector(CCSmartSprite::removeSelf)),
                                       NULL));

    background->runAction(CCSequence::create(CCDelayTime::create(1.5),
                                             CCMoveTo::create(0.2f, ccp(background->getPosition().x, winSize.height)),
                                             CCCallFunc::create(this, callfunc_selector(GameWinningMenu::showTotalBuildingCreated)),
                                             NULL));
    
//    showTotalBuildingCreated();
    
    // FIXME
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/cheering.mp3");

    scheduleUpdate();
    
    return true;
}

void GameWinningMenu::showEnemyKillerScore()
{
    unschedule(schedule_selector(GameWinningMenu::showEnemyKillerScore));
    
    //CCPoint pos = ccp(m_pBackground->getContentSize().width * 0.8,  215);
    CCLabelBMFont* value = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
    value->setPosition(ccp(m_pBackground->getContentSize().width * 0.85,m_pBackground->getContentSize().height * 0.5));
    
    value->setString("0");
    m_pBackground->addChild(value);

    m_CurrentValue = 0;
    m_MaxValue = GameScene::getInstance()->sharedGameStage->getMonsterKillerScore();
    m_pTargetLabel = value;
    
    //updateAnimationValues();

//    scheduleOnce(schedule_selector(GameWinningMenu::showTotalBuildingCreated), 0.5);
    m_pNextCallback =  schedule_selector(GameWinningMenu::showAwardedExp);
}

void GameWinningMenu::showTotalBuildingCreated()
{
    unschedule(schedule_selector(GameWinningMenu::showTotalBuildingCreated));
    CCLog("show total building created");
    
    //CCPoint pos = ccp(m_pBackground->getContentSize().width * 0.8, 340);
    CCLabelBMFont* value = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
    value->setPosition(ccp(m_pBackground->getContentSize().width * 0.85,m_pBackground->getContentSize().height * 0.69));

    int maxValue = GameScene::getInstance()->sharedGameStage->getBuildingManager()->getTotalBuildingsCreated();
    value->setString(CCString::createWithFormat("%d", maxValue)->getCString());
    m_pBackground->addChild(value);
    
    /*
    m_CurrentValue = 0;
    m_MaxValue = GameScene::getInstance()->sharedGameStage->getBuildingManager()->getTotalBuildingsCreated();
    m_pTargetLabel = value;
     */

   // updateAnimationValues();
    
    m_pNextCallback = schedule_selector(GameWinningMenu::showTotalBuildingDestroyed);
}

void GameWinningMenu::showTotalBuildingDestroyed()
{
    unschedule(schedule_selector(GameWinningMenu::showTotalBuildingDestroyed));

    //CCPoint pos = ccp(m_pBackground->getContentSize().width * 0.8, m_pBackground->getContentSize().height * 0.52);
    CCLabelBMFont* value = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
    value->setPosition(ccp(m_pBackground->getContentSize().width * 0.85,m_pBackground->getContentSize().height * 0.63));
    
    /*
    m_CurrentValue = 0;
    m_MaxValue = GameScene::getInstance()->sharedGameStage->getBuildingManager()->getTotalBuildingDestroyed();
    m_pTargetLabel = value;
     */
    
    int maxValue = GameScene::getInstance()->sharedGameStage->getBuildingManager()->getTotalBuildingDestroyed();
    value->setString(CCString::createWithFormat("%d", maxValue)->getCString());
    m_pBackground->addChild(value);

    //updateAnimationValues();

    m_pNextCallback = schedule_selector(GameWinningMenu::showLoseRate);
}

void GameWinningMenu::showLoseRate()
{
    unschedule(schedule_selector(GameWinningMenu::showLoseRate));
    
    //CCPoint pos = ccp(m_pBackground->getContentSize().width * 0.8, m_pBackground->getContentSize().height * 0.45);
    CCLabelBMFont* value = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
    value->setPosition(ccp(m_pBackground->getContentSize().width * 0.85,m_pBackground->getContentSize().height * 0.56));
    
    float loseRate = getLoseRate();
    
    value->setString(CCString::createWithFormat("%d%%", (int)(loseRate * 100))->getCString());
    m_pBackground->addChild(value);
    
    Player::getInstance()->setMapScore(GameScene::getInstance()->sharedGameStage->getMapId(), 100 - (int)(loseRate*100));
    
    m_pNextCallback = schedule_selector(GameWinningMenu::showEnemyKillerScore);
}


void GameWinningMenu::showAwardedExp()
{
    unschedule(schedule_selector(GameWinningMenu::showAwardedExp));
    
    CCSize backSize = m_pBackground->getContentSize();

    CCSprite* expIcon = CCSprite::create("UI/winning_menu/exp_icon.png");
    //expIcon->setPosition(ccp(pos.x - expIcon->getContentSize().width/2, pos.y + value->getContentSize().height/2));
    expIcon->setPosition(ccp(backSize.width * 0.35,backSize.height * 0.23));
    m_pBackground->addChild(expIcon);
    
    CCLabelBMFont* value = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
    value->setPosition(ccp(expIcon->getPosition().x + expIcon->getContentSize().width/2 + value->getContentSize().width/2 + 15,backSize.height * 0.23));
    value->setAnchorPoint(ccp(0.5,0.5));
    float loseRate = getLoseRate();
    const MapConfig* pConfig = GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId());
    /*
    m_CurrentValue = 0;
    m_MaxValue = pConfig->bonusExperience * (1-loseRate);
    m_pTargetLabel = value;
     */
    
    int maxValue = pConfig->bonusExperience * (1-loseRate);
    value->setString(CCString::createWithFormat("%d", maxValue)->getCString());
    m_pBackground->addChild(value);

    CCLabelBMFont* awardTitle = CCLabelBMFont::create(GameData::getText("game_winning_congratulation"), "font/winning_menu.fnt");
    awardTitle->setScale(0.7f);
    awardTitle->setColor(ccc3(0,0,0));
    awardTitle->setAnchorPoint(ccp(0, 0.5));
    awardTitle->setPosition(ccp(m_pBackground->getContentSize().width * 0.07, m_pBackground->getContentSize().height * 0.23));
    m_pBackground->addChild(awardTitle);
    
    //updateAnimationValues();

    m_pNextCallback = schedule_selector(GameWinningMenu::showAwardedMoney);
}


void GameWinningMenu::showAwardedMoney()
{
    unschedule(schedule_selector(GameWinningMenu::showAwardedMoney));

    
    CCPoint pos = ccp(530, 150);
    CCSize backSize = m_pBackground->getContentSize();
    
    CCSprite* coinIcon = CCSprite::create("UI/winning_menu/coin_icon.png");
    coinIcon->setPosition(ccp(backSize.width * 0.7,backSize.height * 0.23));
    m_pBackground->addChild(coinIcon);
    
    CCLabelBMFont* value = CCLabelBMFont::create("", "font/winning_menu_number.fnt", m_pBackground->getContentSize().width * 0.2);
    value->setPosition(ccp(coinIcon->getPosition().x + coinIcon->getContentSize().width/2 + value->getContentSize().width/2 + 15,backSize.height * 0.23));
    value->setAnchorPoint(ccp(0.5f, 0.5f));
    float loseRate = getLoseRate();
    const MapConfig* pConfig = GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId());
    /*
    m_CurrentValue = 0;
    m_MaxValue = pConfig->bonusMoney * (1-loseRate);
    m_pTargetLabel = value;
     */
    int maxValue = pConfig->bonusMoney * (1-loseRate);
    value->setString(CCString::createWithFormat("%d",maxValue)->getCString());
    m_pBackground->addChild(value);   
    
    //updateAnimationValues();

    // player experience is updated here because the experience bonus animation is just done
    Player::getInstance()->updateExperience((1 - loseRate) * pConfig->bonusExperience);

    
    m_pNextCallback = schedule_selector(GameWinningMenu::showStarLevel);
}

void GameWinningMenu::showButtons()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/coin1.wav");
    
    unschedule(schedule_selector(GameWinningMenu::showButtons));
    
    float loseRate = getLoseRate();
    
    const MapConfig* pConfig = GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId());

    // same as experience update. Money bonus animation is just done here.
    Player::getInstance()->updateMoney(pConfig->bonusMoney * (1-loseRate));


    m_pCloseBtn->setVisible(true);
//    m_pShareBtn->setVisible(true);
    
    m_pNextCallback = NULL;
    m_pTargetLabel = NULL;
    
}

void GameWinningMenu::showStarLevel()
{
    unschedule(schedule_selector(GameWinningMenu::showStarLevel));
    int stageId = GameScene::getInstance()->sharedGameStage->getMapId();
    int score = Player::getInstance()->getMapScore(stageId);
    starCount = 0;
    if(score >= 0 && score < 70){
        starCount = 1;
    }else if(score >= 70 && score < 90){
        starCount = 2;
    }else if(score >= 90){
        starCount = 3;
    }
    addStarCallFunc();
    m_pNextCallback = NULL;
}

void GameWinningMenu::addStarCallFunc()
{
    float x = m_pBackground->getContentSize().width * 0.287;
    float y = m_pBackground->getContentSize().height * 0.905;
    if(starCount > 0){
        x += m_pBackground->getContentSize().width * 0.205 * (3 - (starCount--));
        CCSprite* star = CCSprite::create("UI/winning_menu/star.png");
        star->setScale(5.0f);
        star->setPosition(ccp(x,y));
        CCFiniteTimeAction * actions = CCSequence::create(CCScaleTo::create(0.3f, 1.0f),
                                                          CCCallFunc::create(this, callfunc_selector(GameWinningMenu::addStarCallFunc)),
                                                          NULL);
        star->runAction(actions);
        m_pBackground->addChild(star);
    }else{
        showButtons();
    }
}

/*void GameWinningMenu::updateAnimationValues()
{
    if (m_MaxValue - m_CurrentValue < 10)
    {
        m_ValueIncrement = (m_MaxValue - m_CurrentValue)/0.5;
    }
    else if (m_MaxValue - m_CurrentValue < 500)
    {
        m_ValueIncrement = (m_MaxValue - m_CurrentValue)/1.0;
    }
    else
    {
        m_ValueIncrement = (m_MaxValue - m_CurrentValue)/2.0;
    }
}*/


float GameWinningMenu::getLoseRate()
{
    if (GameScene::getInstance()->sharedGameStage->getBuildingManager()->getTotalBuildingsCreated() == 0)
    {
        return 0;
    }
    
    float loseRate = (float)GameScene::getInstance()->sharedGameStage->getBuildingManager()->getTotalBuildingDestroyed()/(float)GameScene::getInstance()->sharedGameStage->getBuildingManager()->getTotalBuildingsCreated();
    
    return loseRate;
}


void GameWinningMenu::update(float dt)
{
    if (m_pNextCallback){
        scheduleOnce(m_pNextCallback, 0.1f);
    }
    /*
    if (m_pTargetLabel)
    {
        m_CurrentValue = m_MaxValue;
        CCLabelBMFont* pLabel = m_pTargetLabel;
        m_pTargetLabel = NULL;
        if (m_pNextCallback){
            scheduleOnce(m_pNextCallback, 0.1);
        }
        char buffer[128] = {0};
        sprintf(buffer, "%d", (int)m_CurrentValue);
        pLabel->setString(buffer);
    }
     */
}

//void GameWinningMenu::share()
//{
//    SnsHelper::shareScreenShot(SCORESHARE);
//}

void GameWinningMenu::gotoNextMap(cocos2d::CCObject *pSender)
{
    Player::getInstance()->reset();

    CCDirector::sharedDirector()->resume();
    this->removeFromParentAndCleanup(true);
 
    int currentMapId = GameScene::getInstance()->sharedGameStage->getMapId();

    // save the used count for each "next map"
    map<int, int> counters;
    
    const map<int,MapConfig>& maps = GameData::getAllMapConfigs();

    map<int, MapConfig>::const_iterator it= maps.begin();
    while(it != maps.end())
    {
        // loop 1, find all maps requiring current map
        const MapConfig& config = it->second;
        
        if (config.requireMapId == currentMapId)
        {
            if (counters.find(config.mapId) == counters.end())
            {
                // set the id if it's new
                counters[config.mapId] = 1;
            }
        }
        
        ++it;
    }
    
    map<int, int>::iterator itCounters;
    it= maps.begin();
    while(it != maps.end())
    {
        // loop 2, for each id in the found maps, check if it's required by others
        itCounters = counters.begin();
        
        while (itCounters != counters.end())
        {
            if (it->second.requireMapId == itCounters->first)
            {
                itCounters->second++;
            }
            
            ++itCounters;
        }
        
        ++it;
    }

    int nextMapId = -1;
    int maxCount = -1;
    itCounters = counters.begin();
    while(itCounters != counters.end())
    {
        if (itCounters->second > maxCount)
        {
            maxCount = itCounters->second;
            nextMapId = itCounters->first;
        }
        ++itCounters;
    }

    if (nextMapId >= 0)
    {
//        GameScene::getInstance()->loadStageMap(nextMapId);
        LoadingScreen* pNextScreen = LoadingScreen::create(nextMapId);
        CCDirector::sharedDirector()->pushScene(pNextScreen);
    }
    
}

void GameWinningMenu::gotoWorldMap(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    CCDirector::sharedDirector()->resume();
    this->removeFromParentAndCleanup(true);

    Player::getInstance()->save();
    
    
    int stageId = GameScene::getInstance()->sharedGameStage->getMapId();
    int score = Player::getInstance()->getMapScore(stageId);
    starCount = 0;
    if(score >= 0 && score < 70){
        starCount = 1;
    }else if(score >= 70 && score < 90){
        starCount = 2;
    }else if(score >= 90){
        starCount = 3;
    }
    
    if (GameScene::getInstance()->sharedGameStage->matchWinningCondition() && starCount >= 2)
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showSlotMachine();
    }
    else
    {
        GameScene::getInstance()->exitStageMap();
    }
}

