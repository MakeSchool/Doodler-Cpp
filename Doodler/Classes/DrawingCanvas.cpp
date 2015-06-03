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
#include "Constants.h"
#include "SceneManager.h"

using namespace cocos2d;

#pragma mark -
#pragma mark Lifecycle

bool DrawingCanvas::init()
{
    if (! Node::init())
    {
        return false;
    }

    this->setNetworkedSession(false);
    
    drawNode = DrawNode::create();
    background = LayerColor::create(Color4B(COLOR_WHITE));
    selectedColor = COLOR_GREEN;
    
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
}

void DrawingCanvas::setupMenus()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    ui::Button* clearButton = ui::Button::create();
    clearButton->setAnchorPoint(Vec2(1.0f, 1.0f));
    clearButton->setPosition(Vec2(visibleSize.width, visibleSize.height));
    clearButton->loadTextures("clearButton.png", "clearButtonPressed.png");
    clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::clearPressed, this));
    this->addChild(clearButton);
    
    ui::Button* backButton = ui::Button::create();
    backButton->setAnchorPoint(Vec2(0.0f, 1.0f));
    backButton->setPosition(Vec2(0.0f, visibleSize.height));
    backButton->loadTextures("backButton.png", "backButtonPressed.png");
    backButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::backButtonPressed, this));
    this->addChild(backButton);
    
    check = Sprite::create("checkMark.png");
    check->setAnchorPoint(Vec2(0.5f, 0.5f));
    check->setNormalizedPosition(Vec2(0.5f, 0.5f));
    
    Node* colorButtonLayout = Node::create();
    colorButtonLayout->setContentSize(Size(visibleSize.width, visibleSize.height * 0.2f));
    colorButtonLayout->setAnchorPoint(Vec2(0.5f, 0.0f));
    colorButtonLayout->setPosition(Vec2(visibleSize.width / 2.0f, 0.0f));
    this->addChild(colorButtonLayout);
    
    for (int i = 1; i <= 5; ++i)
    {
        ui::Button* colorButton = ui::Button::create();
        colorButton->setAnchorPoint(Vec2(0.5f, 0.0f));
        colorButton->setPosition(Vec2(visibleSize.width * (i * 0.16666f), 0.0f));
        colorButton->loadTextures("colorSwatch.png", "colorSwatch.png");
        colorButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::colorChangePressed, this));
        
        Color4F buttonColor;
        
        switch (i)
        {
            case 1: buttonColor = COLOR_RED; break;
            case 2: buttonColor = COLOR_YELLOW; break;
            case 3: buttonColor = COLOR_GREEN; break;
            case 4: buttonColor = COLOR_BLUE; break;
            case 5: buttonColor = COLOR_PURPLE; break;
        }
        
        if (buttonColor == COLOR_GREEN)
        {
            colorButton->addChild(check);
        }
        
        colorButton->setColor(Color3B(buttonColor));
        colorButtonLayout->addChild(colorButton);
    }
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
        
        drawNode->drawSegment(lastTouchPos, touchPos, radius, selectedColor);
        
        if (this->networkedSession)
        {
            this->sendStrokeOverNetwork(lastTouchPos, touchPos, radius, selectedColor);
        }
        
        lastRadius = radius;
        lastTouchPos = touchPos;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

#pragma mark -
#pragma mark Public Methods

void DrawingCanvas::setNetworkedSession(bool networkedSession)
{
    this->networkedSession = networkedSession;
}

void DrawingCanvas::receivedData(const void* data, unsigned long length)
{
    rapidjson::Document document;
    const char* cstr = reinterpret_cast<const char*>(data);
    document.Parse<0>(cstr);
    
    rapidjson::Value& startDoc = document["startPoint"];
    rapidjson::Value& endDoc = document["endPoint"];
    rapidjson::Value& colorDoc = document["color"];
    
    Vec2 start = Vec2(startDoc["x"].GetDouble(), startDoc["y"].GetDouble());
    Vec2 end = Vec2(endDoc["x"].GetDouble(), endDoc["y"].GetDouble());
    float radius = document["radius"].GetDouble();
    
    float r = colorDoc["r"].GetDouble();
    float g = colorDoc["g"].GetDouble();
    float b = colorDoc["b"].GetDouble();
    float a = colorDoc["a"].GetDouble();
    Color4F color = Color4F(r, g, b, a);
    
    drawNode->drawSegment(start, end, radius, color);
}

#pragma mark -
#pragma mark Private Methods

void DrawingCanvas::clearPressed(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        drawNode->clear();
    }
}

void DrawingCanvas::backButtonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->returnToLobby();
    }
}

void DrawingCanvas::colorChangePressed(Ref *pSender, ::ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    
    switch (eEventType)
    {
        case ui::Widget::TouchEventType::BEGAN:
            pressedButton->setScale(0.85f);
            break;
            
        case ui::Widget::TouchEventType::MOVED:
            break;
            
        case ui::Widget::TouchEventType::CANCELED:
            pressedButton->setScale(1.0f);
            break;
            
        case ui::Widget::TouchEventType::ENDED:
            
            Color3B pressedColor = pressedButton->getColor();
            selectedColor = Color4F(pressedColor);
            
            // Reposition checkmark
            check->retain();
            check->removeFromParent();
            pressedButton->addChild(check);
            check->release();
            
            pressedButton->setScale(1.0f);
            break;
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
    
    SceneManager::getInstance()->sendData(buffer.GetString(), buffer.Size());
    
//    CCLOG("%s", buffer.GetString());
}