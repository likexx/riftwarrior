//
//  AnimationManager.cpp
//  tdgame
//
//  Created by Like Zhang on 3/4/13.
//
//

#include "AnimationManager.h"

AnimationManager* AnimationManager::getInstance()
{
    static AnimationManager* pInstance = nullptr;

	if (!pInstance)
    {
        pInstance = new AnimationManager();
        pInstance->init();
    }
    
    return pInstance;
}

void AnimationManager::init()
{
    initBulletAnimationFrames();
    initStatusAnimationFrames();
    initMagicAnimationFrames();
    initSkillAnimationFrames();
    
    initEnemySpecialAnimations();
    
    initEnvironmentAnimationFrames();
    
    CCArray* frames = CCArray::createWithCapacity(2);
    char name[128] = {0};
    
    for(int i=0;i<24;++i)
    {
        sprintf(name, "action_place_building_%d.png", i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    sharedBuildingConstructionFrames = frames;
    sharedBuildingConstructionFrames->retain();
    
    frames = CCArray::createWithCapacity(2);
    for(int i=0;i<24;++i)
    {
        sprintf(name, "action_upgrade_building_%d.png", i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    sharedBuildingUpgradeFrames = frames;
    sharedBuildingUpgradeFrames->retain();
    
    frames = CCArray::createWithCapacity(8);
    for(int i=0;i<24;++i)
    {
        sprintf(name, "action_set_player_destination_%d.png", i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    m_ActionSetPlayerDestination = CCAnimation::createWithSpriteFrames(frames, 0.05f);
    m_ActionSetPlayerDestination->retain();

    frames = CCArray::createWithCapacity(8);
    for(int i=0;i<24;++i)
    {
        sprintf(name, "destination_goal_%d.png", i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    m_ActionSetUnitDestination = CCAnimation::createWithSpriteFrames(frames, 0.05f);
    m_ActionSetUnitDestination->retain();
    
    
    frames = CCArray::createWithCapacity(8);
    for(int i=0;i<24;++i)
    {
        sprintf(name, "action_recover_hp_%d.png", i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    m_ActionRecoverHp = CCAnimation::createWithSpriteFrames(frames, 0.1f);
    m_ActionRecoverHp->retain();
    
}

void AnimationManager::initBulletAnimationFrames()
{
	static struct
	{
		ENUM_BULLET_ANIMATION key;
		const char * name;
	}
	keyNamePairs[] =
	{
		{ ENUM_BULLET_ANIMATION_STONE, "bullet_stone" },
		{ ENUM_BULLET_ANIMATION_FIRE_STONE, "bullet_fire_stone" },
		{ ENUM_BULLET_ANIMATION_FIRE_ARROW, "bullet_fire_arrow" },
		{ ENUM_BULLET_ANIMATION_ARROW, "bullet_arrow" },
		{ ENUM_BULLET_ANIMATION_ROCK, "bullet_rock" },
		{ ENUM_BULLET_ANIMATION_SPEAR, "bullet_spear" },
		{ ENUM_BULLET_ANIMATION_FIREGUN_BULLET, "bullet_firegun" },
		{ ENUM_BULLET_ANIMATION_ROCKET, "bullet_rocket" },
		{ ENUM_BULLET_ANIMATION_SNAKE_VENOM, "bullet_snake_venom" },
		{ ENUM_BULLET_ANIMATION_SPIDER_WEB, "bullet_spider_web" },
		{ ENUM_BULLET_ANIMATION_SNIPER_FIRE, "bullet_sniper_fire" }
    };

	for (const auto & keyNamePair : keyNamePairs)
    {
        CCAnimation* pAnimation = createAnimation(keyNamePair.name);
		m_BulletAnimationFrames[keyNamePair.key] = pAnimation;
		m_BulletAnimationNames[keyNamePair.key] = keyNamePair.name;

		pAnimation->retain();
    }
    
}

void AnimationManager::removeEnvironmentAnimationFrames()
{
    map<ENUM_ENVIRONMENT_ANIMATION_TYPE, CCAnimation*>::iterator it = m_EnvironmentAnimationFrames.begin();
    
    vector<ENUM_ENVIRONMENT_ANIMATION_TYPE> types;
    
    while (it != m_EnvironmentAnimationFrames.end())
    {
        types.push_back(it->first);
        ++it;
    }

	for (const ENUM_ENVIRONMENT_ANIMATION_TYPE& type : types)
    {
        it = m_EnvironmentAnimationFrames.find(type);
        if (it != m_EnvironmentAnimationFrames.end())
        {
            if (it->second)
            {
                it->second->release();
            }
            
            m_EnvironmentAnimationFrames.erase(it);
        }
    }

    m_EnvironmentAnimationNames.clear();
}


void AnimationManager::initEnvironmentAnimationFrames()
{
	static struct
	{
		ENUM_ENVIRONMENT_ANIMATION_TYPE key;
		const char * name;
	}
	keyNamePairs[] =
	{
		{ ENUM_ENVIRONMENT_ANIMATION_MAP101_SEAWAVE1,	"environment_map101_seawave1" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP101_SEAWAVE2,	"environment_map101_seawave2" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP101_SEAWAVE3,	"environment_map101_seawave3" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP101_SEAWAVE4,	"environment_map101_seawave4" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP101_SUNSHINE,	"environment_map101_sunshine" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP102_RABBIT,		"environment_map102_rabbit" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP102_FLAG,		"environment_map102_flag" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP105_WATERFALL1, "environment_map105_waterfall1" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP105_WATERFALL2, "environment_map105_waterfall3" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP105_WATERFALL3, "environment_map105_waterfall2" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP105_WAVE1,		"environment_map105_wave1" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP105_WAVE2,		"environment_map105_wave2" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP105_WAVE3,		"environment_map105_wave3" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP105_WAVE4,		"environment_map105_wave4" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP105_WAVE5,		"environment_map105_wave5" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP106_FOUNTAIN,	"environment_map106_fountain" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP106_WAVE1,		"environment_map106_wave1" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP106_WAVE2,		"environment_map106_wave2" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP108_WATERFALL1, "environment_map108_waterfall1" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP108_WATERFALL2, "environment_map108_waterfall2" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP108_WAVE1,		"environment_map108_wave1" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP108_WAVE2,		"environment_map108_wave2" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP108_WAVE3,		"environment_map108_wave3" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP109_SEAWEED1,	"environment_map109_seaweed1" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP109_SEAWEED2,	"environment_map109_seaweed2" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP109_SEAWEED3,	"environment_map109_seaweed3" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP109_SEAWEED4,	"environment_map109_seaweed4" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP109_SEAWEED5,	"environment_map109_seaweed5" },
		{ ENUM_ENVIRONMENT_ANIMATION_MAP110_LIGHTENING, "environment_map110_lightening" }
    };
    
	for (const auto & keyNamePair: keyNamePairs)
	{
        CCAnimation* pAnimation = createAnimation(keyNamePair.name, 0.3f);
		m_EnvironmentAnimationFrames[keyNamePair.key] = pAnimation;
		m_EnvironmentAnimationNames[keyNamePair.key] = keyNamePair.name;
        
        pAnimation->retain();
    }
    
}

void AnimationManager::initStatusAnimationFrames()
{
	static struct
	{
		ENUM_BUFF_STATUS key;
		const char * name;
	} 
	keyNamePairs[] =
	{
		{ ENUM_BUFF_STATUS_DIZZY, "hit_effect_dizzy" },
		{ ENUM_BUFF_STATUS_SPIDER_WEB, "hit_effect_spider_web" }
	};
    
	for (const auto& keyNamePair : keyNamePairs)
    {
        CCAnimation* pAnimation = createAnimation(keyNamePair.name);
        m_StatusAnimationFrames[keyNamePair.key] = pAnimation;
		m_StatusAnimationNames[keyNamePair.key] = keyNamePair.name;
        
        pAnimation->retain();
    }
}

void AnimationManager::initMagicAnimationFrames()
{
	static struct
	{
		ENUM_MAGIC_TYPE key;
		const char * name;
	}
	keyNamePairs[] = 
	{
		{ ENUM_MAGIC_TYPE_1, "magic_effect_1" },
		{ ENUM_MAGIC_TYPE_2, "magic_effect_2" }
	};
	
	for (const auto& keyNamePair : keyNamePairs)
	{
		CCAnimation* pAnimation = createAnimation(keyNamePair.name);
		m_MagicAnimationFrames[keyNamePair.key] = pAnimation;
		m_MagicAnimationNames[keyNamePair.key] = keyNamePair.name;
        
        pAnimation->retain();
    }
    
}

void AnimationManager::initSkillAnimationFrames()
{
	static struct
	{
		ENUM_PLAYER_PERFORM_SKILL_TYPE key;
		const char * name;
	}
	keyNamePairs[] =
	{
		{ ENUM_PLAYER_PERFORM_SKILL_EARTH_SLASH, "skill_effect_earth_slash" },
		{ ENUM_PLAYER_PERFORM_SKILL_RECOVER, "action_recover_hp" }
    };

	for (const auto& keyNamePair : keyNamePairs)
    {
        CCAnimation* pAnimation = createAnimation(keyNamePair.name);
		m_SkillAnimationFrames[keyNamePair.key] = pAnimation;
		m_SkillAnimationNames[keyNamePair.key] = keyNamePair.name;
        
        pAnimation->retain();
    }
}


void AnimationManager::initEnemySpecialAnimations()
{
	static struct
	{
		ENUM_ENEMY_ATTACK_TYPE key;
		const char * name;
	}
	keyNamePairs[] =
	{
		{ ENUM_ENEMY_ATTACK_SHOCK_WAVE, "skill_effect_shock_wave" }
    };
    
	for (const auto& keyNamePair : keyNamePairs)
	{
        CCAnimation* pAnimation = createAnimation(keyNamePair.name);
		m_EnemySpecialAttackAnimationFrames[keyNamePair.key] = pAnimation;
		m_EnemySpecialAttackAnimationNames[keyNamePair.key] = keyNamePair.name;
        
        pAnimation->retain();
    }
}


CCSmartSprite* AnimationManager::getEnemySpecialAttackAnimation(ENUM_ENEMY_ATTACK_TYPE type)
{
    if (m_EnemySpecialAttackAnimationFrames.find(type) == m_EnemySpecialAttackAnimationFrames.end())
    {
        return NULL;
    }
    
    CCAnimation* animation = m_EnemySpecialAttackAnimationFrames[type];

    CCArray* frames = animation->getFrames();
    
    CCAnimationFrame* pAnimFrame = static_cast<CCAnimationFrame*>(frames->objectAtIndex(0));
    
    CCSpriteFrame* firstFrame = pAnimFrame->getSpriteFrame();
    
    float totalTime = animation->getFrames()->count() * animation->getDelayPerUnit();
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    
    CCSmartSprite* pSprite = CCSmartSprite::createWithSpriteFrame(firstFrame);
    
    pSprite->runAction(animate);
    
    pSprite->runAction(CCSequence::create(CCDelayTime::create(totalTime),
                                          CCCallFunc::create(pSprite, callfunc_selector(CCSmartSprite::removeSelf)),
                                          NULL));
    return pSprite;
}


CCSprite* AnimationManager::createBulletAnimation(ENUM_BULLET_ANIMATION animationType)
{
    CCAnimation* animation = m_BulletAnimationFrames[animationType];
    
    CCAnimate* animate = CCAnimate::create(animation);

    
    CCActionInterval* pAnimation = CCRepeatForever::create(animate);

    char name[128]={0};
    
    
    sprintf(name, "%s_1.png", m_BulletAnimationNames[animationType].c_str());
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName(name);

    if (animationType != ENUM_BULLET_ANIMATION_SNIPER_FIRE)
    {
        pSprite->runAction(pAnimation);
    }
    else
    {
        pSprite->runAction(animate);
    }
    
    return pSprite;
}


CCSprite* AnimationManager::createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_TYPE type)
{
    CCAnimation* animation = m_EnvironmentAnimationFrames[type];
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    
    CCActionInterval* pAnimation = CCRepeatForever::create(animate);
    
    char name[128]={0};
    
    
    sprintf(name, "%s_1.png", m_EnvironmentAnimationNames[type].c_str());
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName(name);
    
    pSprite->runAction(pAnimation);
    
    return pSprite;
}

CCSmartSprite* AnimationManager::createStatusAnimation(ENUM_BUFF_STATUS status, float duration)
{
    CCAnimation* animation = m_StatusAnimationFrames[status];
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    CCActionInterval* pAnimation = CCRepeatForever::create(animate);
    
    char name[128]={0};
    
    
    sprintf(name, "%s_1.png", m_StatusAnimationNames[status].c_str());
    CCSmartSprite* pSprite = CCSmartSprite::createWithSpriteFrameName(name);
    
    pSprite->runAction(pAnimation);
 
    pSprite->runAction(CCSequence::create(CCDelayTime::create(duration),
                                          CCCallFunc::create(pSprite, callfunc_selector(CCSmartSprite::removeSelf)),
                                          NULL));
    return pSprite;
    
}


CCSmartSprite* AnimationManager::createMagicAnimation(ENUM_MAGIC_TYPE type, float duration)
{
    CCAnimation* animation = m_MagicAnimationFrames[type];

    float totalTime = animation->getFrames()->count() * 0.1;
    
    if (totalTime > duration)
    {
        totalTime = duration;
    }
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    char name[128]={0};
    
    
    sprintf(name, "%s_1.png", m_MagicAnimationNames[type].c_str());
    CCSmartSprite* pSprite = CCSmartSprite::createWithSpriteFrameName(name);
    
    pSprite->runAction(animate);
    
    pSprite->runAction(CCSequence::create(CCDelayTime::create(totalTime),
                                          CCCallFunc::create(pSprite, callfunc_selector(CCSmartSprite::removeSelf)),
                                          NULL));
    return pSprite;
    
}

CCSmartSprite* AnimationManager::createSkillAnimation(ENUM_PLAYER_PERFORM_SKILL_TYPE type, float duration)
{
    CCAnimation* animation = m_SkillAnimationFrames[type];
    
    float totalTime = animation->getFrames()->count() * 0.1;
    
    if (totalTime > duration)
    {
        totalTime = duration;
    }
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    char name[128]={0};
    
    
    sprintf(name, "%s_1.png", m_SkillAnimationNames[type].c_str());
    CCSmartSprite* pSprite = CCSmartSprite::createWithSpriteFrameName(name);
    
    pSprite->runAction(animate);
    
    pSprite->runAction(CCSequence::create(CCDelayTime::create(totalTime),
                                          CCCallFunc::create(pSprite, callfunc_selector(CCSmartSprite::removeSelf)),
                                          NULL));
    return pSprite;
}

CCAnimation* AnimationManager::createAnimation(const char *animationName, float frameRate)
{
    char name[128]={0};

    CCArray* frames = CCArray::createWithCapacity(7);
    for (int i=1;i<=8;++i)
    {
        sprintf(name, "%s_%d.png", animationName, i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        if (!frame)
        {
            break;
        }
        frames->addObject(frame);
    }

    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, frameRate);
    
    return animation;
}


CCSprite* AnimationManager::getPlayerSetDestinationAnimation()
{
    CCAnimation* animation = m_ActionSetPlayerDestination;
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    CCSmartSprite* pSprite = CCSmartSprite::createWithSpriteFrameName("action_set_player_destination_1.png");
    
    pSprite->runAction(CCSequence::create(animate,
                                          CCCallFunc::create(pSprite, callfunc_selector(CCSmartSprite::removeSelf)),
                                          NULL));

    return static_cast<CCSprite*>(pSprite);

}

CCSprite* AnimationManager::getUnitSetDestinationAnimation()
{
    CCAnimation* animation = m_ActionSetUnitDestination;
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    CCSmartSprite* pSprite = CCSmartSprite::createWithSpriteFrameName("destination_goal_1.png");
    
    pSprite->runAction(CCSequence::create(animate,
                                          CCCallFunc::create(pSprite, callfunc_selector(CCSmartSprite::removeSelf)),
                                          NULL));
    
    return static_cast<CCSprite*>(pSprite);
    
}


CCSprite* AnimationManager::getRecoverHpAnimation()
{
    CCAnimation* animation = m_ActionRecoverHp;
    
    CCAnimate* animate = CCAnimate::create(animation);
    
    CCSmartSprite* pSprite = CCSmartSprite::createWithSpriteFrameName("action_recover_hp_1.png");
    
    pSprite->runAction(CCSequence::create(animate,
                                          CCCallFunc::create(pSprite, callfunc_selector(CCSmartSprite::removeSelf)),
                                          NULL));
    
    return static_cast<CCSprite*>(pSprite);
}


CCSprite* AnimationManager::createDestinationGoal()
{
    
    CCSprite* sprite = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/tile_mask.png"));
 
    sprite->setOpacity(64);
    sprite->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(0.5, 255),
                                                                                    CCFadeTo::create(0.5, 64),
                                                                                    NULL)));
    return sprite;
}