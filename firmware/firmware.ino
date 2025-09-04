#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Fonts/FreeMono9pt7b.h>

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

// credentials move this to header da
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // UTC+5:30 India :)

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

unsigned long previousMillis = 0;
unsigned long lastBatteryCheck = 0;
const long interval = 1000;
int fakeTemp = 25;
int fakeHumidity = 45;
float batteryVoltage = 0.0;
int batteryPercentage = 100;

// Fonts metrics for FreeMono9pt7b, change it for other fontz
const int FONT_HEIGHT = 18;
const int FONT_BASELINE = 14;
const int LINE_SPACING = 22;
const int LABEL_WIDTH = 90;

#define TERMINAL_GREEN 0x07E0
#define TERMINAL_AMBER 0xFD20
#define TERMINAL_BG ILI9341_BLACK
#define TERMINAL_HEADER 0x03A0

void setCurrentFont() {
  tft.setFont(&FreeMono9pt7b);
  tft.setTextSize(1);
  tft.setTextColor(TERMINAL_GREEN);
}

int getTextWidth(const char *text) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return w;
}

void drawText(const char *text, int x, int y, uint16_t color = TERMINAL_GREEN) {
  setCurrentFont();
  tft.setTextColor(color);
  tft.setCursor(x, y + FONT_BASELINE);
  tft.print(text);
}

void clearArea(int x, int y, int width, int height) {
  tft.fillRect(x, y, width, height, TERMINAL_BG);
}

float readBatteryVoltage() {
  int analogValue = analogRead(BATTERY_PIN);
  
  float voltageAtPin = (analogValue / 4095.0) * 3.3;
  
  float actualVoltage = voltageAtPin * VOLTAGE_DIVIDER;
  
  return actualVoltage;
}

int calculateBatteryPercentage(float voltage) {

  if (voltage >= MAX_BATTERY_VOLTAGE) return 100;
  if (voltage <= MIN_BATTERY_VOLTAGE) return 0;
  
  int percentage = map(voltage * 100, MIN_BATTERY_VOLTAGE * 100, MAX_BATTERY_VOLTAGE * 100, 0, 100);
  return percentage;
}

String getBatteryStatus() {

  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%.1fV (%d%%)", batteryVoltage, batteryPercentage);
  return String(buffer);
}

uint16_t getBatteryColor() {
  if (batteryPercentage < 10) return ILI9341_RED;
  if (batteryPercentage < 30) return TERMINAL_AMBER;
  return TERMINAL_GREEN;
}

void drawTerminalHeader() {
  tft.fillRect(0, 0, tft.width(), 30, TERMINAL_HEADER);
  
  const char* title = "HackOchi v0.3";
  int textWidth = getTextWidth(title);
  drawText(title, (tft.width() - textWidth) / 2, 8, ILI9341_WHITE);
}

void drawStaticLayout() {
  tft.drawRect(0, 0, tft.width(), tft.height(), TERMINAL_GREEN);
  
  int yPos = 40;
  drawText("Time:", 15, yPos);
  drawText("Temp:", 15, yPos + LINE_SPACING);
  drawText("Humi:", 15, yPos + LINE_SPACING * 2);
  drawText("WiFi:", 15, yPos + LINE_SPACING * 3);
  drawText("Battery:", 15, yPos + LINE_SPACING * 4);
  drawText("Memory :", 15, yPos + LINE_SPACING * 5);
  drawText("Uptime :", 15, yPos + LINE_SPACING * 6);
  
  tft.drawFastHLine(10, 35, tft.width()-20, TERMINAL_GREEN);
  tft.drawFastHLine(10, 40 + LINE_SPACING * 7, tft.width()-20, TERMINAL_GREEN);
}

void updateBatteryReadings() {
  batteryVoltage = readBatteryVoltage();
  batteryPercentage = calculateBatteryPercentage(batteryVoltage);
}

