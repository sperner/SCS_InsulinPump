//
// File: Scheduler.h
//
// Date: 07.01.15 16:54
//
// Description:  main() routine of insulin pump simulation
//
// Author:       Sven Sperner, sillyconn@gmail.com

#include <QApplication>
#include "UserInterface.h"

/**
 * Initiation of the Userinterface, Humanbody- and Insulinpumpsimulation.
 *
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    // Create User Interface
    QApplication application(argc, argv);
    UserInterface window;

    // Show UI
    window.show();

    return application.exec();
}
