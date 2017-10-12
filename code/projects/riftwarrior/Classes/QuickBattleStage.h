//
//  QuickBattleStage.h
//  tdgame
//
//  Created by Like Zhang on 5/6/13.
//
//

#ifndef __tdgame__QuickBattleStage__
#define __tdgame__QuickBattleStage__

#include <iostream>
#include "GameStage.h"
#include "ComputerPlayer.h"

class QuickBattleStage : public GameStage
{
public:
    virtual bool init(int mapId);
    
    static QuickBattleStage* create(int mapId);
    
    virtual void reset();
    virtual void clear();
    virtual void loadEnemies();
    
    virtual ComputerPlayer* getComputerPlayer();
    virtual Building* getPlayerBase();
    
    virtual Building* getComputerBase();
    
    virtual void removeBuilding(Building* pBuilding);
    
private:
    
    virtual void setupWinningCondition();
    
    Building* m_pPlayerTower;
    Building* m_pComputerTower;
    
    ComputerPlayer* m_pComputer;

    vector<int> m_UsableEnemies;
    
    
};

#endif /* defined(__tdgame__QuickBattleStage__) */
