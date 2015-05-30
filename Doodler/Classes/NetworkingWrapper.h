//
//  NetworkingWrapper.h
//  Doodler
//
//  Created by Daniel Haaser on 5/25/15.
//
//

#ifndef __Doodler__NetworkingWrapper__
#define __Doodler__NetworkingWrapper__

#include "NetworkManagerDelegate.h"

#ifdef __OBJC__
@class NetworkManager;
#else
typedef struct objc_object NetworkManager;
#endif

// Classes that want to be notified by networking activity should inherit from this class
// and set themselves as the delegate
class NetworkingDelegate
{
public:
    virtual void receivedData(const void* data) = 0;
    virtual void stateChanged(ConnectionState state) = 0;
};

class NetworkingWrapper : public NetworkManagerDelegate
{
public:
    static NetworkingWrapper* getInstance();
    
    void setDelegate(NetworkingDelegate* delegate);
    
    void attemptToJoinGame();
    
    void sendData(const void* data, unsigned long length);
    
private:
    NetworkManager* networkManager;
    NetworkingDelegate* delegate;
    
    NetworkingWrapper();
    ~NetworkingWrapper();
    
    void receivedData(const void* data);
    void stateChanged(ConnectionState state);
};

#endif /* defined(__Doodler__NetworkingWrapper__) */
