//
//  StageOpeningAnimation.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/26/12.
//
//

#ifndef __ike_tdgame__StageOpeningAnimation__
#define __ike_tdgame__StageOpeningAnimation__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

class StageOpeningAnimation
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public CCBMemberVariableAssigner,
  public CCNodeLoaderListener

{
public:
    virtual bool init();
    
    void close(float /* dummy parameter to get compile pass. */);
    
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(StageOpeningAnimation, create);

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

    void setOpening(bool value)
    {
        m_IsOpening = value;
    }
    
//    void onBackClicked(cocos2d::CCObject * pSender);
    
    cocos2d::CCLabelTTF* durationLabel;
    CCLabelTTF* stageIdLabel;
    
    CCLayer* background;
    
    vector<CCLabelTTF*> textLabels;
    
    
private:
    
    void playStageBackgroundMusic();
    void playStageSound(float /* dummy paramter to get compile pass. */);
    
    bool m_IsOpening;
};

#endif /* defined(__ike_tdgame__StageOpeningAnimation__) */
