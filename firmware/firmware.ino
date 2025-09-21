#include "credentials.h"
#include "config.h"
#include "ui.h"
#include "sensors.h"
#include "networks.h"
#include "boot.h"
#include <esp_wifi.h>
#include <esp_wifi_types.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
DHT dht(DHT_PIN, DHT_TYPE);

unsigned long previousMillis = 0;
unsigned long lastBatteryCheck = 0;
unsigned long lastSensorRead = 0;
unsigned long lastEncoderPress = 0;
const long interval = 1000;
const long encoderDebounce = 200;

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

int packetHistory[NUM_CHANNELS][MAX_PACKET_HISTORY] = {0};
int currentChannel = 6;
int selectedWaterfallButton = 0;
bool inWaterfallScreen = false;

uint32_t packetCount = 0;
uint32_t lastCount = 0;
uint32_t maxPps = 0;
unsigned long lastPacketTime = 0;
int monitorChannel = 6;
bool promiscuousModeActive = false;

int encoderPos = 0;
int lastEncoderPos = 0;
bool encoderButtonPressed = false;
int lastEncoderCLK = HIGH;
bool lastButtonState = HIGH;

void ICACHE_RAM_ATTR sniffer_callback(void* buf, wifi_promiscuous_pkt_type_t type)
{
    packetCount++;
}

void setPromiscuousMode(bool enable, int channel)
{
    if (enable == promiscuousModeActive)
    {
        if (enable && channel != monitorChannel)
        {
            monitorChannel = channel;
            esp_wifi_set_channel(monitorChannel, WIFI_SECOND_CHAN_NONE);
            Serial.printf("Switched promiscuous mode to channel %d\n", monitorChannel);
        }
        return;
    }

    if (enable)
    {
        Serial.printf("Enabling promiscuous mode on channel %d\n", channel);
        promiscuousModeActive = true;
        monitorChannel = channel;
        esp_wifi_stop();
        esp_wifi_deinit();
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        esp_wifi_init(&cfg);
        esp_wifi_set_storage(WIFI_STORAGE_RAM);
        esp_wifi_set_mode(WIFI_MODE_NULL);
        esp_wifi_start();
        esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
        esp_wifi_set_promiscuous_rx_cb(&sniffer_callback);
        esp_wifi_set_promiscuous(true);
        packetCount = 0;
        lastCount = 0;
    }
    else
    {
        Serial.println("Disabling promiscuous mode and returning to STA mode");
        promiscuousModeActive = false;
        esp_wifi_set_promiscuous(false);
        esp_wifi_stop();
        esp_wifi_deinit();
        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        esp_wifi_init(&cfg);
        esp_wifi_set_storage(WIFI_STORAGE_RAM);
        esp_wifi_set_mode(WIFI_MODE_STA);
        esp_wifi_start();
        WiFi.begin(ssid, password);
    }
}

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
    setupEncoder();
    runBootSequence();
    drawMainScreen();
    Serial.println("HackOchi Terminal Started");
    Serial.println("Rotary Encoder Navigation Ready");
}

void loop()
{
    unsigned long currentMillis = millis();
    
    int currentCLK = digitalRead(ENCODER_CLK);
    if (lastEncoderCLK != currentCLK)
    {
        lastEncoderCLK = currentCLK;
        if (currentCLK == LOW)
        {
            if (digitalRead(ENCODER_DT) == HIGH)
            {
                encoderPos--;
            }
            else
            {
                encoderPos++;
            }
        }
    }
    
    if (encoderPos != lastEncoderPos)
    {
        if (encoderPos > lastEncoderPos)
        {
            handleEncoderRotation(1);
        }
        else
        {
            handleEncoderRotation(-1);
        }
        lastEncoderPos = encoderPos;
    }
    
    bool currentButtonState = digitalRead(ENCODER_SW);
    if (currentButtonState == LOW && lastButtonState == HIGH)
    {
        if (millis() - lastEncoderPress > encoderDebounce)
        {
            handleEncoderButton();
            lastEncoderPress = millis();
        }
    }
    lastButtonState = currentButtonState;
    
    if (inMainScreen)
    {
        if (currentMillis - previousMillis >= interval)
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
    }
    else if (inWifiScanScreen)
    {
        if (currentMillis - previousMillis >= 100)
        {
            previousMillis = currentMillis;
            updateWifiScanScreen();
        }
    }
    else if (inWaterfallScreen)
    {
        if (currentMillis - lastPacketTime >= 1000)
        {
            lastPacketTime = currentMillis;
            updatePacketWaterfall();
            updateWaterfallScreen();
        }
    }
    else if (!inWifiScreen)
    {
        if (currentMillis - previousMillis >= 100)
        {
            previousMillis = currentMillis;
            showGraphScreen();
        }
    }
    
    delay(10);
}