//
//  NpcStatusMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 4/25/13.
//
//

#include "NpcStatusMenu.h"
#include "GameScene.h"
#include "NPC.h"

NpcStatusMenu* NpcStatusMenu::create(int id)
{
    NpcStatusMenu* menu = new NpcStatusMenu();
    if (menu && menu->init(id))
    {
        menu->autorelease();
        return menu;
    }
    
    CC_SAFE_DELETE(menu);
    return NULL;
}


bool NpcStatusMenu::init(int id)
{
    CCSprite* background = CCSprite::create("UI/npc_box.png");
    background->setAnchorPoint(CCPointZero);

    char buffer[128] = {0};
    sprintf(buffer, "npc/npc%d_talk.png", id);
    
    CCSprite* headImg = CCSprite::create(buffer);
    float scaleY = background->getContentSize().height * 0.60 /headImg->getContentSize().height;
    float scaleX = background->getContentSize().width * 0.89 / headImg->getContentSize().width;
    headImg->setScaleX(scaleX);
    headImg->setScaleY(scaleY);
    
    headImg->setAnchorPoint(ccp(0,0));
    headImg->setPosition(ccp(background->getContentSize().width * 0.07, background->getContentSize().height * 0.37));
    
    background->addChild(headImg);
    
    addChild(background, -1);
    
    this->setAnchorPoint(ccp(0,0));
    
    this->setContentSize(background->getContentSize());
    
    m_NpcId = id;
    
    scheduleUpdate();
    
    return true;
}

void NpcStatusMenu::draw()
{
    NPC* npc = static_cast<NPC*>(GameScene::getInstance()->sharedGameStage->getNpc(m_NpcId));
    
    float width = getContentSize().width * 0.75;
    
    glLineWidth(5.0f);

    CCPoint start = ccp(getContentSize().width * 0.1, getContentSize().height * 0.1);
    CCPoint end = ccp(start.x + width, start.y);

    ccDrawColor4B(0, 255, 0, 255);
    end.x = start.x + width * npc->getHp()/npc->getMaxHp();
    if (end.x < start.x)
    {
        end.x = start.x;
    }
    ccDrawLine(start, end);
    
    glLineWidth(1);
}