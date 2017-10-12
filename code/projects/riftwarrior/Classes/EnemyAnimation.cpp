//
//  EnemyAnimation.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/8/13.
//
//

#include "EnemyAnimation.h"
#include "Function.h"
#include "GameData.h"
#include "EnemySetting.h"
#include "Enemy.h"

#define WALK_ANIMATION 1
#define ATTACK_ANIMATION 2
#define DEAD_ANIMATION 3

EnemyAnimation* EnemyAnimation::create(int enemyId)
{
    return create(enemyId, 0);
}

EnemyAnimation* EnemyAnimation::create(int enemyId, int parentId)
{
    EnemyAnimation* pAnimation = new EnemyAnimation();
    if (pAnimation && pAnimation->init(enemyId, parentId))
    {
        pAnimation->autorelease();
        return pAnimation;
    }
    
    CC_SAFE_DELETE(pAnimation);
    
    return NULL;
}


bool EnemyAnimation::init(int enemyId, int parentId)
{
    m_pSetting = GameData::getEnemySetting(enemyId);
    m_AttackAnimationInterval = 0;
    
    int animId = parentId ? parentId : enemyId;
    
    initAnimation(animId, 1, WALK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_MOVING_UP);
    initAnimation(animId, 2, WALK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_MOVING_DOWN);
    initAnimation(animId, 3, WALK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_MOVING_LEFT);
    initAnimation(animId, 4, WALK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_MOVING_RIGHT);

    initAnimation(animId, 1, DEAD_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_DEAD_UP);
    initAnimation(animId, 2, DEAD_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_DEAD_DOWN);
    initAnimation(animId, 3, DEAD_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_DEAD_LEFT);
    initAnimation(animId, 4, DEAD_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_DEAD_RIGHT);

    
    initAnimation(animId, 1, ATTACK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_ATTACK_RIGHT);
    initAnimation(animId, 2, ATTACK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_ATTACK_TOPRIGHT);
    initAnimation(animId, 3, ATTACK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_ATTACK_TOP);
    initAnimation(animId, 4, ATTACK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_ATTACK_TOPLEFT);
    initAnimation(animId, 5, ATTACK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_ATTACK_LEFT);
    initAnimation(animId, 6, ATTACK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_ATTACK_LEFTBOTTOM);
    initAnimation(animId, 7, ATTACK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_ATTACK_BOTTOM);
    initAnimation(animId, 8, ATTACK_ANIMATION, ccp(m_pSetting->anchorX, m_pSetting->anchorY), ENUM_NPC_ANIMATION_ATTACK_RIGHTBOTTOM);

    m_pBossBackground = NULL;
    m_pShadow = NULL;
    
    m_pShadow = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("npc_shadow.png"));
    m_pShadow->setAnchorPoint(ccp(m_pSetting->anchorX, m_pSetting->anchorY + 0.3));
    addChild(m_pShadow);
    

    return true;
}

void EnemyAnimation::initAnimation(int id, int direction, int animationType, CCPoint anchorPoint, ENUM_NPC_ANIMATION enumAnimValue)
{

    bool flipX = true;

    const char* animStr = NULL;
    
    if (animationType == WALK_ANIMATION)
    {
        animStr = "walk";
        switch (direction)
        {
            case 3:
                direction = 1;
                break;
            case 4:
                direction = 2;
                break;
            default:
                flipX = false;
                break;
        }
    }
    else if (animationType == ATTACK_ANIMATION)
    {
        animStr = "attack";
        switch (direction)
        {
            case 4:
                direction = 2;
                break;
            case 5:
                direction = 1;
                break;
            case 8:
                direction = 6;
                break;
            default:
                flipX = false;
                break;
        }
    }
    else
    {
        animStr = "dead";
        switch (direction)
        {
            case 3:
                direction = 1;
                break;
            case 4:
                direction = 2;
                break;
            default:
                flipX = false;
                break;
        }
    }
    
    char name[128]={0};
    sprintf(name, "enemy_%d_%s_%d_1.png", id, animStr, direction);

    CCArray* frames = CCArray::createWithCapacity(2);
    CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
    if (!frame1)
    {
        return;
    }
    
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName(name);
    pSprite->setAnchorPoint(anchorPoint);
    pSprite->retain();
    pSprite->setFlipX(flipX);
    
    if (id != m_pSetting->id)
    {
        // the animation id is different than enemy id. it should be a child from something
        pSprite->setColor(ccc3(128,128,128));
    }

    for (int i=1;i<=8;++i)
    {
        sprintf(name, "enemy_%d_%s_%d_%d.png", id, animStr, direction, i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        frames->addObject(frame);
    }
    
    if (animationType == ATTACK_ANIMATION)
    {
        m_AttackAnimationInterval = 0.125f * frames->count();
    }

    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.125f);
    CCAnimate* animate = CCAnimate::create(animation);
    
    m_Sprites[enumAnimValue] = pSprite;

    if (animationType == WALK_ANIMATION)
    {
        CCActionInterval* pAnimation = CCRepeatForever::create(animate);
        pAnimation->retain();
        //        CCRepeatForever* repeat = CCRepeatForever::create(animate);
    
        m_AnimationActions[enumAnimValue] = pAnimation;
    }
    else
    {
        animate->retain();
        m_AnimationActions[enumAnimValue] = animate;
    }
    
    
    
}

