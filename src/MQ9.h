/**************************************************************************/
/*!
@file     MQ9.h
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3
First version of an Arduino Library for the MQ9 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.
@section  HISTORY
v1.0 - First release
*/
/**************************************************************************/
#ifndef MQ9_H
#define MQ9_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/// The load resistance on the board
#define RLOAD 1.0 //1.0
/// Calibration resistance at atmospheric CO level
#define RZERO 5.29 //1.52 || 5.29
/// Parameters for calculating ppm of CO from sensor resistance
#define PARA 116.6020682
#define PARB 2.769034857

/// Parameters to model temperature and humidity dependence
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018

/// Atmospheric CO level for calibration purposes
#define ATMOCO2 0.06

class MQ9 {
 private:
  uint8_t _pin;

 public:
  MQ9(uint8_t pin);
  float getCorrectionFactor(float t, float h);
  float getResistance();
  float getCorrectedResistance(float t, float h);
  float getPPM();
  float getCorrectedPPM(float t, float h);
  float getRZero();
  float getCorrectedRZero(float t, float h);
};
#endif
