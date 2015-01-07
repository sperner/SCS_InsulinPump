// 
// File: Pump.cpp
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:

#include "Pump.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <QObject>

using namespace std;

#define BUFLEN  100
#define EXIT__FAILURE   -1

int     i;
int     main (void);
int     fdes_body_to_pump; // fildescriptor for Body --> Pump
int     fdes_pump_to_body; // fildescriptor for Pump --> Body



// Injects the insulin into the body.
// 
// Parameter:
// - amount: The amount of insulin which is injected into the body.
bool Pump::injectInsulin(float amount)
{
	return true;
}

// Injects the glucagon into the body.
// 
// Parameter:
// - amount: The amount of glucagon which is injected into the body.
bool Pump::injectGlucagon(float amount)
{
	return true;
}

// Checks the blood sugar concentration and returns the value.
float Pump::getBloodsugar()
{
}

// refills insulin and returns “true” when done
void Pump::refillInsulin()
{
    // Update UI
    emit updateInsulinReservoir(100);
}

// refills glucagon and returns “true” when done
void Pump::refillGlucagon()
{
    // Update UI
    emit updateGlucagonReservoir(100);
}

// decreases insulin level in reservoir when injected to body and returns 
// “true” when done 
// 
// Parameter:
// - amount: The amount of insulin which is injected into the body needs to be 
//     reduced in the reservoir. 
bool Pump::decreaseInsulinLevel(float amount)
{
	return true;
}

bool Pump::decreaseGlucagonLevel(float amount)
{
	return true;
}

// Checks the battery status and returns the value in percent.
// In case of a critical status (level smaller than 15%) the user will be 
// notified acoustically and the incident will be logged by the tracer. 
int Pump::getBatteryStatus()
{
}

// Checks the entire pump (reservoir, mechanical parts) and returns “true” when 
// everything is working fine. 
bool Pump::getStatus()
{
	return true;
}

// Calculates the amount of insulin needed based on the blood sugar levels.
float Pump::calculateNeededInsulin()
{
}

// Calculates the amount of glucagon needed based on the blood sugar levels.
float Pump::calculateNeededGlucagon()
{
}

// Returns the insulin level in the reservoir.
float Pump::getInsulinLevel()
{
}

// Returns the glucagon level in the reservoir.
float Pump::getGlucagonLevel()
{
}

//runable for Pump. Gets triggered by Scheduler.
int Pump::runPump(){
    return 0;
}

//
//  Pump.cpp
//
//  Created by Johannes Kinzig on 04.01.15.
//  Copyright (c) 2015 Johannes Kinzig. All rights reserved.
//
/****************************************************************
 *               used to store data for receiving               *
 ****************************************************************/

/**********************************
 * transmit_hormone_injection      *
 **********************************/

struct transmit_injection_hormones {
    float injected_insulin;
    float injected_glucagon;
} Injecting;

/**********************************
 * transmit_bloodsugar      *
 **********************************/

struct transmit_bloodsugar {
    float bloodSugarLevel;
} BodyStatus;

/****************************************************************
 *                          END                                 *
 ****************************************************************/


int main_ofPump(void) {
    
    // testing values
    Injecting.injected_insulin = 10.00;
    Injecting.injected_glucagon = 90.00;
    
    // open pipe Body --> Pump
    if((fdes_body_to_pump=open("/Users/johanneskinzig/Documents/XcodeDev/body_to_pump",O_RDONLY))==(-1)) {
        printf("Failure 'open pipe'");
        exit(-1);
    }
    
    // open pipe Pump --> Body
    if((fdes_pump_to_body=open("/Users/johanneskinzig/Documents/XcodeDev/pump_to_body",O_WRONLY))==(-1)) {
        printf("Failure 'open pipe'");
        exit(-1);
    }
    
    // read Body --> Pump
    read(fdes_body_to_pump, &BodyStatus, BUFLEN);
    cout << BodyStatus.bloodSugarLevel;
    cout << "\n";
    close(fdes_body_to_pump);
    
    // write Pump --> Body
    if((i=write(fdes_pump_to_body, &Injecting, BUFLEN)) != BUFLEN) {
        printf("Fehler 'write-call'");
        exit(EXIT__FAILURE);
    }
    close(fdes_pump_to_body);
    
    exit(0);
} /* END_main() */



