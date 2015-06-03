//
//  Lobby.cpp
//  Doodler
//
//  Created by Daniel Haaser on 5/22/15.
//
//

#include "Lobby.h"
#include "DrawingCanvas.h"
#include "Constants.h"
#include "SceneManager.h"

using namespace cocos2d;

#pragma mark -
#pragma mark Lifecycle

bool Lobby::init()
{
    if (! Node::init())
    {
        return false;
    }
    
    LayerColor* background = LayerColor::create(Color4B(COLOR_WHITE));
    this->addChild(background);
    
    return true;
}

void Lobby::onEnter()
{
    Node::onEnter();
    
    this->setupUI();
}

void Lobby::setupUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    ui::Button* soloButton = ui::Button::create();
    soloButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    soloButton->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height * 0.4f));
    soloButton->loadTextures("soloButton.png", "soloButtonPressed.png");
    soloButton->addTouchEventListener(CC_CALLBACK_2(Lobby::soloPressed, this));
    this->addChild(soloButton);
    
    ui::Button* duoButton = ui::Button::create();
    duoButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    duoButton->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height * 0.25f));
    duoButton->loadTextures("duoButton.png", "duoButtonPressed.png");
    duoButton->addTouchEventListener(CC_CALLBACK_2(Lobby::duoPressed, this));
    this->addChild(duoButton);
    
    Sprite* logo = Sprite::create("doodlerLogo.png");
    logo->setAnchorPoint(Vec2(0.5f, 0.5f));
    logo->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height * 0.75f));
    this->addChild(logo);
    
    connectionLabel = ui::Text::create("Not Connected", "Helvetica", 36.0f);
    connectionLabel->setColor(Color3B(COLOR_BLACK));
    connectionLabel->setPosition(Vec2(visibleSize.width / 2.0f, visibleSize.height / 10.0f));
    
    this->addChild(connectionLabel);
}

#pragma mark -
#pragma mark UI Methods

void Lobby::soloPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->enterSoloGame();
    }
}

void Lobby::duoPressed(Ref* pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->connectAndEnterNetworkedGame();
    }
}