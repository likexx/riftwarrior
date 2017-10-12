//
//  Function.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/16/12.
//
//

#include "Function.h"
#include "MapHelper.h"
#include "GameScene.h"
#include <memory>

static bool s_bInitialized = false;
static CCGLProgram* s_pShader = NULL;
static int s_nColorLocation = -1;
static int s_nPointSizeLocation = -1;
static GLfloat s_fPointSize = 1.0f;

void Function::lazy_init()
{
    if( ! s_bInitialized ) {
        
        //
        // Position and 1 color passed as a uniform (to simulate glColor4ub )
        //
        s_pShader = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_Position_uColor);
        
        s_nColorLocation = glGetUniformLocation( s_pShader->getProgram(), "u_color");
        CHECK_GL_ERROR_DEBUG();
        s_nPointSizeLocation = glGetUniformLocation( s_pShader->getProgram(), "u_pointSize");
        CHECK_GL_ERROR_DEBUG();
        
        s_bInitialized = true;
    }
}

void Function::setupShader(GLfloat* pColor)
{
    s_pShader->use();
    s_pShader->setUniformsForBuiltins();
    s_pShader->setUniformLocationWith4fv(s_nColorLocation, pColor, 1);
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
}

void Function::drawSolidCircleOnMap( const CCPoint& center, float radius, float angle, unsigned int segments, ccColor4F color, float scaleX, float scaleY)
{
    lazy_init();
    
    CCPoint centerMapPos = MapHelper::isoMapPosToOrthMapPos(center);
    
    int additionalSegment = 1;
    
    const float coef = 2.0f * (float)M_PI/segments;
    
    GLfloat *vertices = (GLfloat*)calloc( sizeof(GLfloat)*2*(segments+2), 1);
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + angle) * scaleX + centerMapPos.x;
        GLfloat k = radius * sinf(rads + angle) * scaleY + centerMapPos.y;
        
        CCPoint screenPos = MapHelper::mapPosToGroundMapPos(ccp(j,k));
        j = screenPos.x;
        k = screenPos.y;
        
        vertices[i*2] = j;
        vertices[i*2+1] = k;
    }
    
    vertices[(segments+1)*2] = center.x;
    vertices[(segments+1)*2+1] = center.y;
    
    s_pShader->use();
    s_pShader->setUniformsForBuiltins();
    s_pShader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &color.r, 1);
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) segments+additionalSegment);
    
    free( vertices );
    
    CC_INCREMENT_GL_DRAWS(1);
    
}

void Function::drawSolidEclipse(const CCPoint& center, float width, float height, float angle, unsigned int segments, ccColor4F color, float scaleX , float scaleY)
{
    lazy_init();
    
    int additionalSegment = 1;
    
    const float coef = 2.0f * (float)M_PI/segments;
    
    GLfloat *vertices = (GLfloat*)calloc( sizeof(GLfloat)*2*(segments+2), 1);
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = width* cosf(rads + angle) * scaleX + center.x;
        GLfloat k = height * sinf(rads + angle) * scaleY + center.y;
        
        vertices[i*2] = j;
        vertices[i*2+1] = k;
    }
    vertices[(segments+1)*2] = center.x;
    vertices[(segments+1)*2+1] = center.y;
    
    s_pShader->use();
    s_pShader->setUniformsForBuiltins();
    s_pShader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &color.r, 1);
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) segments+additionalSegment);
    
    free( vertices );
    
    CC_INCREMENT_GL_DRAWS(1);
    
}

void Function::drawSolidFan(const CCPoint& center, float radius, float start_angle, float fan_angle, unsigned int segments, ccColor4F color)
{
    lazy_init();
    
    int additionalSegment = 2;
    
    float coef =  fan_angle/segments;
    
    if (fan_angle > M_PI)
    {
        coef = M_PI/segments;
    }
    
    GLfloat *vertices = (GLfloat*)calloc( sizeof(GLfloat)*2*(segments+2), 1);
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + start_angle)  + center.x;
        GLfloat k = radius * sinf(rads + start_angle)  + center.y;
        
        vertices[i*2] = j;
        vertices[i*2+1] = k;
    }
    vertices[(segments+1)*2] = center.x;
    vertices[(segments+1)*2+1] = center.y;
    
    s_pShader->use();
    s_pShader->setUniformsForBuiltins();
    s_pShader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &color.r, 1);
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) segments+additionalSegment);
    
    free( vertices );
    
    CC_INCREMENT_GL_DRAWS(1);
    
    // draw again if fan angle > M_PI
    if (fan_angle > M_PI)
    {
        drawSolidFan(center, radius, start_angle+M_PI, fan_angle - M_PI, segments, color);
    }
}



