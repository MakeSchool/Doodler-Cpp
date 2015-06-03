//
//  Lobby.h
//  Doodler
//
//  Created by Daniel Haaser on 5/22/15.
//
//

#ifndef __Doodler__Lobby__
#define __Doodler__Lobby__

#include "cocos2d.h"
#include "CocosGUI.h"
#include "NetworkingWrapper.h"

class Lobby : public cocos2d::Node, NetworkingDelegate
{
public:
    CREATE_FUNC(Lobby);
    
    bool init() override;
    
    void onEnter() override;
    
    void setupUI();
    
private:
    cocos2d::ui::Text* connectionLabel;
    
    void loadDrawingScene();
    
    // NetworkingDelegate methods
    void receivedData(const void* data, unsigned long length);
    void stateChanged(ConnectionState state);
    
    // UI Methods
    void findMatchPressed(Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
};

#endif /* defined(__Doodler__Lobby__) */
