//
//  SkillTouchAnimation.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 1/19/13.
//
//

#include "SkillTouchAnimation.h"
#include "Player.h"
#include "GameScene.h"
#include "GameStage.h"
#include "PerformEffectAnimation.h"
#include "MapHelper.h"

bool SkillTouchAnimation::init()
{
    skillStartPosition = CCPointZero;
    skillCurrentPosition = CCPointZero;
    
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture);
    setShaderProgram(program);
    mSpeed = 100;
    mYaw = 0;
    mPitch = 90;
    mRoll = 0;
    
    m_pPerformingSkillAnimation = NULL;
    
//    mTexture = CCTextureCache::sharedTextureCache()->addImage("UI/ball.jpg");
    this->scheduleUpdate();
    
    return true;
}


void SkillTouchAnimation::touchBegan(const cocos2d::CCPoint &touchPos)
{
    GameStage* pStage =  GameScene::getInstance()->sharedGameStage;

    int performingSkillId = Player::getInstance()->getPerformingSkillId();
    
    if (performingSkillId == 0)
    {
        return;
    }
    
    
    switch (performingSkillId)
    {
        case 2:
            {
                if (Player::getInstance()->getMp() >= 10)
                {
                    Player::getInstance()->updateMp(-10);
                    
                    CCPoint groundMapPos = pStage->getGroundMap()->convertToNodeSpace(touchPos);
                    PerformEffectAnimation* pEffect = PerformEffectAnimation::create();
                    pEffect->perform(performingSkillId, groundMapPos);
                }
            }
            break;
        case 4:
            skillStartPosition = touchPos;
            skillCurrentPosition = touchPos;
            break;
        default:
            {
                CCPoint groundMapPos = pStage->getGroundMap()->convertToNodeSpace(touchPos);
                PerformEffectAnimation* pEffect = PerformEffectAnimation::create();
                pEffect->perform(performingSkillId, groundMapPos);
                Player::getInstance()->stopPerformingSkill();
            }
            break;
            
    }

}


void SkillTouchAnimation::touchMoved(const cocos2d::CCPoint &touchPos)
{
    
    int performingSkillId = Player::getInstance()->getPerformingSkillId();
    
    if (performingSkillId > 0)
    {
        GameStage* pStage =  GameScene::getInstance()->sharedGameStage;

        switch (performingSkillId)
        {
            case 3:
                {
//                    CCPoint groundMapPos = pStage->getGroundMap()->convertToNodeSpace(touchPos);
//                    m_pPerformingSkillAnimation->setPosition(groundMapPos);
                }
                break;
            case 4:
                skillCurrentPosition = touchPos;
                break;
            default:
                break;
        }
        
    }

}

void SkillTouchAnimation::touchEnded(const cocos2d::CCPoint &touchPos)
{
    int performingSkillId = Player::getInstance()->getPerformingSkillId();
    
    if (performingSkillId > 0)
    {
//        GameStage* pStage =  GameScene::getInstance()->sharedGameStage;

        switch (performingSkillId)
        {
            case 2:
                break;
            case 3:
//                m_pPerformingSkillAnimation->removeFromParentAndCleanup(true);
//                Player::getInstance()->stopPerformingSkill();
                break;
            case 4:
            {
                if (Player::getInstance()->getMp() >= 30)
                {
                    Player::getInstance()->updateMp(-30);

                    for (const CCPoint& tile : getTilePosFromLine(skillStartPosition, skillCurrentPosition))
                    {
                        CCPoint groundMapPos = MapHelper::tilePosToGroundMapPos(tile);
                        PerformEffectAnimation* pEffect = PerformEffectAnimation::create();
                        pEffect->perform(performingSkillId, groundMapPos);
                    }
                    
                }

                skillCurrentPosition = CCPointZero;
                skillStartPosition = CCPointZero;
                Player::getInstance()->stopPerformingSkill();

            }
                break;
                
            default:
                skillCurrentPosition = CCPointZero;
                skillStartPosition = CCPointZero;
                Player::getInstance()->stopPerformingSkill();
                break;
        }
        
    }
    
}


void SkillTouchAnimation::update(float dt)
{
    int skillId = Player::getInstance()->getPerformingSkillId();
    
    switch (skillId) {
        case 2:
            skillElapsedTime += dt;
            break;
        case 3:
            {
            if (dt > 0.0167f)
                dt = 0.0167f;
            
            mYaw += dt*mSpeed;
            mPitch += dt*mSpeed;
            mRoll += dt*mSpeed;
            
            }
            break;
    }
}

