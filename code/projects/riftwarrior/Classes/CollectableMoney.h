//
//  CollectableMoney.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/14/12.
//
//

#ifndef __ike_tdgame__CollectableMoney__
#define __ike_tdgame__CollectableMoney__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class CollectableMoney : public CCNode, public CCTouchDelegate
{
public:
    
    virtual bool init();
    virtual void update(float dt);
    
    CREATE_FUNC(CollectableMoney);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
  
    void remove();

    inline void setBuildingTileIndex(int index)
    {
        buildingTileIndex = index;
    }
    
    inline void setOffsetToBuilding(const CCPoint& offset)
    {
        offsetToBuilding = offset;
    }
    
    static const int MONEY_TAG = 101;

private:
	CollectableMoney() :
		maxLifeSeconds(10.0f),
		currentLifeTime(0.0f),
		collected(false),
		sprite(nullptr) {};

	const float maxLifeSeconds;
	float currentLifeTime;
    bool collected;
    CCSprite* sprite;
    int buildingTileIndex;
    CCPoint offsetToBuilding;
    
    void onClick(CCObject* pSender);

};

#endif /* defined(__ike_tdgame__CollectableMoney__) */
