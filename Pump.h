// 
// File: Pump.h
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description: Measure Blood Sugar Level, Calculate Units of Hormone (Insulin or Glucagon) and Inject Calculated Units
//
// Authors: Jenny Kreger and Markus Ernst

/* abbreviations:
 *
 * HSF/hsf  =   Hormone Sensitivity Factor;
 * BSL      =   Blood Sugar Level;
 * cBSL     =   current Blood Sugar Level;
 * tBSL     =   target Blood Sugar Level;
 *
 *
 */

#ifndef pump_
#define pump_

#define MAX_BATTERY_CHARGE  100

#include "Tracer.h"
#include <QObject>

using namespace std;



class Pump : public QObject
{
    Q_OBJECT

// ATTRIBUTES
private:
        //for logging purposes
        Tracer tracer;

        /* When pump is active injecting insulin the value will be 1, when
        // injecting glucagon the value will be 2 and when inactive the value
        // will be 0.
        */
        int active;
        int insulinReservoirLevel;
        int glucagonReservoirLevel;

        // current blood sugar level
        int currentBloodSugarLevel;

        // the blood sugar level in the latest cycle
        int latestBloodSugarLevel;

        // maximal healthy blood sugar level
        int maxBloodSugarLevel;

        // minimum healthy blood sugar level
        int minBloodSugarLevel;

        // target blood sugar level
        int targetBloodSugarLevel;

        // target to decrease blood sugar level with insulin injection
        int upperTargetBloodSugarLevel;

        // target to increase blood sugar level with glucagon injection
        int lowerTargetBloodSugarLevel;

        // inject insulin or glucagon, true when insulin
        bool insulin;

        // true if there was an injection in the last cycle
        bool delay;

        // current battery power level
        int batteryPowerLevel=100;

        // hormone sensitivity factor
        int hormoneSensitivityFactor;
// END ATTRBUTES


// FUNCTIONS
public:
        /* "main"-function for pump triggered by Scheduler.
        */
        bool runPump();

        /* Checks the battery status and returns the value in percent.
         */
        int checkPumpBatteryStatus(void);

        /* Drains power from battery.
         *
         * Parameters:
         * - powerdrain : int   amount of powerdrainage.
         */
        void drainBatteryPower(int powerdrain);

private:
        /* Injects either insulin or glucagon into the body.
        *
        * Parameters:
        * - amount : int       the amount of FU that should be injected
        * - insulin : bool     true if the hormone to inject is insulin, false if it is glucagon
        * - return : bool      returns false if injection failed.
        */
        /**
         * @brief injectHormone
         * @param targetBloodSugarLevel
         * @param insulin
         * @param amount
         * @return false on failed injection
         */
        bool injectHormone(int targetBloodSugarLevel, bool insulin, int amount);

        /* Decreases either the insulin or the glucagon level in the reservoir when a hormone is
        * injected to the body
        *
        * Parameters:
        * - amount : int       the amount of FU by that the reservoir should be decreased
        * - insulin : bool     true if the hormone to inject is insulin, false if it is glucagon
        * - return : bool      returns true, if decrease was ok.
        */
        /**
         * @brief decreaseHormoneLevel
         * @param amount
         * @param insulin
         * @return true on successful decrease
         */
        bool decreaseHormoneReservoire(int amount, bool insulin);

        /**
         * @brief   Pump::readBloodSugarSensor
         *          Reads the current blood sugar level via pipe from body
         *
         * @return  the current blood sugar level
         */
        int readBloodSugarSensor();

        /**
         * @brief   Pump::injectHormoneToBody
         *          Injects the calculated amount of hormones to the body via pipe
         *
         * @param   amount
         *          the amount of hormones to inject
         *
         * @param   insulin
         *          true if insulin, false if glucagon
         *
         * @return  true if anything is ok
         */
        bool injectHormoneToBody(int amount, bool insulin);

 /*
 * author: Markus
 */
        /* Calculates the amount of hormone needed based on the blood sugar levels.
        * Returns fictional Units of specified hormone, see parameters.
        *
        * Parameter:
        * - targetBloodSugarLevel:     predefined value to raise or reduce blood sugar level to,
        *                              e.g. 90mg/dl -> targetBloodSugarLevel = 90;
        *
        * - currentBloodSugarLevel:    current value of BSL, e.g. 160mg/dl -> currentBloodSugarLevel = 160;
        *
        * - hormoneSensitivityFactor:  hormone sensitivity factor. Factor which indicates how much blood sugar
        *                              one unit of used hormone raises or reduces , e.g. 1:5 -> 1 unit hormone
        *                              raises/reduces 5mg/dl glucose -> hsf = 5;
        *
        * - hormone:                   what sort of hormone is used, e.g. insulin or glucagon.
        *
        * >> jenny's solution <<       strings as parameters are bad. really bad. so lets just use a bool
        *                              called insulin that is true when insulin should be injected and false in
        *                              case of glucagon.
        * - insulin:                   true when insulin should be injected, false when glucagon should be injected
        */
        /**
         * @brief calculateNeededHormone
         * @param targetBloodSugarLevel
         * @return units of hormone
         */
        int calculateNeededHormone(int targetBloodSugarLevel);