void SkillTouchAnimation::draw()
{
    int skillId = Player::getInstance()->getPerformingSkillId();
    
    if (skillId == 0)
    {
        return;
    }
    
    
    switch (skillId) {
        case 2:
            drawTimerBar();
            break;
        case 3:
//            drawSphere();
            break;
        case 4:
            drawLineCrossTiles();
            break;
        default:
            break;
    }

}

void SkillTouchAnimation::drawSphere()
{
//    
//    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords );
//    
//	getShaderProgram()->use();
//    
//
//	CCSize size = CCDirector::sharedDirector()->getWinSize();
//    
//	float scale = size.width/480.0f;
//	
//    kmMat4 matrixP;
//    kmMat4 matrixMV;
//    kmMat4 matrixMVP;
//    
//    kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
//    kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
//    
//	kmQuaternion quat;
//	kmQuaternionRotationYawPitchRoll(&quat, mYaw, mPitch, mRoll);
//    
//	kmMat3 rotation;
//	kmMat3RotationQuaternion(&rotation, &quat);
//    
//	kmVec3 translation;
//	kmVec3Fill(&translation, size.width/2, size.height/2.2f, 0);
//    
//	kmMat4 rotationAndMove;
//	kmMat4RotationTranslation(&rotationAndMove, &rotation, &translation);
//    
//    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
//	kmMat4Multiply(&matrixMVP, &matrixMVP, &rotationAndMove);
//    
//    GLuint matrixId = glGetUniformLocation(getShaderProgram()->getProgram(), "u_MVPMatrix");
//    getShaderProgram()->setUniformLocationwithMatrix4fv(matrixId, matrixMVP.mat, 1);
//
//    
//    ccGLBindTexture2D( mTexture->getName() );
//
////    ccDrawLine(ccp(100,100), ccp(400,400));
//
//	ccVertex3F vertices[4];
//	ccVertex2F uv[4];
//
//    GLfloat points[3*10000];
//	ccVertex3F tex_uv[4];
//    
//    tex_uv[0] = vertex3(0, 0, 0);
//	tex_uv[1] = vertex3(1, 0, 0);
//	tex_uv[2] = vertex3(1, 1, 0);
//	tex_uv[3] = vertex3(0, 1, 0);
//
//    
////	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);
////    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, uv);
//    
//	float x = 0;
//	float y = 0;
//	float len = 40*scale;
//
//    const float PI=3.14159;
///*
//    int lats = 10;
//    int longs = 10;
//    int i, j;
//    for(i = 0; i <= lats; i++)
//    {
//        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
//        double z0  = sin(lat0);
//        double zr0 =  cos(lat0);
//        
//        double lat1 = M_PI * (-0.5 + (double) i / lats);
//        double z1 = sin(lat1);
//        double zr1 = cos(lat1);
//        
//        for(j = 0; j <= longs; j++) {
//            double lng = 2 * M_PI * (double) (j - 1) / longs;
//            double x = cos(lng);
//            double y = sin(lng);
//            
//            glNormal3f(x * zr0, y * zr0, z0);
//            glVertex3f(x * zr0, y * zr0, z0);
//            glNormal3f(x * zr1, y * zr1, z1);
//            glVertex3f(x * zr1, y * zr1, z1);
//        }
//    }
//*/
//    int count = 0;
//    for(float i=0;i<2*PI;i+=2*PI/100)
//    {
//        for(float j=0;j<PI;j+=PI/100)
//        {
//            float x = len * sin(i) * cos(j);
//            float y = len * sin(i) * sin(j);
//            float z = len * cos(i);
//            
//            points[count] = x;
//            points[count+1] = y;
//            points[count+2] = z;
//            
//            count+=3;
//        }
//        
//    }
//    
//    glVertexAttribPointer(kCCVertexAttrib_Position,3, GL_FLOAT, GL_FALSE, 0, points);
//    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 3, GL_FLOAT, GL_FALSE, 0, tex_uv);
//    
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 10000);
//    
//    
//    return;
//    
////    float z = getVertexZ();
//	/////////// front
//	vertices[0] = vertex3(x-len,y-len,len);
//	vertices[1] = vertex3(x-len,y+len,len);
//	vertices[2] = vertex3(x+len,y-len,len);
//	vertices[3] = vertex3(x+len,y+len,len);
//    
//	uv[0] = vertex2(0, 1);
//	uv[1] = vertex2(0, 0);
//	uv[2] = vertex2(1, 1);
//	uv[3] = vertex2(1, 0);
//    
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    
//	////////// right
//	vertices[0] = vertex3(x+len,y-len,len);
//	vertices[1] = vertex3(x+len,y+len,len);
//	vertices[2] = vertex3(x+len,y-len,-len);
//	vertices[3] = vertex3(x+len,y+len,-len);
//    
//	uv[0] = vertex2(0, 1);
//	uv[1] = vertex2(0, 0);
//	uv[2] = vertex2(1, 1);
//	uv[3] = vertex2(1, 0);
//    
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    
//	///////// back
//	vertices[0] = vertex3(x+len,y-len,-len);
//	vertices[1] = vertex3(x+len,y+len,-len);
//	vertices[2] = vertex3(x-len,y-len,-len);
//	vertices[3] = vertex3(x-len,y+len,-len);
//    
//	uv[0] = vertex2(0, 1);
//	uv[1] = vertex2(0, 0);
//	uv[2] = vertex2(1, 1);
//	uv[3] = vertex2(1, 0);
//    
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    
//	////////// left
//	vertices[0] = vertex3(x-len,y-len,len);
//	vertices[1] = vertex3(x-len,y+len,len);
//	vertices[2] = vertex3(x-len,y-len,-len);
//	vertices[3] = vertex3(x-len,y+len,-len);
//    
//	uv[0] = vertex2(0, 1);
//	uv[1] = vertex2(0, 0);
//	uv[2] = vertex2(1, 1);
//	uv[3] = vertex2(1, 0);
//    
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	
//    
//	///////// top
//	vertices[0] = vertex3(x+len,y+len,len);
//	vertices[1] = vertex3(x-len,y+len,len);
//	vertices[2] = vertex3(x+len,y+len,-len);
//	vertices[3] = vertex3(x-len,y+len,-len);
//    
//	uv[0] = vertex2(0, 0);
//	uv[1] = vertex2(1, 0);
//	uv[2] = vertex2(0, 1);
//	uv[3] = vertex2(1, 1);
//    
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    
//	///////// bottom
//	vertices[0] = vertex3(x+len,y-len,len);
//	vertices[1] = vertex3(x-len,y-len,len);
//	vertices[2] = vertex3(x+len,y-len,-len);
//	vertices[3] = vertex3(x-len,y-len,-len);
//    
//	uv[0] = vertex2(0, 0);
//	uv[1] = vertex2(1, 0);
//	uv[2] = vertex2(0, 1);
//	uv[3] = vertex2(1, 1);
//    
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    
}

