#include <Arduino.h>

#include "MQ135.h"
#include "MQ9.h"
#include "dht.h"

#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <SdFat.h>

#define MQ135_PIN A0
#define MQ9_PIN A1
#define DHTPIN A2
#define DHTTYPE DHT22

#define OLED_RESET 5 //idk what this does, it was on 4 by default
Adafruit_SSD1306 display(OLED_RESET);

const int chipSelect = 4;

File myFile;

dht DHT;

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
  if (!SD.begin(4))
  {
    Serial.println("initialization failed!");
    while (1);
  }

  Serial.println("initialization done.");
  myFile = SD.open("test.txt", FILE_WRITE);

  //write header ti file
  /*if (myFile)
  {
    Serial.print("Writing to test.txt...");
    myFile.println("CO,NOx,Temperature,Humidity");
    myFile.close();
    Serial.println("done.");
  }
  else
  {
    Serial.println("error opening test.txt");
  }

  if (!sd.begin(chipSelect, SPI_HALF_SPEED)){
    sd.initErrorHalt();
    Serial.println("Error initializing SD card");
  }

  if (!myFile.open("output.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("Opening output.txt for write failed");
  }

  Serial.println("Writing header to output.txt...");
  myFile.println("CO,NOx,Temperature,Humidity");
  myFile.close();
  Serial.println("done.");*/
}

void loop()
{
  // Get values
  MQ135 gasSensor135 = MQ135(MQ135_PIN);
  float ppm_NOX = gasSensor135.getPPM();
  float rz135 = gasSensor135.getRZero();

  MQ9 gasSensor9 = MQ9(MQ9_PIN);
  float ppm_CO = gasSensor9.getPPM();
  float rz9 = gasSensor9.getRZero();

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
  String rzero9 = "RZero MQ9: ";
  rzero9 += rz9;
  String rzero135 = "RZero MQ135: ";
  rzero135 += rz135;
  Serial.println(rzero9);
  Serial.println(rzero135);
  Serial.println("===================");

  delay(5000);
}