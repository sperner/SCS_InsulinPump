//
// File: Body.cpp
//
// Date: 24.12.14 17:11
//
// Generated by: Idatto, version 1.3
//
// Description: Simulates a body suffering from diabetes.

#include "Body.h"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#define BUFLEN          100
#define EXIT__FAILURE   -1

int     main            (void);
int     main_offline    (void); // is working!

int     fdes_body_to_pump; // fildescriptor for Body --> Pump
int     fdes_pump_to_body; // fildescriptor for Pump --> Body
char    buffer;
int     i;

//
//  Body.cpp
//  Body
//
//  Created by Johannes Kinzig on 04.01.15.
//  Copyright (c) 2015 Johannes Kinzig. All rights reserved.
//

/****************************************************************
 *               used to store data for transmission             *
 *****************************************************************/

/**********************************
 * transmit_hormone_injection      *
 **********************************/

struct transmit_injection_hormones {
    float injected_insulin;
    float injected_glucagon;
} Injecting; // will be send over pipe: pump_to_body

/**********************************
 * transmit_bloodsugar      *
 **********************************/

struct transmit_bloodsugar {
    float bloodSugarLevel;
} BodyStatus; // will be send over pipe: body_to_pump

/****************************************************************
 *                          END                                 *
 ****************************************************************/

// constructor
Body::Body(float BSL, int constant){
    BloodsugarLevel = BSL; // unit: mg/dL
    insulin_constant = constant; // unit: mg/dL sinking per iteration => one iteration estimated as 0.5 hours
};

// destructor
Body::~Body(void){
    
}


// changes the blood sugar level;
// increasing: if True: rising; if False: falling
// strength: the factor the BSL is rising or falling
bool Body::changeBloodSugarLevel(float strength, bool increasing, bool use_insulin_constant) {
    /******************************************************
     *   defined 3 levels for increasing/decreasing:      *
     *      Level 1: calm   --> 1.03                      *
     *      Level 2: middle --> 1.06                      *
     *      Level 3: fast   --> 1.09                      *
     ******************************************************/
    
    /**********************************
     * deciding if rising or falling  *
     **********************************/
    
    // rising
    if (increasing == true) {
        this->BloodsugarLevel = this->BloodsugarLevel * strength; // body factor influences BSL
        
        if (use_insulin_constant == true) {
            this->BloodsugarLevel = this->BloodsugarLevel - this->insulin_constant; // injected Insulin influences BSL per 0.5 hours
        }
    }

    // falling
    if (increasing == false) {
        this->BloodsugarLevel = this->BloodsugarLevel / strength;
        if (use_insulin_constant == true) {
            this->BloodsugarLevel = this->BloodsugarLevel - this->insulin_constant; // injected Insulin influences BSL per 0.5 hours
        }
    }
    return true;
}

void Body::setBloodSugarLevel(float BSL) {
    this->BloodsugarLevel = BSL;
}

float Body::getBloodSugarLevel() {
    return this->BloodsugarLevel;
}

Body body(110.00, 5);

// simulating BSL - should be inside a seperate thread
int main(void) {
    cout << "I am the body!\n";
    
    cout << body.getBloodSugarLevel();
    cout << "\n";
    
    body.changeBloodSugarLevel(1.03, false, 0);
    return 0;
}

// communication via pipes
int main_offline (void) {
    
    BodyStatus.bloodSugarLevel = 29.00;
    
    // generate pipe for Body --> Pump
    mknod("/Users/johanneskinzig/Documents/XcodeDev/body_to_pump",S_IFIFO | 0666,0);
    
    if((fdes_body_to_pump=open("/Users/johanneskinzig/Documents/XcodeDev/body_to_pump",O_WRONLY))==(-1)) {
        puts("Fehler 'open pipe'");
        exit(EXIT__FAILURE);
    }
    
    // generate pipe for Pump --> Body
    mknod("/Users/johanneskinzig/Documents/XcodeDev/pump_to_body",S_IFIFO | 0666,0);
    
    if((fdes_pump_to_body=open("/Users/johanneskinzig/Documents/XcodeDev/pump_to_body",O_RDONLY))==(-1)) {
        puts("Fehler 'open pipe'");
        exit(EXIT__FAILURE);
    }
    
    // write Body --> Pump
    if((i=write(fdes_body_to_pump, &BodyStatus, BUFLEN)) != BUFLEN) {
        printf("Fehler 'write-call'");
        exit(EXIT__FAILURE);
    }
    close(fdes_body_to_pump);
    
    // read Pump --> Body
    read(fdes_pump_to_body, &Injecting, BUFLEN);
    cout << Injecting.injected_insulin;
    cout << "\n";
    cout << Injecting.injected_glucagon;
    cout << "\n";
    close(fdes_pump_to_body);
    
    exit(0);
    
} /* END_MAIN() */
