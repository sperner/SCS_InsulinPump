/**
 * @file:   UserInterface.cpp
 * @class:  UserInterface
 *
 * @author: Andreij Robert,
 *          Sven Sperner, sillyconn@gmail.com
 *          - added stuff for displaying operation time
 *          - added stuff to change minimum battery load
 *          - added testing stuff for actual battery load
 *          - added stuff to change maximum operation time
 *          - added testing stuff for actual operation time
 *
 * @date:   19.01.2015
 * Created: 24.12.14 17:11 with Idatto, version 1.3
 *
 * @brief:  Check the systems health status
 *          Write via Tracer to logfile & to UI
 *
 * Copyright (c) 2015 All Rights Reserved
 */

#include "UserInterface.h"
#include "ui_UserInterface.h"
#include "iostream"
#include <string>
#include <QString>
#include <QTime>
#include <QTimer>

using namespace std;


UserInterface::UserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);

    // Init Testing section
    ui->mTestingBatterySlider->setMaximum(ui->mBatteryProgressBar->maximum());
    ui->mTestingGlucagonSlider->setMaximum(ui->mGlucagonProgressBar->maximum());
    ui->mTestingInsulinSlider->setMaximum(ui->mInsulinProgressBar->maximum());

    // Init Time
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateClock()));
    timer->start();
    updateClock();
}

UserInterface::~UserInterface()
{
    delete ui;
}

/**
 * Updates the Batteries power level in the Progressbar
 *
 * @param level - new battery power level
 */
void UserInterface::batteryPowerLevelChanged(int level)
{
    ui->mBatteryProgressBar->setValue(level);
}

/**
 * Updates the Batteries minimum power level in the spin box
 *
 * @param level - new battery power level
 */
void UserInterface::minBatteryLevelChanged(int level)
{
    ui->mMinBatLoadSpinner->setValue(level);
}

/**
 * Updates the Insulin amount in the Progressbar
 *
 * @param amount - new insulin amount
 */
void UserInterface::insulinAmountInReservoirChanged(float amount)
{
    // Update Color
    if (amount < 51 && amount >= 26)
    {
        ui->mInsulinProgressBar->setStyleSheet(ui->mInsulinProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(240, 240, 0); }");
    } else if(amount < 26)
    {
        ui->mInsulinProgressBar->setStyleSheet(ui->mInsulinProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(255, 0, 0); }");
    } else
    {
        ui->mInsulinProgressBar->setStyleSheet(ui->mInsulinProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(0, 210, 0); }");
    }

    // Set Amount
    ui->mInsulinProgressBar->setValue(amount);
}

/**
 * Updates the Glucagon amount in the Progressbar
 *
 * @param amount - new glucagon amount
 */
void UserInterface::glucagonAmountInReservoirChanged(float amount)
{
    // Update Color
    if (amount < 51 && amount >= 26)
    {
        ui->mGlucagonProgressBar->setStyleSheet(ui->mGlucagonProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(240, 240, 0); }");
    } else if(amount < 26)
    {
        ui->mGlucagonProgressBar->setStyleSheet(ui->mGlucagonProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(255, 0, 0); }");
    } else
    {
        ui->mGlucagonProgressBar->setStyleSheet(ui->mGlucagonProgressBar->property("defaultStyleSheet").toString() +
                                                   "QProgressBar { border: 1px solid grey; border-radius: 4px; background-color: rgb(213, 213, 213); }" +
                                                    "QProgressBar::chunk { background: rgb(0, 210, 0); }");
    }

    // Set Amount
    ui->mGlucagonProgressBar->setValue(amount);
}

/**
 * Updates the operation time in the Progressbar
 *
 * @param hours - new operation time in hours
 */
void UserInterface::operationTimeChanged(int hours)
{
    QString text = QString::number(hours) + " hours";
    ui->mOpTimeValue->setText(text);
}

/**
 * Updates the maximum operation time in the spin box
 *
 * @param hours - new maximum operation time in hours
 */
void UserInterface::maxOperationTimeChanged(int hours)
{
    ui->mMaxOpTimeSpinner->setValue(hours);
}

