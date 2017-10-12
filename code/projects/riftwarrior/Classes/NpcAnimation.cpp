//
//  NpcAnimation.cpp
//  tdgame
//
//  Created by Like Zhang on 4/24/13.
//
//

#include "NpcAnimation.h"
#include "Function.h"


NpcAnimation* NpcAnimation::create(int id)
{
    NpcAnimation* pAnimation = new NpcAnimation();
    if (pAnimation && pAnimation->init(id))
    {
        pAnimation->autorelease();
        return pAnimation;
    }
    CC_SAFE_DELETE(pAnimation);
    return NULL;
}


bool NpcAnimation::init(int id)
{
    m_CurrentAnimationType = ENUM_NPC_ANIMATION_NONE;
    m_pCurrentSprite = NULL;
    
    m_NpcName = CCString::createWithFormat("npc%d", id)->getCString();
    
    initAnimations();

    return true;
}

void NpcAnimation::initAnimations()
{
    ENUM_NPC_ANIMATION animTypes[] = {ENUM_NPC_ANIMATION_STAND_UP,
        ENUM_NPC_ANIMATION_STAND_DOWN,
        ENUM_NPC_ANIMATION_STAND_LEFT,
        ENUM_NPC_ANIMATION_STAND_RIGHT,
        ENUM_NPC_ANIMATION_ATTACK_RIGHT,
        ENUM_NPC_ANIMATION_ATTACK_TOPRIGHT,
        ENUM_NPC_ANIMATION_ATTACK_TOP,
        ENUM_NPC_ANIMATION_ATTACK_TOPLEFT,
        ENUM_NPC_ANIMATION_ATTACK_LEFT,
        ENUM_NPC_ANIMATION_ATTACK_LEFTBOTTOM,
        ENUM_NPC_ANIMATION_ATTACK_BOTTOM,
        ENUM_NPC_ANIMATION_ATTACK_RIGHTBOTTOM,
        ENUM_NPC_ANIMATION_DEAD_UP,
        ENUM_NPC_ANIMATION_DEAD_DOWN,
        ENUM_NPC_ANIMATION_DEAD_LEFT,
        ENUM_NPC_ANIMATION_DEAD_RIGHT,
        ENUM_NPC_ANIMATION_MOVING_UP,
        ENUM_NPC_ANIMATION_MOVING_DOWN,
        ENUM_NPC_ANIMATION_MOVING_LEFT,
        ENUM_NPC_ANIMATION_MOVING_RIGHT,
        
        ENUM_NPC_ANIMATION_USE_SKILL_1_UP,
        ENUM_NPC_ANIMATION_USE_SKILL_1_DOWN,
        ENUM_NPC_ANIMATION_USE_SKILL_1_LEFT,
        ENUM_NPC_ANIMATION_USE_SKILL_1_RIGHT,
        
        ENUM_NPC_ANIMATION_USE_SKILL_2_UP,
        ENUM_NPC_ANIMATION_USE_SKILL_2_DOWN,
        ENUM_NPC_ANIMATION_USE_SKILL_2_LEFT,
        ENUM_NPC_ANIMATION_USE_SKILL_2_RIGHT,


    };
    
    size_t count = sizeof(animTypes)/sizeof(ENUM_NPC_ANIMATION);
    
    for (const ENUM_NPC_ANIMATION& type : animTypes)
    {
        char buffer[128] = {0};
        bool flipX = false;
        const char* pAnimName = NULL;
        int sequence = 0;
        bool repeatForever = true;
        
        switch (type)
        {
            case ENUM_NPC_ANIMATION_STAND_UP:
                sequence = 1;
                pAnimName = "idle";
                break;
            case ENUM_NPC_ANIMATION_STAND_DOWN:
                sequence = 2;
                pAnimName = "idle";
                break;
            case ENUM_NPC_ANIMATION_STAND_LEFT:
                flipX = true;
                sequence = 1;
                pAnimName = "idle";
                break;
            case ENUM_NPC_ANIMATION_STAND_RIGHT:
                flipX = true;
                sequence = 2;
                pAnimName = "idle";
                break;
            case ENUM_NPC_ANIMATION_ATTACK_RIGHT:
                pAnimName = "attack";
                sequence = 1;
                break;
            case ENUM_NPC_ANIMATION_ATTACK_TOPRIGHT:
                pAnimName = "attack";
                sequence = 2;
                break;
            case ENUM_NPC_ANIMATION_ATTACK_TOP:
                pAnimName = "attack";
                sequence = 3;
                break;
            case ENUM_NPC_ANIMATION_ATTACK_TOPLEFT:
                flipX = true;
                sequence = 2;
                pAnimName = "attack";
                break;
            case ENUM_NPC_ANIMATION_ATTACK_LEFT:
                flipX = true;
                sequence = 1;
                pAnimName = "attack";
                break;
            case ENUM_NPC_ANIMATION_ATTACK_LEFTBOTTOM:
                pAnimName = "attack";
                sequence = 6;
                break;
            case ENUM_NPC_ANIMATION_ATTACK_BOTTOM:
                pAnimName = "attack";
                sequence = 7;
                break;
            case ENUM_NPC_ANIMATION_ATTACK_RIGHTBOTTOM:
                pAnimName = "attack";
                sequence = 6;
                flipX = true;
                break;
            case ENUM_NPC_ANIMATION_DEAD_UP:
                pAnimName = "dead";
                sequence = 1;
                break;
            case ENUM_NPC_ANIMATION_DEAD_DOWN:
                pAnimName = "dead";
                sequence = 2;
                break;
            case ENUM_NPC_ANIMATION_DEAD_LEFT:
                flipX = true;
                pAnimName = "dead";
                sequence = 1;
                break;
            case ENUM_NPC_ANIMATION_DEAD_RIGHT:
                flipX = true;
                pAnimName = "dead";
                sequence = 2;
                break;
            case ENUM_NPC_ANIMATION_MOVING_UP:
                pAnimName="walk";
                sequence = 1;
                break;
            case ENUM_NPC_ANIMATION_MOVING_DOWN:
                pAnimName="walk";
                sequence = 2;
                break;
            case ENUM_NPC_ANIMATION_MOVING_LEFT:
                pAnimName="walk";
                sequence = 1;
                flipX = true;
                break;
            case ENUM_NPC_ANIMATION_MOVING_RIGHT:
                pAnimName="walk";
                sequence = 2;
                flipX = true;
                break;
                
            case ENUM_NPC_ANIMATION_USE_SKILL_1_UP:
            case ENUM_NPC_ANIMATION_USE_SKILL_1_DOWN:
            case ENUM_NPC_ANIMATION_USE_SKILL_1_LEFT:
            case ENUM_NPC_ANIMATION_USE_SKILL_1_RIGHT:
                pAnimName="skill_1";
                sequence = 1;
                break;
                
            case ENUM_NPC_ANIMATION_USE_SKILL_2_UP:
                pAnimName="skill_2";
                sequence = 1;
                repeatForever = false;
                break;

            case ENUM_NPC_ANIMATION_USE_SKILL_2_DOWN:
                pAnimName="skill_2";
                sequence = 2;
                repeatForever = false;
                break;
            case ENUM_NPC_ANIMATION_USE_SKILL_2_LEFT:
                pAnimName="skill_2";
                sequence = 1;
                flipX = true;
                repeatForever = false;
                break;
            case ENUM_NPC_ANIMATION_USE_SKILL_2_RIGHT:
                pAnimName="skill_2";
                sequence = 2;
                flipX = true;
                repeatForever = false;
                break;
                
        }
        sprintf(buffer, "%s_%s_%d_1.png", m_NpcName.c_str() ,pAnimName, sequence);
        
        CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
        if (!spriteFrame)
        {
            continue;
        }
        
        CCSprite* pSprite = CCSprite::createWithSpriteFrame(spriteFrame);
        m_Sprites[type] = pSprite;
        m_Sprites[type]->retain();
        pSprite->setAnchorPoint(ccp(0.5,0.5));
        pSprite->setFlipX(flipX);
        
        CCArray* frames = CCArray::createWithCapacity(4);
        
        const int maxFrameNumber = 8;
        
        for(int j=0;j<maxFrameNumber;++j)
        {
            sprintf(buffer, "%s_%s_%d_%d.png", m_NpcName.c_str() ,pAnimName, sequence, j+1);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
            if (!frame)
            {
                break;
            }
            
            frames->addObject(frame);
        }
        
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.1f);
        
        if (type == ENUM_NPC_ANIMATION_ATTACK_BOTTOM ||
            type == ENUM_NPC_ANIMATION_ATTACK_LEFT ||
            type == ENUM_NPC_ANIMATION_ATTACK_TOP ||
            type == ENUM_NPC_ANIMATION_ATTACK_RIGHT)
        {
            m_AttackAnimationInterval = 0.1 * frames->count();
        }
        
        CCActionInterval* pAnimation;
        CCAnimate* animate = CCAnimate::create(animation);
        if (repeatForever)
        {
            pAnimation = CCRepeatForever::create(animate);
        }
        else
        {
            pAnimation = animate;
        }
        
        pAnimation->retain();
        
        m_AnimationActions[type] = pAnimation;
        
    }
    
    //add shadow
    addShadow();
    
}


