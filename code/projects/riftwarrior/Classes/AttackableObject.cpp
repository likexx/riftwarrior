//
//  AttackableObject.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/17/12.
//
//

#include "AttackableObject.h"
#include "GameScene.h"
#include "MapHelper.h"
#include "AnimationManager.h"
#include "NpcAnimation.h"

int AttackableObject::getId()
{
    return 0;
}

void AttackableObject::createDestructionAnimation(CCSprite* pSprite)
{
    
    CCTexture2D* texture = pSprite->getTexture();
    CCRect rect = pSprite->getTextureRect();
    CCSize size = CCSizeMake(rect.getMaxX()- rect.getMinX(), rect.getMaxY()-rect.getMinY());
    
/*
    float scale = pSprite->getScale();
    
    // make the texture even smaller than the original sprite
    //    sprite->setScale(scale * 0.6);
    
    scale = pSprite->getScale();
    size.width *= scale;
    size.height *= scale;
*/
    CCPoint originPos = getAttackablePosition();
    
    for(int i=0;i<3;++i)
    {
        for (int j=0;j<3;++j)
        {
            int y = rect.getMinY() + i * size.height/3 - 1;
            int x = rect.getMinX() + j * size.width/3 - 1;
            
            CCSprite* clip = CCSprite::createWithTexture(texture, CCRectMake(x, y, size.width/3, size.height/3));
            clip->setPosition(originPos);
//            clip->setScale(0.85);
            clip->setScale(pSprite->getScale());
            int len = MapHelper::MAP_TILE_LENGTH;
            float degree = rand()%360;
            float r = len * ((float)(rand()%50)/50.0f + 0.5);
            
            createDestructionClipAnimation(clip, r * cos(degree), r * sin(degree));

        }
    }

    pSprite->stopAllActions();
    pSprite->setVisible(false);
}

void AttackableObject::createDestructionClipAnimation(cocos2d::CCNode *pClip, float dx, float dy)
{
    
    CCPoint originPos = pClip->getPosition();
    
    float targetX = originPos.x + dx;
    float targetY = originPos.y + dy;
    
    CCPoint targetPos = ccp(targetX, targetY);
    
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    
    pStage->getGroundMap()->addChild(pClip);
    
    
//    float duration = (float)(rand()%60)/60.0f + 0.5;
    float duration = 0.5;
    
    pClip->runAction((CCActionInterval*)CCSequence::create(CCRotateBy::create(duration, dx > 0 ? -rand()%90 : rand()%90), NULL));
//    pClip->runAction(CCSequence::create(CCDelayTime::create(duration/2), NULL));
    pClip->runAction(CCSequence::create(CCMoveTo::create(duration, targetPos),NULL));

    pClip->runAction(CCSequence::create(CCDelayTime::create(2),
                                        CCFadeOut::create(2),
                                        CCCallFuncO::create(this, callfuncO_selector(AttackableObject::removeClip), pClip),
                                        NULL));
//    pClip->runAction(CCSequence::create(CCMoveTo::create(duration, targetPos), CCCallFuncO::create(this, callfuncO_selector(Building::removeClip), pClip), NULL));
    
}

void AttackableObject::removeClip(cocos2d::CCNode *pClip)
{
    if (pClip->numberOfRunningActions() > 0)
    {
        pClip->stopAllActions();
    }
    
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    pStage->getGroundMap()->removeChild(pClip, true);
}


void AttackableObject::setStatus(ENUM_BUFF_STATUS status, float value, float duration)
{
    
    if (m_StatusEffect.find(status) != m_StatusEffect.end() &&
        m_StatusEffect[status] >= value)
    {
        return;
    }
    
    m_StatusEffect[status] = value;
    m_StatusDuration[status] = 0;
    m_StatusMaxDuration[status] = duration;
    
    if (status == ENUM_BUFF_STATUS_DIZZY)
    {
        CCSmartSprite* dizzyAnim = AnimationManager::getInstance()->createStatusAnimation(status, duration);
        CCNode* pAnimPart = getAnimationPart();
        pAnimPart->addChild(dizzyAnim, 0, DIZZY_ANIMATION_TAG);
    }
    else if (status == ENUM_BUFF_STATUS_SPIDER_WEB)
    {
        CCSmartSprite* web = AnimationManager::getInstance()->createStatusAnimation(ENUM_BUFF_STATUS_SPIDER_WEB, 5);
        CCNode* pNode = getAnimationPart();
        web->setPosition(ccp(pNode->getContentSize().width/2, 0));
        web->setAnchorPoint(ccp(0.5, 0.1));
        pNode->addChild(web, 0, SPIDERWEB_ANIMATION_TAG);
        
        CCLabelBMFont* tip = CCLabelBMFont::create(GameData::getText("action_tap"), "font/ingame_hint.fnt");
        tip->setPosition(ccp(web->getContentSize().width/2, web->getContentSize().height/2));
        web->addChild(tip);
        
        tip->runAction(CCRepeatForever::create((CCActionInterval*)(CCSequence::create(CCScaleTo::create(0.3f, 0.5f),
                                                                                      CCScaleTo::create(0.3f, 1.0f),
                                                                                      NULL))));
        
        m_TapCount = 3;
    }

}

