//
//  AnimationManager.h
//  tdgame
//
//  Created by Like Zhang on 3/4/13.
//
//

#ifndef __tdgame__AnimationManager__
#define __tdgame__AnimationManager__

#include <iostream>
#include "enums.h"
#include "cocos2d.h"
#include <map>
#include <string>
#include "CCSmartSprite.h"
using namespace cocos2d;
using namespace std;

class AnimationManager
{
public:

    static AnimationManager* getInstance();
    
    void init();
    void initEnvironmentAnimationFrames();
    void removeEnvironmentAnimationFrames();
    
    CCArray* sharedBuildingConstructionFrames;
    CCArray* sharedBuildingUpgradeFrames;
    
    CCSprite* createBulletAnimation(ENUM_BULLET_ANIMATION animationType);
    CCSmartSprite* createStatusAnimation(ENUM_BUFF_STATUS status, float duration);
    CCSmartSprite* createMagicAnimation(ENUM_MAGIC_TYPE type, float duration);
    
    CCSmartSprite* createSkillAnimation(ENUM_PLAYER_PERFORM_SKILL_TYPE type, float duration);

    CCSprite* createEnvironmentAnimation(ENUM_ENVIRONMENT_ANIMATION_TYPE type);
    
    CCSprite* createDestinationGoal();
    
    CCSprite* getPlayerSetDestinationAnimation();
    
    CCSprite* getUnitSetDestinationAnimation();
    
    CCSprite* getRecoverHpAnimation();
    
    CCSmartSprite* getEnemySpecialAttackAnimation(ENUM_ENEMY_ATTACK_TYPE type);
    
private:
    void initBulletAnimationFrames();
    void initStatusAnimationFrames();
    void initMagicAnimationFrames();
    void initSkillAnimationFrames();
    void initEnemySpecialAnimations();
    
    CCAnimation* createAnimation(const char* animationName, float frameRate = 0.1f);
    
    map<ENUM_BULLET_ANIMATION, CCAnimation*> m_BulletAnimationFrames;
    map<ENUM_BULLET_ANIMATION, string> m_BulletAnimationNames;
    
    map<ENUM_BUFF_STATUS, CCAnimation*> m_StatusAnimationFrames;
    map<ENUM_BUFF_STATUS, string> m_StatusAnimationNames;

    map<ENUM_MAGIC_TYPE, CCAnimation*> m_MagicAnimationFrames;
    map<ENUM_MAGIC_TYPE, string> m_MagicAnimationNames;

    map<ENUM_ENVIRONMENT_ANIMATION_TYPE, CCAnimation*> m_EnvironmentAnimationFrames;
    map<ENUM_ENVIRONMENT_ANIMATION_TYPE, string> m_EnvironmentAnimationNames;
    
    map<ENUM_PLAYER_PERFORM_SKILL_TYPE, CCAnimation*> m_SkillAnimationFrames;
    map<ENUM_PLAYER_PERFORM_SKILL_TYPE, string> m_SkillAnimationNames;

    map<ENUM_ENEMY_ATTACK_TYPE, CCAnimation*> m_EnemySpecialAttackAnimationFrames;
    map<ENUM_ENEMY_ATTACK_TYPE, string> m_EnemySpecialAttackAnimationNames;
    
    CCAnimation* m_ActionSetPlayerDestination;
    
    CCAnimation* m_ActionSetUnitDestination;
    
    CCAnimation* m_ActionRecoverHp;
};

#endif /* defined(__tdgame__AnimationManager__) */
