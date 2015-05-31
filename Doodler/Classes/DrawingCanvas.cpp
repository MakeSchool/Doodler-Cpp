//
//  DrawingCanvas.cpp
//  Doodler
//
//  Created by Daniel Haaser on 5/19/15.
//
//

#include "DrawingCanvas.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "NetworkingWrapper.h"

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
    
    this->setupMenus();
    this->setupTouchHandling();
    
    NetworkingWrapper::getInstance()->setDelegate(this);
}

void DrawingCanvas::setupMenus()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    ui::Button* clearButton = ui::Button::create();
    clearButton->setAnchorPoint(Vec2(1.0f, 1.0f));
    clearButton->setPosition(Vec2(visibleSize.width, visibleSize.height));
    clearButton->setTouchEnabled(true);
    clearButton->loadTextures("CloseNormal.png", "CloseSelected.png");
    clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::clearPressed, this));
    this->addChild(clearButton);
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
        
        this->sendStrokeOverNetwork(lastTouchPos, touchPos, radius, Color4F(0.2f, 0.2f, 0.2f, 1.0f));
        
        lastRadius = radius;
        lastTouchPos = touchPos;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void DrawingCanvas::clearPressed(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        drawNode->clear();
    }
}

void DrawingCanvas::sendStrokeOverNetwork(Vec2 startPoint, Vec2 endPoint, float radius, Color4F color)
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value object(rapidjson::kObjectType);
    
    rapidjson::Value startPt(rapidjson::kObjectType);
    startPt.AddMember("x", startPoint.x, document.GetAllocator());
    startPt.AddMember("y", startPoint.y, document.GetAllocator());
    
    rapidjson::Value endPt(rapidjson::kObjectType);
    endPt.AddMember("x", endPoint.x, document.GetAllocator());
    endPt.AddMember("y", endPoint.y, document.GetAllocator());
    
    rapidjson::Value lineColor(rapidjson::kObjectType);
    lineColor.AddMember("r", color.r, document.GetAllocator());
    lineColor.AddMember("g", color.g, document.GetAllocator());
    lineColor.AddMember("b", color.b, document.GetAllocator());
    lineColor.AddMember("a", color.a, document.GetAllocator());
    
    document.AddMember("startPoint", startPt, document.GetAllocator());
    document.AddMember("endPoint", endPt, document.GetAllocator());
    document.AddMember("radius", radius, document.GetAllocator());
    document.AddMember("color", lineColor, document.GetAllocator());
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    
    NetworkingWrapper::getInstance()->sendData(buffer.GetString(), buffer.Size());
    
//    CCLOG("%s", buffer.GetString());
}

void DrawingCanvas::receivedData(const void* data, unsigned long length)
{
    rapidjson::Document document;
    const char* cstr = reinterpret_cast<const char*>(data);
    document.Parse<0>(cstr);
    
    rapidjson::Value& startPoint = document["startPoint"];
    rapidjson::Value& endPoint = document["endPoint"];
    rapidjson::Value& color = document["color"];
    
    Vec2 startVec = Vec2(startPoint["x"].GetDouble(), startPoint["y"].GetDouble());
    Vec2 endVec = Vec2(endPoint["x"].GetDouble(), endPoint["y"].GetDouble());
    float radius = document["radius"].GetDouble();
    
    drawNode->drawSegment(startVec, endVec, radius, Color4F(0.0f, 0.0f, 1.0f, 1.0f));
}

void DrawingCanvas::stateChanged(ConnectionState state)
{
    
}