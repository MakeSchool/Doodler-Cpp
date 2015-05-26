//
//  NetworkingWrapper.cpp
//  Doodler
//
//  Created by Daniel Haaser on 5/25/15.
//
//

#include "NetworkingWrapper.h"

static NetworkingWrapper* sharedNetworkingWrapper = nullptr;

NetworkingWrapper* NetworkingWrapper::getInstance()
{
    if (! sharedNetworkingWrapper)
    {
        sharedNetworkingWrapper = new NetworkingWrapper();
    }
    
    return sharedNetworkingWrapper;
}

NetworkingWrapper()
{
    
}