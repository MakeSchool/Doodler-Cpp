//
//  NetworkingWrapper.cpp
//  Doodler
//
//  Created by Daniel Haaser on 5/25/15.
//
//

#include "NetworkingWrapper.h"
#include "NetworkManager.h"

#pragma mark -
#pragma mark Lifecycle

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

void NetworkingWrapper::sendData(const void *data, unsigned long length)
{
    NSData* dataToSend = [NSData dataWithBytes:data length:length];
    [this->networkManager sendData:dataToSend];
}

void NetworkingWrapper::disconnect()
{
    [this->networkManager disconnect];
}

#pragma mark -
#pragma mark NetworkManager Delegate Methods

void NetworkingWrapper::receivedData(const void *data, unsigned long length)
{
    if (this->delegate)
    {
        this->delegate->receivedData(data, length);
    }
}

void NetworkingWrapper::stateChanged(ConnectionState state)
{
    if (this->delegate)
    {
        this->delegate->stateChanged(state);
    }
}