/**
 *  Schulich Delta Central Communication System
 *  Made for the Mbed LPC-1768
 *  By Jordan Heinrichs for the University of Calgary Solar Car Team
 *
 *  Copyright (C) 2015 University of Calgary Solar Car Team
 *
 *  This file is part of the Schulich Delta Central Communication System
 *
 *  The Schulich Delta Central Communication System is free software: 
 *  you can redistribute it and/or modify it under the terms 
 *  of the GNU Affero General Public License as published by 
 *  the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  The Schulich Delta Central Communication System is distributed 
 *  in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 *  without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero 
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General 
 *  Public License along with the Schulich Delta Central Communication 
 *  System. If not, see <http://www.gnu.org/licenses/>.
 *
 *  For further contact, email <software@calgarysolarcar.ca>
 */

#pragma once

#include <Dashboard.h>
#include <DriverControl.h>
#include <LedBmuErrorOutputService.h>
#include <Lights.h>
#include <MotorControllerCan.h>
#include <MpptCan.h>
#include <TelemetryReporting.h>
#include <VehicleData.h>

class Ccs
{
public:
   Ccs();

   void initalizeActions();
   void performActions();

private:
   VehicleData vehicleData_;
   Lights lights_;
   MotorControllerCan motorControllerCan_;
   MpptCan mpptCan_;
   DriverControl driverControl_;
   Dashboard dashboard_;
   TelemetryReporting telemetryReporting_;
   LedBmuErrorOutputService ledBmuErrorOutputService_;

   int telemetryTimer_;
   int displayTimer_;
};
