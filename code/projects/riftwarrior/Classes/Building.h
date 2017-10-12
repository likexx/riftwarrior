//
//  Building.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/15/12.
//
//

#ifndef __ike_tdgame__Building__
#define __ike_tdgame__Building__

#include <iostream>
#include "Enemy.h"
#include "cocos2d.h"
#include "Function.h"
#include "BuildingUpgradeMenu.h"
#include "AttackComponent.h"
#include "GameData.h"
#include <vector>
#include <set>
#include <unordered_set>

using namespace cocos2d;
using namespace std;

class Building : public AttackableObject, public CCTouchDelegate
{
public:
    
    static int getBuildingTileIndex(int x, int y);
    
    Building(int id, bool unplaced);
    static Building* create(int id, int x, int y, bool unplaced);

    virtual void update(float dt);
    virtual void draw();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    virtual bool isActive();
    virtual void activate();
    virtual CCSize getTileSize();
    
    virtual void showBuildingActionMenu();
    
    virtual CCPoint getFireGroundPosition();

    virtual float getAttackRate();
    virtual void actWhenAttacked(AttackableObject* pTarget);
    virtual const CCPoint& getAttackableMapPosition();
    virtual CCPoint getShootPosition();
    virtual CCPoint getAttackablePosition();
    virtual void reduceHp(float damage, bool ignoreDefense = false);
    virtual bool isDead();
    virtual void remove();
    virtual void destroy();
    
    virtual void finishAttack();
    virtual bool ableToAttack();
    
    virtual CCNode* getAnimationPart();
    virtual void setStatus(ENUM_BUFF_STATUS status, float value, float duration);
    
    void moveToTile(const CCPoint& tilePos);
    
    void setColor(ccColor3B color);
    void select();
    void unselect();
    bool isSelected();
    float getOrderZ();
    int getId();
    
    virtual float getDamage();
    virtual float getAttackRange();
    virtual float getAttackInterval();
    
    virtual float getBaseSpeed();
    virtual void setSpeed(float speed);


    inline bool isUnplaceable()
    {
        return m_Unplaceable;
    }
    
    inline void setUnplaceable(bool value)
    {
        m_Unplaceable = value;
    }
    
    inline int getRepairTimes()
    {
        return m_RepairTimes;
    }
    
    inline const CCPoint& getCenterGroundOffset()
    {
        return centerGroundOffset;
    }
    
    inline const CCPoint& getOriginTilePosition()
    {
        return originTilePos;
    }
    
    inline const vector<int>& getAttackStones()
    {
        return attackStones;
    }
    
    inline const vector<int>& getDefenseStones()
    {
        return defenseStones;
    }
    
    inline int getLevel()
    {
        return m_Level;
    }
    
    inline float getHp()
    {
        return m_CurrentHp;
    }
    
    inline float getMaxHp()
    {
        return m_MaxHp;
    }
    
    
    void recalculateMaxHp();
    
    int getBaseCost();
    
    int getUpgradeCost();
    
    int getRepairCost();
    int getRepairGemCost();
    
    int getGemCost();
    int getUpgradeGemCost();
    
    
    
    void repair();
    
    inline void addAttackStone(int id)
    {
        attackStones.push_back(id);
    }
    
    inline void addDefenseStone(int id)
    {
        defenseStones.push_back(id);
    }
    
    inline CCSprite* getRotatingPart()
    {
        return m_pRotatingSprite;
    }
    
    inline ENUM_DAMAGE_TYPE getDamageType()
    {
        return GameData::getBuildingSetting(id).damageType;
    }
    
    inline CCPoint getWorldPosition()
    {
        return m_pBaseSprite->convertToWorldSpace(m_pBaseSprite->getPosition());
    }
    
    inline CCSprite* getBaseSprite()
    {
        return m_pBaseSprite ? m_pBaseSprite : m_pRotatingSprite;
    }
    
    
    inline bool isBaseVisible()
    {
        return m_pBaseSprite != NULL;
    }
    
    inline void setPlayerBuilding(bool value)
    {
        m_IsPlayerBuilding = value;
    }
    
    inline bool isPlayerBuilding()
    {
        return m_IsPlayerBuilding;
    }
    
//    bool isWithinAttackRange(const CCPoint& tilePos);
    
    inline AttackComponent* getAttackComponent()
    {
        return m_pAttackComponent;
    }
    
    int getMaxLevel();
    
    void showConstructionAnimation();
    void showUpgradeAnimation();

    void upgradeLevel();
    void removeBuildingMenu();
    
    void dropMoney();
    
    
protected:
    bool init(int x, int y, bool unplaced);
    virtual void initBuilding();

    virtual void showAttackRange();
    virtual void showMapTiles();
    

//    void resetAttackableTiles();
    void showExtraAnimation(CCArray* frames, CCPoint anchorPoint, float duration);
    void removeExtraAnimation();

    void showAssistingTileAnimations();
    
    bool isTouched(const CCPoint& touchPos);
    
    void initFromSetting();
    
    void updateScale();
    
    int id;
    CCSprite* m_pBaseSprite;
    CCSprite* m_pRotatingSprite;
    
    float m_CurrentOrderZ;
    bool active;
    
    bool m_Unplaceable;
    
    CCLabelBMFont* m_LevelLabel;
    
    float m_AttackInterval;
    float m_AttackDamage;
    float m_AttackRange;
    float m_CurrentHp;
    float m_MaxHp;
    
    vector<int> attackStones;
    vector<int> defenseStones;
    
    bool selected;
    int m_Level;
    
    int m_RepairTimes;
    
    int tileWidth;
    int tileLength;

    CCPoint centerGroundOffset;
    CCPoint centerMapOffset;
    
    CCPoint originTilePos;
    CCPoint m_CenterMapPos;
    CCPoint m_CenterIsoMapPos;
    
    CCNode* m_pBuildingMenu;
    
    std::vector<CCSprite*> levelStars;
    
    float timeElasped;
    
//    float m_TapStarted;
    CCPoint m_TapStartPosition;
    
    AttackComponent* m_pAttackComponent;
    
    const BuildingSetting* m_pSetting;
    
    vector<CCNode*> m_AdditionalNodes;
    
    bool m_IsPlayerBuilding;
    
//    __gnu_cxx::hash_set<int> m_AttackableTiles;
    

};

#endif /* defined(__ike_tdgame__Building__) */
