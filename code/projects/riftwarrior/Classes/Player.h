//
//  Player.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/7/12.
//
//

#ifndef __ike_tdgame__Player__
#define __ike_tdgame__Player__

#include <iostream>
#include "PlayerAnimation.h"
#include "AttackableObject.h"
#include "AttackComponent.h"
#include "PlayerItems.h"
#include "PlayerSkills.h"
#include "PlayerAttackComponent.h"
#include "cocos2d.h"
#include "SystemHelper.h"
#include "Enemy.h"
#include <set>
#include <unordered_set>
#include <unordered_map>
using namespace cocos2d;
using namespace std;

#define GAME_VERSION 1.8f

class Player : public AttackableObject, public CCTouchDelegate
{
public:
    static Player* getInstance();
    
    Player();
    
    virtual void update(float dt);
    
    virtual void draw();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    // implement AttackableObject
    virtual CCPoint getShootPosition();
    virtual CCPoint getAttackablePosition();
    virtual void reduceHp(float damage, bool ignoreDefense = false);
    virtual bool isDead();
    virtual void remove();
    virtual bool isActive();
    virtual const CCPoint& getAttackableMapPosition();
    virtual void destroy();

    virtual void actWhenAttacked(AttackableObject* pTarget);
    virtual float getDamage();
    virtual float getAttackRange();
    virtual float getAttackRate();
    
    virtual float getBaseSpeed();
    virtual void setSpeed(float speed);

    virtual CCNode* getAnimationPart();

    virtual bool ableToAttack();
    
    
    void finishAttack();

    bool isTouched(const CCPoint& touchPos);
    
    void load();
    void save();
    void saveMoney();
    void reset();
    
    void showOnMap();
    void showMessage(const char* pMessage);
    void addPointsToSkill(int skillId,bool cost);
    
    void updateNearbyBuildableTiles();
    
    void setMoveDestination(const CCPoint& groundMapPos, bool placeMark);
    void stopMove();
    
    inline PlayerAnimation* getAnimation()
    {
        return m_pAnimation;
    }
    
    inline int getMoney()
    {
        return m_Money;
    }
    
    inline int getGems()
    {
        return m_Gems;
    }
    
    void updateMoney(int delta);
    
    void updateGems(int delta);
    
    void purchaseMap(int mapId);
    
    inline void setLevel(int level)
    {
        m_Level = level;
    }
    
    inline int getLevel()
    {
        return m_Level;
    }
    
    inline int getHp()
    {
        return m_Hp;
    }
    
    inline int getMaxHp()
    {
        return m_MaxHp;
    }
    
    inline bool hasPurchasedMap(int id)
    {
        return m_MapPurchased[id] > 0;
    }
    
    void updateHp(float dt);
    void addHp(float value);
    
    void updateMp(float dt);
    
    inline int getSkillPoints()
    {
        return m_SkillPoints;
    }
    
    // return new skill points
    inline int updateSkillPoints(int delta)
    {
        m_SkillPoints += delta;
        
        if (m_SkillPoints < 0)
        {
            m_SkillPoints = 0;
        }
        
        return m_SkillPoints;
    }
    
    inline int getSkillLevel(int skillId)
    {
        if (m_Skills.find(skillId) == m_Skills.end())
        {
            return 0;
        }
        
        return m_Skills[skillId];
    }
    
    inline int getMp()
    {
        return m_Mp;
    }
    
    inline int getMaxMp()
    {
        return m_MaxMp;
    }
    
    
    inline void setExperience(unsigned long experience)
    {
        m_Experience = experience;
    }
    
    void updateExperience(int delta);
    
    inline unsigned long getExperience()
    {
        return m_Experience;
    }
    
    inline unsigned long getNextLevelExperience()
    {
        if (m_Level >= 50)
        {
            // max level is 60 for now
            
            return 0;
        }
        
        unsigned long value = 0;
        
        if (m_Level <= 5)
        {
           value =  (unsigned long)(150 * pow(m_Level, 2));
        }
        else if (m_Level <= 10)
        {
            value =  (unsigned long)(300 * pow(m_Level, 2));
        }
        else if (m_Level <= 20)
        {
            value =  (unsigned long)(1000 * pow(m_Level, 2));
        }
        else if (m_Level <= 40)
        {
            value =  (unsigned long)(3000 * pow(m_Level, 2));
        }
        else if (m_Level <= 50)
        {
            value =  (unsigned long)(6000 * pow(m_Level, 2));
        }
        
        return value;
        
    }
    