float Function::getScaleForBuilding(const cocos2d::CCSize &spriteSize, float expectHeight)
{
    float height = spriteSize.height;

    return expectHeight>=height ? 1 : expectHeight/height;
}

CCTexture2D* Function::tryGetCachedTexture(const char *pTextureName)
{
    CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->textureForKey(pTextureName);
    if (!pTexture)
    {
        pTexture = CCTextureCache::sharedTextureCache()->addImage(pTextureName);
    }
    
    return pTexture;
}

CCSprite* Function::createAnimationSpriteWithImageFrames(const char *spriteName, const char *spriteAnimName, int number, float interval, bool repeat)
{
    CCSprite* sprite = CCSprite::create();
    
    if (sprite->initWithSpriteFrameName(spriteName))
    {
        int count = number;
        char name[128];
        CCArray* frames = CCArray::createWithCapacity(count);
        for(int i=1;i<=count;++i)
        {
            sprintf(name, "%s_%d.png", spriteAnimName, i);
            
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
            if (!frame)
            {
                break;
            }
            frames->addObject(frame);
        }
        
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, interval);
        
        CCAnimate* animate = CCAnimate::create(animation);

        if (repeat)
        {
            CCRepeatForever* repeat = CCRepeatForever::create(animate);
            sprite->runAction(repeat);
        }
        else
        {
            sprite->runAction(animate);
        }
        
        sprite->scheduleUpdate();
    }
    
    return sprite;
}

vector< pair<float, float> > Function::splitStringToValuePairs(const string& value, const char* seperator)
{
    vector< pair<float, float> > result;
    
    size_t prev_pos = 0;
    size_t pos = value.find(seperator, 0);
    
    while (pos != string::npos)
    {
        string s = value.substr(prev_pos, pos - prev_pos);
        
        float x = atof(s.c_str());
        
        prev_pos = pos + 1;
        pos = value.find(seperator, prev_pos);
        
        if (pos != string::npos)
        {
            s = value.substr(prev_pos, pos - prev_pos);
        }
        else
        {
            // reaching end
            s = value.substr(prev_pos);
        }
        
        float y = atof(s.c_str());
        
        result.push_back(pair<float, float>(x, y));
        
        if (pos != string::npos)
        {
            prev_pos = pos + 1;
            pos = value.find(seperator, prev_pos);
        }
    }
    
    return result;

}

vector<string> Function::splitString(const string &value, const char *seperator)
{
    vector<string> result;
 
    size_t prev_pos = 0;
    size_t pos = value.find(seperator, 0);
    
    while (pos != string::npos)
    {
        string s = value.substr(prev_pos, pos - prev_pos);
        
        if (s.length()>0)
        {
            if (s[0]=='\n')
            {
                s = s.substr(1);
            }
            result.push_back(s);
        }
        
        prev_pos = pos + 1;
        pos = value.find(seperator, prev_pos);
        
    }
    
    // reaching end
    string s = value.substr(prev_pos);
    
    if (s.length()>0)
    {
        if (s[0]=='\n')
        {
            s = s.substr(1);
        }
        result.push_back(s);
    }
    
    return result;
}

vector<int> Function::splitStringToIntegers(const string &value, const char *seperator)
{
    vector<int> result;
    
    size_t prev_pos = 0;
    size_t pos = value.find(seperator, 0);
    
    while (pos != string::npos)
    {
        string s = value.substr(prev_pos, pos - prev_pos);
        
        if (s.length()>0)
        {
            int x = atoi(s.c_str());
            result.push_back(x);
        }
        
        prev_pos = pos + 1;
        pos = value.find(seperator, prev_pos);
        
    }

    // reaching end
    string s = value.substr(prev_pos);
    
    if (s.length() > 0)
    {
        result.push_back(atoi(s.c_str()));
    }

    return result;
}

CCPoint Function::getLastTile(const CCPoint& currentTilePos, ENUM_MOVE_DIRECTION moveDirection)
{
    CCPoint tilePos = ccp((int)currentTilePos.x, (int)currentTilePos.y);
    
    switch (moveDirection)
    {
        case ENUM_MOVE_DIRECTION_RIGHT:
            tilePos.x--;
            break;
        case ENUM_MOVE_DIRECTION_LEFT:
            tilePos.x++;
            break;
        case ENUM_MOVE_DIRECTION_DOWN:
            tilePos.y--;
            break;
        case ENUM_MOVE_DIRECTION_UP:
            tilePos.y++;
            break;
    }
    
    if (tilePos.x < 0)
    {
        tilePos.x = 0;
    }
    if (tilePos.y < 0)
    {
        tilePos.y = 0;
    }
    
    return tilePos;
    
}

