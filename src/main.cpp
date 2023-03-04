//TODO implement E-ink display, I2C OLED not working when SD card is active for some reason (mby memory issues?)

#include <Arduino.h>

#include "dht.h"

#include <SD.h>

#include <SPI.h>
#include <Wire.h>

#define MQ135_PIN A0
#define MQ9_PIN A1
#define DHTPIN A2
#define DHTTYPE DHT22
#define SD_CS 4

const int RLED = 3;
const int GLED = 2;

/*
I2C OLED:
  SDA -> A4
  SCL -> A5

SPI E-INK: (epd2in9b_V3)
  DIN -> 11
  CLK -> 13
  CS -> 10
  DC -> 9
  RST -> 8
  BUSY -> 7

SDcard:
  CS -> 4
  SCK -> 13
  MOSI -> 11
  MISO -> 12
*/

dht DHT;

bool SDstatus;

float PPM(uint8_t pin, float R0)
{
  int val = analogRead(pin);
  float resistance = ((1023. / (float)val) * 5. - 1.) * 1.0;
  return 116.6020682 * pow((resistance / R0), -2.769034857);
}

void setup()
{

  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);

  // init
  Serial.begin(9600);

  Serial.println("============");
  Serial.println("Laborky.cz");
  Serial.println("GasSensorV2");
  Serial.println("===========");

  delay(1000);

  Serial.print("SD card initialization... ");
  SDstatus = SD.begin(SD_CS);
  Serial.println(SDstatus);

  if(!SDstatus){
    digitalWrite(RLED, HIGH);
    Serial.println("Failed to load SD card, data logging is disabled.");
  }
  else{
    Serial.println("SD card initialized successfully");
    digitalWrite(RLED, LOW);
  }
}

void loop()
{
  // Get values
  Serial.println("Reading sensor data...");

  float ppm_NOX = PPM(MQ135_PIN, 1.18); //* 1.18  0.8
  float ppm_CO = PPM(MQ9_PIN, 5.29);    //* 5.29  1.52

  DHT.read22(DHTPIN);

  float temperature = DHT.temperature;
  float humidity = DHT.humidity;

  Serial.println("-------------------");
  // print values to console
  Serial.println(ppm_NOX);
  Serial.println(ppm_CO);
  Serial.println(temperature);
  Serial.println(humidity);
  Serial.println("===================");

  // write data to SD card
  if (SDstatus)
  {
    digitalWrite(GLED, HIGH);
    delay(100);

    File myFile = SD.open("data.txt", FILE_WRITE);
    myFile.print(ppm_NOX);
    myFile.print(",");
    myFile.print(ppm_CO);
    myFile.print(",");
    myFile.print(temperature);
    myFile.print(",");
    myFile.println(humidity);
    myFile.close();

    Serial.println("Data written to SD card");
    delay(100);
    digitalWrite(GLED, LOW);
  }
  else{
    Serial.println("Unable to write data to SD card");
  }

  //TODO: write data to E-Ink display

  delay(10000);
}