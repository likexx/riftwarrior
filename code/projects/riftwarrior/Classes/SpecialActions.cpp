//
//  SpecialActions.cpp
//  tdgame
//
//  Created by Like Zhang on 2/27/13.
//
//

#include "SpecialActions.h"
#include "GameScene.h"
#include "GameStage.h"
#include "MapHelper.h"

// MoveGameMapTo

CCMoveToWithBoundary* CCMoveToWithBoundary::create(float duration, const CCPoint& position, const CCRect& boundary)
{
    CCMoveToWithBoundary *pMoveTo = new CCMoveToWithBoundary();
    pMoveTo->initWithDuration(duration, position);
    pMoveTo->autorelease();
    pMoveTo->m_Boundary = boundary;
    
    return pMoveTo;
}
void CCMoveToWithBoundary::update(float time)
{
    if (m_pTarget)
    {
        m_pTarget->setPosition(ccp(m_startPosition.x + m_positionDelta.x * time,
                                   m_startPosition.y + m_positionDelta.y * time));

        alignBoundary();

    }
}

bool CCMoveToWithBoundary::isOutOfBoundary()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bool reachingBoundary = false;
    
    for (float y = 0; y < winSize.height; y+=winSize.height-1)
    {
        for(float x = 0; x < winSize.width; x+=winSize.width-1)
        {
            CCPoint testPos = m_pTarget->convertToNodeSpace(ccp(x,y));

            if (!m_Boundary.containsPoint(testPos))
            {
                reachingBoundary = true;
                break;
            }
            
        }
    }
    
    return reachingBoundary;
}
void CCMoveToWithBoundary::alignBoundary()
{
    GameScene::getInstance()->sharedGameStage->alignMapBoundary();
    
    return;


}



//
// KKActionThrowBy
//
KKActionThrowBy* KKActionThrowBy::actionWithDuration(float duration, const CCPoint& position, float height)
{
    return KKActionThrowBy::create(duration, position, height);
}

KKActionThrowBy* KKActionThrowBy::create(float duration, const CCPoint& position, float height)
{
    KKActionThrowBy *pJumpBy = new KKActionThrowBy();
    pJumpBy->initWithDuration(duration, position, height);
    pJumpBy->autorelease();
    
    return pJumpBy;
}

bool KKActionThrowBy::initWithDuration(float duration, const CCPoint& position, float height)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_delta = position;
        m_height = height;
        
        m_TotalDuration = duration;
        m_TargetPosition = position;

        return true;
    }
    
    return false;
}

CCObject* KKActionThrowBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    KKActionThrowBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (KKActionThrowBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new KKActionThrowBy();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_delta, m_height);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void KKActionThrowBy::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
    
    m_TimeElapsed = 0;
    m_IsDropping = false;
    
    float dx = m_startPosition.x + (m_TargetPosition.x - m_startPosition.x)/2;
    float dy = m_startPosition.y + (m_TargetPosition.y - m_startPosition.y)/2;
    
    m_MiddlePosition = ccp(dx, dy);
    
    m_MiddlePosition.y += MapHelper::MAP_TILE_LENGTH * 3;

}

void KKActionThrowBy::update(float time)
{
    // parabolic jump (since v0.8.2)
    if (m_pTarget)
    {
        float frac = fmodf(time , 1.0f);
        float y = m_height * 4 * frac * (1 - frac);
        y += m_delta.y * time;
        
        float x = m_delta.x * time;
        
        CCPoint prevPos = m_pTarget->getPosition();


        
        m_pTarget->setPosition(ccp(m_startPosition.x + x, m_startPosition.y + y));
        
        m_TimeElapsed+= time;
        
        CCPoint target = m_TimeElapsed < m_TotalDuration/3 ? m_MiddlePosition : m_TargetPosition;
        
        CCPoint normalized = ccpNormalize(ccp(target.x - m_pTarget->getPosition().x, target.y - m_pTarget->getPosition().y));
        float rotateDegree = CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x));
        m_pTarget->setRotation( rotateDegree + 180);


    }
}

CCActionInterval* KKActionThrowBy::reverse(void)
{
    return KKActionThrowBy::create(m_fDuration, ccp(-m_delta.x, -m_delta.y),
                            m_height);
}

//
// KKActionThrowTo
//
KKActionThrowTo* KKActionThrowTo::actionWithDuration(float duration, const CCPoint& position, float height)
{
    return KKActionThrowTo::create(duration, position, height);
}

KKActionThrowTo* KKActionThrowTo::create(float duration, const CCPoint& position, float height)
{
    KKActionThrowTo *pJumpTo = new KKActionThrowTo();
    pJumpTo->initWithDuration(duration, position, height);
    pJumpTo->autorelease();
    
    return pJumpTo;
}

CCObject* KKActionThrowTo::copyWithZone(CCZone* pZone)
{
    CCZone* pNewZone = nullptr;
    KKActionThrowTo* pCopy = nullptr;
    if (pZone 
		&& pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (KKActionThrowTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new KKActionThrowTo();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    // Weird code. Not sure what it was designed and coded for.	- wingc
	// KKActionThrowTo::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_delta, m_height);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void KKActionThrowTo::startWithTarget(CCNode *pTarget)
{
    KKActionThrowBy::startWithTarget(pTarget);
    m_delta = ccp(m_delta.x - m_startPosition.x, m_delta.y - m_startPosition.y);
}

