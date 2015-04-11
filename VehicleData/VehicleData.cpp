/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <VehicleData.h>

VehicleData::VehicleData()
: reportedMotorCurrent(0.0f)
, driverSetCurrent(0.0f)
, busCurrent(0.0f)
, busVoltage(0.0f)
, vehicleVelocity(0.0f)
, motorVelocityRpm(0.0f)
, phaseCCurrent(0.0f)
, phaseBCurrent(0.0f)
, motorVoltageReal(0.0f)
, motorVoltageImaginary(0.0f)
, motorCurrentReal(0.0f)
, motorCurrentImaginary(0.0f)
, backEmfImaginary(0.0f)
, ipmHeatSinkTemp(0.0f)
, motorTemp(0.0f)
, dspBoardTemp(0.0f)
, dcBusAmpHours(0.0f)
, odometer(0.0f)
, receivedErrorCount(0)
, transmittedErrorCount(0)
, activeMotor(0)
, errorFlags(0)
, limitFlags(0)
, bmuStatusFlagsExtended(0)
, pc(USBTX, USBRX)
{
}
