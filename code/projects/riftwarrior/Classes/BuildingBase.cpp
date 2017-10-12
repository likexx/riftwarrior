//
//  BuildingBase.cpp
//  tdgame
//
//  Created by Like Zhang on 5/8/13.
//
//

#include "BuildingBase.h"
#include "Enemy.h"
#include "Building.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Explosion.h"
#include "Player.h"
#include "ComputerPlayer.h"

BuildingBase::~BuildingBase()
{
    
}

bool BuildingBase::init()
{
    
    bool result = BuildingAttackComponent::init();
    
    m_Initialized = false;
    
    m_WaitingForComputerRevival = false;
    m_WaitingForPlayerRevival = false;
    
    m_DestinationTile = GameScene::getInstance()->sharedGameStage->getGroundMap()->getTileBySequence(1);
    
    return result;
}

CCActionInterval* BuildingBase::createBulletAction(cocos2d::CCPoint currentMapPos, cocos2d::CCPoint targetMapPos, cocos2d::CCPoint targetPos, float mapSpeed)
{
    return NULL;
}


void BuildingBase::update(float dt)
{
    if (GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    Building* building = static_cast<Building*>(getParent());
    
    if (building->isPlayerBuilding())
    {
        if (Player::getInstance()->isDead() && !m_WaitingForPlayerRevival)
        {
            scheduleOnce(schedule_selector(BuildingBase::revivePlayer), 10);
            m_WaitingForPlayerRevival = true;
        }
    }
    else
    {
        if (GameScene::getInstance()->sharedGameStage->getComputerPlayer()->isDead() && !m_WaitingForComputerRevival)
        {
            scheduleOnce(schedule_selector(BuildingBase::reviveComputerPlayer), 10);
            m_WaitingForComputerRevival = true;
        }
    }
    
    
}


void BuildingBase::reviveComputerPlayer()
{
    unschedule(schedule_selector(BuildingBase::reviveComputerPlayer));
    
    ComputerPlayer* pComputer = (ComputerPlayer*)GameScene::getInstance()->sharedGameStage->getComputerPlayer();
    
    if (!pComputer->isDead())
    {
        return;
    }
    
    pComputer->Enemy::init(pComputer->getLevel());

    pComputer->revive();
    
    m_WaitingForComputerRevival = false;
}


void BuildingBase::revivePlayer()
{
    unschedule(schedule_selector(BuildingBase::revivePlayer));
    
    if (!Player::getInstance()->isDead())
    {
        return;
    }
    
    Player::getInstance()->showOnMap();
    
    m_WaitingForPlayerRevival = true;
}