//
//  BuildingHpBar.cpp
//  tdgame
//
//  Created by Like Zhang on 3/2/13.
//
//

#include "BuildingStatusBar.h"
#include "Building.h"
#include "GameScene.h"

#define LEVEL_STAR_TAG 1000

BuildingStatusBar* BuildingStatusBar::create(Building* pBuilding)
{
    BuildingStatusBar* pBar = new BuildingStatusBar();
    if (pBar && pBar->init(pBuilding))
    {
        pBar->autorelease();
        return pBar;
    }
    
    CC_SAFE_DELETE(pBar);
    return NULL;
}

bool BuildingStatusBar::init(Building* pBuilding)
{
    m_pBuilding = pBuilding;
    
//    m_StatusBox = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/status_label_box.png"));
    
    CCSize tileSize = GameScene::getInstance()->sharedGameStage->getGroundMap()->getTileSize();
    CCSize barSize = CCSizeMake(tileSize.width * 0.45, tileSize.height/2);
    setContentSize(barSize);

    BuildingHpBar* hpBar = BuildingHpBar::create(pBuilding);
    hpBar->setAnchorPoint(ccp(0,0.5));
    hpBar->setPosition(ccp(0, getContentSize().height/3));
    CCSize hpBarSize = CCSizeMake(barSize.width, barSize.height * 0.5);
    hpBar->setContentSize(hpBarSize);
    

    addChild(hpBar);

    m_CurrentLevel = 0;
    
//    m_StatusBox->setOpacity(0);
    
    scheduleUpdate();
    return true;
}

void BuildingStatusBar::update(float dt)
{
    
    if (m_CurrentLevel != m_pBuilding->getLevel())
    {
        m_CurrentLevel = m_pBuilding->getLevel();

        resetLevelStars();
        
    }
}


void BuildingStatusBar::resetLevelStars()
{
    while (getChildByTag(LEVEL_STAR_TAG))
    {
        removeChildByTag(LEVEL_STAR_TAG, true);
    }

    
    for (int i = 1; i <= (1 + (m_CurrentLevel-1)%3); ++i)
    {
        CCSprite* lvStar = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/lv.png"));
        lvStar->setAnchorPoint(ccp(0,0.3));
        
        // offset 1 star's position for each level
        lvStar->setPosition(ccp(lvStar->getContentSize().width * (i - 1),getContentSize().height/2));
        addChild(lvStar, 0, LEVEL_STAR_TAG);
        
        if (m_CurrentLevel > 3)
        {
            if (m_CurrentLevel < 7)
            {
                lvStar->setColor(ccc3(0,255,0));
            }
            else if (m_CurrentLevel<10)
            {
                lvStar->setColor(ccc3(255,0,0));
            }
            else if (m_CurrentLevel<13)
            {
                lvStar->setColor(ccc3(128,0,255));
            }
            
        }
    }
}



BuildingHpBar* BuildingHpBar::create(Building* pBuilding)
{
    BuildingHpBar* pBar = new BuildingHpBar();
    if (pBar && pBar->init(pBuilding))
    {
        pBar->autorelease();
        return pBar;
    }
    
    CC_SAFE_DELETE(pBar);
    return NULL;
}

bool BuildingHpBar::init(Building* pBuilding)
{
    m_pBuilding = pBuilding;
/*
    CCSprite* background = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/hp.png"));
    setContentSize(background->getContentSize());
    
    background->setAnchorPoint(ccp(0,0.5));
    background->setPosition(ccp(0,getContentSize().height/2));
    
    addChild(background);
*/    
    return true;
}

void BuildingHpBar::draw()
{
    if (m_pBuilding->isActive() && !m_pBuilding->isDead())
    {
        CCSize size = getContentSize();
        
        glLineWidth( size.height/2 );
        CCPoint startPos = CCPointMake(0, size.height/2);
        
        ccDrawColor4B(255,0,0,128);
        ccDrawLine( startPos, CCPointMake(startPos.x+size.width, startPos.y) );
        
        ccDrawColor4B(128,255,128,255);
        ccDrawLine( startPos, CCPointMake(startPos.x+size.width * m_pBuilding->getHp()/m_pBuilding->getMaxHp(), startPos.y) );
        
        // restore line width
        glLineWidth(1.0f);
    }
}