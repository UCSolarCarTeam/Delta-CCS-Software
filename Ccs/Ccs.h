#pragma once
/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <VehicleData.h>
#include <TelemetryReporting.h>
#include <CanInterface.h>
#include <DriverControl.h>
#include <Dashboard.h>
#include <Lights.h>
#include <LedBmuErrorOutputService.h>

class Ccs
{
public:
   Ccs();

   void initalizeActions();
   void performActions();

private:
   VehicleData vehicleData_;
   Lights lights_;
   CanInterface canInterface_;
   DriverControl driverControl_;
   Dashboard dashboard_;
   TelemetryReporting telemetryReporting_;
   LedBmuErrorOutputService ledBmuErrorOutputService_;

   int telemetryTimer_;
   int displayTimer_;
};
