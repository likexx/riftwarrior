//
//  SpecialActions.h
//  tdgame
//
//  Created by Like Zhang on 2/27/13.
//
//

#ifndef __tdgame__SpecialActions__
#define __tdgame__SpecialActions__

#include <iostream>
#include <Cocos2d.h>

using namespace cocos2d;

class CCMoveToWithBoundary : public CCMoveTo
{
public:
    virtual void update(float time);
    
    /** creates the action */
    static CCMoveToWithBoundary* create(float duration, const CCPoint& position, const CCRect& boundary);
    
protected:
    bool isOutOfBoundary();
    void alignBoundary();
    
    CCRect m_Boundary;
};


class KKActionThrowBy : public CCActionInterval
{
public:
    /** initializes the action */
    bool initWithDuration(float duration, const CCPoint& position, float height);
    
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual CCActionInterval* reverse(void);
    
public:
    /** creates the action
     @deprecated: This interface will be deprecated sooner or later.
     */
    CC_DEPRECATED_ATTRIBUTE static KKActionThrowBy* actionWithDuration(float duration, const CCPoint& position, float height);
    /** creates the action */
    static KKActionThrowBy* create(float duration, const CCPoint& position, float height);
protected:
    CCPoint            m_startPosition;
    CCPoint            m_TargetPosition;
    CCPoint            m_MiddlePosition;
    CCPoint            m_delta;
    float            m_TotalDuration;
    float            m_TimeElapsed;
    float            m_height;
    
    bool             m_IsDropping;
};

/** @brief Moves a CCNode object to a parabolic position simulating a jump movement by modifying it's position attribute.
 */
class KKActionThrowTo : public KKActionThrowBy
{
public:
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCObject* copyWithZone(CCZone* pZone);
    
public:
    /** creates the action
     @deprecated: This interface will be deprecated sooner or later.
     */
    CC_DEPRECATED_ATTRIBUTE static KKActionThrowTo* actionWithDuration(float duration, const CCPoint& position, float height);
    /** creates the action */
    static KKActionThrowTo* create(float duration, const CCPoint& position, float height);
};


#endif /* defined(__tdgame__SpecialActions__) */
