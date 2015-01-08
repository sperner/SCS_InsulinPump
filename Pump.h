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
        // Checks the battery status and returns the value in percent.
        // In case of a critical status (level smaller than 15%) the user will 
        // be notified acoustically and the incident will be logged by the 
        // tracer. 
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
        // When pump is active injecting insulin the value will be 1, when 
        // injecting glucagon the value will be 2 and when inactive the value 
        // will be 0. 
        int active;
        float insulinLevel;
        float glucagonLevel;

        // current level
        float currentBloodSugarLevel;
        // current battery power level
        int batteryPowerLevel;
        //for logging purposes
        Tracer tracer;
/*
 * FUNCTIONS
 */

        // Calculates the amount of insulin needed based on the blood sugar levels. Returns calculated fictional Units when done.
        //
        // Parameter:
        // - targetInsValue: user defined vale to reduce blood sugar level  to, e.g. 110mg/dl
        // - currentBloodSugarLevel: current value of BSL, e.g. 160mg/dl
        // - isf: insulin sensitivity factor. Factor which indicates how much blood sugar one unit of insulin reduces, e.g.
        //        1:5 -> 1 unit insulin reduces 5mg/dl glucose
        virtual float calculateNeededInsulin(int targetInsValue, float currentBloodSugarLevel, int isf);

        // Calculates the amount of glucagon needed based on the blood sugar levels.
        //
        // Parameter:
        // - targetGlucValue: user defined vale to raise blood sugar level  to, e.g. 80mg/dl
        // - currentBloodSugarLevel: current value of BSL, e.g. 60mg/dl
        // - gsf: glucagon sensitivity factor. Factor which indicates how much blood sugar one unit of glucagon , e.g.
        //        1:5 -> 1 unit glucagon raises 5mg/dl glucose
        virtual float calculateNeededGlucagon(int targetGlucValue, float currentBloodSugarLevel, int gsf);


/*
 * BEGIN <<<<< meine bevorzugte loesung
 */
        // Calculates the amount of glucagon needed based on the blood sugar levels.
        //
        // Parameter:
        // - targetBloodSugarLevel: user defined value to raise or reduce blood sugar level to, e.g. 90mg/dl
        // - currentBloodSugarLevel: current value of BSL, e.g. 160mg/dl
        // - hsf: hormon sensitivity factor. Factor which indicates how much blood sugar one unit of used hormone
        //        raises or reduces , e.g. 1:5 -> 1 unit hormone raises/reduces 5mg/dl glucose.
        // - hormone: insulin or glucagon
        virtual int calculateNeededHormone(int targetBloodSugarLevel, float currentBloodSugarLevel, int hsf, string &hormone);
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
        virtual float getCurrentBloodSugarLevel();

/*
 * SETTER
 */
        // Injects the insulin into the body.
        //
        // Parameter:
        // - amount: The amount of insulin which is injected into the body.
        virtual bool injectInsulin(float amount);

        // Injects the glucagon into the body.
        //
        // Parameter:
        // - amount: The amount of glucagon which is injected into the body.
        virtual bool injectGlucagon(float amount);

        // decreases insulin level in reservoir when injected to body and
        // returns “True” when done
        //
        // Parameter:
        // - amount: The amount of insulin which is injected into the body
        //     needs to be reduced in the reservoir.
        virtual bool decreaseInsulinLevel(float amount);
        virtual bool decreaseGlucagonLevel(float amount);

/*
 * SIGNALS
 */
signals:
    // Callback for updating Insulin Reservoir in the UI.
    //
    // Parameter:
    // - The current amount of insulin in the reservoir
    void updateInsulinReservoir(float amount);
    // Callback for updating Glucagon Reservoir in the UI.
    //
    // Parameter:
    // - The current amount of glucagon in the reservoir
    void updateGlucagonReservoir(float amount);

};

#endif
