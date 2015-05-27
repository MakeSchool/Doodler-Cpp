//
//  NetworkingWrapper.cpp
//  Doodler
//
//  Created by Daniel Haaser on 5/25/15.
//
//

#include "NetworkingWrapper.h"
#include "NetworkManager.h"

static NetworkingWrapper* sharedNetworkingWrapper = nullptr;

#pragma mark -
#pragma mark Lifecycle

NetworkingWrapper* NetworkingWrapper::getInstance()
{
    if (! sharedNetworkingWrapper)
    {
        sharedNetworkingWrapper = new NetworkingWrapper();
    }
    
    return sharedNetworkingWrapper;
}

NetworkingWrapper::NetworkingWrapper()
{
    this->networkManager = [[NetworkManager alloc] init];
    [this->networkManager setDelegate:this];
    [networkManager retain];
}

NetworkingWrapper::~NetworkingWrapper()
{
    [networkManager release];
    networkManager = nil;
}

#pragma mark -
#pragma mark Public Methods

void NetworkingWrapper::setDelegate(NetworkingDelegate* delegate)
{
    this->delegate = delegate;
}

void NetworkingWrapper::attemptToJoinGame()
{
    [this->networkManager attemptToJoinGame];
}

#pragma mark -
#pragma mark NetworkManager Delegate Methods

void NetworkingWrapper::receivedData(const void *data)
{
    if (this->delegate)
    {
        this->delegate->receivedData(data);
    }
}

void NetworkingWrapper::stateChanged(ConnectionState state)
{
    if (this->delegate)
    {
        this->delegate->stateChanged(state);
    }
}