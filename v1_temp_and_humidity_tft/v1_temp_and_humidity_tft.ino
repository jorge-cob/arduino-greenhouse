
#include <Adafruit_BME280.h>
#include "ST7789_AVR.h"
#include <Wire.h>
#include <SPI.h>

/**
 * Set maxTemp, minTemp, maxHumid and minHumid values for the greenhouse
 * They will display in the TFT screen in red if the value is outside of the range
 */

const int maxTemp = 28;
const int minTemp = 22;
const int maxHumid = 99;
const int minHumid = 86;

const int TFT_DC = 10;
const int TFT_RST = 9;

Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

ST7789_AVR display = ST7789_AVR(TFT_DC, TFT_RST); // Initialize display

bool inTempRange, inHumidityRange = false;
float temp = 0;
int humidity = 0;

void setup()
{
  Serial.begin(9600);
  display.init(240, 240); // Init ST7789 240x240
  display.setRotation(2); // Rotate depending on your display positioning
  display.fillScreen(BLACK);
  if (!bme.begin(0x76))
  {
    Serial.println(F("BME280 not found"));
    while (1)
      delay(10);
  }
}

void loop(void)
{
  int currentHumidity = bme.readHumidity();
  float currentTemp = bme.readTemperature();

  if (currentTemp != temp)
  {
    Serial.println("repaint temp");
    inTempRange = currentTemp <= maxTemp && currentTemp >= minTemp;
    uint16_t tempColor = inTempRange ? GREEN : RED;
    display.setCursor(40, 50);
    display.setTextColor(tempColor, BLACK);
    display.setTextSize(5);
    display.print(currentTemp, 1);
    display.drawCircle(168, 55, 4, tempColor);
    display.drawCircle(168, 55, 5, tempColor);
    display.setCursor(180, 50);
    display.print("C");
    temp = currentTemp;
    Serial.println(temp);
  }
  if (currentHumidity != humidity)
  {
    inHumidityRange = currentHumidity <= maxHumid && currentTemp >= minHumid;
    uint16_t humidColor = inHumidityRange ? GREEN : RED;
    humidity = currentHumidity;
    display.setCursor(80, 140);
    display.setTextColor(humidColor, BLACK);
    display.setTextSize(6);
    display.print(currentHumidity);
    display.print("%");
    humidity = currentHumidity;
    Serial.println(humidity);
  }
  delay(2000);
}