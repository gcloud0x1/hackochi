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

#define TFT_CS 14
#define TFT_DC 27
#define TFT_RST 33
#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_CLK 18

#define BATTERY_PIN 34
#define VOLTAGE_DIVIDER 2.0
#define MAX_BATTERY_VOLTAGE 4.2
#define MIN_BATTERY_VOLTAGE 3.3
#define BATTERY_CHECK_INTERVAL 30000

#define DHT_PIN 4
#define DHT_TYPE DHT11

// Rotary Encoder Pins (using safe GPIO pins)
#define ENCODER_CLK 32    // Safe GPIO
#define ENCODER_DT 34     // Safe GPIO  
#define ENCODER_SW 21     // Safe GPIO

#define WIFI_SCAN_INTERVAL 10000

#define TERMINAL_GREEN 0x07E0
#define TERMINAL_AMBER 0xFD20
#define TERMINAL_BLUE 0x001F
#define TERMINAL_BG ILI9341_BLACK
#define TERMINAL_HEADER 0x03A0
#define BUTTON_BG 0x18C3
#define BUTTON_SELECTED 0x2FE0

#define MAX_VISIBLE_WIFI_ROWS 13
#define WIFI_TABLE_Y_START 14
#define WIFI_ROW_HEIGHT 18
#define WIFI_VISIBLE_ROWS 13
#define SSID_MAX_CHARS 10
#define RSSI_BAR_MAX_WIDTH 70
#define SSID_COLUMN_WIDTH 160
#define SIGNAL_COLUMN_WIDTH 80
#define ENC_COLUMN_WIDTH 50
#define CH_COLUMN_WIDTH 30

#define MAX_PACKET_HISTORY 180
#define NUM_CHANNELS 14

const int FONT_HEIGHT = 18;
const int FONT_BASELINE = 14;
const int LINE_SPACING = 22;
const int LABEL_WIDTH = 90;
const int BUTTON_HEIGHT = 25;
const int BUTTON_SPACING = 5;

struct WiFiNetworkInfo
{
    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t channel;
    String bssid;
    bool isHidden;
};

extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
extern Adafruit_ILI9341 tft;
extern DHT dht;

extern unsigned long previousMillis;
extern unsigned long lastBatteryCheck;
extern unsigned long lastSensorRead;
extern unsigned long lastEncoderPress;
extern const long interval;
extern const long encoderDebounce;

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

extern String lastWifiScan;
extern int wifiScanResultCount;
extern int wifiScrollOffset;
extern int selectedWifiButton;
extern bool inWifiScreen;
extern bool inWifiScanScreen;
extern WiFiNetworkInfo wifiNetworks[20];

extern int packetHistory[NUM_CHANNELS][MAX_PACKET_HISTORY];
extern int currentChannel;
extern int selectedWaterfallButton;
extern bool inWaterfallScreen;

extern int encoderPos;
extern int lastEncoderPos;
extern bool encoderButtonPressed;
extern int lastEncoderCLK;
extern bool lastButtonState;

extern uint32_t packetCount;
extern uint32_t lastCount;
extern uint32_t maxPps;
extern unsigned long lastPacketTime;
extern int monitorChannel;
extern bool promiscuousModeActive;

void setPromiscuousMode(bool enable, int channel = -1);

#endif