void AttackableObject::updateStatusByTap()
{
    if (hasStatus(ENUM_BUFF_STATUS_SPIDER_WEB))
    {
        m_TapCount--;
    }
    
    if (m_TapCount <= 0)
    {
        removeStatus(ENUM_BUFF_STATUS_SPIDER_WEB);
    }
}

bool AttackableObject::hasStatus(ENUM_BUFF_STATUS status)
{
    map<ENUM_BUFF_STATUS, float>::iterator statusIt = m_StatusEffect.find(status);
    
    if (statusIt != m_StatusEffect.end())
    {
        return statusIt->second > 0.0f;
    }
    
    return false;
    
}

void AttackableObject::resetStatus()
{
    updateStatus(10000);
    
}

void AttackableObject::removeStatus(ENUM_BUFF_STATUS status)
{
    map<ENUM_BUFF_STATUS, float>::iterator statusIt = m_StatusEffect.find(status);

    if (statusIt != m_StatusEffect.end())
    {
        m_StatusEffect[status] = 0;
        m_StatusDuration[status] = 0;
        m_StatusMaxDuration[status] = 0;
        getAnimationPart()->removeChildByTag(SPIDERWEB_ANIMATION_TAG, true);
    }
}

void AttackableObject::updateStatus(float dt)
{
    
    map<ENUM_BUFF_STATUS, float>::iterator statusIt = m_StatusEffect.begin();
    
    // check for effective status
    while (statusIt != m_StatusEffect.end())
    {
        float value = statusIt->second;
        ENUM_BUFF_STATUS status = statusIt->first;
        
        m_StatusDuration[status] += dt;
        
        if (value != 0)
        {
            if (m_StatusDuration[status] >= m_StatusMaxDuration[status])
            {
                m_StatusEffect[status] = 0;
                m_StatusDuration[status] = 0;
                m_StatusMaxDuration[status] = 0;
                
                // restore status
                switch (status)
                {
                    case ENUM_BUFF_STATUS_SLOWDOWN:
                    {
                        setSpeed(getBaseSpeed());
                        break;
                    }
                    case ENUM_BUFF_STATUS_POISON:
                    {
                        NpcAnimation* pAnimation = static_cast<NpcAnimation*>(getAnimationPart());
                        if (pAnimation->getCurrentSprite())
                        {
                            pAnimation->getCurrentSprite()->setColor(ccc3(255,255,255));
                        }
                        setSpeed(getBaseSpeed());
                        break;
                    }
                    case ENUM_BUFF_STATUS_DIZZY:
                    case ENUM_BUFF_STATUS_NORMAL:
                    case ENUM_BUFF_STATUS_SPIDER_WEB:
                    {
                        // NOT handled yet.
//                        assert(false);
                        break;
                    }
                }
            }
            else
            {
                // dot (damage over time)
                switch (status)
                {
                    case ENUM_BUFF_STATUS_SLOWDOWN:
                        setSpeed(getBaseSpeed() * (1-value));
                        break;
                    case ENUM_BUFF_STATUS_POISON:
                        setSpeed(getBaseSpeed() * 0.5);
                        reduceHp(value * dt, true);
                        

                        NpcAnimation* pAnimation = static_cast<NpcAnimation*>(getAnimationPart());
                        if (pAnimation->getCurrentSprite())
                        {
                            pAnimation->getCurrentSprite()->setColor(ccc3(0,255,0));                            
                        }

                        break;
                }
            }
            
        }
        
        statusIt++;
    }
    
    
}