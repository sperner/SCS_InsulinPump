//
//
//  Body.cpp
//  Body
//
//  Created by Johannes Kinzig on 09.01.15.
//  Copyright (c) 2015 Johannes Kinzig. All rights reserved.
//
//  Description: Simulates a body suffering from diabetes and reacting to insulin and/or glucagon.
//

#ifndef bodyThreadController_
#define bodyThreadController_

#include <vector>
#include <string>

//class for communicating with the BSL-generating-Thread
class BodyThreadController {
public:
    // constructor, destructor
    BodyThreadController();
    ~BodyThreadController();
    
    // ThreadBodyFactor -- tells the thread the body factor
    virtual void setThreadBodyFactor(float factor);
    virtual float getThreadBodyFactor(void);
    
    // ThreadRising -- tells the thread to rise or fall the BSL level
    virtual void setThreadRising(bool value);
    virtual bool getThreadRising(void);
    
    // ThreadGlucagonUnits -- tells the thread the amount of fictive glucagon units to use
    virtual void setThreadGlucagonUnits(int units);
    virtual int getThreadGlucagonUnits(void);
    virtual void minusThreadGlucagonUnits(int);
    
    // ThreadUseInsulinUnits -- tells the thread the amount of fictive inuslin units to use
    virtual void setThreadInsulinUnits(int units);
    virtual int getThreadInsulinUnits(void);
    virtual void minusThreadInsulinUnits(int); // subtracts the argument from ThreadInsulinUnits
    
    // ThreadEndThread -- tells the thread to terminate
    virtual void setThreadEndThread(bool value);
    virtual bool getThreadEndThread(void);
    
    
private:
    float   ThreadBodyFactor;
    bool    ThreadRising;
    int     ThreadInsulinUnits;
    int     ThreadGlucagonUnits;
    bool    ThreadEndThread;

};

#endif
