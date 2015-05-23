//
//  DrawingCanvas.cpp
//  Doodler
//
//  Created by Daniel Haaser on 5/19/15.
//
//

#include "DrawingCanvas.h"
#include "CocosGUI.h"

using namespace cocos2d;

bool DrawingCanvas::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    drawNode = DrawNode::create();
    background = LayerColor::create(Color4B(255, 255, 255, 255));
    
    this->addChild(background);
    this->addChild(drawNode);
    
    return true;
}

void DrawingCanvas::onEnter()
{
    Node::onEnter();
    
    Size visibleSize = CCDirector::getInstance()->getVisibleSize();
    
    this->setContentSize(visibleSize);
    drawNode->setContentSize(visibleSize);
    background->setContentSize(visibleSize);
    
    this->setupTouchHandling();
}

void DrawingCanvas::setupTouchHandling()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    
    static Vec2 lastTouchPos;
    static float lastRadius = 2.0f;
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
        lastRadius = 2.0f;
        return true;
    };
    
    touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
        
        // Compute velocity, use velociy to affect segment radius
        float distance = lastTouchPos.distance(touchPos);
        
        float radius = MIN(24.0f, distance);
        
        //lowpass filter
        float dt = 1.0f / 60.0f;
        float rc = 2.0f;
        float alpha = dt / (rc + dt);
        radius = (alpha * distance) + (1.0f - alpha) * lastRadius;
        
        drawNode->drawSegment(lastTouchPos, touchPos, radius, Color4F(0.2f, 0.2f, 0.2f, 1.0f));
        
        lastRadius = radius;
        lastTouchPos = touchPos;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}