#include <Arduino.h>

//#include "MQ135.h"
//#include "MQ9.h"
#include "dht.h"

#include <SD.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MQ135_PIN A0
#define MQ9_PIN A1
#define DHTPIN A2
#define DHTTYPE DHT22

#define OLED_RESET 5
Adafruit_SSD1306 display(OLED_RESET);

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

float PPM(uint8_t pin, float R0){
  int val = analogRead(pin);
  float resistance = ((1023./(float)val) * 5. - 1.)*1.0;
  return 116.6020682 * pow((resistance/R0), -2.769034857);
}


void setup()
{
  // init
  Serial.begin(9600);

  display.begin(16, 2);
  display.clearDisplay();
  display.display();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();

  // Splash screen
  delay(500);
  display.setCursor(5, 10);
  display.setTextSize(2);
  display.write("Laborky.cz");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();

  //! Fucks up DHT sensor for some reason, I'm losing my fucking mind
  Serial.print("SD card initialization... ");
  SDstatus = SD.begin(10);
  //Serial.println(SDstatus);

  /*if (SDstatus)
  {
    Serial.println(" Done");
    CSVfile = SD.open("output.txt", FILE_WRITE);
  }

  else
  {
    Serial.println(" failed!");
    display.setCursor(1, 10);
    display.setTextSize(1);
    display.write("Nepodarilo se nacist SD kartu");
    display.display();
    while(1);
  }*/
}

void loop()
{
  if (SDstatus)
  {
    // Get values

    Serial.println("Reading sensor data...");

    float ppm_NOX = PPM(MQ135_PIN, 1.18);
    float ppm_CO = PPM(MQ9_PIN, 5.29);

    DHT.read22(DHTPIN);

    float temperature = DHT.temperature;
    float humidity = DHT.humidity;

    Serial.println("Writing data to display...");

    // Display values
    /*display.clearDisplay();
    display.setTextSize(1);

    display.setCursor(26, 0);
    display.print("NOx:");
    display.setCursor(55, 0);
    display.print(ppm_NOX);

    display.setCursor(32, 8);
    display.print("CO:");
    display.setCursor(55, 8);
    display.print(ppm_CO);

    display.setCursor(2, 16);
    display.print("Teplota:");
    display.setCursor(55, 16);
    display.print(temperature);

    display.setCursor(2, 24);
    display.print("Vlhkost:");
    display.setCursor(55, 24);
    display.print(humidity);

    display.display();*/

    Serial.println("-------------------");
    //print raw analog values to console
    Serial.println(ppm_NOX);    
    Serial.println(ppm_CO);
    Serial.println(temperature);
    Serial.println(humidity);
    Serial.println("===================");
    //write data to SD card
    File myFile = SD.open("test.txt", FILE_WRITE);
    myFile.print(ppm_NOX);
    myFile.print(",");
    myFile.print(ppm_CO);
    myFile.print(",");
    myFile.print(temperature);
    myFile.print(",");
    myFile.println(humidity);
    myFile.close();

    Serial.println("Data written to SD card");
    delay(10000);
  }
}