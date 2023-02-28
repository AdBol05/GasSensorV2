//TODO implement E-ink display, I2C OLED not working for when SD card is active some reason

#include <Arduino.h>

#include "dht.h"

#include <SD.h>

#include <SPI.h>
#include <Wire.h>

#define MQ135_PIN A0
#define MQ9_PIN A1
#define DHTPIN A2
#define DHTTYPE DHT22

const int RLED = 9;
const int GLED = 8;

/*
I2C OLED:
  SDA -> A4
  SCL -> A5

SPI E-INK:
  TBD

SDcard:
  CS -> 10
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

  Serial.print("SD card initialization... ");
  SDstatus = SD.begin(10);
  Serial.println(SDstatus);

  if(!SDstatus){
    digitalWrite(RLED, HIGH);
    Serial.println("Failed to load SD card, data will not be written to SD card.");
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

  float ppm_NOX = PPM(MQ135_PIN, 1.18);
  float ppm_CO = PPM(MQ9_PIN, 5.29);

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
  delay(10000);
}