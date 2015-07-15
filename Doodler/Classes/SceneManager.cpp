//
//  SceneManager.cpp
//  Doodler
//
//  Created by Daniel Haaser on 6/2/15.
//
//

#include "SceneManager.h"
#include "DrawingCanvas.h"
#include "cocos2d.h"

using namespace cocos2d;

#pragma mark -
#pragma mark Lifecycle

static SceneManager* sharedSceneManager = nullptr;

SceneManager* SceneManager::getInstance()
{
    if (! sharedSceneManager)
    {
        sharedSceneManager = new SceneManager();
    }
    
    return sharedSceneManager;
}

SceneManager::SceneManager()
{
    drawingCanvas = nullptr;
    networkingWrapper = new NetworkingWrapper();
    networkingWrapper->setDelegate(this);
}

SceneManager::~SceneManager()
{
    delete networkingWrapper;
}

#pragma mark -
#pragma mark Public Methods

void SceneManager::enterSoloGame()
{
    this->loadDrawingScene(false);
}

void SceneManager::connectAndEnterNetworkedGame()
{
    networkingWrapper->attemptToJoinGame();
}

void SceneManager::returnToLobby()
{
    if (drawingCanvas)
    {
        Director::getInstance()->popScene();
        drawingCanvas = nullptr;
        networkingWrapper->disconnect();
    }
}

void SceneManager::sendData(const void *data, unsigned long length)
{
    networkingWrapper->sendData(data, length);
}

#pragma mark -
#pragma mark Private Methods

void SceneManager::loadDrawingScene(bool networked)
{
    auto scene = Scene::create();
    
    drawingCanvas = DrawingCanvas::create();
    drawingCanvas->setNetworkedSession(networked);
    
    scene->addChild(drawingCanvas);
    
    Director::getInstance()->pushScene(scene);
}

#pragma mark -
#pragma mark NetworkingWrapper Delegate Methods

void SceneManager::receivedData(const void *data, unsigned long length)
{
    if (drawingCanvas)
    {
        drawingCanvas->receivedData(data, length);
    }
}

void SceneManager::stateChanged(ConnectionState state)
{
    switch (state)
    {
        case ConnectionState::NOT_CONNECTED:
            CCLOG("Not Connected");
            break;
            
        case ConnectionState::CONNECTING:
            CCLOG("Connecting...");
            break;
            
        case ConnectionState::CONNECTED:
            CCLOG("Connected!");
            if (!drawingCanvas)
            {
                this->loadDrawingScene(true);
            }
            break;
    }
}