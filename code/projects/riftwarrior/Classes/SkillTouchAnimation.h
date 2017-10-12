//
//  SkillTouchAnimation.h
//  ike-tdgame
//
//  Created by Like Zhang on 1/19/13.
//
//

#ifndef __ike_tdgame__SkillTouchAnimation__
#define __ike_tdgame__SkillTouchAnimation__

#include <iostream>
#include "cocos2d.h"
#include <vector>
using namespace cocos2d;
using namespace std;

class PerformEffectAnimation;

class SkillTouchAnimation : public CCNode
{
public:
	SkillTouchAnimation() : skillStartTime(0.0f), skillElapsedTime(0.0f) {};
    virtual bool init();
    virtual void update(float dt);
    virtual void draw();

    void touchBegan(const CCPoint& touchPos);
    void touchMoved(const CCPoint& touchPos);
    void touchEnded(const CCPoint& touchPos);

    void UpdateMoltenBallBasedOnAccelerometer();
    
    CREATE_FUNC(SkillTouchAnimation);
    
    inline PerformEffectAnimation* getSkillAnimation()
    {
        return m_pPerformingSkillAnimation;
    }
    
    inline void setAnimationOffset(float x, float y)
    {
        m_Offset.x = x;
        m_Offset.y = y;
    }
    
    void createMoltenBall();
    
    CCPoint skillStartPosition;
    CCPoint skillCurrentPosition;

    float skillStartTime;
    float skillElapsedTime;
    
private:
    
    vector<CCPoint> getTilePosFromLine(const CCPoint& startPos, const CCPoint& endPos);

    void drawLineCrossTiles();
    void drawTimerBar();
    void drawSphere();
    
    void removeAnimation();
    
    PerformEffectAnimation* m_pPerformingSkillAnimation;
    
    CCPoint m_Offset;  // for skill 3 (accelerometer based)
    CCTexture2D* mTexture;
    float mSpeed;
	float mYaw;
	float mPitch;
	float mRoll;

};

#endif /* defined(__ike_tdgame__SkillTouchAnimation__) */
