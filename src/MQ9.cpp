/**************************************************************************/
/*!
@file     MQ9.cpp
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3
First version of an Arduino Library for the MQ9 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.
@section  HISTORY
v1.0 - First release
*/
/**************************************************************************/

#include "MQ9.h"

/**************************************************************************/
/*!
@brief  Default constructor
@param[in] pin  The analog input pin for the readout of the sensor
*/
/**************************************************************************/

MQ9::MQ9(uint8_t pin) {
  _pin = pin;
}


/**************************************************************************/
/*!
@brief  Get the correction factor to correct for temperature and humidity
@param[in] t  The ambient air temperature
@param[in] h  The relative humidity
@return The calculated correction factor
*/
/**************************************************************************/
float MQ9::getCorrectionFactor(float t, float h) {
  return CORA_9 * t * t - CORB_9 * t + CORC_9 - (h-33.)*CORD_9;
}

/**************************************************************************/
/*!
@brief  Get the resistance of the sensor, ie. the measurement value
@return The sensor resistance in kOhm
*/
/**************************************************************************/
float MQ9::getResistance() {
  int val = analogRead(_pin);
  return ((1023./(float)val) * 5. - 1.)*RLOAD_9;
}

/**************************************************************************/
/*!
@brief  Get the resistance of the sensor, ie. the measurement value corrected
        for temp/hum
@param[in] t  The ambient air temperature
@param[in] h  The relative humidity
@return The corrected sensor resistance kOhm
*/
/**************************************************************************/
float MQ9::getCorrectedResistance(float t, float h) {
  return getResistance()/getCorrectionFactor(t, h);
}

/**************************************************************************/
/*!
@brief  Get the ppm of CO2 sensed (assuming only CO2 in the air)
@return The ppm of CO2 in the air
*/
/**************************************************************************/
float MQ9::getPPM() {
  return PARA_9 * pow((getResistance()/RZERO_9), -PARB_9);
}

/**************************************************************************/
/*!
@brief  Get the ppm of CO2 sensed (assuming only CO2 in the air), corrected
        for temp/hum
@param[in] t  The ambient air temperature
@param[in] h  The relative humidity
@return The ppm of CO2 in the air
*/
/**************************************************************************/
float MQ9::getCorrectedPPM(float t, float h) {
  return PARA_9 * pow((getCorrectedResistance(t, h)/RZERO_9), -PARB_9);
}

/**************************************************************************/
/*!
@brief  Get the resistance RZero of the sensor for calibration purposes
@return The sensor resistance RZero in kOhm
*/
/**************************************************************************/
float MQ9::getRZero() {
  return getResistance() * pow((ATMOCO_9/PARA_9), (1./PARB_9));
}

/**************************************************************************/
/*!
@brief  Get the corrected resistance RZero of the sensor for calibration
        purposes
@param[in] t  The ambient air temperature
@param[in] h  The relative humidity
@return The corrected sensor resistance RZero in kOhm
*/
/**************************************************************************/
float MQ9::getCorrectedRZero(float t, float h) {
  return getCorrectedResistance(t, h) * pow((ATMOCO_9/PARA_9), (1./PARB_9));
}
