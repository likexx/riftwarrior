//
//  HorizontalMap.cpp
//  tdgame
//
//  Created by Like Zhang on 2/18/13.
//
//

#include "HorizontalMap.h"
#include <stack>
using namespace std;


// suppose start from left and end at top right
queue<CCPoint> HorizontalMap::createRandomWayPoints(const cocos2d::CCPoint &startTilePos, const cocos2d::CCPoint &endTilePos)
{
    
    queue<CCPoint> result;
    int areaWidth = (int)(m_MaxRoadTilePosition.x - m_MinRoadTilePosition.x);
    int areaHeight = (int)(m_MaxRoadTilePosition.y - m_MinRoadTilePosition.y);
    
    srand(time(NULL));
    
    bool startFromLeft = startTilePos.x < endTilePos.x;
    CCPoint startPos = startFromLeft ? startTilePos : endTilePos;
    CCPoint endPos = startFromLeft ? endTilePos : startTilePos;
    
    CCPoint prev = startPos;
    
    while (true)
    {
        int newY = m_MinRoadTilePosition.y + rand() % areaHeight;
        
        int delta  = 2 + rand()%(areaWidth/4);
        
        int newX = prev.x + delta;
        if (newX == endPos.x - 1 && newY == endPos.y)
        {
            newX--;
        }
        
        if (newX >= endPos.x)
        {
            
            if (newX > m_MaxRoadTilePosition.x)
            {
                newX = m_MaxRoadTilePosition.x;
            }
            
            if (prev.y != endPos.y)
            {
                if (prev.y > endPos.y)
                {
                    newY = endPos.y + rand() % (int)(prev.y - endPos.y);
                }
                else
                {
                    newY = endPos.y - rand() % (int)(endPos.y - prev.y);
                }
                
                result.push(ccp(newX, newY));
            }
            break;
        }
        
        prev = ccp(newX, newY);
        result.push(prev);
    }
    
    if (!startFromLeft)
    {
        // reverse the points order
        stack<CCPoint> positionStack;
        while(!result.empty())
        {
            positionStack.push(result.front());
            result.pop();
        }
        
        while(!positionStack.empty())
        {
            result.push(positionStack.top());
            positionStack.pop();
        }
        
    }
    
    return result;
}