void SkillTouchAnimation::drawLineCrossTiles()
{
    glLineWidth(3.0f);
    if (skillCurrentPosition.x != skillStartPosition.x ||
        skillCurrentPosition.y != skillStartPosition.y)
    {
        //            glLineWidth(20.0f);
        //            ccDrawLine(skillPerformStartPosition, skillPerformCurrentPosition);
        for (const CCPoint& tile : getTilePosFromLine(skillStartPosition, skillCurrentPosition))
        {
            CCPoint pos[4];
            pos[0] = MapHelper::tilePosToStagePos(tile);
            pos[1] = MapHelper::tilePosToStagePos(ccp(tile.x + 1, tile.y));
            pos[2] = MapHelper::tilePosToStagePos(ccp(tile.x + 1, tile.y + 1));
            pos[3] = MapHelper::tilePosToStagePos(ccp(tile.x, tile.y + 1));
            ccDrawSolidPoly(pos, 4, ccc4f(1,0,0,0.5));
        }
        
    }
    glLineWidth(1.0f);

}


void SkillTouchAnimation::drawTimerBar()
{
    if (skillElapsedTime > 3)
    {
        skillElapsedTime = 0;
        Player::getInstance()->stopPerformingSkill();
        return;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint startPos = ccp(winSize.width/2 - 100, winSize.height/2);
    glLineWidth( 5.0f );
    
    ccDrawLine(startPos, ccp(startPos.x + 200 * (3-skillElapsedTime) / 3, startPos.y));
    
    // restore line width
    glLineWidth(1.0f);

}

vector<CCPoint> SkillTouchAnimation::getTilePosFromLine(const cocos2d::CCPoint &startPos, const cocos2d::CCPoint &endPos)
{
    CCTMXTiledMap* pGroundMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    vector<CCPoint> result;
    CCPoint groundMapStartPos = pGroundMap->convertToNodeSpace(startPos);
    CCPoint groundMapEndPos = pGroundMap->convertToNodeSpace(endPos);
    
    CCPoint tileStartPos = MapHelper::groundMapPosToTilePos(groundMapStartPos, pGroundMap, true);
    CCPoint tileEndPos = MapHelper::groundMapPosToTilePos(groundMapEndPos, pGroundMap, true);
    
    CCPoint mapStartPos = MapHelper::tilePosToMapPos(tileStartPos.x, tileStartPos.y, pGroundMap);
    CCPoint mapEndPos = MapHelper::tilePosToMapPos(tileEndPos.x, tileEndPos.y, pGroundMap);
    
    // y = kx +b
    float k = (mapEndPos.y - mapStartPos.y) / (mapEndPos.x - mapStartPos.x);
    float b = mapEndPos.y - k * mapEndPos.x;
    
    CCPoint start = tileStartPos.x < tileEndPos.x ? tileStartPos : tileEndPos;
    CCPoint end = tileStartPos.x < tileEndPos.x ? tileEndPos : tileStartPos;
    CCLog("Start from %f,%f to %f, %f", start.x, start.y, end.x, end.y);
    
    for(int x = start.x; x <= end.x; ++x)
    {
        for(int y = start.y;;)
        {
            CCPoint mapPos = MapHelper::tilePosToMapPos(x, y, pGroundMap);
            float minX = mapPos.x;
            float maxX = mapPos.x + MapHelper::MAP_TILE_LENGTH;
            float minY = mapPos.y;
            float maxY = mapPos.y + MapHelper::MAP_TILE_LENGTH;
            
            bool isInside = true;
            
            float y1 = k * minX + b;
            float y2 = k * maxX + b;
            float x1 = (minY - b)/k;
            float x2 = (maxY - b)/k;
            
            
            if ( (y1 < minY && y2 < minY) ||
                (y1 > maxY && y2 > maxY) ||
                (x1 < minX && x2 < minX) ||
                (x1 > maxX && x2 > maxX))
            {
                isInside = false;
            }
            
            if (isInside)
            {
                result.push_back(ccp(x, y));
                //                CCLog("cross tile: %d, %d", x, y);
            }
            
            if ((start.y <= end.y && y > end.y) ||
                (start.y >= end.y && y < end.y))
            {
                break;
            }
            
            y += (start.y<=end.y ? 1 : -1);
            
        }
        
    }
    
    return result;

}

void SkillTouchAnimation::UpdateMoltenBallBasedOnAccelerometer()
{
    if (!m_pPerformingSkillAnimation)
    {
        return;
    }
    
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    CCPoint pos = m_pPerformingSkillAnimation->getPosition();
    pos.x += m_Offset.x;
    pos.y += m_Offset.y;
    m_pPerformingSkillAnimation->setPosition(pos);
    
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(pos, pMap, false);
    
    // cost player MP for existing molten ball
    // accelerometer update interval is 1/25 second
    float mpCostPerSecond = 5;
    Player::getInstance()->updateMp(-mpCostPerSecond/25);

    if (Player::getInstance()->getMp() <= 0 || !pMap->isRoadTile(tilePos))
    {
        m_pPerformingSkillAnimation->stopAllActions();
        m_pPerformingSkillAnimation->getCurrentSprite()->runAction(CCFadeOut::create(1));
        m_pPerformingSkillAnimation->runAction(CCSequence::create(CCFadeOut::create(1), CCCallFunc::create(this, callfunc_selector(SkillTouchAnimation::removeAnimation)), NULL));
        Player::getInstance()->stopPerformingSkill();
    }
}

void SkillTouchAnimation::removeAnimation()
{
    if (m_pPerformingSkillAnimation)
    {
        m_pPerformingSkillAnimation->removeFromParentAndCleanup(true);
        m_pPerformingSkillAnimation = NULL;
    }
}

void SkillTouchAnimation::createMoltenBall()
{
    if (m_pPerformingSkillAnimation)
    {
        return;
    }
    
    CCPoint endTile = GameScene::getInstance()->sharedGameStage->getGroundMap()->getEndTilePosition();
    CCPoint groundMapPos = MapHelper::tilePosToGroundMapPos(endTile);
    
    m_pPerformingSkillAnimation = PerformEffectAnimation::create();
    m_pPerformingSkillAnimation->setScale(0.4f);
    m_pPerformingSkillAnimation->perform(3,  groundMapPos);

}
