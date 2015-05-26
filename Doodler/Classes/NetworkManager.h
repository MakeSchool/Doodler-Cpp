//
//  Networking.h
//  Doodler
//
//  Created by Daniel Haaser on 5/25/15.
//
//

#import <Foundation/Foundation.h>
#import <MultipeerConnectivity/MultipeerConnectivity.h>

@interface NetworkManager : NSObject <MCSessionDelegate, MCNearbyServiceAdvertiserDelegate, MCNearbyServiceBrowserDelegate>

- (void)attemptToJoinGame;

@end
