//
//  NetworkManagerDelegate.h
//  Doodler
//
//  Created by Daniel Haaser on 5/26/15.
//
//

#ifndef __Doodler__NetworkManagerDelegate__
#define __Doodler__NetworkManagerDelegate__

enum class ConnectionState
{
    NOT_CONNECTED,
    CONNECTING,
    CONNECTED
};

class NetworkManagerDelegate
{
public:
    virtual void receivedData(const void* data, unsigned long length) = 0;
    virtual void stateChanged(ConnectionState state) = 0;
};

#endif /* defined(__Doodler__NetworkManagerDelegate__) */
