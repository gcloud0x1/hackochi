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

void setup()
{
    Serial.begin(115200);
    
    setupSensors();
    setupDisplay();
    setupButtons();
    
    runBootSequence();
    drawMainScreen();
    
    setupNetwork();
    
    Serial.println("HackOchi Terminal Started");
    Serial.println("3-Button Navigation Ready");
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
        }
    }
    
    if (!inMainScreen && currentMillis - previousMillis >= 100)
    {
        previousMillis = currentMillis;
        showGraphScreen();
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