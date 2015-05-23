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

class DrawingCanvas : public cocos2d::Node
{
public:
    CREATE_FUNC(DrawingCanvas);
    
    bool init() override;
    
    void onEnter() override;
    
protected:
    cocos2d::DrawNode* drawNode;
    cocos2d::LayerColor* background;
    
    void setupMenus();
    void setupTouchHandling();
    void clear(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
};

#endif /* defined(__Doodler__DrawingCanvas__) */