    inline bool isInCriticalCondition()
    {
        return m_Hp > 0 && (m_Hp < 100 || m_Hp < getMaxHp() * 0.3);
    }
    
    inline int getCurrentMapId()
    {
        return m_CurrentMapId;
    }
    
    inline void completeMap(int mapId)
    {
        if (hasCompletedMap(mapId))
        {
            return;
        }
        
        m_CompletedMapScores[mapId] = 1;
    }
    
    inline void setMapScore(int mapId, int score)
    {
        
        if (hasCompletedMap(mapId) && m_CompletedMapScores[mapId] >= score)
        {
            return;
        }
        
        m_CompletedMapScores[mapId] = score;
    }
    
    inline int getMapScore(int mapId)
    {
        if (hasCompletedMap(mapId))
        {
            return m_CompletedMapScores[mapId];
        }
        
        return 0;
    }
    
    inline bool hasCompletedMap(int mapId)
    {
        return m_CompletedMapScores.find(mapId) != m_CompletedMapScores.end();
    }
    
    
    inline PlayerItems& playerItems()
    {
        return items;
    }
    
    inline bool canUseSkill(int id)
    {
        const ActiveSkillSetting* pSetting = GameData::getActiveSkillSetting(id);
        if(pSetting->requireLevel > getLevel())
        {
            return false;
        }
        
        return true;
        
    }
    
    inline bool canPerformSkill(int id)
    {
        const ActiveSkillSetting* pSetting = GameData::getActiveSkillSetting(id);
        
        float mpCost = pSetting->mpCost * (1 - 0.05 * getSkillLevel(ENUM_SKILL_SKILL_FOCUS));
        
        if(pSetting->requireLevel > getLevel() ||
           mpCost > m_Mp ||
           m_PerformingSkillType != ENUM_PLAYER_PERFORM_SKILL_NONE)
        {
            return false;
        }
        
        return true;
    }
    
    inline void stopPerformingSkill()
    {
        currentPerformingSkillId = 0;
    }
    
    bool performSkill(int id);
    
    inline int getPerformingSkillId()
    {
        return currentPerformingSkillId;
    }
    
    inline unsigned int getProgressFlags()
    {
        return m_ProgressFlags;
    }
    
    inline bool hasCompletedTutorial()
    {
        return (m_ProgressFlags & ENUM_COMPLETE_BASIC_TUTORIAL) > 0;
    }
    
    inline void setProgressFlag(unsigned int flag)
    {
        m_ProgressFlags |= flag;
    }
    
    inline void increaseKillCount()
    {
        m_KillCount++;
    }
    
    inline int getKillCount()
    {
        return m_KillCount;
    }
    
    inline bool isStanding()
    {
        ENUM_NPC_ANIMATION animation = m_pAnimation->getCurrentAnimationType();
        
        if (animation == ENUM_NPC_ANIMATION_STAND_UP ||
            animation == ENUM_NPC_ANIMATION_STAND_DOWN ||
            animation == ENUM_NPC_ANIMATION_STAND_LEFT ||
            animation == ENUM_NPC_ANIMATION_STAND_RIGHT)
        {
            return true;
        }
        
        return false;
    }
    
    inline bool isFighting()
    {
        return attackComponent->hasTarget();
    }
    
    inline bool isAtTile(const CCPoint& tilePos)
    {
        return (int)m_CurrentTilePosition.x == tilePos.x && (int)m_CurrentTilePosition.y == tilePos.y;
    }
    
    inline const CCPoint& getCurrentMapTile()
    {
        return m_CurrentTilePosition;
    }
    
    inline const CCPoint& getCurrentMapPosition()
    {
        return m_CurrentMapPosition;
    }
    
    inline void setForceToMove(bool value)
    {
        m_ForceToMove = value;
    }
    
    inline bool isForceToMove()
    {
        return m_ForceToMove;
    }
    
    
    inline bool isPerformingSKill()
    {
        return m_PerformingSkillType != ENUM_PLAYER_PERFORM_SKILL_NONE;
    }
    
    inline void setWaitingForPlaceUnitDestination(bool value)
    {
        m_WaitForSetUnitDestination = value;
    }
    
    inline bool isWaitingForPlaceUnitDestination()
    {
        return m_WaitForSetUnitDestination;
    }
    
    inline void enableMove()
    {
        m_Movable = true;
    }
    
    inline void disableMove()
    {
        m_Movable = false;
    }
    
