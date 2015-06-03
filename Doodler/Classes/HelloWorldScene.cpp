#include "HelloWorldScene.h"
#include "Lobby.h"
#include "DrawingCanvas.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//    Lobby* lobby = Lobby::create();
    DrawingCanvas* lobby = DrawingCanvas::create();
    
    lobby->setAnchorPoint(Vec2(0.0f, 0.0f));
    lobby->setPosition(Vec2(0.0f, 0.0f));
    
    this->addChild(lobby);
    
    return true;
}

