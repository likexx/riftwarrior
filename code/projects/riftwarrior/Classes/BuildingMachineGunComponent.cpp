//
//  BuildingMachineGunComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 4/27/13.
//
//

#include "BuildingMachineGunComponent.h"
#include "Building.h"
#include "SimpleAudioEngine.h"

BuildingMachineGunComponent::~BuildingMachineGunComponent()
{
    
}

CCPoint BuildingMachineGunComponent::getFirePosition()
{
    float xOffsets[] = {260, 231, 137, 40, 136};
    float yOffsets[] = {109,  51,  22, 161,192};
    
    return calculateFirePosition(xOffsets, yOffsets);
}


void BuildingMachineGunComponent::playAttackAnimation()
{
    Building* building = static_cast<Building*>(this->getParent());
    CCSprite* pRotatingPart = building->getRotatingPart();
    
    if (m_Quadrant == 4 ||
        m_Quadrant == 5 ||
        m_Quadrant == 8)
    {
        pRotatingPart->setFlipX(true);
    }
    else
    {
        pRotatingPart->setFlipX(false);
    }
    
    
    CCFiniteTimeAction* actions = CCSequence::create(m_AnimationActions[m_Quadrant],
//                                                     CCCallFunc::create(this, callfunc_selector(BuildingAttackComponent::startAttack)),
                                                     CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::resetAnimation)),
                                                     NULL);

    pRotatingPart->runAction(actions);
    
    startAttack();
//    pRotatingPart->runAction(CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::startAttack)));
    
    
    char buffer[128] = {0};
    sprintf(buffer, "sound/building_%d_fire.mp3", building->getId());
    // FIXME
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(buffer);
    
    //    pRotatingPart->runAction(m_AnimationActions[m_Quadrant]);
    //    pRotatingPart->scheduleUpdate();
}


void BuildingMachineGunComponent::startAttack()
{
    removeTarget();
    
    float centerDegree = 45 * m_Quadrant;
    
//    Building* building = static_cast<Building*>(this->getParent());
//    CCPoint bulletPos = getFirePosition();
/*
    float minDegree = centerDegree - 45;
    float maxDegree = centerDegree + 45;
    float step = 15;
*/
    float interval = m_pSetting->attackAnimationTime/7;
    
    m_CurrentDegree = centerDegree - 45;
    m_FireCount = 0;
    m_waitingForAttack = true;
    
    runAction(CCSequence::create(CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::fireNext)),
                                 CCDelayTime::create(interval),
                                 CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::fireNext)),
                                 CCDelayTime::create(interval),
                                 CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::fireNext)),
                                 CCDelayTime::create(interval),
                                 CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::fireNext)),
                                 CCDelayTime::create(interval),
                                 CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::fireNext)),
                                 CCDelayTime::create(interval),
                                 CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::fireNext)),
                                 CCDelayTime::create(interval),
                                 CCCallFunc::create(this, callfunc_selector(BuildingMachineGunComponent::fireNext)),
                                 NULL));

}

void BuildingMachineGunComponent::fireNext()
{
    CCSprite* bullet = createBullet(m_CurrentDegree);
    
    bullet->setVisible(false);
    
    m_CurrentDegree+=15;
    m_FireCount++;
    
    if (m_FireCount == 6)
    {
        m_waitingForAttack = false;
    }
    
}
