//
//  VerticalMap.cpp
//  tdgame
//
//  Created by Like Zhang on 2/18/13.
//
//

#include "VerticalMap.h"
#include <stack>
using namespace std;

// suppose start from top/down or down/top
queue<CCPoint> VerticalMap::createRandomWayPoints(const cocos2d::CCPoint &startTilePos, const cocos2d::CCPoint &endTilePos)
{
    
    queue<CCPoint> result;
    int areaWidth = (int)(m_MaxRoadTilePosition.x - m_MinRoadTilePosition.x);
    int areaHeight = (int)(m_MaxRoadTilePosition.y - m_MinRoadTilePosition.y);
    
    bool startFromTop = startTilePos.y < endTilePos.y;
    CCPoint startPos = startFromTop ? startTilePos : endTilePos;
    CCPoint endPos = startFromTop ? endTilePos : startTilePos;

    // divide the width to 3 sections,
    // make sure each new point is in seciont 1 or 3 (opposite to previous), not in middle
    int x1 = m_MinRoadTilePosition.x + areaWidth/3;
    int x2 = m_MinRoadTilePosition.x + areaWidth*2/3;
    
    CCPoint prev = startPos;
    
    while (prev.y < endPos.y)
    {
        int delta  = 2 + rand()%(areaHeight/4);
        
        int newY = prev.y + delta;
        int newX;
        
        if (prev.x <= x1)
        {
            newX = x2 + rand()%(areaWidth/3);
        }
        else if (prev.x >= x2)
        {
            newX = m_MinRoadTilePosition.x + rand()%(areaWidth/3);
        }
        else
        {
            if (rand()%2 == 0)
            {
                newX = m_MinRoadTilePosition.x + rand()%2;
            }
            else
            {
                newX = m_MaxRoadTilePosition.x - rand()%2;
            }
        }

        if (newY >= endPos.y)
        {
            if (newY > endPos.y && newY >m_MaxRoadTilePosition.y)
            {
                int len = m_MaxRoadTilePosition.y - endPos.y;
                if (len > 1)
                {
                    newY = endPos.y + rand()%len;
                    result.push(ccp(newX, newY));
                }
            }
            
            break;
        }
        
        prev = ccp(newX, newY);
        result.push(prev);
    }
    
    if (!startFromTop)
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