bool Function::isAroundTileCenter(const CCPoint& tilePos, ENUM_MOVE_DIRECTION moveDirection)
{
    const float padding = 0.3f;
    float minX = floor(tilePos.x) + padding;
    float maxX = minX + (1-2*padding);
    float minY = floor(tilePos.y) + 0.3;
    float maxY = minY + (1-2*padding);
    
    bool result = false;
    
    if (tilePos.x > minX && tilePos.x < maxX &&
        tilePos.y > minY && tilePos.y < maxY)
    {
        result = rand()%4 == 0;
    }
    
    
    if (!result)
    {
        switch (moveDirection)
        {
            case ENUM_MOVE_DIRECTION_RIGHT:
                if (tilePos.x > maxX)
                {
                    result = true;
                }
                break;
            case ENUM_MOVE_DIRECTION_LEFT:
                if (tilePos.x < minX)
                {
                    result = true;
                }
                break;
            case ENUM_MOVE_DIRECTION_DOWN:
                if (tilePos.y > maxY)
                {
                    result = true;
                }
                break;
            case ENUM_MOVE_DIRECTION_UP:
                if (tilePos.y < minY)
                {
                    result = true;
                }
                break;
        }
    }
    
    return result;
}

bool Function::updateDirection(const CCPoint& currentTilePos, const CCPoint& destinationTilePos, ENUM_MOVE_DIRECTION *pCurrentDirection)
{
    StageMap* pGroundMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    // this is the tile position to be used !
    CCPoint tilePos = isAroundTileCenter(currentTilePos, *pCurrentDirection) ? currentTilePos : getLastTile(currentTilePos, *pCurrentDirection);

    ENUM_TILE_DIRECTION currentTileDirection = pGroundMap->getTileDirection(tilePos);
    
    ENUM_MOVE_DIRECTION oldDirection = *pCurrentDirection;
    
    if ((int)currentTilePos.x == (int)destinationTilePos.x && (int)currentTilePos.y == (int)destinationTilePos.y)
    {
        // reach end.
        return false;
    }
    
    if (currentTileDirection == TileNone)
    {
        //        tilePos = MapHelper::mapPosToTilePos(m_CurrentMapPosition, pGroundMap, true);
        tilePos = currentTilePos;
        currentTileDirection = pGroundMap->getTileDirection(tilePos);
    }
    
    int destSequence = pGroundMap->getSequenceByTile(destinationTilePos);
    int currentSequence = pGroundMap->getSequenceByTile(tilePos);
    
    if (currentSequence == 0 || currentSequence == destSequence)
    {
        return false;
    }
    
    CCPoint nextTile = pGroundMap->getTileBySequence(destSequence > currentSequence ? currentSequence + 1 : currentSequence - 1);

    int currentX = tilePos.x;
    int currentY = tilePos.y;
    
    if (currentX != nextTile.x ||
        currentY != nextTile.y)
    {
        if (currentX == nextTile.x)
        {
            *pCurrentDirection = nextTile.y > currentY ? ENUM_MOVE_DIRECTION_DOWN : ENUM_MOVE_DIRECTION_UP;
        }
        else if (currentY == nextTile.y)
        {
            *pCurrentDirection = nextTile.x > currentX ? ENUM_MOVE_DIRECTION_RIGHT: ENUM_MOVE_DIRECTION_LEFT;
        }
        
    }
    
     
    return *pCurrentDirection != oldDirection;
}

CCRect Function::getRect(cocos2d::CCNode *pNode)
{
    CCPoint pos = pNode->getPosition();
    CCSize size = pNode->getContentSize();
    CCPoint anchor = pNode->getAnchorPoint();
    
    return CCRectMake(pos.x - size.width * anchor.x, pos.y - size.height * anchor.y, size.width, size.height);

}


bool Function::hasPlayerData()
{
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    
    float gameVersion = pUserDefault->getFloatForKey("game_version");
    
    if (gameVersion == 0)
    {
        int level = pUserDefault->getIntegerForKey("player_level");
        int experience = pUserDefault->getIntegerForKey("player_exp");
        if (level == 0 || (level==1 && experience == 0))
        {
            // no real data exist
            return false;
        }
    }
    
    // otherwise we have either legacy level data or game verstion data, 
    return true;
}


void Function::scaleNodeForScreen(cocos2d::CCNode *pNode, const cocos2d::CCPoint &anchor, const cocos2d::CCPoint &position)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    float scale = winSize.height/640;
    
    pNode->setContentSize(winSize);
    pNode->ignoreAnchorPointForPosition(false);
    pNode->setAnchorPoint(anchor);
    pNode->setPosition(position);
    pNode->setScale(scale);
}

