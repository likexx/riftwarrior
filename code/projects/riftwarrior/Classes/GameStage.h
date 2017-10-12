//
//  GroundLayer.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/5/12.
//
//

#ifndef __ike_tdgame__GameStage__
#define __ike_tdgame__GameStage__

#include <iostream>
#include "cocos2d.h"
#include "Enemy.h"
#include "StageMap.h"
#include "BuildingManager.h"
#include "StageShadowLayer.h"
#include "MapNpcMessage.h"
#include "MapConfig.h"
#include "Unit.h"
#include "MapWinningCondition.h"
#include "SkillTouchAnimation.h"
#include <queue>
using namespace cocos2d;
using namespace std;

#define POP_MENU_TAG 3001
#define TEMP_BUILDING_TAG 3002
#define PLAYER_MOVE_DESTINATION_MARK_TAG 3003

enum
{
    GroundMapNodeTag = 0,
    GroundMapBuildingLayerTag = 1,
};

class GameStage : public CCLayer
{
public:
	GameStage() : 
		mapMoveSpeedRatio(1),
		m_CurrentWave(0), 
		touchMoveDx(0.0f),
		touchMoveDy(0.0f),
		m_TotalStageRunningTime(0.0f),
		m_pSkillTouchAnimation(nullptr) {};
    virtual bool init(int mapId);
    
    static GameStage* create(int mapId);

    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
    virtual void reset();
    virtual void clear();
    
	virtual void loadEnemies();
	virtual void loadEnemies(float /* dummy paramter for schedule_selector(SEL_SCHEDULE, float). */) { loadEnemies(); }

    virtual void update(float delta);
    virtual void draw();
    
    void cacheStageData();
    void clearStageData();
    
    void showBuildingToPlace(const CCPoint& tilePos);
    void removeUnplacedBuilding();
    void placePlayerMoveMark(const CCPoint& groundMapPos);
    void hidePlayerMoveMark();
    
    void placeUnitMoveMark(const CCPoint& groundMapPos);
    
    void moveToWithinBoundary(const CCPoint& tilePos, CCCallFunc* pCallback);
    void alignMapBoundary();

    void scrollToPlayer(float time = 0.2f);
    void followPlayer();
    
    void start();

    virtual void removeEnemy(Enemy* pEnemy);
    virtual void removeUnit(Unit* pUnit);
    
    
    virtual AttackableObject* getComputerPlayer();
    
    virtual Building* getPlayerBase();
    virtual Building* getComputerBase();
    
    bool hasEnemy(CCObject* pEnemy);
    
    bool hasUnit(CCObject* pUnit);
    
    StageMap* getGroundMap();
    BuildingManager* getBuildingManager();

    CCPoint locationFromTouch(CCTouch* pTouch);
    CCPoint locationFromTouches(CCSet* pTouches);
    
    inline bool isInsideMapBoundary(CCPoint pos)
    {
        return m_Boundary.containsPoint(pos);
    }

    inline int getMapId()
    {
        return m_MapId;
    }
    
    inline bool isReady()
    {
        return m_StageReady;
    }
    
    void checkFollowPlayer();
    
    
    inline float getBestZoomOutScale()
    {
        return m_InitialMapScale;
    }
    
    inline CCRect getMapBoundary()
    {
        return m_Boundary;
    }
    
    inline bool isCompleted()
    {
        return m_CurrentWave > m_MaxWave;
    }
    
    inline int getCurrentWave()
    {
        return m_CurrentWave;
    }
    
    inline AttackableObject* getNpc(int id)
    {
        return static_cast<AttackableObject*>(m_Npcs->objectForKey(id));
    }
    
    
    inline int getPassedEnemies()
    {
        return m_PassedEnemies;
    }
    
    inline void updatePassedEnemies(int value)
    {
        m_PassedEnemies += value;
    }
    
    inline const MapWinningCondition* getWinningCondition()
    {
        return &m_CurrentWinningCondition;
    }
    
    inline float getStageRunningTime()
    {
        return m_TotalStageRunningTime;
    }
    
    void pause();
    
    void resume();
    
    inline bool isPaused()
    {
        return m_Paused;
    }
    
    inline void stopFollowPlayer()
    {
        m_FollowPlayer = false;
    }
    
    inline long getMonsterKillerScore()
    {
        return m_MonsterKillerScore < 10 ? 0 : m_MonsterKillerScore/10;
    }
    
    inline void addMonsterKillScore(long score)
    {
        m_MonsterKillerScore += score;
    }
    
    virtual void removeBuilding(Building* pBuilding);
    
    void pauseAllEnemies();
    void resumeAllEnemies();
    
    void showWinningCondition();
    
    bool matchWinningCondition();
    
    void lightOnTile(int x, int y, int range);
    
    CCArray* enemies;
    
    CCArray* units;

protected:


    void setAnchorPointToStartPos();
    void resetAnchorPoint();
    bool isOutOfMapBoundary();
    bool isNearMapBoundary();
    
    void quickZoomOut(CCCallFunc* callback);
    
    queue< pair<int, int> > parseEnemies(string enemies);
    vector<int> getCurrentStageEnemies();
    
    void createEnviromentEffect();
    
    void showNextNpcMessage();
	void showOpeningAnimation();
	inline void showOpeningAnimation(float /* dummy parameter for schedule(SEL_SCHEDULE,  float). */) { showOpeningAnimation(); }
    virtual void setupWinningCondition();
    
    
    void completeStage();
    
    void loadCurrentWaveData();

    void zoomOut();
    void zoomIn();
    
    void updateTileMapPosition();
    void updateEnemies(const CCPoint& prevMapPos);
    void moveSelectedBuilding(const CCPoint& location);
    void updateShadowLayer();
    
    
    int getDynamicOpacityValue(int value, const CCPoint& current, const CCPoint& center, int initValue, int delta );
    void setDynamicOpacityValue(int value, const CCPoint& tilePos);
    
    void centerMapOnTileCoord(CCPoint tilePos);
    
    StageMap* pGroundMap;
    BuildingManager* pBuildingManager;
    SkillTouchAnimation* m_pSkillTouchAnimation;
    StageShadowLayer* m_pStageShadowLayer;
    
    
    CCPoint screenCenter;
    CCRect m_Boundary;
    CCPoint m_MapCenterPos;
    
    CCPoint m_TouchStartLocation;
    clock_t m_TouchStartTime;
    int mapMoveSpeedRatio;
    
    CCPoint m_TouchMoveLocation;
    
    float touchInitialDistance;
    
    float touchMoveDx;
    float touchMoveDy;
    float m_TotalStageRunningTime;
    bool m_StageReady;
    float m_InitialMapScale;
    
    int m_CurrentWave;
    int m_MaxWave;
    
    bool m_Paused;
    
    int m_PassedEnemies;
    
    int m_MapId;
    int m_TotalEnemyNumber;
    
    bool m_FollowPlayer;
    bool m_IsScrolling;
    
    queue<string> m_Enemies;
    queue<MapNpcMessage> npcMessageQueue;
    const MapConfig* m_pCurrentMapConfig;
    MapWinningCondition m_CurrentWinningCondition;
    
    CCDictionary* m_Npcs;
    
    long m_MonsterKillerScore;
    
};

#endif /* defined(__ike_tdgame__GameStage__) */