        /* recharges battery up to 100% of charge.
         *
         * Parameters:
         * - power: level of recharge energy.
         */
        /**
          * @brief rechargeBatteryPower
          * @param charge
          */
         void rechargeBatteryPower(int charge);

// END FUNCTIONS


// GETTER
public:
        /* Checks the entire pump (reservoir, mechanical parts) and returns
        * “True” when everything is working fine.
        */
        bool getPumpStatus() const;

        /* Returns target blood sugar value.
         */
        int getTargetBloodSugarLevel(void) const;

         /* Returns the insulin level in the reservoir.
         */
        int getInsulinReservoirLevel() const;

        /* Returns the glucagon level in the reservoir.
        */
        int getGlucagonReservoirLevel() const;

        /* Checks the blood sugar concentration and returns the value.
         */
        int getCurrentBloodSugarLevel() const;

        /* returns battery power level
        // In case of a critical status (level smaller than 15%) the user will
        // be notified acoustically and the incident will be logged by the
        // tracer.
        */
        int getBatteryPowerLevel() const;

        /* returns pump status active*/
        int getActive() const;

        /* returns latest blood sugar level*/
        int getLatestBloodSugarLevel() const;

        /* returns HSF*/
        int getHormoneSensitivityFactor() const;

        /* returns true if delay flag is set*/
        bool getDelay() const;

        /* returns true if hormone is insulin, false if glucagon */
        bool getInsulin() const;

        /* returns lower threshold of blood sugar level */
        int getLowerTargetBloodSugarLevel() const;

        /* returns upper threshold of BSL */
        int getUpperTargetBloodSugarLevel() const;

        /* returns minimum blood sugar level */
        int getMinBloodSugarLevel() const;

        /* returns maximum blood sugar level */
        int getMaxBloodSugarLevel() const;

// END GETTER


// SETTER
         /* sets power level. power is decreasing due to usage of pump. only decreases when pump is in use.
         *
         * Parameters:
         * - powerdrain: int    amount of power drained from battery.
         */
        /**
          * @brief setBatteryPowerLevel
          * @param powerdrain
          */
         void setBatteryPowerLevel(int powerdrain);

         /* set target blood sugar level.
          *
          * Parameter:
          * - tbsl: int predefined value for target BSL.
          */
         /**
          * @brief setTargetBloodSugarLevel
          * @param tbsl
          */
         void setTargetBloodSugarLevel(int tbsl);
         /* set pump on active.
          */
         /**
          * @brief setActive
          * @param value
          */
         void setActive(int value);

         /*sets latest blood sugar level
          */
         /**
          * @brief setLatestBloodSugarLevel
          * @param value
          */
         void setLatestBloodSugarLevel(int value);

         /* set HSF*/
         /**
          * @brief setHormoneSensitivityFactor
          * @param value
          */
         void setHormoneSensitivityFactor(int value);

         /* set delay flag*/
         /**
          * @brief setDelay
          * @param value
          */
         void setDelay(bool value);

         /* set insulin value true if insulin*/
         /**
          * @brief setInsulin
          * @param value
          */
         void setInsulin(bool value);

         /* set lower BSL*/
         /**
          * @brief setLowerTargetBloodSugarLevel
          * @param value
          */
         void setLowerTargetBloodSugarLevel(int value);

         /* set upper BSL*/
         /**
          * @brief setUpperTargetBloodSugarLevel
          * @param value
          */
         void setUpperTargetBloodSugarLevel(int value);

         /* set minimum BSL*/
         /**
          * @brief setMinBloodSugarLevel
          * @param value
          */
         void setMinBloodSugarLevel(int value);

         /* set maximum BSL*/
         /**
          * @brief setMaxBloodSugarLevel
          * @param value
          */
         void setMaxBloodSugarLevel(int value);

// END SETTER

         // SLOTS
public slots:
         /**
         * Refills the Insulin in the Reservoir of the Pump
         */
        void refillInsulinReservoir();
        /**
         * Refills the Glucagon in the Reservoir of the Pump
         */
        void refillGlucagonReservoir();

// END SLOTS


// SIGNALS
signals:
    /* Callback for updating Insulin Reservoir in the UI.
    *
    * Parameter:
    * - The current amount of insulin in the reservoir
    */
    void updateInsulinReservoir(float amount);

    /* Callback for updating Glucagon Reservoir in the UI.
    *
    * Parameter:
    * - The current amount of glucagon in the reservoir
    */
    void updateGlucagonReservoir(float amount);

    /* Callback for updating BSL in UI.
     *
     * Parameter:
     * - bsl: int               target blood sugar level.
     * - amountHormone: int     amount of hormone used.
     * - injectHormUnits: int   units of injected hormone
     */
    void updateBloodSugarLevel(int tbsl, int amountHormone, int injectHormUnits);

// END SIGNALS
}; //END HEADER

#endif