    inline float getDefense()
    {
        float k = m_Level - 1;
        float defense = ceil(k*k/2);
        return defense;
    }
    
    inline void addKilledEnemies(int id)
    {
        m_KilledEnemies[id]++;
    }
    
    inline unsigned int getEnemiesKillCount(int id)
    {
        return m_KilledEnemies[id];
    }
    
    inline bool canTrainSkill()
    {
        return m_CurrentTrainingSkills.size() < 2;
    }
    
    inline unsigned long getTrainingSkillStartTime(int id)
    {
        map<int, unsigned long>::iterator it = m_CurrentTrainingSkills.find(id);
        if (it == m_CurrentTrainingSkills.end())
        {
            return 0;
        }
        
        return it->second;
    }
    
    inline bool isSkillStartTraining(int id)
    {
        map<int, unsigned long>::iterator it = m_CurrentTrainingSkills.find(id);
        if(it != m_CurrentTrainingSkills.end()){
            return true;
        }else{
            return false;
        }
    }
    
    bool isBuildableTile(const CCPoint& tile);
    
    void completeSkillTrainingWithGems(int id,int cost);
    
    inline unsigned long getTrainingSkillLackTime(int id)
    {
        unsigned long time = SystemHelper::getCurrentTimeInSeconds() - this->getTrainingSkillStartTime(id);
        return Function::getSkillTrainingTime(id) - time;
    }
    
    bool tryStartTrainSkill(int id);
    bool isSkillAlreadyTraining(int id);    
    
    void updateTrainingSkills();
    
    void resetSkills();
    
    bool canUseBuilding(int buildingId);
    bool canUseMagic(int skillId);

    void startWhirlWind();
    void stopWhirlWind();

    void showEarthSlashSprite(CCObject* pObject);
    
    void getKillBonus(Enemy* pEnemy);
    void getHitBonus(Enemy* pEnemy);
    

protected:
    
    bool updateDirection();
    bool reachedDestination();
    void tryToStand();
    void showLevelUpAnimation(float hpDiff, float attackDiff);
    bool isWithdrawing();
    
    void resetToPrevAnimation();
    void performWhirlWind(float dt);
    void performEarthSlash();
    void performRecovery();
    
    void hurtWithEarthSlash(CCObject * pObject);
    
    void showSkillNameAnimation(int id);
    
    ENUM_MOVE_DIRECTION getAttackDirection(ENUM_NPC_ANIMATION prevAnimation);
    
    void saveGems();
    void updatePlayerData(size_t i, const string& value);
    
    int m_Level;
    int m_Money;
    long m_Gems;
    float m_Hp;
    int m_MaxHp;
    float m_Mp;    // float value for increasing by each frame
    int m_MaxMp;
    int m_SkillPoints;
    unsigned long m_Experience;
    int m_AttackRange;
    float m_MeleeDamage;
    int m_Speed;
    
    int m_CurrentMapId;
    map<int, int> m_CompletedMapScores;
    
    unsigned int m_ProgressFlags;
    
    PlayerItems items;
//    PlayerSkills m_Skills;
    map<int,int> m_Skills;
    
    map<int,unsigned int> m_KilledEnemies;
    
    map<int, int> m_MapPurchased;
    
    PlayerAnimation* m_pAnimation;
    CCPoint m_CurrentMapPosition;
    CCPoint m_CurrentTilePosition;
    PlayerAttackComponent* attackComponent;

    ENUM_MOVE_DIRECTION m_CurrentMoveDirection;
    ENUM_NPC_ANIMATION m_OriginalStandAnimation;
    ENUM_NPC_ANIMATION m_PreviousAnimation;
    
    
    int currentPerformingSkillId;
    ENUM_PLAYER_PERFORM_SKILL_TYPE m_PerformingSkillType;

    
    //bool m_Selected;
    CCPoint m_TapStartPosition;
    
    
    CCPoint m_MoveDestinationMapPos;
    bool m_DestinationChanged;
    bool m_MovingToEndPosition;

    bool m_IsSetToMove;
    bool m_ForceToMove;
    bool m_Movable;
    
    bool m_WaitForSetUnitDestination;
    
    int m_KillCount;
    
    map<int, unsigned long> m_CurrentTrainingSkills;
    
    unordered_map<int, CCSprite*> m_NearbyBuildingTiles;
    
    float m_TimeElapsedSinceLastUpdate;
    
    CCNode* m_CriticalLayer;
};

#endif /* defined(__ike_tdgame__Player__) */
