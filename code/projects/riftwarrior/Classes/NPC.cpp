//
//  NPC.cpp
//  tdgame
//
//  Created by Like Zhang on 4/24/13.
//
//

#include "NPC.h"
#include "GameScene.h"
#include "MapHelper.h"

NPC::~NPC()
{
    if (m_pAnimation)
    {
        m_pAnimation->clear();
        m_pAnimation->removeFromParentAndCleanup(true);
    }
}

NPC::NPC(int id)
{
    m_pAnimation = NULL;
    m_NpcId = id;
    m_Active = false;
}

int NPC::getId()
{
    return m_NpcId;
}

void NPC::showOnMap(const cocos2d::CCPoint &tilePosition)
{
    if (m_pAnimation)
    {
        m_pAnimation->clear();
        m_pAnimation->removeFromParentAndCleanup(true);
    }
    
    m_pAnimation = NpcAnimation::create(m_NpcId);
    m_pAnimation->setScale(0.6f);
    
    CCPoint realTilePos = ccp(tilePosition.x + 0.5, tilePosition.y + 0.5);
    StageMap* pGroundMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint mapGroundPos = MapHelper::tilePosToGroundMapPos(realTilePos);
    m_CurrentMapPosition = MapHelper::tilePosToMapPos(realTilePos.x, realTilePos.y, pGroundMap);
    m_CurrentTilePosition = realTilePos;
    m_GroundMapPosition = MapHelper::tilePosToGroundMapPos(realTilePos);
    
    m_pAnimation->setPosition(mapGroundPos);
    this->addChild(m_pAnimation);
    m_pAnimation->scheduleUpdate();

    
    m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_STAND_DOWN);

    m_Active = true;
    
    this->scheduleUpdate();
}


NPC* NPC::create(int id)
{
    NPC* npc = new NPC(id);
    
    if (npc && npc->init(id))
    {
        npc->autorelease();
        return npc;
    }
    
    CC_SAFE_DELETE(npc);
    
    return NULL;
    
}

bool NPC::init(int id)
{
    m_pSetting = GameData::getNpcSetting(id);
    m_MaxHp = m_pSetting->maxHp;
    m_Hp = m_pSetting->maxHp;
    
    return true;
}


CCPoint NPC::getShootPosition()
{
    return m_GroundMapPosition;
}

CCPoint NPC::getAttackablePosition()
{
    return m_GroundMapPosition;
}

void NPC::reduceHp(float damage, bool ignoreDefense)
{
    if (isDead())
    {
        return;
    }
    
    m_Hp-=damage;
    if (m_Hp < 0)
    {
        m_Hp = 0;
    }
}

void NPC::actWhenAttacked(AttackableObject* pTarget)
{
    
}


bool NPC::isDead()
{
    return m_Hp <= 0;
}

void NPC::remove()
{
    
}

bool NPC::isActive()
{
    return m_Active;
}

const CCPoint& NPC::getAttackableMapPosition()
{
    return m_CurrentMapPosition;
}

void NPC::destroy()
{
    if (!isActive())
    {
        return;
    }
    
    if (m_pAnimation)
    {
        m_pAnimation->clear();
        m_pAnimation->removeFromParentAndCleanup(true);
        m_pAnimation = NULL;
    }
    
    m_Active = false;
}

bool NPC::ableToAttack()
{
    return false;
}

void NPC::finishAttack()
{
    
}

float NPC::getDamage()
{
    return 0;
}

float NPC::getAttackRange()
{
    return 0;
}

float NPC::getAttackRate()
{
    return 0;
}

float NPC::getBaseSpeed()
{
    return 0;
}

CCNode* NPC::getAnimationPart()
{
    return m_pAnimation;
}

void NPC::setSpeed(float speed)
{
}
