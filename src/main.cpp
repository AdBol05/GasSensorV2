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
// #include <SdFat.h>

#define MQ135_PIN A0
#define MQ9_PIN A1
#define DHTPIN A2
#define DHTTYPE DHT22

/*
I2C OLED:
  SDA -> A4
  SCL -> A5

SDcard:
  CS -> 10 (4)
  SCK -> 13
  MOSI -> 11
  MISO -> 12
*/

#define OLED_RESET 5 //idk what this does, it was on 4 by default
Adafruit_SSD1306 display(OLED_RESET);

dht DHT;

File myFile;

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

  //SD card init
  Serial.print("Initializing SD card...");
  if (!SD.begin(10))
  {
    Serial.println("initialization failed!");
    while (1);
  }

  Serial.println("initialization done.");
  myFile = SD.open("test.txt", FILE_WRITE);
}

void loop()
{
  // Get values
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

  DHT.read22(DHTPIN);

  // Format values
  String hum = "Vlhkost: ";
  hum += DHT.humidity;
  hum += "%";

  String temp = "Teplota: ";
  temp += DHT.temperature;
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

  String csv_val = "";
  csv_val += ppm_NOX;
  csv_val += ",";
  csv_val += ppm_CO;
  csv_val += ",";
  csv_val += temp;
  csv_val += ",";
  csv_val += hum;

  Serial.println(csv_val);
  Serial.println("===================");

  delay(5000);
}