void NpcAnimation::addShadow()
{
    CCSprite* shadow = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("npc_shadow.png"));
    shadow->setAnchorPoint(ccp(0.5, 0.8));
    addChild(shadow);

}

void NpcAnimation::setAnimation(ENUM_NPC_ANIMATION animType)
{
    map< ENUM_NPC_ANIMATION, CCSprite*>::iterator it = m_Sprites.find(animType);
    
    if ( it == m_Sprites.end())
    {
        return;
    }
    
    if (m_pCurrentSprite == it->second)
    {
        if (m_pCurrentSprite->numberOfRunningActions() == 0)
        {
            m_pCurrentSprite->runAction(m_AnimationActions[animType]);
        }
        
        return;
    }
    
    if (m_pCurrentSprite != NULL)
    {
        m_pCurrentSprite->stopAllActions();
        this->removeChild(m_pCurrentSprite, true);
    }
    
    m_pCurrentSprite = it->second;
    this->addChild(m_pCurrentSprite);
    
    updateSprite();
    
    CCFiniteTimeAction* action = m_AnimationActions[animType];
    m_pCurrentSprite->runAction(action);
    
    m_CurrentAnimationType = animType;
}

void NpcAnimation::setAnimationWithCallback(ENUM_NPC_ANIMATION animType, cocos2d::CCCallFunc *callback, float delay)
{
    setAnimation(animType);
    
    if (delay)
    {
        runAction(CCSequence::create(CCDelayTime::create(delay),
                                     CCDelayTime::create(m_AttackAnimationInterval- 0.1),
                                     callback, NULL));
    }
    else
    {
        runAction(CCSequence::create(CCDelayTime::create(m_AttackAnimationInterval- 0.1),
                                     callback, NULL));
    }
    
    return;
    
    /*
    
    map< ENUM_NPC_ANIMATION, CCSprite*>::iterator it = m_Sprites.find(animType);
    
    if ( it == m_Sprites.end())
    {
        return;
    }
    
    if (m_pCurrentSprite != NULL)
    {
        m_pCurrentSprite->stopAllActions();
        this->removeChild(m_pCurrentSprite, true);
    }
    
    m_pCurrentSprite = it->second;
    this->addChild(m_pCurrentSprite);
    
    updateSprite();
    
    CCFiniteTimeAction* action = m_AnimationActions[animType];
    
    if (delay == 0)
    {
        m_pCurrentSprite->runAction(CCSequence::create(action,
                                                       NULL));
        m_pCurrentSprite->runAction(CCSequence::create(CCDelayTime::create(m_AttackAnimationInterval- 0.1),
                                                       callback, NULL));
    }
    else
    {
        m_pCurrentSprite->runAction(CCSequence::create(CCDelayTime::create(delay),
                                                       action,
                                                       NULL));
        m_pCurrentSprite->runAction(CCSequence::create(CCDelayTime::create(delay),
                                                       CCDelayTime::create(m_AttackAnimationInterval- 0.1),
                                                       callback, NULL));

    }
    m_CurrentAnimationType = animType;
    */
}

void NpcAnimation::updateSprite()
{
    m_pCurrentSprite->setAnchorPoint(ccp(0.5, 0.3));
    m_pCurrentSprite->setScale(0.5);
    m_pCurrentSprite->setVisible(true);
    m_pCurrentSprite->setColor(ccc3(255, 255,255));
}

void NpcAnimation::clear()
{
    map< ENUM_NPC_ANIMATION, CCSprite*>::iterator itSprite = m_Sprites.begin();
    
    while (itSprite != m_Sprites.end())
    {
        CCSprite* sprite = itSprite->second;
        sprite->release();
        itSprite++;
    }
    
    map< ENUM_NPC_ANIMATION, CCFiniteTimeAction*>::iterator itAnimation = m_AnimationActions.begin();
    
    while (itAnimation != m_AnimationActions.end())
    {
        CCFiniteTimeAction* animation = itAnimation->second;
        animation->release();
        itAnimation++;
    }
}