void updateDisplay() {
  static String lastTime, lastTemp, lastHumidity, lastBattery, lastHeap, lastUptime, lastWifi;
  
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
  }
  String newTime = timeClient.getFormattedTime();
  
  static unsigned long lastSensorUpdate = 0;
  if (millis() - lastSensorUpdate > 5000) {
    lastSensorUpdate = millis();
    fakeTemp = 25 + random(-2, 3);
    fakeHumidity = 45 + random(-5, 6);
  }
  
  String newTemp = String(fakeTemp) + "C";
  String newHumidity = String(fakeHumidity) + "%";
  
  if (millis() - lastBatteryCheck > BATTERY_CHECK_INTERVAL) {
    lastBatteryCheck = millis();
    updateBatteryReadings();
  }
  String newBattery = getBatteryStatus();
  
  String newHeap = String(ESP.getFreeHeap() / 1024) + "KB";
  String newUptime = String(millis() / 60000) + "m";
  
  String newWifi = WiFi.status() == WL_CONNECTED ? "Connected" : "Offline";
  uint16_t wifiColor = WiFi.status() == WL_CONNECTED ? TERMINAL_GREEN : ILI9341_RED;

  int valueColumn = 15 + LABEL_WIDTH;
  
  if (newTime != lastTime) {
    clearArea(valueColumn, 40, 100, FONT_HEIGHT);
    drawText(newTime.c_str(), valueColumn, 40, TERMINAL_AMBER);
    lastTime = newTime;
  }
  
  if (newTemp != lastTemp) {
    clearArea(valueColumn, 40 + LINE_SPACING, 60, FONT_HEIGHT);
    drawText(newTemp.c_str(), valueColumn, 40 + LINE_SPACING);
    lastTemp = newTemp;
  }
  
  if (newHumidity != lastHumidity) {
    clearArea(valueColumn, 40 + LINE_SPACING * 2, 60, FONT_HEIGHT);
    drawText(newHumidity.c_str(), valueColumn, 40 + LINE_SPACING * 2);
    lastHumidity = newHumidity;
  }
  
  if (newWifi != lastWifi) {
    clearArea(valueColumn, 40 + LINE_SPACING * 3, 100, FONT_HEIGHT);
    drawText(newWifi.c_str(), valueColumn, 40 + LINE_SPACING * 3, wifiColor);
    lastWifi = newWifi;
  }
  
  if (newBattery != lastBattery) {
    clearArea(valueColumn, 40 + LINE_SPACING * 4, 120, FONT_HEIGHT);
    drawText(newBattery.c_str(), valueColumn, 40 + LINE_SPACING * 4, getBatteryColor());
    lastBattery = newBattery;
  }
  
  if (newHeap != lastHeap) {
    clearArea(valueColumn, 40 + LINE_SPACING * 5, 80, FONT_HEIGHT);
    drawText(newHeap.c_str(), valueColumn, 40 + LINE_SPACING * 5);
    lastHeap = newHeap;
  }
  
  if (newUptime != lastUptime) {
    clearArea(valueColumn, 40 + LINE_SPACING * 6, 80, FONT_HEIGHT);
    drawText(newUptime.c_str(), valueColumn, 40 + LINE_SPACING * 6);
    lastUptime = newUptime;
  }
}

void bootSequence() {
  tft.fillScreen(TERMINAL_BG);
  setCurrentFont();
  
  int yPos = 0;
  drawText("HackOchi BIOS v2.1", 15, yPos);
  delay(500);
  
  yPos += LINE_SPACING;
  drawText("CPU: ESP32 @ 240MHz", 15, yPos);
  delay(300);
  
  yPos += LINE_SPACING;
  drawText("RAM: 320KB Test OK", 15, yPos);
  delay(300);
  
  yPos += LINE_SPACING;
  drawText("FLASH: 16MB Ready", 15, yPos);
  delay(300);
  
  yPos += LINE_SPACING;
  drawText("Display: ILI9341 320x240", 15, yPos);
  delay(400);
  
  yPos += LINE_SPACING;
  drawText("Battery: Initializing...", 15, yPos);
  
  updateBatteryReadings();
  delay(300);
  
  yPos += LINE_SPACING;
  clearArea(15, yPos, 200, FONT_HEIGHT);
  char battMsg[30];
  snprintf(battMsg, sizeof(battMsg), "Battery: %.1fV (%d%%)", batteryVoltage, batteryPercentage);
  drawText(battMsg, 15, yPos, getBatteryColor());
  
  yPos += LINE_SPACING;
  drawText("Network: Scanning...", 15, yPos);
  
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    tft.print(".");
    attempts++;
  }
  
  yPos += LINE_SPACING;
  clearArea(15, yPos, 200, FONT_HEIGHT);
  if (WiFi.status() == WL_CONNECTED) {
    drawText("Network: Connected", 15, yPos, TERMINAL_GREEN);
    delay(300);
    
    yPos += LINE_SPACING;
    drawText("Time: Syncing with NTP", 15, yPos);
    timeClient.begin();
    timeClient.forceUpdate();
    
    yPos += LINE_SPACING;
    clearArea(15, yPos, 200, FONT_HEIGHT);
    drawText("Time: Synchronized", 15, yPos, TERMINAL_GREEN);
  } else {
    drawText("Network: Failed", 15, yPos, ILI9341_RED);
    delay(300);
    
    yPos += LINE_SPACING;
    drawText("Offline mode", 15, yPos, TERMINAL_AMBER);
  }
  
  delay(800);
  
  yPos += LINE_SPACING;
  drawText("System Ready >", 15, yPos, TERMINAL_GREEN);
  delay(1200);
}

void setup() {
  Serial.begin(115200);
  
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(BATTERY_PIN, INPUT);
  
  tft.begin();
  tft.setRotation(1);
  setCurrentFont();
  tft.setTextWrap(false);
  
  bootSequence();
  
  tft.fillScreen(TERMINAL_BG);
  
  drawTerminalHeader();
  drawStaticLayout();
  
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.begin();
    timeClient.forceUpdate();
  }
  
  Serial.println("HackOchi Terminal Started");
  Serial.println("Using 10K resistor Vdiv");
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateDisplay();
    
    static unsigned long lastDebug = 0;
    if (currentMillis - lastDebug > 10000) {
      lastDebug = currentMillis;
      Serial.print("Battery: ");
      Serial.print(batteryVoltage, 2);
      Serial.print("V (");
      Serial.print(batteryPercentage);
      Serial.println("%)");
      
      if (batteryPercentage < 15) {
        Serial.println("LOW BATTERY WARNING!");
      }
    }
  }
  
  delay(100);
}