void EnemyAnimation::removeEffect()
{
    if (m_pShadow)
    {
        m_pShadow->removeFromParentAndCleanup(true);
        m_pShadow = NULL;
    }
    
    if (m_pBossBackground)
    {
        m_pBossBackground->removeFromParentAndCleanup(true);
        m_pBossBackground = NULL;
    }
    
    if (m_pBossStar)
    {
        m_pBossStar->removeFromParentAndCleanup(true);
        m_pBossStar = NULL;
    }
}

void EnemyAnimation::setupBossAnimation()
{
    if (m_pBossBackground)
    {
        m_pBossBackground->removeFromParentAndCleanup(true);
        m_pBossBackground = NULL;
    }
    
    if (m_pBossStar)
    {
        m_pBossStar->removeFromParentAndCleanup(true);
        m_pBossStar = NULL;
    }
    
    char buffer[128] = {0};
    sprintf(buffer, "boss_background_1.png");
    CCSprite* background = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer));
    
    CCArray* frames = CCArray::create();
    for(int i=0;i<8;++i)
    {
        sprintf(buffer, "boss_background_%d.png", i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.1f);
    CCAnimate* animate = CCAnimate::create(animation);
    CCActionInterval* action = CCRepeatForever::create(animate);
    
    background->runAction(action);
    
    addChild(background);
    background->setAnchorPoint(ccp(m_pSetting->anchorX, m_pSetting->anchorY));
    
    m_pBossBackground=background;
    m_pBossBackground->setVisible(false);

    m_pBossStar = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/lv.png"));
    m_pBossStar->setAnchorPoint(ccp(0.5, 0));
    m_pBossStar->setPosition(ccp(background->getContentSize().width/2, background->getTextureRect().size.height));
    addChild(m_pBossStar);

    
}

void EnemyAnimation::reset()
{
    //    animationType = ENUM_NPC_ANIMATION_STAND;
    if (m_pCurrentSprite)
    {
        m_pCurrentSprite->stopAllActions();
        this->removeChild(m_pCurrentSprite, true);
        m_pCurrentSprite = NULL;
    }
}


void EnemyAnimation::updateSprite()
{
    if (!m_pCurrentSprite)
    {
        return;
    }
    
    m_pCurrentSprite->setVisible(true);
    m_pCurrentSprite->setColor(ccc3(255, 255,255));
    
    float expectedScale =  m_pSetting->scale;
    if (m_pBossBackground)
    {
        // is boss
        expectedScale *= 2;
    }
    
    float scale = m_pCurrentSprite->getScale();
    
    if (scale != expectedScale)
    {
        m_pCurrentSprite->setScale(expectedScale);
        
        if (m_pBossBackground)
        {
            m_pBossBackground->setScale(expectedScale * (m_pCurrentSprite->getTextureRect().size.width / (m_pBossBackground->getTextureRect().size.width)));
            
            float scale = m_pBossBackground->getScale();
            m_pBossBackground->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCScaleTo::create(0.4f, scale+0.05),
                                                                                                     CCScaleBy::create(0.4f, scale),
                                                                                                     NULL)));
            m_pBossBackground->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(0.15f, 0),
                                                                                                        CCFadeTo::create(0.15f, 255),
                                                                                                        CCDelayTime::create(0.1f),
                                                                                                        NULL)));
            m_pBossBackground->setVisible(true);
        }
        
        if (m_pShadow)
        {
            float scale = m_pSetting->scale * (m_pCurrentSprite->getTextureRect().size.width / (m_pShadow->getTextureRect().size.width));
            scale *=1.5;
//            scale *= 0.6;
            m_pShadow->setScale(scale);
        }
        
        if (m_pBossStar)
        {
            m_pBossStar->setPosition(ccp(m_pCurrentSprite->getPosition().x, m_pCurrentSprite->getPosition().y + m_pCurrentSprite->getTextureRect().size.height * (1-m_pSetting->anchorY) * m_pCurrentSprite->getScale()));
        }
    }
    
}


