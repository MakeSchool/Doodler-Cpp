//
//  Lobby.cpp
//  Doodler
//
//  Created by Daniel Haaser on 5/22/15.
//
//

#include "Lobby.h"
#include "DrawingCanvas.h"

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
    
    NetworkingWrapper::getInstance()->setDelegate(this);
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
    
    connectionLabel = ui::Text::create("Not Connected", "Helvetica", 36.0f);
    connectionLabel->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 2.0f));
    
    this->addChild(connectionLabel);
    this->addChild(findMatchButton);    
}

void Lobby::loadDrawingScene()
{
    auto scene = Scene::create();
    
    DrawingCanvas* drawingCanvas = DrawingCanvas::create();
    
    scene->addChild(drawingCanvas);
    
    Director::getInstance()->pushScene(scene);
}

#pragma mark -
#pragma mark UI Methods

void Lobby::findMatchPressed(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        NetworkingWrapper::getInstance()->attemptToJoinGame();
    }
}

#pragma mark -
#pragma mark NetworkingWrapper Delegate

void Lobby::receivedData(const void* data, unsigned long length)
{
    
}

void Lobby::stateChanged(ConnectionState state)
{
    switch (state)
    {
        case ConnectionState::NOT_CONNECTED:
            connectionLabel->setString("Not Connected");
            break;
            
        case ConnectionState::CONNECTING:
            connectionLabel->setString("Connecting...");
            break;
            
        case ConnectionState::CONNECTED:
            connectionLabel->setString("Connected!");
            this->loadDrawingScene();
            break;
    }
}