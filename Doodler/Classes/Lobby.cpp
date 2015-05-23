//
//  Lobby.cpp
//  Doodler
//
//  Created by Daniel Haaser on 5/22/15.
//
//

#include "Lobby.h"

using namespace cocos2d;

bool Lobby::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    return true;
}

void Lobby::onEnter()
{
    Node::onEnter();
    
    this->setupMenu();
}

void Lobby::setupMenu()
{
     Size visibleSize = Director::getInstance()->getVisibleSize();
    
    ui::Button* findMatchButton = ui::Button::create();
    findMatchButton->setAnchorPoint(Vec2(1.0f, 1.0f));
    findMatchButton->setPosition(Vec2(visibleSize.width, visibleSize.height));
    findMatchButton->setTouchEnabled(true);
    findMatchButton->loadTextures("CloseNormal.png", "CloseSelected.png");
    findMatchButton->addTouchEventListener(CC_CALLBACK_2(Lobby::findMatchPressed, this));
    
}

void Lobby::findMatchPressed(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    
}