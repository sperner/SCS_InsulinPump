// 
// File: Pump.cpp
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:


//TODO
/*
 * 1. clean up code.
 * 2. summarize decreaseInsulin and decreaseGlucagon into one.
 * 3. summarize injectInsulin and injectGlucagon into one.
 * 4. code runable and rest of methods.
 * 5. test calculateNeededHormone for non-empty wrong value strings.
 * 6. refactor code. especially code that is redundant.
 * 7. test pump.
 */

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


/*
 * FUNCTIONS
 */

/*
 * SUMMARIZE!
 */
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

// decreases insulin level in reservoir when injected to body and returns 
// “true” when done 
// 
// Parameter:
// - amount: The amount of insulin which is injected into the body needs to be 
//     reduced in the reservoir. 
bool Pump::decreaseInsulinLevel(float amount)
{
    QString err = "Reservoir Insulin too low!";
    if (amount <= this->getInsulinLevel())
    {
            insulinLevel-=amount;
            emit updateInsulinReservoir(insulinLevel);
            return true;
    }
    tracer.writeCriticalLog(err);
    return false;
}

// decreases glucagon level in reservoir when injected to body and returns
// “true” when done
//
// Parameter:
// - amount: The amount of glucagon which is injected into the body needs to be
//     reduced in the reservoir.
bool Pump::decreaseGlucagonLevel(float amount)
{
    QString err = "Reservoir Glucagon too low!";
    if (amount <= this->getGlucagonLevel())
    {
            glucagonLevel-=amount;
            emit updateGlucagonReservoir(glucagonLevel);
            return true;
    }
    tracer.writeCriticalLog(err);
    return false;
}
/*
 * END SUMMARIZE!
 */

/*
 * still necessary?
 */
// Calculates the amount of insulin needed based on the blood sugar levels. Returns calculated fictional Units when done.
//
// Parameter:
// - targetInsValue: user defined vale to reduce blood sugar level  to, e.g. 110mg/dl
// - currentBloodSugarLevel: current value of BSL, e.g. 160mg/dl
// - isf: insulin sensitivity factor. Factor which indicates how much blood sugar one unit of insulin reduces, e.g.
//        1:5 -> 1 unit insulin reduces 5mg/dl glucose
float Pump::calculateNeededInsulin(int targetInsValue, float currentBloodSugarLevel, int isf)
{
    int difference, fictInsUnit;
    difference = currentBloodSugarLevel - targetInsValue;
    fictInsUnit = difference / isf;
    return fictInsUnit;
}

// Calculates the amount of glucagon needed based on the blood sugar levels.
//
// Parameter:
// - targetGlucValue: user defined vale to raise blood sugar level  to, e.g. 80mg/dl
// - currentBloodSugarLevel: current value of BSL, e.g. 60mg/dl
// - gsf: glucagon sensitivity factor. Factor which indicates how much blood sugar one unit of glucagon , e.g.
//        1:5 -> 1 unit glucagon raises 5mg/dl glucose
float Pump::calculateNeededGlucagon(int targetGlucValue, float currentBloodSugarLevel, int gsf)
{
    int difference, fictGlucUnit;
    difference = targetGlucValue - currentBloodSugarLevel;
    fictGlucUnit = difference / gsf;
    return fictGlucUnit;
}
/*
 * END still necessary?
 */


/*
 * author: Markus
 * BEGIN <<<<< meine bevorzugte loesung. mit sicherheit noch buggy!
 */
//see header!
//clean up! refactor code!

/*
 * what happens with non-empty string with value other than insulin or glucagon?
 */
float Pump::calculateNeededHormone(int targetBloodSugarLevel, int currentBloodSugarLevel, int hsf, string hormone)
{
    int difference;
    float fictInsUnit=0,fictGlucUnit=0;
    QString err = "Error! No valid hormone found!";
    string ins = "insulin";
    string gluc= "glucagon";

//    //test prints
//    cout << "hallo pumpe!" << endl; //<<-- for testing
//    cout << "variable ausgeben: \n" << "tBSL: " << targetBloodSugarLevel << " cBSL: " << currentBloodSugarLevel << " HSF: " << hsf << " Hormon: " << hormone << endl;

    if (!hormone.empty())
    {
        if(hormone == ins)
        {
//            cout << "verzweigung fuer insulin. erhaltenes hormon: " << hormone << endl;
//            cout << "insulin berechnen" << endl; //<<-- for testing
            difference = currentBloodSugarLevel - targetBloodSugarLevel;
            fictInsUnit = difference / hsf;
            return fictInsUnit;
        }

        else if(hormone == gluc)
        {
//            cout << "verzweigung fuer glucagon. erhaltenes hormon: " << hormone << endl;
//            cout << "glucagon berechnen" << endl; //<<-- for testing
            difference = targetBloodSugarLevel - currentBloodSugarLevel;
            fictInsUnit = difference / hsf;
            return fictGlucUnit;
        }
    }
//    cout << err << endl; //<<-- for testing
    tracer.writeCriticalLog(err);
    return -1;
}
/*
 * END
 */


/*
 * GETTER
 */

// Checks the battery status and returns the value in percent.
// In case of a critical status (level smaller than 15%) the user will be 
// notified acoustically and the incident will be logged by the tracer. 
int Pump::getBatteryStatus()
{
    return this->batteryPowerLevel;
}

// Checks the entire pump (reservoir, mechanical parts) and returns “true” when 
// everything is working fine. 
bool Pump::getStatus()
{
	return true;
}

// Checks the blood sugar concentration and returns the value.
// Returns current blood sugar level.
float Pump::getCurrentBloodSugarLevel()
{
   return this->currentBloodSugarLevel;
};

// Returns the insulin level in the reservoir.
float Pump::getInsulinLevel()
{
    return this->insulinLevel;
}

// Returns the glucagon level in the reservoir.
float Pump::getGlucagonLevel()
{
    return this->glucagonLevel;
}

/*
 * SETTER
 */

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

/*
 * RUNABLE
 */

//runable for Pump. Gets triggered by Scheduler.
bool Pump::runPump()
{
    return true;
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

/* RUNABLE */
/*
int main_ofPump(void) {
    
    // testing values
    Injecting.injected_insulin = 10.00;
    Injecting.injected_glucagon = 90.00;
    
    // open pipe Body --> Pump
    if((fdes_body_to_pump=open("body_to_pump",O_RDONLY))==(-1)) {
        printf("Failure 'open pipe'");
        exit(-1);
    }
    
    // open pipe Pump --> Body
    if((fdes_pump_to_body=open("pump_to_body",O_WRONLY))==(-1)) {
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
}
*/





