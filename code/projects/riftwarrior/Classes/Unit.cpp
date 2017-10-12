//
//  Unit.cpp
//  tdgame
//
//  Created by Like Zhang on 5/9/13.
//
//

#include "Unit.h"
#include "GameStage.h"
#include "GameScene.h"
#include "MapHelper.h"
#include "UnitAttackComponent.h"
#include "BuildingBase.h"

Unit* Unit::create(int id, CCPoint startTilePos)
{
    Unit* unit = new Unit(id);
    
    if (unit && unit->init())
    {
        unit->autorelease();
    }
    
    unit->pAnimation->reset();
    unit->currentHp = unit->maxHp;
    
    unit->setToTile(startTilePos.x, startTilePos.y);
    
    return unit;
}

Unit::Unit(int id)
:Enemy(id)
{
}

bool Unit::init()
{
    m_Team = ENUM_TEAM_PLAYER;
    
    m_DestinationTile = GameScene::getInstance()->sharedGameStage->getGroundMap()->getTileBySequence(1);
    return true;
}

void Unit::remove()
{
    
    CCNode* parent = this->getParent();
    if (!parent)
    {
        return;
    }
    
    
    this->removeFromParentAndCleanup(true);
    
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    
    pStage->removeUnit(this);
    
    this->active = false;
    this->unscheduleUpdate();
    
    
}



void Unit::createAttackComponent()
{
    m_pAttackComponent = UnitAttackComponent::create();
}


void Unit::updateDestination()
{
    if (GameScene::getInstance()->sharedGameStage->getPlayerBase())
    {
        BuildingBase* base = static_cast<BuildingBase*>(GameScene::getInstance()->sharedGameStage->getPlayerBase()->getAttackComponent());
        
        m_DestinationTile = base->getDestinationTile();
        
    }
    
}


void Unit::update(float dt)
{

    Enemy::update(dt);
}