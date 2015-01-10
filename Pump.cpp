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
 * STRUCTS
 */
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

/*
 * END STRUCTS
 */



/*
 * FUNCTIONS
 */

/*
 * Injects either insulin or glucagon into the body.
 *
 * Parameters:
 * - amount : int       the amount of FU to be injected
 * - insulin : bool     true if the hormone to inject is insulin, false if it is glucagon
 *
 */
bool Pump::injectHormone(int amount, bool insulin)
{
    if (insulin)
    {
        // inject insulin here
    } else
    {
        // inject glucagon here
    }
    return true;
}


/*
 * Decreases the hormone level in either the insulin or the glucagon reservoir
 *
 * Parameters:
 * - amount: the amount by that the reservoir is reduced
 * - insulin: true if the hormone is insulin, false if it is glucagon
 */
bool Pump::decreaseHormoneLevel(int amount, bool insulin)
{
    if (insulin)
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
    else
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

}

/*
 * author: Markus
 * BEGIN <<<<< meine bevorzugte loesung. mit sicherheit noch buggy!
 */
//see header!
//clean up! refactor code!

/*
 * what happens with non-empty string with value other than insulin or glucagon?
 *
 * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 * jenny: just take a bool named 'insulin': true if the hormone is insulin, false if glucagon.
 * worst thing to happen in case of a bug: insulin is false by default. blood sugar will get higher instead of lower.
 * not good in long term, but no bug will do good in long term.
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 *
 * // TODO
 * we need to take care of the delay. insulin only has an effect after half an hour.
 * we need to take care of the long term effect. insulin will have an effect over max. 12 hrs
 *          -> how can we know/remember how much insulin there is in the blood circle?
 */
/*
 * summarize and refactor mehtod. return value only needs to be calculated from tBSL and cBSL.
 */
int Pump::calculateNeededHormone(int targetBloodSugarLevel)
{
    int difference;
    float fictInsUnit = 0, fictGlucUnit = 0;
    QString err = "Error! No valid hormone found!";
    string ins = "insulin";
    string gluc= "glucagon";

    if(insulin)
    {
        difference = currentBloodSugarLevel - targetBloodSugarLevel;
        fictInsUnit = difference / hsf;
        return fictInsUnit;
    }

    else
    {
        difference = targetBloodSugarLevel - currentBloodSugarLevel;
        fictInsUnit = difference / hsf;
        return fictGlucUnit;
    }

    tracer.writeCriticalLog(err);
    return -1;
}
/*
 * END
 */
/*
 * END FUNCTIONS
 */


/*
 * GETTER
 */

/*
 * Checks the battery status and returns the value in percent.
 * In case of a critical status (level smaller than 15%) the user will be
 * notified acoustically and the incident will be logged by the tracer.
 */
int Pump::getBatteryStatus()
{
    return this->batteryPowerLevel;
}

/*
 * Checks the entire pump (reservoir, mechanical parts) and returns “true” when
 * everything is working fine.
 */
bool Pump::getStatus()
{
	return true;
}

/*
 * Checks the blood sugar concentration and returns the value.
 * Returns current blood sugar level.
 */
int Pump::getCurrentBloodSugarLevel()
{
   return this->currentBloodSugarLevel;
};

/*
 * Returns the insulin level in the reservoir.
 */
float Pump::getInsulinLevel()
{
    return this->insulinLevel;
}

/*
 * Returns the glucagon level in the reservoir.
 */
float Pump::getGlucagonLevel()
{
    return this->glucagonLevel;
}
/*
 * END GETTER
 */


/*
 * SETTER
 */
/*
 * refills insulin and returns “true” when done
 */
void Pump::refillInsulin()
{
    // Update UI
    emit updateInsulinReservoir(100);
}

/*
 * refills glucagon and returns “true” when done
 */
void Pump::refillGlucagon()
{
    // Update UI
    emit updateGlucagonReservoir(100);
}
/*
 * END SETTER
 */


/*
 * RUNABLE
 */
/*
 * runable for Pump. Gets triggered by Scheduler.
 */
bool Pump::runPump()
{
    latestBloodSugarLevel = currentBloodSugarLevel;
    currentBloodSugarLevel = getCurrentBloodSugarLevel();

    // inject insulin
    if (currentBloodSugarLevel > maxBloodSugarLevel)
    {
        if (currentBloodSugarLevel > latestBloodSugarLevel)
        {
            insulin = true;
            if (delay)
            {
                // TODO
                calculateNeededHormone(latestBloodSugarLevel);
            }
            else
            {
                // TODO
                calculateNeededHormone(upperTargetBloodSugarLevel);
            }
        }
    }

    // inject glucagon
    if (currentBloodSugarLevel < minBloodSugarLevel)
    {
        if (currentBloodSugarLevel < latestBloodSugarLevel)
        {
            insulin = false;
            if (delay)
            {
                // TODO
                calculateNeededHormone(latestBloodSugarLevel);
            }
            else
            {
                // TODO
                calculateNeededHormone(lowerTargetBloodSugarLevel);
            }
        }
    }



    // TODO!!
    return true;
}
/*
 * END RUNABLE
 */





