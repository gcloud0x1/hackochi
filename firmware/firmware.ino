#include "credentials.h"
#include "config.h"
#include "ui.h"
#include "sensors.h"
#include "networks.h"
#include "boot.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
DHT dht(DHT_PIN, DHT_TYPE);

unsigned long previousMillis = 0;
unsigned long lastBatteryCheck = 0;
unsigned long lastSensorRead = 0;
unsigned long lastButtonPress = 0;
const long interval = 1000;
const long buttonDebounce = 200;

float batteryVoltage = 0.0;
int batteryPercentage = 100;
float temperature = 0.0;
float humidity = 0.0;
int selectedButton = 0;
bool inMainScreen = true;

String lastTime = "";
String lastTemp = "";
String lastHumidity = "";
String lastBattery = "";
String lastHeap = "";
String lastUptime = "";
String lastWifi = "";

String lastWifiScan = "";
int wifiScanResultCount = 0;
int wifiScrollOffset = 0;
int selectedWifiButton = 0;
bool inWifiScreen = false;
bool inWifiScanScreen = false;
WiFiNetworkInfo wifiNetworks[20];

void setup()
{
    Serial.begin(115200);
    Serial.println("Testing WiFi radio...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int testScan = WiFi.scanNetworks(false, true);
    Serial.printf("Initial WiFi scan test returned: %d\n", testScan);
    if (testScan < 0)
    {
        Serial.println("WiFi radio test failed! Check antenna or hardware.");
    }
    else if (testScan == 0)
    {
        Serial.println("No networks found in test scan. Check environment.");
    }
    else
    {
        Serial.printf("Test scan found %d networks. WiFi radio OK.\n", testScan);
    }
    WiFi.scanDelete();
    setupSensors();
    setupDisplay();
    setupButtons();
    runBootSequence();
    drawMainScreen();
    Serial.println("HackOchi Terminal Started");
    Serial.println("4-Button Navigation Ready");
}

void loop()
{
    unsigned long currentMillis = millis();
    if (inMainScreen && currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        updateDisplay();
        static unsigned long lastDebug = 0;
        if (currentMillis - lastDebug > 10000)
        {
            lastDebug = currentMillis;
            printDebugInfo();
            Serial.print("WiFi Status: ");
            Serial.println(WiFi.status());
            Serial.print("Scan Results: ");
            Serial.println(wifiScanResultCount);
        }
    }
    if (!inMainScreen && currentMillis - previousMillis >= 100)
    {
        previousMillis = currentMillis;
        if (inWifiScanScreen)
        {
            updateWifiScanScreen();
        }
        else if (!inWifiScreen)
        {
            showGraphScreen();
        }
    }
    if (digitalRead(BUTTON_LEFT) == LOW)
    {
        handleButtonPress(BUTTON_LEFT);
        delay(300);
    }
    if (digitalRead(BUTTON_RIGHT) == LOW)
    {
        handleButtonPress(BUTTON_RIGHT);
        delay(300);
    }
    if (digitalRead(BUTTON_SELECT) == LOW)
    {
        handleButtonPress(BUTTON_SELECT);
        delay(300);
    }
    delay(50);
}