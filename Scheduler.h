// 
// File: Scheduler.h
// 
// Date: 24.12.14 17:11
// 
// Generated by: Idatto, version 1.3
// 
// Description:


#ifndef scheduler_
#define scheduler_

#include <QSettings>
#include <QElapsedTimer>
#include "Pump.h"



class Scheduler
{
    public:
        Scheduler();
        ~Scheduler();

        // answers ControlSystem’s call for checkScheduler()
        virtual bool getStatus();

    private:
        int TimerResetValueSec;

        // Times of the Scheduler for measuring operation hours
        QElapsedTimer Timer;

        // File for saving total operation time
        QSettings *SaveFile;

        // Counter of operation time for noticing the user when an inspection
        // is needed or when system lifetime (because of mechanical outwear) is
        // reached.
        qint64 TotalOperationTime;

        // triggers the pump which then checks the blood sugar level
        virtual bool triggerPump();

        // resets the timer and sets the countdown time according to parameter
        virtual bool resetTimer(int time_min);

        // Starts the counter for operation hours and returns “True” when 
        // successfully started. The value will be written to 
        // “TotalOperationHours”.  
        virtual bool startOperationHoursCounter();

        // Stops the counter for operation hours and returns “True” when 
        // successfully stopped. The value will be written to 
        // “TotalOperationHours”. 
        virtual bool stopOperationHoursCounter();

};

#endif
