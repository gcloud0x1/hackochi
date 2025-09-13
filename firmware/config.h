#ifndef CONFIG_H
#define CONFIG_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Fonts/FreeMono9pt7b.h>
#include <DHT.h>

#define TFT_CS   14
#define TFT_DC   27
#define TFT_RST  33
#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_CLK  18

#define BATTERY_PIN 34
#define VOLTAGE_DIVIDER 2.0
#define MAX_BATTERY_VOLTAGE 4.2
#define MIN_BATTERY_VOLTAGE 3.3
#define BATTERY_CHECK_INTERVAL 30000

#define DHT_PIN 4
#define DHT_TYPE DHT11

#define BUTTON_LEFT 13
#define BUTTON_RIGHT 12
#define BUTTON_SELECT 15

#define TERMINAL_GREEN 0x07E0
#define TERMINAL_AMBER 0xFD20
#define TERMINAL_BLUE 0x001F
#define TERMINAL_BG ILI9341_BLACK
#define TERMINAL_HEADER 0x03A0
#define BUTTON_BG 0x18C3
#define BUTTON_SELECTED 0x2FE0

const int FONT_HEIGHT = 18;
const int FONT_BASELINE = 14;
const int LINE_SPACING = 22;
const int LABEL_WIDTH = 90;
const int BUTTON_HEIGHT = 25;
const int BUTTON_SPACING = 5;

extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
extern Adafruit_ILI9341 tft;
extern DHT dht;

extern unsigned long previousMillis;
extern unsigned long lastBatteryCheck;
extern unsigned long lastSensorRead;
extern unsigned long lastButtonPress;
extern const long interval;
extern const long buttonDebounce;

extern float batteryVoltage;
extern int batteryPercentage;
extern float temperature;
extern float humidity;
extern int selectedButton;
extern bool inMainScreen;

extern String lastTime;
extern String lastTemp;
extern String lastHumidity;
extern String lastBattery;
extern String lastHeap;
extern String lastUptime;
extern String lastWifi;

#endif