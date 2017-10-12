//
//  LoadingScreen.cpp
//  tdgame
//
//  Created by Like Zhang on 3/28/13.
//
//

#include "LoadingScreen.h"
#include "GameScene.h"
#include "PlayerAnimation.h"
#include "EnemyAnimation.h"
#include "Player.h"
#include "constant.h"

LoadingScreen* LoadingScreen::create(int mapId)
{
    
    LoadingScreen* pScreen = new LoadingScreen();
    if (pScreen && pScreen->init(mapId))
    {
        pScreen->autorelease();
        return pScreen;
    }
    
    CC_SAFE_DELETE(pScreen);
    return NULL;
}

bool LoadingScreen::init(int mapId)
{
    if (!CCScene::init())
    {
        return false;
    }
    
    m_MapId = mapId;

    const char* tips[] = {
        GameData::getText("tip_1"),
        GameData::getText("tip_2"),
        GameData::getText("tip_3"),
        GameData::getText("tip_4"),
        GameData::getText("tip_5"),
    };
    
    size_t tipSize = sizeof(tips)/sizeof(tips[0]);
    
    string tip = "";
    tip += tips[rand()%tipSize];
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    m_LoadingInformation = CCLabelTTF::create(GameData::getText("loading_game_data"), STANDARD_FONT_NAME, 48);
    m_LoadingInformation->setColor(ccc3(128, 255, 0));
    m_HintLabel = CCLabelTTF::create(tip.c_str(), STANDARD_FONT_NAME, 36);
    m_HintLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    m_LoadingInformation->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(m_LoadingInformation);
    
    m_HintLabel->setPosition(ccp(winSize.width/2, m_HintLabel->getContentSize().height));
    addChild(m_HintLabel);

    m_TotalRunningTime = 0;
    
    m_LoadNormalStage = true;
    
    scheduleOnce(schedule_selector(LoadingScreen::startLoading), 0.5);
    scheduleUpdate();
    
    return true;
}

void LoadingScreen::startLoading()
{
    
    unschedule(schedule_selector(LoadingScreen::startLoading));
    
    GameScene::getInstance()->loadData();
    

    if (m_MapId > 0)
    {
        // enter map directly
        GameScene::getInstance()->loadStageMap(m_MapId, m_LoadNormalStage);
    }
    else
    {
        // no specific map is assigned, show world map
        CCDirector* pDirector = CCDirector::sharedDirector();
        Player* player = Player::getInstance();
        
//        player->save();
        player->load();
        
        GameScene::getInstance()->sharedMainUI->showWorldMap();
        CCScene *pScene = GameScene::getInstance()->sharedScene;
        pScene->resumeSchedulerAndActions();
        pDirector->replaceScene(CCTransitionFade::create(1, pScene));

    }

}

void LoadingScreen::update(float dt)
{
    
    m_TotalRunningTime += dt;
    
    
    int tailValue = (int)m_TotalRunningTime%10;

    string value = GameData::getText("loading_game_data");

    for(int i=0;i<tailValue;++i)
    {
        value += ".";
    }
    
    m_LoadingInformation->setString(value.c_str());

}