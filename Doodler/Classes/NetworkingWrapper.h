//
//  NetworkingWrapper.h
//  Doodler
//
//  Created by Daniel Haaser on 5/25/15.
//
//

#ifndef __Doodler__NetworkingWrapper__
#define __Doodler__NetworkingWrapper__

#import "Networking.h"

// Classes that want to be notified of
class NetworkingDelegate
{
    
};

class NetworkingWrapper
{
public:
    static NetworkingWrapper* getInstance();
    
private:
    Networking* networkManager;
};

#endif /* defined(__Doodler__NetworkingWrapper__) */
