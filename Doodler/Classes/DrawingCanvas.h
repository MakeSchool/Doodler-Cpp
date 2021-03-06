//
//  DrawingCanvas.h
//  Doodler
//
//  Created by Daniel Haaser on 5/19/15.
//
//

#ifndef __Doodler__DrawingCanvas__
#define __Doodler__DrawingCanvas__

#include "cocos2d.h"
#include "CocosGUI.h"
#include "NetworkingWrapper.h"

class DrawingCanvas : public cocos2d::Node
{
public:
    CREATE_FUNC(DrawingCanvas);
    
    void receivedData(const void* data, unsigned long length);
    
    void setNetworkedSession(bool networkedSession);
    
protected:
    bool networkedSession;
    cocos2d::DrawNode* drawNode;
    cocos2d::LayerColor* background;
    cocos2d::Sprite* check;
    cocos2d::Color4F selectedColor;
    
    bool init() override;
    void onEnter() override;
    void setupMenus();
    void setupTouchHandling();
    void clearPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void backPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void colorChangePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void sendStrokeOverNetwork(cocos2d::Vec2 startPoint, cocos2d::Vec2 endPoint, float radius, cocos2d::Color4F color);
};

#endif /* defined(__Doodler__DrawingCanvas__) */
