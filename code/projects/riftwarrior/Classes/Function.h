//
//  Function.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/16/12.
//
//

#ifndef __ike_tdgame__Function__
#define __ike_tdgame__Function__

#include <iostream>
#include "cocos2d.h"
#include <vector>
#include "CCSmartSprite.h"
#include "enums.h"
using namespace cocos2d;
using namespace std;

#define TIME_STYLE_WORD 1
#define TIME_STYLE_POINT    2
#define TIME_STYLE_MAX  3


namespace Function {
    void lazy_init();
    void setupShader(GLfloat* pColor);
    
    
    void drawSolidCircleOnMap( const CCPoint& center, float radius, float angle, unsigned int segments, ccColor4F color, float scaleX = 1.0f, float scaleY = 1.0f);
    void drawSolidEclipse( const CCPoint& center, float width, float height, float angle, unsigned int segments, ccColor4F color, float scaleX = 1.0f, float scaleY = 1.0f);
    
    void drawSolidFan(const CCPoint& center, float radius, float start_angle, float fan_angle, unsigned int segments, ccColor4F color);
    
    
    float getScaleForBuilding(const CCSize& spriteSize, float expectHeight);

    CCTexture2D* tryGetCachedTexture(const char* pTextureName);
    
    CCSprite* createAnimationSpriteWithImageFrames(const char *spriteName, const char *spriteAnimName, int number, float interval, bool repeat);
    
    
    vector<string> splitString(const string& value, const char* seperator);
    
    vector< pair<float, float> > splitStringToValuePairs(const string& value, const char* seperator);
    
    vector<int> splitStringToIntegers(const string& value, const char* seperator);
    
    bool updateDirection(const CCPoint& currentTilePos, const CCPoint& destinationTilePos, ENUM_MOVE_DIRECTION *pCurrentDirection);
    
    bool isAroundTileCenter(const CCPoint& tilePos, ENUM_MOVE_DIRECTION moveDirection);
    
    CCPoint getLastTile(const CCPoint& currentTilePos, ENUM_MOVE_DIRECTION moveDirection);

    CCRect getRect(CCNode* pNode);
    
    bool hasPlayerData();
    
    int getSkillTrainingTime(int id);
    int getSkillTrainingCost(int id);
    int getSkillInstantCompletionGemCost(int id);
    std::string getTimeString(unsigned long s,int style);
    
    CCPoint calculateFirePosition(CCSprite* owner, int currentQuadrant,  float *xOffsets, float *yOffsets);
    
    void scaleNodeForScreen(cocos2d::CCNode *pNode, const cocos2d::CCPoint &anchor);
    void scaleNodeForScreen(cocos2d::CCNode *pNode, const cocos2d::CCPoint &anchor, const cocos2d::CCPoint &position);
    
};
#endif /* defined(__ike_tdgame__Function__) */
