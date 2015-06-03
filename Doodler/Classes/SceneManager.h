//
//  SceneManager.h
//  Doodler
//
//  Created by Daniel Haaser on 6/2/15.
//
//

#ifndef __Doodler__SceneManager__
#define __Doodler__SceneManager__

#include "NetworkingWrapper.h"

class DrawingCanvas;

class SceneManager : public NetworkingDelegate
{
public:
    static SceneManager* getInstance();

    void enterSoloGame();
    
    void connectAndEnterNetworkedGame();
    
    void returnToLobby();
    
    void sendData(const void* data, unsigned long length);
    
private:
    NetworkingWrapper* networkingWrapper;
    DrawingCanvas* drawingCanvas;
    
    SceneManager();
    ~SceneManager();
    
    void loadDrawingScene(bool networked);
    
    // Networking Delegate methods
    void receivedData(const void* data, unsigned long length);
    void stateChanged(ConnectionState state);
};

#endif /* defined(__Doodler__SceneManager__) */
