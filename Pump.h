// 
// File: Pump.h
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:

#ifndef pump_
#define pump_

#include "Tracer.h"
#include <QObject>

using namespace std;



class Pump : public QObject
{
    Q_OBJECT

    public:
        /* Checks the battery status and returns the value in percent.
        // In case of a critical status (level smaller than 15%) the user will 
        // be notified acoustically and the incident will be logged by the 
        // tracer.
        */
        virtual int getBatteryStatus();

        // Checks the entire pump (reservoir, mechanical parts) and returns
        // “True” when everything is working fine. 
        virtual bool getStatus();

        // "main"-function for pump
        // triggered by Scheduler.
        virtual bool runPump();

    public slots:
        /**
         * Refills the Insulin in the Reservoir of the Pump
         */
        void refillInsulin();
        /**
         * Refills the Glucagon in the Reservoir of the Pump
         */
        void refillGlucagon();

    private:
        /* When pump is active injecting insulin the value will be 1, when
        // injecting glucagon the value will be 2 and when inactive the value 
        // will be 0.
        */
        int active;
        float insulinLevel;
        float glucagonLevel;

        // current level
        int currentBloodSugarLevel;

        // the blood sugar level in the latest cycle
        int latestBloodSugarLevel;

        // maximal healthy blood sugar level
        int maxBloodSugarLevel;

        // minimum healthy blood sugar level
        int minBloodSugarLevel;

        // target to decrease blood sugar level with insulin injection
        int upperTargetBloodSugarLevel;

        // target to increase blood sugar level with glucagon injection
        int lowerTargetBloodSugarLevel;

        // inject insulin or glucagon, true when insuling
        bool insulin;
        // true if there was an injection in the last cycle
        bool delay;

        // current battery power level
        int batteryPowerLevel;
        //for logging purposes
        Tracer tracer;

        // hormone sensitivity factor
        int hsf;
/*
 * FUNCTIONS
 */

/*
 * author: Markus
 * BEGIN <<<<< meine bevorzugte loesung
 */
        /* Calculates the amount of hormone needed based on the blood sugar levels.
        // Returns fictional Units of specified hormone, see parameters.
        //
        // Parameter:
        // - targetBloodSugarLevel:     predefined value to raise or reduce blood sugar level to,
        //                              e.g. 90mg/dl -> targetBloodSugarLevel = 90;
        //
        // - currentBloodSugarLevel:    current value of BSL, e.g. 160mg/dl -> currentBloodSugarLevel = 160;
        //
        // - hsf:                       hormon sensitivity factor. Factor which indicates how much blood sugar
        //                              one unit of used hormone raises or reduces , e.g. 1:5 -> 1 unit hormone
        //                              raises/reduces 5mg/dl glucose -> hsf = 5;
        //
        // - hormone:                   what sort of hormone is used, e.g. insulin or glucagon.
        //
        // >> jenny's solution <<       strings as parameters are bad. really bad. so lets just use a bool
        //                              called insulin that is true when insulin should be injected and false in
        //                              case of glucagon.
        // - insulin:                   true when insulin should be injected, false when glucagon should be injected
        */
public:
        virtual int calculateNeededHormone(int targetBloodSugarLevel);
/*
 * END
 */

/*
 * GETTER
 */
        // Returns the insulin level in the reservoir.
        virtual float getInsulinLevel();
        // Returns the glucagon level in the reservoir.
        virtual float getGlucagonLevel();
        // Checks the blood sugar concentration and returns the value.
        virtual int getCurrentBloodSugarLevel();

/*
 * SETTER
 */
        /* Injects either insulin or glucagon into the body.
        //
        // Parameters:
        // - amount : int       the amount of FU that should be injected
        // - insulin : bool     true if the hormone to inject is insulin, false if it is glucagon
        */
        virtual bool injectHormone(int amount, bool insulin);

        /* Decreases either the insulin or the glucagon level in the reservoir when a hormone is
        // injected to the body
        //
        // Parameters:
        // - amount : int       the amount of FU by that the reservoir should be decreased
        // - insulin : bool     true if the hormone to inject is insulin, false if it is glucagon
        */
        virtual bool decreaseHormoneLevel(int amount, bool insulin);

/*
 * SIGNALS
 */
signals:
    /* Callback for updating Insulin Reservoir in the UI.
    //
    // Parameter:
    // - The current amount of insulin in the reservoir
    */
    void updateInsulinReservoir(float amount);
    /* Callback for updating Glucagon Reservoir in the UI.
    //
    // Parameter:
    // - The current amount of glucagon in the reservoir
    */
    void updateGlucagonReservoir(float amount);

};

#endif
