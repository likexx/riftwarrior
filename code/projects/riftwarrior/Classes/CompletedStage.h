//
//  CompletedStage.h
//  tdgame
//
//  Created by Like Zhang on 5/7/13.
//
//

#ifndef __tdgame__CompletedStage__
#define __tdgame__CompletedStage__

#include <iostream>
#include "GameStage.h"
#include "Enemy.h"

class CompletedStage : public GameStage
{
public:
    virtual bool init(int mapId);
    
    static CompletedStage* create(int mapId);
    
    virtual void reset();
    virtual void clear();
    virtual void loadEnemies();
    virtual void removeEnemy(Enemy* pEnemy);
    
private:
    
    
    int m_MaxEnemyId;
    
    vector<int> m_CurrentStageEnemies;
    
    Enemy* m_Boss;
    
    
};
#endif /* defined(__tdgame__CompletedStage__) */
