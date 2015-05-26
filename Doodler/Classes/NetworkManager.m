//
//  Networking.m
//  Doodler
//
//  Created by Daniel Haaser on 5/25/15.
//
//

#import "NetworkManager.h"

@interface NetworkManager ()

@property (nonatomic, strong) MCSession* session;
@property (nonatomic, strong) MCNearbyServiceAdvertiser* advertiser;
@property (nonatomic, strong) MCNearbyServiceBrowser* browser;
@property (nonatomic, strong) MCPeerID* peerID;

@end

@implementation NetworkManager

- (instancetype)init
{
    if (self = [super init])
    {
        self.peerID = [[MCPeerID alloc] initWithDisplayName:[UIDevice currentDevice].name];
        self.advertiser = [[MCNearbyServiceAdvertiser alloc] initWithPeer:self.peerID discoveryInfo:nil serviceType:@"doodler-game"];
        self.browser = [[MCNearbyServiceBrowser alloc] initWithPeer:self.peerID serviceType:@"doodler-game"];
        self.session = [[MCSession alloc] initWithPeer:self.peerID securityIdentity:nil encryptionPreference:MCEncryptionNone];
        
        self.advertiser.delegate = self;
        self.browser.delegate = self;
        self.session.delegate = self;
    }
    
    return self;
}

- (void)attemptToJoinGame
{
    [self.advertiser startAdvertisingPeer];
    [self.browser startBrowsingForPeers];
}

- (void)sendData:(NSData*)data
{
    NSError* error;
    [self.session sendData:data toPeers:self.session.connectedPeers withMode:MCSessionSendDataReliable error:&error];
}

#pragma mark -
#pragma mark MCNearbyServiceAdvertiser Delegate Methods

// Incoming invitation request.  Call the invitationHandler block with YES and a valid session to connect the inviting peer to the session.
- (void)advertiser:(MCNearbyServiceAdvertiser *)advertiser didReceiveInvitationFromPeer:(MCPeerID *)peerID withContext:(NSData *)context invitationHandler:(void(^)(BOOL accept, MCSession *session))invitationHandler
{
    invitationHandler(YES, self.session);
}

#pragma mark -
#pragma mark MCNearbyServiceBrowser Delegate Methods

// Found a nearby advertising peer
- (void)browser:(MCNearbyServiceBrowser *)browser foundPeer:(MCPeerID *)peerID withDiscoveryInfo:(NSDictionary *)info
{
    // Only send invitations one way, so both peers don't try to invite each other at the same time
    if ([peerID.displayName compare:self.peerID.displayName] == NSOrderedAscending)
    {
        [browser invitePeer:peerID toSession:self.session withContext:nil timeout:10.0f];
    }
}

// A nearby peer has stopped advertising
- (void)browser:(MCNearbyServiceBrowser *)browser lostPeer:(MCPeerID *)peerID
{
    
}

#pragma mark -
#pragma mark MCSession Delegate Methods

// Remote peer changed state
- (void)session:(MCSession *)session peer:(MCPeerID *)peerID didChangeState:(MCSessionState)state
{
    
}

// Received data from remote peer
- (void)session:(MCSession *)session didReceiveData:(NSData *)data fromPeer:(MCPeerID *)peerID
{

}

// Received a byte stream from remote peer
- (void)session:(MCSession *)session didReceiveStream:(NSInputStream *)stream withName:(NSString *)streamName fromPeer:(MCPeerID *)peerID
{
    
}

// Start receiving a resource from remote peer
- (void)session:(MCSession *)session didStartReceivingResourceWithName:(NSString *)resourceName fromPeer:(MCPeerID *)peerID withProgress:(NSProgress *)progress
{
    
}

// Finished receiving a resource from remote peer and saved the content in a temporary location - the app is responsible for moving the file to a permanent location within its sandbox
- (void)session:(MCSession *)session didFinishReceivingResourceWithName:(NSString *)resourceName fromPeer:(MCPeerID *)peerID atURL:(NSURL *)localURL withError:(NSError *)error
{
    
}

@end