/**
 * Inserts the status message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertStatusLog(QString message)
{
    // Add Message and scroll to bottom
    ui->mMessageList->addItem(new QListWidgetItem(message));
    ui->mMessageList->scrollToBottom();
}

/**
 * Inserts the warning message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertWarningLog(QString message)
{
    // Add Message and scroll to bottom
    QListWidgetItem* item = new QListWidgetItem(message);
    item->setBackgroundColor(Qt::yellow);
    ui->mMessageList->addItem(item);
    ui->mMessageList->scrollToBottom();
}

/**
 * Inserts the critical message in to the QListWidgetItem
 *
 * @param message - string message to insert
 */
void UserInterface::insertCriticalLog(QString message)
{
    // Add Message and scroll to bottom
    QListWidgetItem* item = new QListWidgetItem(message);
    item->setBackgroundColor(Qt::red);
    ui->mMessageList->addItem(item);
    ui->mMessageList->scrollToBottom();
}

/**
 * Refill the Insulinreservoir in the Pump
 */
void UserInterface::on_mInsulinRefillButton_clicked()
{
    // Trigger Callback for refilling Insulin Reservoir in the Pump
    emit refillInsulinInPump();
}

/**
 * Refill the Glucagonreservoir in the Pump
 */
void UserInterface::on_mGlucagonRefillButton_clicked()
{
    // Trigger Callback for refilling Glucagon Reservoir in the Pump
    emit refillGlucagonInPump();
}

/**
 * Updates the Time in the UI
 */
void UserInterface::updateClock()
{
    // Get Time
    QTime time = QTime::currentTime();
    // Format and set Time
    QString text = time.toString("hh:mm:ss");
    ui->mTimeValue->setText(text);
}

/**
 * Updates the Bloodsugar in the UI
 *
 * @param amount - current bloodsugar
 */
void UserInterface::updateBloodsugarLevel(int bloodsugarLevel, int hormone, int amountInjected)
{
    // Timestamp
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    // Insert Message
    if (hormone == INSULIN)
    {
        ui->mBloodsugarLog->addItem(new QListWidgetItem(text + "  injected " + QString::number(amountInjected) + " units Insulin"));
        ui->mBloodsugarLog->scrollToBottom();
    } else if(hormone == GLUCAGON)
    {
        ui->mBloodsugarLog->addItem(new QListWidgetItem(text + "  injected " + QString::number(amountInjected) + " units Glucagon"));
        ui->mBloodsugarLog->scrollToBottom();
    }
    // Remove oldest item if count is above 25
    if (ui->mBloodsugarLog->count() > 26)
    {
        ui->mBloodsugarLog->takeItem(0);
        //delete item;
    }
    ui->mBloodSugarValue->setValue(bloodsugarLevel);
}

/**
 * Testing onBatteryButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setBatteryLevel().
 */
void UserInterface::on_mTestingBatteryButton_clicked()
{
    emit setBatteryPowerLevel(ui->mTestingBatterySlider->value());
}

/**
 * Testing onMinBatLoadButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setMinBatteryLevel().
 */
void UserInterface::on_mMinBatLoadButton_clicked()
{
    emit setMinBatteryLevel(ui->mMinBatLoadSpinner->value());
}

/**
 * Testing onGlucagonButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setGlucagonReservoirLevel().
 */
void UserInterface::on_mTestingGlucagonButton_clicked()
{
    emit setGlucagonReservoirLevel(ui->mTestingGlucagonSlider->value());
}

/**
 * Testing onInsulinButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setInsulinReservoirLevel().
 */
void UserInterface::on_mTestingInsulinButton_clicked()
{
    emit setInsulinReservoirLevel(ui->mTestingInsulinSlider->value());
}

/**
 * Testing onOpTimeButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setInsulinReservoirLevel().
 */
void UserInterface::on_mTestingOpTimeButton_clicked()
{
    emit setOperationTime(ui->mTestingOpTimeSlider->value());
}

/**
 * Testing onmMaxOpTimeButtonClicked
 *
 * Reads the value from the slider and calls the SIGNAL method setMaxOperationTime();.
 */
void UserInterface::on_mMaxOpTimeButton_clicked()
{
    emit setMaxOperationTime(ui->mMaxOpTimeSpinner->value());
}


