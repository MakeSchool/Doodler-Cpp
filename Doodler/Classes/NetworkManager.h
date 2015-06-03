//
//  NetworkManager.h
//  Doodler
//
//  Created by Daniel Haaser on 5/25/15.
//
//

#import <Foundation/Foundation.h>
#import <MultipeerConnectivity/MultipeerConnectivity.h>

class NetworkManagerDelegate;

@interface NetworkManager : NSObject <MCSessionDelegate, MCNearbyServiceAdvertiserDelegate, MCNearbyServiceBrowserDelegate>

- (instancetype)init;

- (void)setDelegate:(NetworkManagerDelegate*)delegate;

- (void)attemptToJoinGame;

- (void)sendData:(NSData*)data;

- (void)disconnect;

@end
