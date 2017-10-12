//
//  StageOpeningAnimation.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/26/12.
//
//

#include "StageOpeningAnimation.h"
#include "GameScene.h"
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "GameData.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool StageOpeningAnimation::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    background = NULL;
    m_IsOpening = true;

    textLabels.resize(10);
    
    return true;
}

void StageOpeningAnimation::close(float dummyParameter = 0.0f)
{
    CCDirector::sharedDirector()->popScene();

//    GameScene::getInstance()->setupStage()
//    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, GameScene::getInstance()->sharedScene));
    CCScene* currentScene = CCDirector::sharedDirector()->getRunningScene();

    if (currentScene != GameScene::getInstance()->sharedScene)
    {
        // when starting new game, the root scene is the start menu, needs to be replaced
        CCDirector::sharedDirector()->replaceScene(GameScene::getInstance()->sharedScene);
    }
}

SEL_MenuHandler StageOpeningAnimation::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
    
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBackClicked", TestHeaderLayer::onBackClicked);
    
    return NULL;
}

SEL_CCControlHandler StageOpeningAnimation::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    
    return NULL;
}

bool StageOpeningAnimation::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char  *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "durationLabel", CCLabelTTF *, this->durationLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "background", CCLayer *, this->background);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "stageIdLabel", CCLabelTTF *, this->stageIdLabel);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "textLabel1", CCLabelTTF *, this->textLabels[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "textLabel2", CCLabelTTF *, this->textLabels[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "textLabel3", CCLabelTTF *, this->textLabels[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "textLabel4", CCLabelTTF *, this->textLabels[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "textLabel5", CCLabelTTF *, this->textLabels[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "textLabel6", CCLabelTTF *, this->textLabels[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "textLabel7", CCLabelTTF *, this->textLabels[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "textLabel8", CCLabelTTF *, this->textLabels[7]);

    return false;
}

void StageOpeningAnimation::onNodeLoaded(cocos2d::CCNode *pNode, cocos2d::extension::CCNodeLoader *pNodeLoader)
{
    const char* pData = durationLabel->getString();
    
    float duration = atof(pData);
    
    scheduleOnce(schedule_selector(StageOpeningAnimation::close), duration);

    if (stageIdLabel)
    {
        if (strcmp(stageIdLabel->getString(), "1") == 0)
        {
            scheduleOnce(schedule_selector(StageOpeningAnimation::playStageSound), 7.8f);
        }
        
        char buffer[128] = {0};
        
        for(size_t i = 0; i < textLabels.size(); ++i)
        {
            CCLabelTTF* label = textLabels[i];
            if (!label)
            {
                continue;
            }
            
            sprintf(buffer, "stage_%s_text_%d", stageIdLabel->getString(), i+1);
            label->setString(GameData::getText(buffer));
        }

    }
    
}

void StageOpeningAnimation::playStageSound(float dummyParameter = 0.0f)
{
    unschedule(schedule_selector(StageOpeningAnimation::playStageSound));
    // FIXME
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/building_5_bullet_explosion.mp3");
}

void StageOpeningAnimation::playStageBackgroundMusic()
{
    
}

