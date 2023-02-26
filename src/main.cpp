#include <Arduino.h>

#include "MQ135.h"
#include "MQ9.h"
#include "dht.h"

#include <SdFat.h>
SdFat SD;

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MQ135_PIN A0
#define MQ9_PIN A1
#define DHTPIN A2
#define DHTTYPE DHT22

#define OLED_RESET 5 // idk what this does, it was on 4 by default
Adafruit_SSD1306 display(OLED_RESET);

/*
I2C OLED:
  SDA -> A4
  SCL -> A5

SPI E-INK:
  TBD

SDcard:
  CS -> 10 (4)
  SCK -> 13
  MOSI -> 11
  MISO -> 12
*/

dht DHT;

bool SDstatus;

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

  Serial.print("SD card initialization... ");
  SDstatus = SD.begin(10);
  Serial.print(SDstatus);

  if (SDstatus)
  {
    Serial.println(" Done");
    // File CSVfile;  //! Fucks up DHT sensor for some reason, I'm losing my fucking mind
  }

  else
  {
    Serial.println(" Initialization failed!");
    display.setCursor(1, 10);
    display.setTextSize(1);
    display.write("Nepodarilo se nacist SD kartu");
    display.display();
  }
}

void loop()
{
  if (SDstatus)
  {
    // Get values
    DHT.read22(DHTPIN);
    float temperature = DHT.temperature;
    float humidity = DHT.humidity;

    MQ135 gasSensor135 = MQ135(MQ135_PIN);
    float ppm_NOX = gasSensor135.getPPM();

    MQ9 gasSensor9 = MQ9(MQ9_PIN);
    float ppm_CO = gasSensor9.getPPM();

    String nox = "NOx: ";
    nox += ppm_NOX;
    nox += " ppm";

    String co = "CO: ";
    co += ppm_CO;
    co += " ppm";

    // Format values
    String hum = "Vlhkost: ";
    hum += humidity;
    hum += "%";

    String temp = "Teplota: ";
    temp += temperature;
    temp += "C";

    // Display values
    display.clearDisplay();
    display.setTextSize(1);

    display.setCursor(2, 0);
    display.print(nox);

    display.setCursor(2, 8);
    display.print(co);

    display.setCursor(2, 16);
    display.print(temp);

    display.setCursor(2, 24);
    display.print(hum);

    display.display();

    // Print values to console
    Serial.println(nox);
    Serial.println(co);
    Serial.println(hum);
    Serial.println(temp);
    Serial.println("-------------------");
    Serial.println("===================");

    delay(5000);
  }
}