void Function::scaleNodeForScreen(cocos2d::CCNode *pNode, const cocos2d::CCPoint &anchor)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float scale = winSize.height/640;
    pNode->ignoreAnchorPointForPosition(false);
    pNode->setAnchorPoint(anchor);
    pNode->setScale(scale);
}

CCPoint Function::calculateFirePosition(CCSprite* owner, int currentQuadrant, float *xOffsets, float *yOffsets)
{
    // only use quadrant 1,2,3,6,7
    
    bool flipX = false;
    
    int quadrant = currentQuadrant;
    
    switch (quadrant) {
        case 4:
            quadrant = 2;
            flipX = true;
            break;
        case 5:
            quadrant = 1;
            flipX = true;
            break;
        case 8:
            quadrant = 6;
            flipX = true;
        default:
            break;
    }
    
    int index = quadrant - 1;
    switch (index) {
        case 5:
            index = 3;
            break;
        case 6:
            index = 4;
        default:
            break;
    }
    
    float y = owner->getContentSize().height - yOffsets[index];
    float x = xOffsets[index];
    
    if (flipX)
    {
        
        x = owner->getContentSize().width - xOffsets[index];
    }
    
    CCPoint worldPos = owner->convertToWorldSpace(ccp(x, y));
    CCPoint mapPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->convertToNodeSpace(worldPos);
    return mapPos;
    
}

string Function::getTimeString(unsigned long s,int style)
{
    unsigned long time = s;
    unsigned long date[] = {0,0,0,0};
    unsigned long dict[] = {60,60,24,30};
    int maxShowIndex = 0;
    int maxIndex = sizeof(date) / sizeof(unsigned long) - 1;
    for (int count = 0; count <= maxIndex && time != 0; ++count) {
        date[count] = time % dict[count];
        if(date[count] > 0){
            maxShowIndex = count;
        }
        time /= dict[count];
    }
    std::string timeString;
    std::vector<string> timeStyle;
    timeStyle.resize(4);
    switch (style) {
        case TIME_STYLE_WORD:
            timeStyle[0] = GameData::getText("time_style_sec");
            timeStyle[1] = GameData::getText("time_style_minu");
            timeStyle[2] = GameData::getText("time_style_hours");
            timeStyle[3] = GameData::getText("time_style_days");
            break;
        case TIME_STYLE_POINT:
            timeStyle[0] = "";
            timeStyle[1] = ":";
            timeStyle[2] = timeStyle[1];
            timeStyle[3] = timeStyle[1];
            break;
        case TIME_STYLE_MAX:{
            timeStyle[0] = "s";
            timeStyle[1] = "m";
            timeStyle[2] = "h";
            timeStyle[3] = "d";
            char buffer[12] = {0};
            sprintf(buffer,(std::string("%d") + timeStyle[maxShowIndex]).c_str(),date[maxShowIndex]);
            timeString = buffer;
        }
        default:
            return timeString;
    }

    for (int count = 0; count <= maxShowIndex; ++count) {
        char buffer[12] = {0};
        if(date[count] >= 10){
            sprintf(buffer,"%ld%s",date[count],timeStyle[count].c_str());
        }else if(date[count] < 10 && date[count] > 0){
            sprintf(buffer,"%d%ld%s",0,date[count],timeStyle[count].c_str());
        }else{
            sprintf(buffer,"%ld%ld%s",date[count],date[count],timeStyle[count].c_str());
        }
        std::string bufferString = buffer;
        timeString.insert(timeString.begin(), bufferString.begin(), bufferString.end());
    }
    return timeString;
}

int Function::getSkillTrainingTime(int id)
{
    int level = Player::getInstance()->getSkillLevel(id);
    
    const Skill* pSkill = GameData::getSkill(id);
    
    int time = pSkill->trainingBaseTime + pSkill->trainingTimePerLevel * level;
    
    return time;
}


//
// return the training coins cost
//
int Function::getSkillTrainingCost(int id)
{
    int level = Player::getInstance()->getSkillLevel(id);
    
    const Skill* pSkill = GameData::getSkill(id);
    
    return pSkill->cost * (1 + level);
}


//
//  return the gem cost for instant completion
//
int Function::getSkillInstantCompletionGemCost(int id)
{
    unsigned long timeLeft = Player::getInstance()->getTrainingSkillLackTime(id);
    
    float originalTrainingTime = getSkillTrainingTime(id);
    
    float completionRatio = (float)timeLeft / originalTrainingTime;
    
    int baseGemCost = 10 * originalTrainingTime/3600.0f;  // 10 gems per hour
    int result = baseGemCost * completionRatio;
    return result < 5 ? 5 : result;
}

