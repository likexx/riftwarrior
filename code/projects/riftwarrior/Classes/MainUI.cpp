//
//  MainUI.cpp
//  tdgame
//
//  Created by Like Zhang on 2/28/13.
//
//

#include "MainUI.h"
#include "ActionMenu.h"
#include "PlayerStatusView.h"
#include "GameStatusView.h"
#include "GameControlMenu.h"
#include "GameOverMenu.h"
#include "GameScene.h"
#include "WorldMap.h"

#define STAGE_UI_TAG 1001
#define WORLDMAP_TAG 1002

bool MainUI::init()
{

    return true;
}


StageUI* MainUI::getStageUI()
{
    CCNode* node = getChildByTag(STAGE_UI_TAG);
    if (node)
    {
        return static_cast<StageUI*>(node);
    }
    
    return NULL;
}

WorldMap* MainUI::getWorldMap()
{
    CCNode* node = getChildByTag(WORLDMAP_TAG);
    if (node)
    {
        return static_cast<WorldMap*>(node);
    }
    
    return NULL;
    
}

void MainUI::initStageUI()
{
    if (getChildByTag(STAGE_UI_TAG))
    {
        // exists
        return;
    }
    
    removeAllChildrenWithCleanup(true);
    
    addChild(StageUI::create(), 0, STAGE_UI_TAG);
}

void MainUI::showWorldMap()
{
    
    removeAllChildrenWithCleanup(true);
    
    addChild(WorldMap::create(), 0, WORLDMAP_TAG);
}

