#ifndef UI_H
#define UI_H

#include "config.h"
#include "sensors.h"
#include "networks.h"
#include "bluetooth.h"
#include "obits.h"
#include <Arduino.h>

int moveMacTextUpSukiCriesHard = 8;
int petFrame = 0;
unsigned long lastFrameUpdate = 0;
unsigned long lastTextUpdate = 0;
String currentMoodText = "";
unsigned long lastMoodChangeTime = 0;
unsigned long lastInteractionTime = 0;
const unsigned long BORED_TIMEOUT = 7000;
const unsigned long SLEEP_TIMEOUT = 60000;
unsigned long specialMoodEndTime = 0;
PetMood moodBeforeSpecial;
bool wasConnected = false;

int currentOpenCount = 0;
int currentClosedCount = 0;
int totalNetworksFoundSinceEntry = 0;
int scanSessionCount = 0;
unsigned long nextScanTime = 0;
const unsigned long SCAN_INTERVAL = 8000;

unsigned long lastAnimationTime = 0;
const unsigned long ANIMATION_INTERVAL = 200;  // Pulse every 200ms
int pulseState = 0;  // For simple pulsing animation (0-3)

void setCurrentFont();
void setSmallFont();
void setTinyFont();
int getTextWidth(const char* text);
int getSmallTextWidth(const char* text);
int getTinyTextWidth(const char* text);
void drawText(const char* text, int x, int y, uint16_t color = TERMINAL_GREEN);
void drawSmallText(const char* text, int x, int y, uint16_t color = TERMINAL_GREEN);
void drawTinyText(const char* text, int x, int y, uint16_t color = TERMINAL_GREEN);
void clearArea(int x, int y, int width, int height);
void drawButton(const char* text, int x, int y, int width, bool selected = false);
void drawButtons();
void drawTerminalHeader();
void drawMainScreen();
void showTemperatureScreen();
void showHumidityScreen();
void showGraphScreen();
void handleEncoderRotation(int direction);
void handleEncoderButton();
void updateDisplay();
void showWifiScreen();
void showWifiScanScreen();
void updateWifiScanScreen();
void drawWifiButtons();
void drawScanningIndicator(const char* text, int x, int y, int durationMs);
void drawWifiTable();
void drawScrollIndicators();
void drawControlsText();
void updateWifiScreen();
void showWaterfallScreen();
void drawWaterfallHeader();
void drawWaterfallGraph(int x, int y, int width, int height);
void drawWaterfallButtons();
void updateWaterfallScreen();
void showBleScreen();
void drawBleButtons();
void showBleScanScreen();
void drawBleTable();
void updateBleScanScreen();
void showClassicScanScreen();
void updateClassicScanScreen();
void drawClassicTable();
void showBleGraphScreen();
void drawBleWaterfallHeader();
void drawBleWaterfallGraph(int x, int y, int width, int height);
void drawBleWaterfallButtons();
void updateBleWaterfallScreen();
void showPetScreen();
void updatePetScreen();
void drawPet(PetMood mood);
void drawPetPopup();

void setCurrentFont()
{
    tft.setFont(&FreeMono9pt7b);
    tft.setTextSize(1);
    tft.setTextColor(TERMINAL_GREEN);
}

void setSmallFont()
{
    tft.setFont(&FreeMono9pt7b);
    tft.setTextSize(0);
    tft.setTextColor(TERMINAL_GREEN);
}

void setTinyFont()
{
    tft.setFont();
    tft.setTextSize(0);
    tft.setTextColor(TERMINAL_GREEN);
}

int getTextWidth(const char* text)
{
    setCurrentFont();
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    return w;
}

int getSmallTextWidth(const char* text)
{
    setSmallFont();
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    setCurrentFont();
    return w;
}

int getTinyTextWidth(const char* text)
{
    setTinyFont();
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    setCurrentFont();
    return w;
}

void drawText(const char* text, int x, int y, uint16_t color)
{
    setCurrentFont();
    tft.setTextColor(color);
    tft.setCursor(x, y + FONT_BASELINE);
    tft.print(text);
}

void drawSmallText(const char* text, int x, int y, uint16_t color)
{
    setSmallFont();
    tft.setTextColor(color);
    tft.setCursor(x, y + 6);
    tft.print(text);
    setCurrentFont();
}

void drawTinyText(const char* text, int x, int y, uint16_t color)
{
    setTinyFont();
    tft.setTextColor(color);
    tft.setCursor(x, y + 4);
    tft.print(text);
    setCurrentFont();
}

void clearArea(int x, int y, int width, int height)
{
    tft.fillRect(x, y, width, height, TERMINAL_BG);
}

void drawButton(const char* text, int x, int y, int width, bool selected)
{
    uint16_t bgColor = selected ? BUTTON_SELECTED : BUTTON_BG;
    uint16_t textColor = selected ? ILI9341_BLACK : TERMINAL_GREEN;
    tft.fillRoundRect(x, y, width, BUTTON_HEIGHT, 4, bgColor);
    if (selected)
    {
        tft.drawRoundRect(x - 1, y - 1, width + 2, BUTTON_HEIGHT + 2, 4, TERMINAL_GREEN);
        tft.drawRoundRect(x, y, width, BUTTON_HEIGHT, 4, TERMINAL_GREEN);
    }
    else
    {
        tft.drawRoundRect(x, y, width, BUTTON_HEIGHT, 4, TERMINAL_GREEN);
    }
    int textWidth = getSmallTextWidth(text);
    int textX = x + (width - textWidth) / 2;
    drawSmallText(text, textX, y + 8, textColor);
}

void drawButtons()
{
    int buttonY = 40 + LINE_SPACING * 7 + 10;
    int buttonWidth = (tft.width() - 20 - BUTTON_SPACING * 3) / 4;
    tft.fillRect(8, buttonY - 2, tft.width() - 16, BUTTON_HEIGHT + 4, TERMINAL_BG);

    const char* buttonTexts[] = {"Temp", "Humi", "Graph", "WiFi", "BLE", "Pet"};
    int numButtons = 6;
    int maxVisibleButtons = 4;
    
    int startButton = 0;
    if (selectedButton >= maxVisibleButtons)
    {
        startButton = selectedButton - (maxVisibleButtons - 1);
    }

    for (int i = 0; i < maxVisibleButtons; i++)
    {
        int buttonIndex = startButton + i;
        if (buttonIndex < numButtons)
        {
            int xPos = 10 + (buttonWidth + BUTTON_SPACING) * i;
            drawButton(buttonTexts[buttonIndex], xPos, buttonY, buttonWidth, selectedButton == buttonIndex);
        }
    }
}

void drawTerminalHeader()
{
    tft.fillRect(0, 0, tft.width(), 30, TERMINAL_HEADER);
    const char* title = "HackOchi v1.0";
    int textWidth = getTextWidth(title);
    drawText(title, (tft.width() - textWidth) / 2, 8, ILI9341_WHITE);
}

void drawMainScreen()
{
    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    int yPos = 40;
    drawText("Time:", 15, yPos);
    drawText("Temp:", 15, yPos + LINE_SPACING);
    drawText("Humi:", 15, yPos + LINE_SPACING * 2);
    drawText("WiFi:", 15, yPos + LINE_SPACING * 3);
    drawText("Battery:", 15, yPos + LINE_SPACING * 4);
    drawText("Memory :", 15, yPos + LINE_SPACING * 5);
    drawText("Uptime :", 15, yPos + LINE_SPACING * 6);
    tft.drawFastHLine(10, 35, tft.width() - 20, TERMINAL_GREEN);
    tft.drawFastHLine(10, 40 + LINE_SPACING * 7, tft.width() - 20, TERMINAL_GREEN);
    drawButtons();
    inMainScreen = true;
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = false;
    inBleScanScreen = false;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;
}


void showTemperatureScreen()
{
    inMainScreen = false;
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = false;
    inBleScanScreen = false;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;
    
    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    int titleWidth = getTextWidth("TEMPERATURE DASHBOARD");
    drawText("TEMPERATURE DASHBOARD", (tft.width() - titleWidth) / 2, 40, TERMINAL_AMBER);
    tft.drawFastHLine(10, 60, tft.width() - 20, TERMINAL_GREEN);

    int gaugeX = 100, gaugeY = 145, radius = 70;
    int minTemp = 0, maxTemp = 60;

    for (int angle = -120; angle <= 120; angle += 2)
    {
        float rad = angle * DEG_TO_RAD;
        int x1 = gaugeX + cos(rad) * (radius - 12);
        int y1 = gaugeY + sin(rad) * (radius - 12);
        int x2 = gaugeX + cos(rad) * radius;
        int y2 = gaugeY + sin(rad) * radius;
        
        uint16_t color;
        if (angle < -60)
        {
            color = tft.color565(0, 0, map(angle, -120, -61, 255, 100));
        }
        else if (angle < 60)
        {
            color = TERMINAL_GREEN;
        }
        else
        {
            color = tft.color565(map(angle, 60, 120, 200, 255), map(angle, 60, 120, 100, 0), 0);
        }
        
        tft.drawLine(x1, y1, x2, y2, color);
    }
    
    float tempAngle = map(constrain(temperature, minTemp, maxTemp), minTemp, maxTemp, -120, 120) * DEG_TO_RAD;
    int nx = gaugeX + cos(tempAngle) * (radius - 8);
    int ny = gaugeY + sin(tempAngle) * (radius - 8);
    tft.drawLine(gaugeX, gaugeY, nx, ny, ILI9341_WHITE);
    tft.drawLine(gaugeX+1, gaugeY, nx+1, ny, ILI9341_WHITE);
    tft.fillCircle(gaugeX, gaugeY, 4, ILI9341_WHITE);

    float leftRad = -120 * DEG_TO_RAD;
    int leftLabelX = gaugeX + cos(leftRad) * (radius + 2) - 22;
    int leftLabelY = gaugeY + sin(leftRad) * (radius + 2) - 2;
    drawTinyText("0C", leftLabelX, leftLabelY, TERMINAL_BLUE);
    
    float rightRad = 120 * DEG_TO_RAD;
    int rightLabelX = gaugeX + cos(rightRad) * (radius + 2) - getTinyTextWidth("60C") - 2;
    int rightLabelY = gaugeY + sin(rightRad) * (radius + 2) - 2;
    drawTinyText("60C", rightLabelX, rightLabelY, ILI9341_RED);
    
    int panelX = 200, panelY = 80, panelW = 100, panelH = 50;
    int cornerRadius = 8;
    
    tft.fillRoundRect(panelX, panelY, panelW, panelH, cornerRadius, TERMINAL_BG);
    tft.drawRoundRect(panelX, panelY, panelW, panelH, cornerRadius, TERMINAL_GREEN);
    int labelWidth = getTinyTextWidth("Current Reading");
    drawTinyText("Current Reading", panelX + (panelW - labelWidth) / 2, panelY + 5, TERMINAL_AMBER);
    char tempStr[10];
    snprintf(tempStr, sizeof(tempStr), "%.1f C", temperature);
    int tempWidth = getTextWidth(tempStr);
    drawText(tempStr, panelX + (panelW - tempWidth) / 2, panelY + 25, ILI9341_WHITE);

    const char* status;
    if (temperature < 18)
        status = "Cold";
    else if (temperature > 28)
        status = "Hot";
    else
        status = "Comfortable";
    tft.fillRoundRect(panelX, panelY + 55, panelW, panelH, cornerRadius, TERMINAL_BG);
    tft.drawRoundRect(panelX, panelY + 55, panelW, panelH, cornerRadius, TERMINAL_GREEN);
    labelWidth = getTinyTextWidth("Status");
    drawTinyText("Status", panelX + (panelW - labelWidth) / 2, panelY + 60, TERMINAL_AMBER);
    int statusWidth = getTextWidth(status);
    drawText(status, panelX + (panelW - statusWidth) / 2, panelY + 80, ILI9341_WHITE);
    
    char minMaxStr[24];
    snprintf(minMaxStr, sizeof(minMaxStr), "Avg Min: 20.0C Avg Max: 35.0C");
    int minMaxWidth = getSmallTextWidth(minMaxStr);
    drawSmallText(minMaxStr, (tft.width() - minMaxWidth) / 2, 230, ILI9341_WHITE);
}

void showHumidityScreen()
{
    inMainScreen = false;
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = false;
    inBleScanScreen = false;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;

    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    int titleWidth = getTextWidth("HUMIDITY DASHBOARD");
    drawText("HUMIDITY DASHBOARD", (tft.width() - titleWidth) / 2, 40, TERMINAL_BLUE);
    tft.drawFastHLine(10, 60, tft.width() - 20, TERMINAL_GREEN);

    int gaugeX = 100, gaugeY = 145, radius = 70;
    int minHum = 0, maxHum = 100;

    for (int angle = -120; angle <= 120; angle += 2)
    {
        float rad = angle * DEG_TO_RAD;
        int x1 = gaugeX + cos(rad) * (radius - 12);
        int y1 = gaugeY + sin(rad) * (radius - 12);
        int x2 = gaugeX + cos(rad) * radius;
        int y2 = gaugeY + sin(rad) * radius;
        
        uint16_t color;
        if (angle < -60)
        {
            color = tft.color565(map(angle, -120, -61, 255, 150), map(angle, -120, -61, 165, 100), 0);
        }
        else if (angle < 60)
        {
            color = TERMINAL_BLUE;
        }
        else
        {
            color = tft.color565(map(angle, 60, 120, 0, 100), 0, map(angle, 60, 120, 255, 100));
        }
        
        tft.drawLine(x1, y1, x2, y2, color);
    }
    
    float humAngle = map(constrain(humidity, minHum, maxHum), minHum, maxHum, -120, 120) * DEG_TO_RAD;
    int nx = gaugeX + cos(humAngle) * (radius - 8);
    int ny = gaugeY + sin(humAngle) * (radius - 8);
    tft.drawLine(gaugeX, gaugeY, nx, ny, ILI9341_WHITE);
    tft.drawLine(gaugeX+1, gaugeY, nx+1, ny, ILI9341_WHITE);
    tft.fillCircle(gaugeX, gaugeY, 4, ILI9341_WHITE);

    float leftRad = -120 * DEG_TO_RAD;
    int leftLabelX = gaugeX + cos(leftRad) * (radius + 2) - 22;
    int leftLabelY = gaugeY + sin(leftRad) * (radius + 2) - 2;
    drawTinyText("0%", leftLabelX, leftLabelY, TERMINAL_AMBER);
    
    float rightRad = 120 * DEG_TO_RAD;
    int rightLabelX = gaugeX + cos(rightRad) * (radius + 2) - getTinyTextWidth("100%") - 2;
    int rightLabelY = gaugeY + sin(rightRad) * (radius + 2) - 2;
    drawTinyText("100%", rightLabelX, rightLabelY, ILI9341_MAGENTA);
    
    int panelX = 200, panelY = 80, panelW = 100, panelH = 50;
    int cornerRadius = 8;
    
    tft.fillRoundRect(panelX, panelY, panelW, panelH, cornerRadius, TERMINAL_BG);
    tft.drawRoundRect(panelX, panelY, panelW, panelH, cornerRadius, TERMINAL_GREEN);
    int labelWidth = getTinyTextWidth("Current Reading");
    drawTinyText("Current Reading", panelX + (panelW - labelWidth) / 2, panelY + 5, TERMINAL_AMBER);
    char humStr[10];
    snprintf(humStr, sizeof(humStr), "%.0f %%", humidity);
    int humWidth = getTextWidth(humStr);
    drawText(humStr, panelX + (panelW - humWidth) / 2, panelY + 25, ILI9341_WHITE);

    const char* status;
    if (humidity < 30)
        status = "Dry";
    else if (humidity > 70)
        status = "Humid";
    else
        status = "Comfortable";
    tft.fillRoundRect(panelX, panelY + 55, panelW, panelH, cornerRadius, TERMINAL_BG);
    tft.drawRoundRect(panelX, panelY + 55, panelW, panelH, cornerRadius, TERMINAL_GREEN);
    labelWidth = getTinyTextWidth("Status");
    drawTinyText("Status", panelX + (panelW - labelWidth) / 2, panelY + 60, TERMINAL_AMBER);
    int statusWidth = getTextWidth(status);
    drawText(status, panelX + (panelW - statusWidth) / 2, panelY + 80, ILI9341_WHITE);
    
    char minMaxStr[24];
    snprintf(minMaxStr, sizeof(minMaxStr), "Avg Min: 30%% Avg Max: 80%%");
    int minMaxWidth = getSmallTextWidth(minMaxStr);
    drawSmallText(minMaxStr, (tft.width() - minMaxWidth) / 2, 230, ILI9341_WHITE);
}

void showGraphScreen()
{
    static bool firstRun = true;
    static unsigned long lastUpdate = 0;
    static const int numPoints = 30;
    static float tempData[30] = {0};
    static float humData[30] = {0};
    static int dataCount = 0;
    const int graphWidth = tft.width() - 2;
    const int graphHeight = (tft.height() - 30) / 2 - 2;
    const int tempGraphY = 30 + 1;
    const int humGraphY = 30 + graphHeight + 3;
    const uint16_t gridColor = 0x0420;

    if (firstRun)
    {
        inMainScreen = false;
        inWifiScreen = false;
        inWifiScanScreen = false;
        inWaterfallScreen = false;
        inBleScreen = false;
        inBleScanScreen = false;
        inClassicScanScreen = false;
        inGraphScreen = true;
        inBleWaterfallScreen = false;
        inPetScreen = false;
        tft.fillScreen(TERMINAL_BG);
        drawTerminalHeader();
        for (int i = 0; i < numPoints; i++)
        {
            tempData[i] = 25.0;
            humData[i] = 50.0;
        }
        readDHTData();
        if (!isnan(temperature) && temperature >= 0 && temperature <= 100)
        {
            tempData[0] = temperature;
        }
        if (!isnan(humidity) && humidity >= 0 && humidity <= 100)
        {
            humData[0] = humidity;
        }
        dataCount = 1;
        firstRun = false;
    }
    if (millis() - lastUpdate >= 3000)
    {
        readDHTData();
        float newTemp = temperature;
        float newHum = humidity;
        if (isnan(newTemp) || newTemp < 0 || newTemp > 100)
        {
            newTemp = tempData[(dataCount - 1) % numPoints];
        }
        if (isnan(newHum) || newHum < 0 || newHum > 100)
        {
            newHum = humData[(dataCount - 1) % numPoints];
        }
        if (dataCount < numPoints)
        {
            tempData[dataCount] = newTemp;
            humData[dataCount] = newHum;
            dataCount++;
        }
        else
        {
            for (int i = 0; i < numPoints - 1; i++)
            {
                tempData[i] = tempData[i + 1];
                humData[i] = humData[i + 1];
            }
            tempData[numPoints - 1] = newTemp;
            humData[numPoints - 1] = newHum;
        }
        lastUpdate = millis();
    }
    clearArea(1, tempGraphY, graphWidth, graphHeight);
    clearArea(1, humGraphY, graphWidth, graphHeight);
    float tempMin = tempData[0];
    float tempMax = tempData[0];
    for (int i = 1; i < dataCount && i < numPoints; i++)
    {
        if (tempData[i] < tempMin) tempMin = tempData[i];
        if (tempData[i] > tempMax) tempMax = tempData[i];
    }
    tempMin -= 5.0;
    tempMax += 5.0;
    if (tempMax - tempMin < 10.0)
    {
        tempMax = tempMin + 10.0;
    }
    tft.drawFastHLine(0, tempGraphY - 1, tft.width(), TERMINAL_GREEN);
    tft.drawFastHLine(0, tempGraphY + graphHeight, tft.width(), TERMINAL_GREEN);
    tft.drawFastVLine(0, tempGraphY - 1, graphHeight + 2, TERMINAL_GREEN);
    tft.drawFastVLine(tft.width() - 1, tempGraphY - 1, graphHeight + 2, TERMINAL_GREEN);
    for (int i = 1; i <= 3; i++)
    {
        int y = tempGraphY + (graphHeight * i) / 4;
        tft.drawFastHLine(1, y, graphWidth, gridColor);
    }
    for (int i = 1; i <= 4; i++)
    {
        int x = 1 + (graphWidth * i) / 5;
        tft.drawFastVLine(x, tempGraphY, graphHeight, gridColor);
    }
    char label[10];
    snprintf(label, sizeof(label), "%.0fC", tempMax);
    drawText(label, 2, tempGraphY - 5, TERMINAL_GREEN);
    snprintf(label, sizeof(label), "%.0fC", (tempMax + tempMin) / 2);
    drawText(label, 2, tempGraphY + graphHeight / 2 - 5, TERMINAL_GREEN);
    snprintf(label, sizeof(label), "%.0fC", tempMin);
    drawText(label, 2, tempGraphY + graphHeight - 5, TERMINAL_GREEN);
    snprintf(label, sizeof(label), "%.1fC", tempData[(dataCount - 1) % numPoints]);
    int textWidth = getTextWidth(label);
    drawText(label, graphWidth - textWidth - 2, tempGraphY - 5, TERMINAL_GREEN);
    float xStep = (float)graphWidth / (numPoints - 1);
    for (int i = 0; i < dataCount - 1 && i < numPoints - 1; i++)
    {
        float temp1 = tempData[i];
        float temp2 = tempData[i + 1];
        int x1 = 1 + (int)(i * xStep);
        int x2 = 1 + (int)((i + 1) * xStep);
        int y1 = tempGraphY + graphHeight - (int)(map(temp1 * 10, tempMin * 10, tempMax * 10, 0, graphHeight));
        int y2 = tempGraphY + graphHeight - (int)(map(temp2 * 10, tempMin * 10, tempMax * 10, 0, graphHeight));
        y1 = constrain(y1, tempGraphY, tempGraphY + graphHeight);
        y2 = constrain(y2, tempGraphY, tempGraphY + graphHeight);
        x1 = constrain(x1, 1, graphWidth);
        x2 = constrain(x2, 1, graphWidth);
        tft.drawLine(x1, y1, x2, y2, TERMINAL_GREEN);
    }
    float humMin = humData[0];
    float humMax = humData[0];
    for (int i = 1; i < dataCount && i < numPoints; i++)
    {
        if (humData[i] < humMin) humMin = humData[i];
        if (humData[i] > humMax) humMax = humData[i];
    }
    humMin -= 10.0;
    humMax += 10.0;
    if (humMax - humMin < 20.0)
    {
        humMax = humMin + 20.0;
    }
    tft.drawFastHLine(0, humGraphY - 1, tft.width(), TERMINAL_GREEN);
    tft.drawFastHLine(0, humGraphY + graphHeight, tft.width(), TERMINAL_GREEN);
    tft.drawFastVLine(0, humGraphY - 1, graphHeight + 2, TERMINAL_GREEN);
    tft.drawFastVLine(tft.width() - 1, humGraphY - 1, graphHeight + 2, TERMINAL_GREEN);
    for (int i = 1; i <= 3; i++)
    {
        int y = humGraphY + (graphHeight * i) / 4;
        tft.drawFastHLine(1, y, graphWidth, gridColor);
    }
    for (int i = 1; i <= 4; i++)
    {
        int x = 1 + (graphWidth * i) / 5;
        tft.drawFastVLine(x, humGraphY, graphHeight, gridColor);
    }
    snprintf(label, sizeof(label), "%.0f%%", humMax);
    drawText(label, 2, humGraphY - 5, TERMINAL_GREEN);
    snprintf(label, sizeof(label), "%.0f%%", (humMax + humMin) / 2);
    drawText(label, 2, humGraphY + graphHeight / 2 - 5, TERMINAL_GREEN);
    snprintf(label, sizeof(label), "%.0f%%", humMin);
    drawText(label, 2, humGraphY + graphHeight - 5, TERMINAL_GREEN);
    snprintf(label, sizeof(label), "%.0f%%", humData[(dataCount - 1) % numPoints]);
    textWidth = getTextWidth(label);
    drawText(label, graphWidth - textWidth - 2, humGraphY - 5, TERMINAL_GREEN);
    for (int i = 0; i < dataCount - 1 && i < numPoints - 1; i++)
    {
        float hum1 = humData[i];
        float hum2 = humData[i + 1];
        int x1 = 1 + (int)(i * xStep);
        int x2 = 1 + (int)((i + 1) * xStep);
        int y1 = humGraphY + graphHeight - (int)(map(hum1, humMin, humMax, 0, graphHeight));
        int y2 = humGraphY + graphHeight - (int)(map(hum2, humMin, humMax, 0, graphHeight));
        y1 = constrain(y1, humGraphY, humGraphY + graphHeight);
        y2 = constrain(y2, humGraphY, humGraphY + graphHeight);
        x1 = constrain(x1, 1, graphWidth);
        x2 = constrain(x2, 1, graphWidth);
        tft.drawLine(x1, y1, x2, y2, TERMINAL_GREEN);
    }
}

void showWifiScreen()
{
    inMainScreen = false;
    inWifiScreen = true;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = false;
    inBleScanScreen = false;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;
    
    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.reconnect();
        delay(2000);
    }
    
    if (WiFi.status() == WL_CONNECTED)
    {
        String ssidStr = WiFi.SSID();
        String ipStr = WiFi.localIP().toString();
        String macStr = WiFi.macAddress();
        int32_t rssi = WiFi.RSSI();
        int channel = WiFi.channel();
        
        int cornerRadius = 8;

        int topLeftX = 10, topLeftY = 35, topLeftW = 140, topLeftH = 80;
        tft.fillRoundRect(topLeftX, topLeftY, topLeftW, topLeftH, cornerRadius, TERMINAL_BG);
        tft.drawRoundRect(topLeftX, topLeftY, topLeftW, topLeftH, cornerRadius, TERMINAL_GREEN);

        int labelX = topLeftX + 8;
        int textY = topLeftY + 0;
        int lineHeight = 20;

        drawTinyText("SSID:", labelX, textY, TERMINAL_AMBER);
        if (ssidStr.length() > 14) ssidStr = ssidStr.substring(0, 14);
        drawTinyText(ssidStr.c_str(), labelX, textY + lineHeight, ILI9341_WHITE);

        drawTinyText("CH:", labelX, textY + lineHeight * 2, TERMINAL_AMBER);
        char chStr[6]; snprintf(chStr, sizeof(chStr), "%d", channel);
        drawTinyText(chStr, labelX, textY + lineHeight * 3, ILI9341_WHITE);
        
        int bottomLeftX = 10, bottomLeftY = 120, bottomLeftW = 140, bottomLeftH = 80;
        tft.fillRoundRect(bottomLeftX, bottomLeftY, bottomLeftW, bottomLeftH, cornerRadius, TERMINAL_BG);
        tft.drawRoundRect(bottomLeftX, bottomLeftY, bottomLeftW, bottomLeftH, cornerRadius, TERMINAL_GREEN);

        labelX = bottomLeftX + 8;
        textY = bottomLeftY + 0;

        drawTinyText("IP:", labelX, textY, TERMINAL_AMBER);
        drawTinyText(ipStr.c_str(), labelX, textY + lineHeight, ILI9341_WHITE);

        drawTinyText("MAC:", labelX, textY + lineHeight * 2, TERMINAL_AMBER);
        drawTinyText(macStr.c_str(), labelX, textY + lineHeight * 3, ILI9341_WHITE);
        
        int topRightX = 160, topRightY = 35, topRightW = 150, topRightH = 80;
        tft.fillRoundRect(topRightX, topRightY, topRightW, topRightH, cornerRadius, TERMINAL_BG);
        tft.drawRoundRect(topRightX, topRightY, topRightW, topRightH, cornerRadius, TERMINAL_GREEN);

        drawTinyText("Signal Gauge", topRightX + 8, topRightY + 0, TERMINAL_AMBER);

        int gaugeX = topRightX + 75, gaugeY = topRightY + 65, radius = 42;
        int arcWidth = 16;
        
        for (int angle = -180; angle <= 0; angle += 1)
        {
            float rad = angle * DEG_TO_RAD;
            int x1 = gaugeX + cos(rad) * (radius - arcWidth);
            int y1 = gaugeY + sin(rad) * (radius - arcWidth);
            int x2 = gaugeX + cos(rad) * radius;
            int y2 = gaugeY + sin(rad) * radius;
            
            uint16_t color;
            if (angle < -120)
            {
                int t = map(angle, -180, -120, 0, 255);
                color = tft.color565(255, t, 0);
            }
            else if (angle < -60)
            {
                int t = map(angle, -120, -60, 0, 255);
                color = tft.color565(255 - t, 255, 0);
            }
            else
            {
                int t = map(angle, -60, 0, 0, 255);
                color = tft.color565(0, 255, t/3);
            }
            
            tft.drawLine(x1, y1, x2, y2, color);
        }
        
        int32_t cappedRssi = max(-100L, min(-30L, (long)rssi));
        float angle = map(cappedRssi, -100, -30, -180, 0) * DEG_TO_RAD;
        int nx = gaugeX + cos(angle) * (radius - 5);
        int ny = gaugeY + sin(angle) * (radius - 5);
        tft.drawLine(gaugeX, gaugeY, nx, ny, ILI9341_WHITE);
        tft.drawLine(gaugeX + 1, gaugeY, nx + 1, ny, ILI9341_WHITE);
        tft.fillCircle(gaugeX, gaugeY, 4, ILI9341_WHITE);
        
        float weakRad = -180 * DEG_TO_RAD;
        int weakLabelX = gaugeX + cos(weakRad) * (radius + 6) - getTinyTextWidth("Weak");
        int weakLabelY = gaugeY + sin(weakRad) * (radius + 6) - 4;
        drawTinyText("Weak", weakLabelX, weakLabelY, ILI9341_RED);
        
        float goodRad = 0 * DEG_TO_RAD;
        int goodLabelX = gaugeX + cos(goodRad) * (radius + 6);
        int goodLabelY = gaugeY + sin(goodRad) * (radius + 6) - 4;
        drawTinyText("Good", goodLabelX, goodLabelY, TERMINAL_GREEN);
        
        int bottomRightX = 160, bottomRightY = 120, bottomRightW = 150, bottomRightH = 80;
        tft.fillRoundRect(bottomRightX, bottomRightY, bottomRightW, bottomRightH, cornerRadius, TERMINAL_BG);
        tft.drawRoundRect(bottomRightX, bottomRightY, bottomRightW, bottomRightH, cornerRadius, TERMINAL_GREEN);
        
        int bottomRightLabelX = bottomRightX + 8;
        int textBottomY = bottomRightY + 0;
        int bottomLineHeight = 16;
        
        drawTinyText("SIGNAL:", bottomRightLabelX, textBottomY, TERMINAL_AMBER);
        char rssiStr[16]; snprintf(rssiStr, sizeof(rssiStr), "%ld dBm", rssi);
        drawTinyText(rssiStr, bottomRightLabelX, textBottomY + bottomLineHeight, ILI9341_WHITE);
        
        drawTinyText("QUALITY:", bottomRightLabelX, textBottomY + bottomLineHeight * 2, TERMINAL_AMBER);
        const char* quality = (rssi > -60) ? "Excellent" : (rssi > -75) ? "Good" : "Weak";
        drawTinyText(quality, bottomRightLabelX, textBottomY + bottomLineHeight * 3, ILI9341_WHITE);
        
        drawTinyText("LOCK:", bottomRightLabelX, textBottomY + bottomLineHeight * 4, TERMINAL_AMBER);
        int lockPercent = map(cappedRssi, -100, -30, 0, 100);
        char lockStr[16]; snprintf(lockStr, sizeof(lockStr), "%d%%", lockPercent);
        drawTinyText(lockStr, bottomRightLabelX + 40, textBottomY + bottomLineHeight * 4, ILI9341_WHITE);
    }
    else
    {
        int discWidth = getTextWidth("WiFi Disconnected");
        drawText("WiFi Disconnected", (tft.width() - discWidth) / 2, 120, ILI9341_RED);
    }
    
    drawWifiButtons();
}

void showWifiScanScreen()
{
    inWifiScreen = false;
    inWifiScanScreen = true;
    inWaterfallScreen = false;
    inBleScreen = false;
    inBleScanScreen = false;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;
    if (wifiScanResultCount == 0)
    {
        scanNetworks();
    }
    tft.fillScreen(TERMINAL_BG);
    drawSmallText("SSID",   10,  8, TERMINAL_AMBER);
    drawSmallText("Signal", 160, 8, TERMINAL_AMBER);
    drawSmallText("Enc",    240, 8, TERMINAL_AMBER);
    drawSmallText("Ch",     280, 8, TERMINAL_AMBER);
    tft.drawFastHLine(0, 24, tft.width(), TERMINAL_GREEN);
    drawWifiTable();
}

void updateWifiScanScreen()
{
    static unsigned long lastScan = 0;
    if (millis() - lastScan > WIFI_SCAN_INTERVAL)
    {
        scanNetworks();
        lastScan = millis();
        Serial.printf("Rescanned: %d networks\n", wifiScanResultCount);
    }
    clearArea(0, 28, tft.width(), tft.height() - 28);
    drawWifiTable();
}

void drawWifiTable()
{
    int startRow = wifiScrollOffset;
    int endRow   = min(startRow + MAX_VISIBLE_WIFI_ROWS, wifiScanResultCount);
    int headerBottom = 28;
    int rowSpacing   = 38;
    int topPadding   = 6;
    clearArea(0, headerBottom, tft.width(), tft.height() - headerBottom);
    for (int i = startRow; i < endRow; i++)
    {
        int rowIndex = i - startRow;
        int y = headerBottom + topPadding + (rowIndex * rowSpacing);
        WiFiNetworkInfo net = wifiNetworks[i];
        String ssidDisplay = net.ssid.substring(0, SSID_MAX_CHARS);
        if (ssidDisplay.length() == 0)
        {
            ssidDisplay = "[Hidden]";
        }
        drawSmallText(ssidDisplay.c_str(), 10, y, net.isHidden ? TERMINAL_AMBER : TERMINAL_GREEN);
        String macDisplay = net.bssid;
        int macTextWidth = getTinyTextWidth(macDisplay.c_str());
        int macBoxHeight = 14;
        int macBoxWidth = macTextWidth + 10;
        int macX = 10;
        int macY = y + 12;
        tft.fillRoundRect(macX, macY, macBoxWidth, macBoxHeight, 6, TERMINAL_BG);
        tft.drawRoundRect(macX, macY, macBoxWidth, macBoxHeight, 6, TERMINAL_BLUE);
        int textX = macX + (macBoxWidth - macTextWidth) / 2;
        int textY = macY + (macBoxHeight / 2) - moveMacTextUpSukiCriesHard;
        drawTinyText(macDisplay.c_str(), textX, textY, TERMINAL_BLUE);
        int32_t cappedRssi = net.rssi > -100 ? net.rssi : -100;
        int rssiStrength = map(cappedRssi, -100, -30, 0, RSSI_BAR_MAX_WIDTH);
        uint16_t barColor = (net.rssi > -60) ? TERMINAL_GREEN : (net.rssi > -80) ? TERMINAL_AMBER : ILI9341_RED;
        tft.fillRect(160, y, RSSI_BAR_MAX_WIDTH, 7, TERMINAL_BG);
        tft.fillRect(160, y, rssiStrength, 7, barColor);
        tft.drawRect(160, y, RSSI_BAR_MAX_WIDTH, 7, TERMINAL_GREEN);
        String enc;
        switch (net.encryptionType) 
        {
            case WIFI_AUTH_OPEN:
                enc = "OPE";
                break;
            case WIFI_AUTH_WEP:
                enc = "WEP";
                break;
            case WIFI_AUTH_WPA_PSK:
                enc = "WPA";
                break;
            case WIFI_AUTH_WPA2_PSK:
                enc = "WP2";
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                enc = "W12";
                break;
            case WIFI_AUTH_WPA3_PSK:
                enc = "WP3";
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                enc = "W2E";
                break;
            case WIFI_AUTH_WPA3_ENT_192:
                enc = "W3E";
                break;
            default:
                enc = "UNK";
                break;
        }
        drawSmallText(enc.c_str(), 240, y, TERMINAL_BLUE);
        char chStr[4];
        snprintf(chStr, sizeof(chStr), "%d", net.channel);
        int chWidth = getSmallTextWidth(chStr);
        drawSmallText(chStr, 280 + (CH_COLUMN_WIDTH - chWidth) / 2, y, TERMINAL_GREEN);
    }
    if (wifiScanResultCount == 0)
    {
        drawSmallText("No networks found", 80, 100, ILI9341_RED);
    }
}

void drawScrollIndicators()
{
}

void drawControlsText()
{
}

void drawWifiButtons()
{
    int buttonY = tft.height() - BUTTON_HEIGHT - 10;
    int buttonWidth = (tft.width() - 20 - BUTTON_SPACING * 2) / 3;
    clearArea(8, buttonY - 2, tft.width() - 16, BUTTON_HEIGHT + 4);
    drawButton("Scan", 10, buttonY, buttonWidth, selectedWifiButton == 0);
    drawButton("Graph", 10 + (buttonWidth + BUTTON_SPACING), buttonY, buttonWidth, selectedWifiButton == 1);
    drawButton("Back", 10 + 2 * (buttonWidth + BUTTON_SPACING), buttonY, buttonWidth, selectedWifiButton == 2);
}

void drawScanningIndicator(const char* text, int x, int y, int durationMs)
{
    drawText(text, x, y);
    delay(durationMs);
}

void updateWifiScreen()
{
    if (!inWifiScreen) return;
    static unsigned long lastWifiUpdate = 0;
    static const unsigned long WIFI_UPDATE_INTERVAL = 5000;
    if (millis() - lastWifiUpdate > WIFI_UPDATE_INTERVAL)
    {
        lastWifiUpdate = millis();
        if (WiFi.status() == WL_CONNECTED)
        {
            int32_t rssi = WiFi.RSSI();
            int topRightX = 160, topRightY = 35, topRightW = 150, topRightH = 80;
            int cornerRadius = 8;
            clearArea(topRightX, topRightY, topRightW, topRightH);
            tft.fillRoundRect(topRightX, topRightY, topRightW, topRightH, cornerRadius, TERMINAL_BG);
            tft.drawRoundRect(topRightX, topRightY, topRightW, topRightH, cornerRadius, TERMINAL_GREEN);
            tft.drawRoundRect(topRightX + 1, topRightY + 1, topRightW - 2, topRightH - 2, cornerRadius - 1, TERMINAL_GREEN);
            drawTinyText("Signal Gauge", topRightX + 5, topRightY + 10, TERMINAL_AMBER);
            int gaugeX = topRightX + 75, gaugeY = topRightY + 45, radius = 28;
            for (int angle = -90; angle <= 90; angle += 2)
            {
                float rad = angle * DEG_TO_RAD;
                int x1 = gaugeX + cos(rad) * (radius - 8);
                int y1 = gaugeY + sin(rad) * (radius - 8);
                int x2 = gaugeX + cos(rad) * radius;
                int y2 = gaugeY + sin(rad) * radius;
                uint16_t color = (angle < -30) ? ILI9341_RED : (angle < 30) ? TERMINAL_AMBER : TERMINAL_GREEN;
                for (int thickness = 0; thickness < 3; thickness++)
                {
                    tft.drawLine(x1 - thickness, y1 - thickness, x2 - thickness, y2 - thickness, color);
                }
            }
            tft.drawCircle(gaugeX, gaugeY, radius, TERMINAL_GREEN);
            tft.drawCircle(gaugeX, gaugeY, radius - 1, TERMINAL_GREEN);
            int32_t cappedRssi = max(-100L, min(-30L, (long)rssi));
            float angle = map(cappedRssi, -100, -30, -90, 90) * DEG_TO_RAD;
            int nx = gaugeX + cos(angle) * (radius - 8);
            int ny = gaugeY + sin(angle) * (radius - 8);
            for (int thickness = 0; thickness < 3; thickness++)
            {
                tft.drawLine(gaugeX - thickness, gaugeY - thickness, nx - thickness, ny - thickness, ILI9341_WHITE);
            }
            tft.fillCircle(gaugeX, gaugeY, 3, ILI9341_WHITE);
            drawTinyText("Weak", gaugeX - 35, gaugeY + radius + 5, ILI9341_RED);
            drawTinyText("Good", gaugeX + 15, gaugeY + radius + 5, TERMINAL_GREEN);
            int bottomRightX = 160, bottomRightY = 120, bottomRightW = 150, bottomRightH = 55;
            clearArea(bottomRightX, bottomRightY, bottomRightW, bottomRightH);
            tft.fillRoundRect(bottomRightX, bottomRightY, bottomRightW, bottomRightH, cornerRadius, TERMINAL_BG);
            tft.drawRoundRect(bottomRightX, bottomRightY, bottomRightW, bottomRightH, cornerRadius, TERMINAL_GREEN);
            tft.drawRoundRect(bottomRightX + 1, bottomRightY + 1, bottomRightW - 2, bottomRightH - 2, cornerRadius - 1, TERMINAL_GREEN);
            char rssiStr[16]; snprintf(rssiStr, sizeof(rssiStr), "%ld dBm", rssi);
            drawTinyText("Signal:", bottomRightX + 5, bottomRightY + 15, TERMINAL_AMBER);
            drawSmallText(rssiStr, bottomRightX + 60, bottomRightY + 15);
            drawTinyText("Quality:", bottomRightX + 5, bottomRightY + 30, TERMINAL_AMBER);
            const char* quality = (rssi > -60) ? "Good" : (rssi > -75) ? "Moderate" : "Weak";
            drawSmallText(quality, bottomRightX + 70, bottomRightY + 30);
            drawTinyText("Lock:", bottomRightX + 5, bottomRightY + 45, TERMINAL_AMBER);
            int lockPercent = map(cappedRssi, -100, -30, 0, 100);
            char lockStr[16]; snprintf(lockStr, sizeof(lockStr), "%d%%", lockPercent);
            drawSmallText(lockStr, bottomRightX + 60, bottomRightY + 45);
        }
        else
        {
            clearArea(60, 100, 200, 20);
            drawText("WiFi Disconnected", 60, 100, ILI9341_RED);
        }
    }
}

void showWaterfallScreen()
{
    inMainScreen = false;
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = true;
    inBleScreen = false;
    inBleScanScreen = false;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;
    maxPps = 0;
    setPromiscuousMode(true, currentChannel);
    tft.fillScreen(TERMINAL_BG);
    drawWaterfallHeader();
    drawWaterfallGraph(10, 15, tft.width() - 20, 180);
    drawWaterfallButtons();
}

void drawWaterfallHeader()
{
    int headerY = 6;
    int screenWidth = tft.width();
    clearArea(0, 0, screenWidth, 14);
    char channelInfo[25];
    int frequency = 2412 + ((currentChannel - 1) * 5);
    snprintf(channelInfo, sizeof(channelInfo), "CH %d (%d MHz)", currentChannel, frequency);
    int channelWidth = getSmallTextWidth(channelInfo);
    drawSmallText(channelInfo, max(2, 5 - channelWidth / 2), headerY, TERMINAL_AMBER);
    int pps = 0;
    if (currentChannel >= 1 && currentChannel <= NUM_CHANNELS)
    {
        pps = packetHistory[currentChannel-1][0];
    }
    if (pps > maxPps)
    {
        maxPps = pps;
    }
    char stats[25];
    snprintf(stats, sizeof(stats), "PPS:%d Max:%d", pps, maxPps);
    int statsWidth = getSmallTextWidth(stats);
    drawSmallText(stats, screenWidth - statsWidth - 2, headerY, TERMINAL_GREEN);
}

void drawWaterfallGraph(int x, int y, int width, int height)
{
    clearArea(x, y, width, height + 15);
    int centerX = x + (width / 2);
    tft.drawFastVLine(centerX, y, height, TERMINAL_GREEN);
    int singleDigitWidth = getSmallTextWidth("0");
    int doubleDigitWidth = getSmallTextWidth("50");
    drawSmallText("0", centerX - (singleDigitWidth / 2) - 3, y + height + 4, TERMINAL_GREEN);
    for (int i = 1; i <= 5; i++)
    {
        int offset = (width / 2) * i / 5;
        char label[4];
        snprintf(label, sizeof(label), "%d", i * 10);
        int rightX = centerX + offset - (doubleDigitWidth / 2) - 2;
        drawSmallText(label, rightX, y + height + 4, TERMINAL_GREEN);
        int leftX = centerX - offset - (doubleDigitWidth / 2) - 2;
        drawSmallText(label, leftX, y + height + 4, TERMINAL_GREEN);
    }
    uint16_t noiseColor = tft.color565(0, 0, 30);
    for (int timeSlot = 0; timeSlot < MAX_PACKET_HISTORY; timeSlot++)
    {
        int lineY = y + timeSlot;
        tft.drawFastHLine(x, lineY, width, noiseColor);
        int packets = packetHistory[currentChannel - 1][timeSlot];
        if (packets > 0)
        {
            int halfLineLength = map(packets, 0, 50, 0, width / 2);
            halfLineLength = constrain(halfLineLength, 0, width / 2);
            int intensity = map(packets, 0, 50, 0, 255);
            intensity = constrain(intensity, 0, 255);
            uint16_t color;
            if (intensity < 64)
            {
                color = tft.color565(0, 0, map(intensity, 0, 63, 30, 255));
            }
            else if (intensity < 128)
            {
                color = tft.color565(0, map(intensity, 64, 127, 0, 255), 255);
            }
            else if (intensity < 192)
            {
                color = tft.color565(map(intensity, 128, 191, 0, 255), 255, map(intensity, 128, 191, 255, 0));
            }
            else
            {
                color = tft.color565(255, 255, map(intensity, 192, 255, 0, 255));
            }
            tft.drawFastHLine(centerX - halfLineLength, lineY, halfLineLength * 2, color);
        }
    }
}

void drawWaterfallButtons()
{
    int buttonY = tft.height() - BUTTON_HEIGHT - 3;
    int buttonWidth = (tft.width() - 20 - BUTTON_SPACING * 3) / 4;
    clearArea(8, buttonY - 2, tft.width() - 16, BUTTON_HEIGHT + 4);
    drawButton("Scan", 10, buttonY, buttonWidth, selectedWaterfallButton == 0);
    drawButton("CH-", 10 + (buttonWidth + BUTTON_SPACING), buttonY, buttonWidth, selectedWaterfallButton == 1);
    drawButton("CH+", 10 + 2*(buttonWidth + BUTTON_SPACING), buttonY, buttonWidth, selectedWaterfallButton == 2);
    drawButton("Back", 10 + 3*(buttonWidth + BUTTON_SPACING), buttonY, buttonWidth, selectedWaterfallButton == 3);
}

void updateWaterfallScreen()
{
    if (!inWaterfallScreen) return;
    drawWaterfallHeader();
    drawWaterfallGraph(10, 15, tft.width() - 20, 180);
}


void drawBleButtons()
{
    const int header_height = 30;
    int centerX = tft.width() / 2;
    int centerY = header_height + (tft.height() - header_height) / 2;
    const int radius = 90;
    const int glowRadius = radius + 10;
    const int coreRadius = 36;
    const int iconSize = 64;
    const int edgePadding = 5;

    clearArea(0, header_height, tft.width(), tft.height() - header_height);

    tft.drawCircle(centerX, centerY, glowRadius, ILI9341_DARKGREEN);
    tft.drawCircle(centerX, centerY, glowRadius - 3, ILI9341_GREEN);
    tft.drawCircle(centerX, centerY, glowRadius - 6, TERMINAL_GREEN);
    tft.drawCircle(centerX, centerY, radius, ILI9341_BLACK);

    tft.fillCircle(centerX, centerY, coreRadius, TERMINAL_BG);
    tft.drawCircle(centerX, centerY, coreRadius, ILI9341_GREEN);
    tft.drawCircle(centerX, centerY, coreRadius - 2, ILI9341_GREEN);

    tft.drawBitmap(centerX - (iconSize / 2), centerY - (iconSize / 2), hackochiIcons[selectedBleButton], iconSize, iconSize, hackochiIconColors[selectedBleButton]);

    const char* buttonTexts[] = {"BLE Scan", "BT Scan", "Graph", "Back"};
    int maxSegWidth = getSmallTextWidth("BLE Scan") + 10;
    int segHeight = BUTTON_HEIGHT + 5;

    int topX = centerX;
    int topY = header_height + edgePadding + (segHeight / 2);
    int bottomX = centerX;
    int bottomY = tft.height() - edgePadding - (segHeight / 2);
    int leftX = edgePadding + (maxSegWidth / 2);
    int leftY = centerY;
    int rightX = tft.width() - edgePadding - (maxSegWidth / 2);
    int rightY = centerY;

    struct { int x, y; int index; } positions[] = {
        {topX, topY, 0},
        {rightX, rightY, 1},
        {bottomX, bottomY, 2},
        {leftX, leftY, 3}
    };

    for (int i = 0; i < 4; i++) {
        int segX = positions[i].x;
        int segY = positions[i].y;
        int btnIndex = positions[i].index;

        uint16_t textColor = (btnIndex == selectedBleButton) ? ILI9341_BLACK : TERMINAL_GREEN;
        uint16_t bgColor = (btnIndex == selectedBleButton) ? TERMINAL_GREEN : BUTTON_BG;

        tft.fillRoundRect(segX - maxSegWidth / 2, segY - segHeight / 2, maxSegWidth, segHeight, 4, bgColor);
        tft.drawRoundRect(segX - maxSegWidth / 2, segY - segHeight / 2, maxSegWidth, segHeight, 4, TERMINAL_GREEN);

        int textWidth = getSmallTextWidth(buttonTexts[btnIndex]);
        int textX = segX - textWidth / 2;
        int textY = segY - 4;
        drawSmallText(buttonTexts[btnIndex], textX, textY, textColor);

        if (btnIndex == selectedBleButton) {
            tft.drawCircle(centerX, centerY, coreRadius + (millis() % 600) / 120, ILI9341_GREEN);
        }
    }
}

void showBleScreen()
{
    inMainScreen = false;
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = true;
    inBleScanScreen = false;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;

    clearArea(0, 30, tft.width(), tft.height() - 30);
    drawTerminalHeader();
    drawBleButtons();
}

void playHackochiActivation(int centerX, int centerY, int coreRadius, int selectedBleButton)
{
    const int iconSize = 64;
    int clearX = centerX - (iconSize / 2) - 5;
    int clearY = centerY - (iconSize / 2) - 5;
    int clearWidth = iconSize + 10;
    int clearHeight = iconSize + 10;

    clearArea(clearX, clearY, clearWidth, clearHeight);
    tft.fillCircle(centerX, centerY, coreRadius, ILI9341_GREEN);
    delay(70);
    clearArea(clearX, clearY, clearWidth, clearHeight);
    tft.fillCircle(centerX, centerY, coreRadius, TERMINAL_BG);
    tft.drawCircle(centerX, centerY, coreRadius, ILI9341_GREEN);
    tft.drawBitmap(centerX - (iconSize / 2), centerY - (iconSize / 2), hackochiIcons[selectedBleButton], iconSize, iconSize, hackochiIconColors[selectedBleButton]);
    delay(50);
    clearArea(clearX, clearY, clearWidth, clearHeight);
    tft.fillCircle(centerX, centerY, coreRadius, TERMINAL_BG);
    delay(30);
    clearArea(clearX, clearY, clearWidth, clearHeight);
    tft.fillCircle(centerX, centerY, coreRadius, TERMINAL_BG);
    tft.drawCircle(centerX, centerY, coreRadius, ILI9341_GREEN);
    int altIconIndex = (selectedBleButton + 1) % 4;
    tft.drawBitmap(centerX - (iconSize / 2), centerY - (iconSize / 2), hackochiIcons[altIconIndex], iconSize, iconSize, hackochiIconColors[altIconIndex]);
    delay(50);
    clearArea(clearX, clearY, clearWidth, clearHeight);
    tft.fillCircle(centerX, centerY, coreRadius, ILI9341_GREEN);
    delay(60);
    clearArea(clearX, clearY, clearWidth, clearHeight);
}

void handleEncoderRotation(int direction)
{
    if (millis() - lastEncoderPress < encoderDebounce) return;

    Serial.print("Encoder direction: ");
    Serial.println(direction);

    if (inPetPopup) 
    {
        selectedPopupButton = (selectedPopupButton + direction + 2) % 2;
        drawPetPopup();
        return;
    }
    if (inPetScreen) 
    {
        currentPetMood = HAPPY;
        lastMoodChangeTime = millis();
        return;
    }

    if (inMainScreen)
    {
        selectedButton = (selectedButton + direction + 6) % 6;
        drawButtons();
    }
    else if (inWifiScreen)
    {
        selectedWifiButton = (selectedWifiButton + direction + 3) % 3;
        drawWifiButtons();
    }
    else if (inWifiScanScreen)
    {
        if (direction > 0)
        {
            int maxOffset = max(0, wifiScanResultCount - MAX_VISIBLE_WIFI_ROWS);
            if (wifiScrollOffset < maxOffset)
            {
                wifiScrollOffset++;
                updateWifiScanScreen();
            }
        }
        else if (direction < 0)
        {
            if (wifiScrollOffset > 0)
            {
                wifiScrollOffset--;
                updateWifiScanScreen();
            }
        }
    }
    else if (inWaterfallScreen)
    {
        selectedWaterfallButton = (selectedWaterfallButton + direction + 4) % 4;
        drawWaterfallButtons();
    }
    else if (inBleScreen)
    {
        int prevSelected = selectedBleButton;
        if (direction > 0) {
            selectedBleButton = (selectedBleButton + 1) % 4;
        } else if (direction < 0) {
            selectedBleButton = (selectedBleButton - 1 + 4) % 4;
        }

        Serial.print("BLE Screen: selectedBleButton = ");
        Serial.println(selectedBleButton);

        const int header_height = 30;
        int centerX = tft.width() / 2;
        int centerY = header_height + (tft.height() - header_height) / 2;
        const int radius = 90;
        const int coreRadius = 36;
        const int iconSize = 64;
        const int edgePadding = 5;
        int maxSegWidth = getSmallTextWidth("BLE Scan") + 10;
        int segHeight = BUTTON_HEIGHT + 5;

        int clearX = centerX - (iconSize / 2) - 5;
        int clearY = centerY - (iconSize / 2) - 5;
        int clearWidth = iconSize + 10;
        int clearHeight = iconSize + 10;
        clearArea(clearX, clearY, clearWidth, clearHeight);
        tft.fillCircle(centerX, centerY, coreRadius, TERMINAL_BG);
        tft.drawCircle(centerX, centerY, coreRadius, ILI9341_GREEN);
        tft.drawCircle(centerX, centerY, coreRadius - 2, ILI9341_GREEN);
        tft.drawBitmap(centerX - (iconSize / 2), centerY - (iconSize / 2), hackochiIcons[selectedBleButton], iconSize, iconSize, hackochiIconColors[selectedBleButton]);

        int topX = centerX;
        int topY = header_height + edgePadding + (segHeight / 2);
        int bottomX = centerX;
        int bottomY = tft.height() - edgePadding - (segHeight / 2);
        int leftX = edgePadding + (maxSegWidth / 2);
        int leftY = centerY;
        int rightX = tft.width() - edgePadding - (maxSegWidth / 2);
        int rightY = centerY;

        struct { int x, y; int index; } positions[] = {
            {topX, topY, 0},
            {rightX, rightY, 1},
            {bottomX, bottomY, 2},
            {leftX, leftY, 3}
        };

        const char* buttonTexts[] = {"BLE Scan", "BT Scan", "Graph", "Back"};
        for (int i = 0; i < 4; i++) {
            if (positions[i].index == prevSelected || positions[i].index == selectedBleButton) {
                int segX = positions[i].x;
                int segY = positions[i].y;
                int btnIndex = positions[i].index;

                clearArea(segX - maxSegWidth / 2, segY - segHeight / 2, maxSegWidth, segHeight);

                uint16_t textColor = (btnIndex == selectedBleButton) ? ILI9341_BLACK : TERMINAL_GREEN;
                uint16_t bgColor = (btnIndex == selectedBleButton) ? TERMINAL_GREEN : BUTTON_BG;

                tft.fillRoundRect(segX - maxSegWidth / 2, segY - segHeight / 2, maxSegWidth, segHeight, 4, bgColor);
                tft.drawRoundRect(segX - maxSegWidth / 2, segY - segHeight / 2, maxSegWidth, segHeight, 4, TERMINAL_GREEN);

                int textWidth = getSmallTextWidth(buttonTexts[btnIndex]);
                int textX = segX - textWidth / 2;
                int textY = segY - 4;
                drawSmallText(buttonTexts[btnIndex], textX, textY, textColor);

                if (btnIndex == selectedBleButton) {
                    tft.drawCircle(centerX, centerY, coreRadius + (millis() % 600) / 120, ILI9341_GREEN);
                }
            }
        }
    }
    else if (inBleWaterfallScreen)
    {
        selectedBleWaterfallButton = (selectedBleWaterfallButton + direction + 2) % 2;
        drawBleWaterfallButtons();
    }
    else if (inBleScanScreen)
    {
        if (direction > 0)
        {
            int maxOffset = max(0, bleScanResultCount - MAX_VISIBLE_WIFI_ROWS);
            if (bleScrollOffset < maxOffset)
            {
                bleScrollOffset++;
                drawBleTable();
            }
        }
        else if (direction < 0)
        {
            if (bleScrollOffset > 0)
            {
                bleScrollOffset--;
                drawBleTable();
            }
        }
    }
    else if (inClassicScanScreen)
    {
        if (direction > 0)
        {
            int maxOffset = max(0, classicScanResultCount - MAX_VISIBLE_WIFI_ROWS);
            if (classicScrollOffset < maxOffset)
            {
                classicScrollOffset++;
                drawClassicTable();
            }
        }
        else if (direction < 0)
        {
            if (classicScrollOffset > 0)
            {
                classicScrollOffset--;
                drawClassicTable();
            }
        }
    }
}

void handleEncoderButton()
{
    if (inPetPopup) 
    {
        if (selectedPopupButton == 0) 
        {
            showPetScreen();
        } 
        else 
        {
            drawMainScreen();
            updateDisplay(); 
        }
        inPetPopup = false;
        return;
    }

    if (inMainScreen)
    {
        switch (selectedButton)
        {
            case 0: showTemperatureScreen(); break;
            case 1: showHumidityScreen(); break;
            case 2: showGraphScreen(); break;
            case 3: showWifiScreen(); break;
            case 4: showBleScreen(); break;
            case 5: showPetScreen(); break;
        }
    } 
    else if (inWifiScreen)
    {
        switch (selectedWifiButton)
        {
            case 0: showWifiScanScreen(); break;
            case 1: showWaterfallScreen(); break;
            case 2: drawMainScreen(); break;
        }
    } 
    else if (inWifiScanScreen)
    {
        inWifiScanScreen = false;
        showWifiScreen();
    } 
    else if (inWaterfallScreen)
    {
        switch (selectedWaterfallButton)
        {
            case 0:
                setPromiscuousMode(false);
                scanNetworks();
                maxPps = 0;
                setPromiscuousMode(true, currentChannel);
                break;
            case 1:
                currentChannel = (currentChannel - 2 + NUM_CHANNELS) % NUM_CHANNELS + 1;
                maxPps = 0;
                setPromiscuousMode(true, currentChannel);
                drawWaterfallHeader();
                break;
            case 2:
                currentChannel = (currentChannel % NUM_CHANNELS) + 1;
                maxPps = 0;
                setPromiscuousMode(true, currentChannel);
                drawWaterfallHeader();
                break;
            case 3:
                setPromiscuousMode(false);
                inWaterfallScreen = false;
                showWifiScreen();
                break;
        }
    }
    else if (inBleScreen)
    {
        int centerX = tft.width() / 2;
        int centerY = 30 + (tft.height() - 30) / 2;
        int coreRadius = 36;
        playHackochiActivation(centerX, centerY, coreRadius, selectedBleButton);
        switch(selectedBleButton)
        {
            case 0: showBleScanScreen(); break;
            case 1: showClassicScanScreen(); break;
            case 2: showBleGraphScreen(); break;
            case 3: drawMainScreen(); break;
        }
    }
    else if (inBleWaterfallScreen)
    {
        switch(selectedBleWaterfallButton)
        {
            case 0:
                blePacketCount = 0;
                lastBleCount = 0;
                maxBlePps = 0;
                break;
            case 1:
                stopBleGraphScan();
                inBleWaterfallScreen = false;
                showBleScreen();
                break;
        }
    }
    else if (inBleScanScreen)
    {
        inBleScanScreen = false;
        showBleScreen();
    }
    else if (inClassicScanScreen)
    {
        inClassicScanScreen = false;
        showBleScreen();
    }
    else
    {
        drawMainScreen();
        lastSensorRead = 0;
        lastBatteryCheck = 0;
        lastTime = "";
        lastTemp = "";
        lastHumidity = "";
        lastWifi = "";
        lastBattery = "";
        lastHeap = "";
        lastUptime = "";
        updateDisplay();
    }
}


void showBleScanScreen()
{
    inBleScreen = false;
    inBleScanScreen = true;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;
    if (bleScanResultCount == 0)
    {
        scanBLE();
    }
    tft.fillScreen(TERMINAL_BG);
    drawSmallText("Name",   10,  8, TERMINAL_AMBER);
    drawSmallText("Signal", 160, 8, TERMINAL_AMBER);
    drawSmallText("Type",   230, 8, TERMINAL_AMBER);
    drawSmallText("RSSI",   280, 8, TERMINAL_AMBER);
    tft.drawFastHLine(0, 24, tft.width(), TERMINAL_GREEN);
    clearArea(0, 28, tft.width(), tft.height() - 28);
    drawBleTable();
}

void updateBleScanScreen()
{
    static unsigned long lastScan = 0;
    if (millis() - lastScan > WIFI_SCAN_INTERVAL)
    {
        scanBLE();
        lastScan = millis();
        Serial.printf("Rescanned: %d BLE devices\n", bleScanResultCount);
    }
    clearArea(0, 28, tft.width(), tft.height() - 28);
    drawBleTable();
}

void drawBleTable()
{
    int startRow = bleScrollOffset;
    int endRow   = min(startRow + MAX_VISIBLE_WIFI_ROWS, bleScanResultCount);
    int headerBottom = 28;
    int rowSpacing   = 38;
    int topPadding   = 6;
    clearArea(0, headerBottom, tft.width(), tft.height() - headerBottom);
    for (int i = startRow; i < endRow; i++)
    {
        int rowIndex = i - startRow;
        int y = headerBottom + topPadding + (rowIndex * rowSpacing);
        BLENetworkInfo dev = bleNetworks[i];
        String nameDisplay = dev.name.substring(0, SSID_MAX_CHARS);
        if (nameDisplay.length() == 0)
        {
            nameDisplay = "[Hidden]";
        }
        drawSmallText(nameDisplay.c_str(), 10, y, dev.name.length() == 0 ? TERMINAL_AMBER : TERMINAL_GREEN);
        String addressDisplay = dev.address;
        int macTextWidth  = getTinyTextWidth(addressDisplay.c_str());
        int macBoxHeight  = 14;
        int macBoxWidth   = macTextWidth + 10;
        int macX = 10;
        int macY = y + 12;
        tft.fillRoundRect(macX, macY, macBoxWidth, macBoxHeight, 6, TERMINAL_BG);
        tft.drawRoundRect(macX, macY, macBoxWidth, macBoxHeight, 6, TERMINAL_BLUE);
        int textX = macX + (macBoxWidth - macTextWidth) / 2;
        int textY = macY + (macBoxHeight / 2) - moveMacTextUpSukiCriesHard;
        drawTinyText(addressDisplay.c_str(), textX, textY, TERMINAL_BLUE);
        int32_t cappedRssi   = dev.rssi > -100 ? dev.rssi : -100;
        int rssiStrength     = map(cappedRssi, -100, -30, 0, RSSI_BAR_MAX_WIDTH);
        uint16_t barColor    = (dev.rssi > -60) ? TERMINAL_GREEN : (dev.rssi > -80) ? TERMINAL_AMBER : ILI9341_RED;
        tft.fillRect(160, y, RSSI_BAR_MAX_WIDTH, 7, TERMINAL_BG);
        tft.fillRect(160, y, rssiStrength, 7, barColor);
        tft.drawRect(160, y, RSSI_BAR_MAX_WIDTH, 7, TERMINAL_GREEN);
        drawSmallText("BLE", 240, y, TERMINAL_BLUE);
        char rssiStr[8];
        snprintf(rssiStr, sizeof(rssiStr), "%d", dev.rssi);
        drawSmallText(rssiStr, 280, y, TERMINAL_GREEN);
    }
    if (bleScanResultCount == 0)
    {
        drawSmallText("No networks found", 80, 100, ILI9341_RED);
    }
}

void showBleGraphScreen()
{
    inMainScreen = false;
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = false;
    inBleScanScreen = false;
    inClassicScanScreen = false;
    inGraphScreen = false;
    inBleWaterfallScreen = true;
    inPetScreen = false;
    
    maxBlePps = 0;
    startBleGraphScan();
    
    tft.fillScreen(TERMINAL_BG);
    drawBleWaterfallHeader();
    drawBleWaterfallGraph(10, 15, tft.width() - 20, 180);
    drawBleWaterfallButtons();
}

void drawBleWaterfallHeader()
{
    int headerY = 6;
    int screenWidth = tft.width();
    clearArea(0, 0, screenWidth, 14);
    
    const char* title = "BLE ADV (CH 37,38,39)";
    int titleWidth = getSmallTextWidth(title);
    drawSmallText(title, (screenWidth / 2) - (titleWidth / 2) - 35, headerY, TERMINAL_AMBER);
    
    int pps = blePacketHistory[0];
    if (pps > maxBlePps) 
    {
        maxBlePps = pps;
    }
    
    char stats[25];
    snprintf(stats, sizeof(stats), "PPS:%d Max:%d", pps, maxBlePps);
    int statsWidth = getSmallTextWidth(stats);
    drawSmallText(stats, screenWidth - statsWidth - 2, headerY, TERMINAL_GREEN);
}

void drawBleWaterfallGraph(int x, int y, int width, int height)
{
    clearArea(x, y, width, height + 15);
    int centerX = x + (width / 2);
    tft.drawFastVLine(centerX, y, height, TERMINAL_GREEN);
    int singleDigitWidth = getSmallTextWidth("0");
    int doubleDigitWidth = getSmallTextWidth("50");
    drawSmallText("0", centerX - (singleDigitWidth / 2) - 3, y + height + 4, TERMINAL_GREEN);
    
    for (int i = 1; i <= 5; i++) 
    {
        int offset = (width / 2) * i / 5;
        char label[4];
        snprintf(label, sizeof(label), "%d", i * 10);
        int rightX = centerX + offset - (doubleDigitWidth / 2) - 2;
        drawSmallText(label, rightX, y + height + 4, TERMINAL_GREEN);
        int leftX = centerX - offset - (doubleDigitWidth / 2) - 2;
        drawSmallText(label, leftX, y + height + 4, TERMINAL_GREEN);
    }

    uint16_t noiseColor = tft.color565(0, 0, 30);
    for (int timeSlot = 0; timeSlot < MAX_PACKET_HISTORY; timeSlot++)
    {
        int lineY = y + timeSlot;
        tft.drawFastHLine(x, lineY, width, noiseColor);
        int packets = blePacketHistory[timeSlot];
        
        if (packets > 0)
        {
            int halfLineLength = map(packets, 0, 50, 0, width / 2);
            halfLineLength = constrain(halfLineLength, 0, width / 2);
            int intensity = map(packets, 0, 50, 0, 255);
            intensity = constrain(intensity, 0, 255);
            
            uint16_t color;
            if (intensity < 64) 
            {
                color = tft.color565(0, 0, map(intensity, 0, 63, 30, 255));
            } 
            else if (intensity < 128) 
            {
                color = tft.color565(0, map(intensity, 64, 127, 0, 255), 255);
            } 
            else if (intensity < 192) 
            {
                color = tft.color565(map(intensity, 128, 191, 0, 255), 255, map(intensity, 128, 191, 255, 0));
            } 
            else 
            {
                color = tft.color565(255, 255, map(intensity, 192, 255, 0, 255));
            }
            
            tft.drawFastHLine(centerX - halfLineLength, lineY, halfLineLength * 2, color);
        }
    }
}

void drawBleWaterfallButtons()
{
    int buttonY = tft.height() - BUTTON_HEIGHT - 3;
    int buttonWidth = (tft.width() - 20 - BUTTON_SPACING) / 2;
    clearArea(8, buttonY - 2, tft.width() - 16, BUTTON_HEIGHT + 4);
    drawButton("Clear", 10, buttonY, buttonWidth, selectedBleWaterfallButton == 0);
    drawButton("Back", 10 + (buttonWidth + BUTTON_SPACING), buttonY, buttonWidth, selectedBleWaterfallButton == 1);
}

void updateBleWaterfallScreen()
{
    if (!inBleWaterfallScreen) return;
    drawBleWaterfallHeader();
    drawBleWaterfallGraph(10, 15, tft.width() - 20, 180);
}

void showClassicScanScreen()
{
    inBleScreen = false;
    inBleScanScreen = false;
    inClassicScanScreen = true;
    inGraphScreen = false;
    inBleWaterfallScreen = false;
    inPetScreen = false;
    if (classicScanResultCount == 0)
    {
        scanClassicBT();
    }
    tft.fillScreen(TERMINAL_BG);
    drawSmallText("Name",   10,  8, TERMINAL_AMBER);
    drawSmallText("Signal", 160, 8, TERMINAL_AMBER);
    drawSmallText("Type",   230, 8, TERMINAL_AMBER);
    drawSmallText("RSSI",   280, 8, TERMINAL_AMBER);
    tft.drawFastHLine(0, 24, tft.width(), TERMINAL_GREEN);
    clearArea(0, 28, tft.width(), tft.height() - 28);
    drawClassicTable();
}

void updateClassicScanScreen()
{
    static unsigned long lastScan = 0;
    if (millis() - lastScan > WIFI_SCAN_INTERVAL)
    {
        scanClassicBT();
        lastScan = millis();
        Serial.printf("Rescanned: %d Classic BT devices\n", classicScanResultCount);
    }
    clearArea(0, 28, tft.width(), tft.height() - 28);
    drawClassicTable();
}

void drawClassicTable()
{
    int startRow = classicScrollOffset;
    int endRow   = min(startRow + MAX_VISIBLE_WIFI_ROWS, classicScanResultCount);
    int headerBottom = 28;
    int rowSpacing   = 38;
    int topPadding   = 6;
    clearArea(0, headerBottom, tft.width(), tft.height() - headerBottom);
    for (int i = startRow; i < endRow; i++)
    {
        int rowIndex = i - startRow;
        int y = headerBottom + topPadding + (rowIndex * rowSpacing);
        BLENetworkInfo dev = classicNetworks[i];
        String nameDisplay = dev.name.substring(0, SSID_MAX_CHARS);
        if (nameDisplay.length() == 0)
        {
            nameDisplay = "[Hidden]";
        }
        drawSmallText(nameDisplay.c_str(), 10, y, dev.name.length() == 0 ? TERMINAL_AMBER : TERMINAL_GREEN);
        String addressDisplay = dev.address;
        int macTextWidth  = getTinyTextWidth(addressDisplay.c_str());
        int macBoxHeight  = 14;
        int macBoxWidth   = macTextWidth + 10;
        int macX = 10;
        int macY = y + 12;
        tft.fillRoundRect(macX, macY, macBoxWidth, macBoxHeight, 6, TERMINAL_BG);
        tft.drawRoundRect(macX, macY, macBoxWidth, macBoxHeight, 6, TERMINAL_BLUE);
        int textX = macX + (macBoxWidth - macTextWidth) / 2;
        int textY = macY + (macBoxHeight / 2) - moveMacTextUpSukiCriesHard;
        drawTinyText(addressDisplay.c_str(), textX, textY, TERMINAL_BLUE);
        tft.fillRect(160, y, RSSI_BAR_MAX_WIDTH, 7, TERMINAL_BG);
        tft.drawRect(160, y, RSSI_BAR_MAX_WIDTH, 7, TERMINAL_GREEN);
        drawSmallText("BT", 235, y, TERMINAL_BLUE);
        drawSmallText("N/A", 280, y, TERMINAL_GREEN);
    }
    if (classicScanResultCount == 0)
    {
        drawSmallText("No devices found", 80, 100, ILI9341_RED);
    }
}


void drawPetStatsDashboard() 
{
    int y1 = 38;
    int y2 = 53;
    int y3 = 68;
    
    int col1_label_x = 15;
    int col1_value_x = 90; 
    
    int col2_label_x = 170;
    int col2_value_x = 270;

    clearArea(0, y1 - 2, tft.width(), 45);

    char valStr[6];

    drawSmallText("Scan:", col1_label_x, y1, TERMINAL_AMBER);
    snprintf(valStr, sizeof(valStr), "%d", scanSessionCount);
    drawSmallText(valStr, col1_value_x, y1, ILI9341_CYAN);

    drawSmallText("Total:", col2_label_x, y1, TERMINAL_AMBER);

    drawSmallText("Open:", col1_label_x, y2, TERMINAL_AMBER);
    snprintf(valStr, sizeof(valStr), "%d", currentOpenCount);
    drawSmallText(valStr, col1_value_x, y2, (currentOpenCount > 0) ? ILI9341_GREEN : ILI9341_WHITE);

    drawSmallText("Current:", col2_label_x, y2, TERMINAL_AMBER);
    snprintf(valStr, sizeof(valStr), "%d", wifiScanResultCount);
    drawSmallText(valStr, col2_value_x, y2, ILI9341_WHITE);

    drawSmallText("Close:", col1_label_x, y3, TERMINAL_AMBER);
    snprintf(valStr, sizeof(valStr), "%d", currentClosedCount);
    drawSmallText(valStr, col1_value_x, y3, ILI9341_WHITE);

    drawSmallText("Session:", col2_label_x, y3, TERMINAL_AMBER);
    snprintf(valStr, sizeof(valStr), "%d", totalNetworksFoundSinceEntry);
    drawSmallText(valStr, col2_value_x, y3, ILI9341_CYAN);
}

void showPetScreen() 
{
    inMainScreen = false;
    inPetScreen = true;

    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    currentPetMood = AWAKE;
    lastInteractionTime = millis();

    currentOpenCount = 0;
    currentClosedCount = 0;
    wifiScanResultCount = 0;
    totalNetworksFoundSinceEntry = 0;
    scanSessionCount = 0;
    
    drawPetStatsDashboard();

    WiFi.scanNetworks(true);
    nextScanTime = millis() + SCAN_INTERVAL + 5000;
}

void playColorStrobeFlash() 
{
    uint16_t colors[] = {ILI9341_RED, ILI9341_YELLOW, ILI9341_GREEN, ILI9341_CYAN, ILI9341_BLUE, ILI9341_MAGENTA};
    int numColors = sizeof(colors) / sizeof(uint16_t);

    for (int i = 0; i < 2; i++) 
    {
        for (int j = 0; j < numColors; j++) 
        {
            tft.fillScreen(colors[j]);
            delay(35);
        }
    }
}


const char* getMoodDescription(PetMood mood) 
{
    switch (mood) 
    {
        case AWAKE: return "Awake & Alert";
        case SCANNING: return "Scanning Networks";
        case FOCUSED: return "Focused on Task";
        case HAPPY: return "Happy & Playful";
        case BORED: return "Feeling Bored";
        case SLEEPING: return "Sleeping Zzz";
        default: return "Unknown";
    }
}

const char* getStatusDescription(PetMood mood, int frame) 
{
    switch (mood) 
    {
        case AWAKE: return "Waiting for the next hunt...";
        case ECSTATIC: return "OPEN NETWORK! AHHHHHYAEE!";
        case HAPPY: return "A good haul! Nice!";
        case SCANNING: return "Hunting for signals...";
        case FOCUSED: return "Shhh... I'm listening.";
        case SAD: return "Aww, the trail went cold...";
        case SLEEPING: return "Zzz... Resting between hunts...";
        default: return "Just a moment...";
    }
}

void drawMoodDescription(PetMood mood) 
{
    const char* description = getMoodDescription(mood);
    const char* status = getStatusDescription(mood, petFrame);

    const int footerY = 200;
    const int footerHeight = 40;

    const int moodTextY = footerY + 4;
    const int statusTextY = footerY + 24;

    clearArea(0, footerY, tft.width(), footerHeight);

    uint16_t textColor = ILI9341_WHITE;
    switch (mood) 
    {
        case HAPPY: textColor = ILI9341_GREEN; break;
        case SCANNING: textColor = TERMINAL_BLUE; break;
        case FOCUSED: textColor = TERMINAL_AMBER; break;
        case SLEEPING: textColor = ILI9341_CYAN; break;
        case BORED: textColor = ILI9341_LIGHTGREY; break;
        default: textColor = ILI9341_WHITE; break;
    }
    
    int moodTextWidth = getTextWidth(description);
    int moodTextX = (tft.width() - moodTextWidth) / 2;
    drawText(description, moodTextX, moodTextY, textColor);
    
    int statusTextWidth = getSmallTextWidth(status);
    int statusTextX = (tft.width() - statusTextWidth) / 2;
    drawSmallText(status, statusTextX, statusTextY, textColor);
}

void drawPet(PetMood mood) 
{
    int petX = tft.width() / 2 - 32;
    int petY = tft.height() / 2 - 15;
    int pixelSize = 4;
    int spriteW = 16 * pixelSize;
    int spriteH = 16 * pixelSize;

    clearArea(petX - 12, petY - 12, spriteW + 24, spriteH + 24);

    uint16_t bodyColor = TERMINAL_AMBER;
    uint16_t earColor = TERMINAL_AMBER;
    uint16_t earInnerColor = ILI9341_PINK;
    uint16_t faceColor = ILI9341_WHITE;
    uint16_t eyeColor = ILI9341_WHITE;
    uint16_t pupilColor = ILI9341_BLACK;
    uint16_t noseColor = ILI9341_BLACK;
    uint16_t mouthColor = ILI9341_BLACK;
    uint16_t tailColor = TERMINAL_AMBER;
    uint16_t tailTipColor = ILI9341_WHITE;
    
    if (petIsCold) 
    {
        bodyColor = TERMINAL_BLUE;
    }
    if (petIsHot) 
    {
        bodyColor = ILI9341_RED;
    }

    int bounceY = 0;
    if (mood == ECSTATIC) 
    {
        bounceY = sin(millis() / 150.0) * 6;
    }
    else if (mood == HAPPY) 
    {
        bounceY = sin(millis() / 500.0) * 2;
    }
    
    int animFrame = petFrame % 2;

    auto drawPixel = [&](int row, int col, uint16_t color) 
    {
        tft.fillRect(petX + col * pixelSize, petY + row * pixelSize + bounceY, pixelSize, pixelSize, color);
    };

    switch (mood) 
    {
        case ECSTATIC:
        {
            for (int r = 5; r <= 10; r++) 
            {
                for (int c = 4; c <= 11; c++) 
                {
                    if ((r == 5 && (c >= 5 && c <= 10)) || (r == 6 && (c >= 4 && c <= 11)) || (r >= 7 && r <= 10 && (c >= 3 && c <= 12))) 
                    {
                        drawPixel(r, c, bodyColor);
                    }
                }
            }
            
            drawPixel(1, 5, earColor);
            drawPixel(1, 10, earColor);
            drawPixel(2, 4, earColor);
            drawPixel(2, 5, earInnerColor);
            drawPixel(2, 10, earColor);
            drawPixel(2, 11, earInnerColor);
            drawPixel(4, 5, faceColor);
            drawPixel(4, 6, faceColor);
            drawPixel(4, 9, faceColor);
            drawPixel(4, 10, faceColor);
            drawPixel(5, 4, faceColor);
            drawPixel(5, 5, faceColor);
            drawPixel(5, 6, faceColor);
            drawPixel(5, 9, faceColor);
            drawPixel(5, 10, faceColor);
            drawPixel(5, 11, faceColor);
            drawPixel(5, 6, pupilColor);
            drawPixel(5, 9, pupilColor);
            drawPixel(7, 6, mouthColor);
            drawPixel(7, 7, mouthColor);
            drawPixel(7, 8, mouthColor);
            drawPixel(7, 9, mouthColor);
            drawPixel(6, 7, mouthColor);
            drawPixel(6, 8, mouthColor);
            
            int tailOffset = animFrame * 2 - 1;
            drawPixel(8, 12, tailColor);
            drawPixel(9, 13 + tailOffset, tailColor);
            drawPixel(10, 13, tailColor);
            drawPixel(11, 12, tailColor);
            drawPixel(11, 13, tailTipColor);
            
            if(animFrame == 0) 
            {
                drawPixel(2, 2, ILI9341_YELLOW);
                drawPixel(10, 1, ILI9341_YELLOW);
                drawPixel(7, 14, ILI9341_YELLOW);
            }
            else 
            {
                drawPixel(1, 13, ILI9341_YELLOW);
                drawPixel(12, 4, ILI9341_YELLOW);
                drawPixel(5, 1, ILI9341_YELLOW);
            }
            break;
        }

        case SAD:
        {
            for (int r = 6; r <= 10; r++) 
            {
                for (int c = 4; c <= 11; c++) 
                {
                    if ((r == 6 && (c >= 5 && c <= 10)) || (r >= 7 && r <= 10 && (c >= 4 && c <= 11))) 
                    {
                        drawPixel(r, c, bodyColor);
                    }
                }
            }
            
            drawPixel(4, 5, earColor);
            drawPixel(4, 10, earColor);
            drawPixel(5, 4, earColor);
            drawPixel(5, 5, earInnerColor);
            drawPixel(5, 10, earColor);
            drawPixel(5, 11, earInnerColor);
            drawPixel(5, 6, faceColor);
            drawPixel(5, 9, faceColor);
            drawPixel(6, 5, faceColor);
            drawPixel(6, 6, faceColor);
            drawPixel(6, 7, faceColor);
            drawPixel(6, 8, faceColor);
            drawPixel(6, 9, faceColor);
            drawPixel(6, 10, faceColor);
            drawPixel(6, 6, pupilColor);
            drawPixel(6, 9, pupilColor);
            drawPixel(8, 7, mouthColor);
            drawPixel(8, 8, mouthColor);
            drawPixel(9, 6, mouthColor);
            drawPixel(9, 9, mouthColor);
            drawPixel(7, 6 + animFrame, ILI9341_BLUE);
            break;
        }
            
        case AWAKE:
        {
            for (int r = 5; r <= 10; r++) 
            {
                for (int c = 4; c <= 11; c++) 
                {
                    if ((r == 5 && (c >= 5 && c <= 10)) || (r == 6 && (c >= 4 && c <= 11)) || (r >= 7 && r <= 10 && (c >= 3 && c <= 12))) 
                    {
                        drawPixel(r, c, bodyColor);
                    }
                }
            }
            
            drawPixel(2, 5, earColor);
            drawPixel(2, 10, earColor);
            drawPixel(3, 4, earColor);
            drawPixel(3, 5, earInnerColor);
            drawPixel(3, 10, earColor);
            drawPixel(3, 11, earInnerColor);
            drawPixel(4, 4, earColor);
            drawPixel(4, 11, earColor);
            drawPixel(4, 5, faceColor);
            drawPixel(4, 6, faceColor);
            drawPixel(4, 9, faceColor);
            drawPixel(4, 10, faceColor);
            drawPixel(5, 4, faceColor);
            drawPixel(5, 5, faceColor);
            drawPixel(5, 6, faceColor);
            drawPixel(5, 9, faceColor);
            drawPixel(5, 10, faceColor);
            drawPixel(5, 11, faceColor);
            drawPixel(6, 5, faceColor);
            drawPixel(6, 10, faceColor);
            drawPixel(5, 6, eyeColor);
            drawPixel(5, 9, eyeColor);
            
            if (animFrame == 0) 
            {
                drawPixel(5, 6, pupilColor);
                drawPixel(5, 9, pupilColor);
            }
            
            drawPixel(6, 7, noseColor);
            drawPixel(6, 8, noseColor);
            drawPixel(8, 12, tailColor);
            drawPixel(9, 13, tailColor);
            drawPixel(10, 13, tailColor);
            drawPixel(11, 12, tailColor);
            drawPixel(11, 13, tailTipColor);
            break;
        }
        
        case SCANNING:
        {
            for (int r = 5; r <= 10; r++) 
            {
                for (int c = 4; c <= 11; c++) 
                {
                    if ((r == 5 && (c >= 5 && c <= 10)) || (r == 6 && (c >= 4 && c <= 11)) || (r >= 7 && r <= 10 && (c >= 3 && c <= 12))) 
                    {
                        drawPixel(r, c, bodyColor);
                    }
                }
            }
            
            drawPixel(2, 5 + animFrame, earColor);
            drawPixel(2, 10 - animFrame, earColor);
            drawPixel(3, 4 + animFrame, earColor);
            drawPixel(3, 5 + animFrame, earInnerColor);
            drawPixel(3, 10 - animFrame, earColor);
            drawPixel(3, 11 - animFrame, earInnerColor);
            drawPixel(4, 5, faceColor);
            drawPixel(4, 6, faceColor);
            drawPixel(4, 9, faceColor);
            drawPixel(4, 10, faceColor);
            drawPixel(5, 4, faceColor);
            drawPixel(5, 5, faceColor);
            drawPixel(5, 6, faceColor);
            drawPixel(5, 9, faceColor);
            drawPixel(5, 10, faceColor);
            drawPixel(5, 11, faceColor);
            drawPixel(5, 6, TERMINAL_BLUE);
            drawPixel(5, 9, TERMINAL_BLUE);
            drawPixel(4, 6, TERMINAL_BLUE);
            drawPixel(6, 6, TERMINAL_BLUE);
            drawPixel(5, 5, TERMINAL_BLUE);
            drawPixel(5, 7, TERMINAL_BLUE);
            drawPixel(4, 9, TERMINAL_BLUE);
            drawPixel(6, 9, TERMINAL_BLUE);
            drawPixel(5, 8, TERMINAL_BLUE);
            drawPixel(5, 10, TERMINAL_BLUE);
            drawPixel(6, 7, noseColor);
            drawPixel(6, 8, noseColor);
            drawPixel(8, 12 + animFrame, tailColor);
            drawPixel(9, 13 + animFrame, tailColor);
            drawPixel(10, 13 + animFrame, tailColor);
            drawPixel(11, 12 + animFrame, tailColor);
            drawPixel(11, 13 + animFrame, tailTipColor);
            break;
        }
        
        case HAPPY:
        {
            for (int r = 5; r <= 10; r++) 
            {
                for (int c = 4; c <= 11; c++) 
                {
                    if ((r == 5 && (c >= 5 && c <= 10)) || (r == 6 && (c >= 4 && c <= 11)) || (r >= 7 && r <= 10 && (c >= 3 && c <= 12))) 
                    {
                        drawPixel(r, c, bodyColor);
                    }
                }
            }
            
            drawPixel(1, 5, earColor);
            drawPixel(1, 10, earColor);
            drawPixel(2, 4, earColor);
            drawPixel(2, 5, earInnerColor);
            drawPixel(2, 10, earColor);
            drawPixel(2, 11, earInnerColor);
            drawPixel(4, 5, faceColor);
            drawPixel(4, 6, faceColor);
            drawPixel(4, 9, faceColor);
            drawPixel(4, 10, faceColor);
            drawPixel(5, 4, faceColor);
            drawPixel(5, 5, faceColor);
            drawPixel(5, 6, faceColor);
            drawPixel(5, 9, faceColor);
            drawPixel(5, 10, faceColor);
            drawPixel(5, 11, faceColor);
            drawPixel(5, 6, pupilColor);
            drawPixel(5, 9, pupilColor);
            drawPixel(7, 6, mouthColor);
            drawPixel(7, 7, mouthColor);
            drawPixel(7, 8, mouthColor);
            drawPixel(7, 9, mouthColor);
            drawPixel(6, 7, mouthColor);
            drawPixel(6, 8, mouthColor);
            
            int tailOffset = animFrame * 2 - 1;
            drawPixel(8, 12, tailColor);
            drawPixel(9, 13 + tailOffset, tailColor);
            drawPixel(10, 13, tailColor);
            drawPixel(11, 12, tailColor);
            drawPixel(11, 13, tailTipColor);
            break;
        }
        
        case FOCUSED:
        {
            for (int r = 5; r <= 10; r++) 
            {
                for (int c = 4; c <= 11; c++) 
                {
                    if ((r == 5 && (c >= 5 && c <= 10)) || (r == 6 && (c >= 4 && c <= 11)) || (r >= 7 && r <= 10 && (c >= 3 && c <= 12))) 
                    {
                        drawPixel(r, c, bodyColor);
                    }
                }
            }
            
            drawPixel(2, 6, earColor);
            drawPixel(2, 9, earColor);
            drawPixel(3, 5, earColor);
            drawPixel(3, 6, earInnerColor);
            drawPixel(3, 9, earColor);
            drawPixel(3, 10, earInnerColor);
            drawPixel(4, 6, faceColor);
            drawPixel(4, 7, faceColor);
            drawPixel(4, 8, faceColor);
            drawPixel(4, 9, faceColor);
            drawPixel(5, 5, faceColor);
            drawPixel(5, 6, faceColor);
            drawPixel(5, 7, faceColor);
            drawPixel(5, 8, faceColor);
            drawPixel(5, 9, faceColor);
            drawPixel(5, 10, faceColor);
            drawPixel(5, 6, pupilColor);
            drawPixel(5, 9, pupilColor);
            drawPixel(4, 6, pupilColor);
            drawPixel(6, 6, pupilColor);
            drawPixel(4, 9, pupilColor);
            drawPixel(6, 9, pupilColor);
            drawPixel(6, 7, noseColor);
            drawPixel(6, 8, noseColor);
            drawPixel(8, 12, tailColor);
            drawPixel(9, 13, tailColor);
            drawPixel(10, 13, tailColor);
            drawPixel(11, 12, tailColor);
            drawPixel(11, 13, tailTipColor);
            break;
        }
        
        case BORED:
        {
            for (int r = 6; r <= 10; r++) 
            {
                for (int c = 4; c <= 11; c++) 
                {
                    if ((r == 6 && (c >= 5 && c <= 10)) || (r >= 7 && r <= 10 && (c >= 4 && c <= 11))) 
                    {
                        drawPixel(r, c, bodyColor);
                    }
                }
            }
            
            drawPixel(4, 5, earColor);
            drawPixel(4, 10, earColor);
            drawPixel(5, 4, earColor);
            drawPixel(5, 5, earInnerColor);
            drawPixel(5, 10, earColor);
            drawPixel(5, 11, earInnerColor);
            drawPixel(5, 6, faceColor);
            drawPixel(5, 9, faceColor);
            drawPixel(6, 5, faceColor);
            drawPixel(6, 6, faceColor);
            drawPixel(6, 7, faceColor);
            drawPixel(6, 8, faceColor);
            drawPixel(6, 9, faceColor);
            drawPixel(6, 10, faceColor);
            drawPixel(6, 6, pupilColor);
            drawPixel(6, 9, pupilColor);
            drawPixel(7, 7, mouthColor);
            drawPixel(7, 8, mouthColor);
            drawPixel(9, 12, tailColor);
            drawPixel(10, 13, tailColor);
            drawPixel(11, 12, tailColor);
            break;
        }
        
        case SLEEPING:
        {
            for (int r = 6; r <= 10; r++) 
            {
                for (int c = 5; c <= 10; c++) 
                {
                    if ((r == 6 && (c >= 6 && c <= 9)) || (r == 7 && (c >= 5 && c <= 10)) || (r >= 8 && r <= 10 && (c >= 4 && c <= 11))) 
                    {
                        drawPixel(r, c, bodyColor);
                    }
                }
            }
            
            drawPixel(5, 6, earColor);
            drawPixel(5, 9, earColor);
            drawPixel(6, 6, faceColor);
            drawPixel(6, 9, faceColor);
            drawPixel(7, 5, faceColor);
            drawPixel(7, 6, faceColor);
            drawPixel(7, 7, faceColor);
            drawPixel(7, 8, faceColor);
            drawPixel(7, 9, faceColor);
            drawPixel(7, 10, faceColor);
            drawPixel(7, 6, pupilColor);
            drawPixel(7, 9, pupilColor);
            drawTinyText("Z", petX - 20, petY - 12 - (animFrame * 2), ILI9341_WHITE);
            drawTinyText("z", petX, petY - 8 - (animFrame * 2), ILI9341_WHITE);
            break;
        }
    }
}

void updatePetScreen() 
{
    if (!inPetScreen) return;

    if (millis() - lastFrameUpdate > 400) 
    {
        petFrame = (petFrame + 1) % 2;
        lastFrameUpdate = millis();
    }
    
    if (specialMoodEndTime > 0 && millis() >= specialMoodEndTime) 
    {
        specialMoodEndTime = 0;
        currentPetMood = AWAKE;
    }

    int n = WiFi.scanComplete();
    if (n >= 0) 
    {
        scanSessionCount++;
        wifiScanResultCount = n;
        currentOpenCount = 0;
        for (int i = 0; i < n; i++) 
        {
            if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) 
            {
                currentOpenCount++;
            }
        }
        currentClosedCount = n - currentOpenCount;
        totalNetworksFoundSinceEntry += n;
        
        drawPetStatsDashboard();

        if (currentOpenCount > 0) 
        {
            currentPetMood = ECSTATIC;
            specialMoodEndTime = millis() + 4000;
            
            playColorStrobeFlash();

            tft.fillScreen(TERMINAL_BG);
            drawTerminalHeader();
            drawPetStatsDashboard();

            
            drawTerminalHeader();
            drawPetStatsDashboard();

        }
        else if (n >= 4) 
        {
            currentPetMood = HAPPY;
            specialMoodEndTime = millis() + 6000;
        } 
        else 
        {
            currentPetMood = SAD;
            specialMoodEndTime = millis() + 3000;
        }
        
        WiFi.scanDelete();
        nextScanTime = millis() + SCAN_INTERVAL;
    }

    if (specialMoodEndTime == 0) 
    {
        if (WiFi.scanComplete() == -1) 
        {
            currentPetMood = SCANNING;
        } 
        else if (millis() >= nextScanTime) 
        {
            WiFi.scanNetworks(true);
            currentPetMood = SCANNING;
        } 
        else if (millis() - lastInteractionTime > SLEEP_TIMEOUT) 
        {
            currentPetMood = SLEEPING;
        } 
        else 
        {
            currentPetMood = AWAKE;
        }
    }
    
    drawPet(currentPetMood);
    String newStatusText = String(getStatusDescription(currentPetMood, petFrame));
    if (newStatusText != currentMoodText) 
    {
        drawMoodDescription(currentPetMood);
        currentMoodText = newStatusText;
    }
}

void drawPetPopup() 
{
    int popupW = 260;
    int popupH = 100;
    int popupX = (tft.width() - popupW) / 2;
    int popupY = (tft.height() - popupH) / 2;

    tft.fillRoundRect(popupX, popupY, popupW, popupH, 8, TERMINAL_BG);
    tft.drawRoundRect(popupX, popupY, popupW, popupH, 8, TERMINAL_AMBER);
    tft.drawRoundRect(popupX+1, popupY+1, popupW-2, popupH-2, 8, TERMINAL_AMBER);
    
    drawText("A new entity detected...", popupX + 20, popupY + 10, TERMINAL_GREEN);
    drawText("Awaken it?", popupX + 70, popupY + 30, TERMINAL_GREEN);
    
    int btnW = 80;
    int btnY = popupY + 65;
    drawButton("Yes", popupX + 30, btnY, btnW, selectedPopupButton == 0);
    drawButton("No", popupX + popupW - btnW - 30, btnY, btnW, selectedPopupButton == 1);
}


/*
void handleEncoderRotation(int direction)
{
    if (millis() - lastEncoderPress < encoderDebounce) return;

    if (inPetPopup) 
    {
        selectedPopupButton = (selectedPopupButton + direction + 2) % 2;
        drawPetPopup();
        return;
    }
    if (inPetScreen) 
    {
        currentPetMood = HAPPY;
        lastMoodChangeTime = millis();
        return;
    }

    if (inMainScreen)
    {
        selectedButton = (selectedButton + direction + 6) % 6;
        drawButtons();
    }
    else if (inWifiScreen)
    {
        selectedWifiButton = (selectedWifiButton + direction + 3) % 3;
        drawWifiButtons();
    }
    else if (inWifiScanScreen)
    {
        if (direction > 0)
        {
            int maxOffset = max(0, wifiScanResultCount - MAX_VISIBLE_WIFI_ROWS);
            if (wifiScrollOffset < maxOffset)
            {
                wifiScrollOffset++;
                updateWifiScanScreen();
            }
        }
        else
        {
            if (wifiScrollOffset > 0)
            {
                wifiScrollOffset--;
                updateWifiScanScreen();
            }
        }
    }
    else if (inWaterfallScreen)
    {
        selectedWaterfallButton = (selectedWaterfallButton + direction + 4) % 4;
        drawWaterfallButtons();
    }
    else if (inBleScreen)
    {
        selectedBleButton = (selectedBleButton + direction + 4) % 4;
        drawBleButtons();
    }
    else if (inBleWaterfallScreen)
    {
        selectedBleWaterfallButton = (selectedBleWaterfallButton + direction + 2) % 2;
        drawBleWaterfallButtons();
    }
    else if (inBleScanScreen)
    {
        if (direction > 0)
        {
            int maxOffset = max(0, bleScanResultCount - MAX_VISIBLE_WIFI_ROWS);
            if (bleScrollOffset < maxOffset)
            {
                bleScrollOffset++;
                drawBleTable();
            }
        }
        else
        {
            if (bleScrollOffset > 0)
            {
                bleScrollOffset--;
                drawBleTable();
            }
        }
    }
    else if (inClassicScanScreen)
    {
        if (direction > 0)
        {
            int maxOffset = max(0, classicScanResultCount - MAX_VISIBLE_WIFI_ROWS);
            if (classicScrollOffset < maxOffset)
            {
                classicScrollOffset++;
                drawClassicTable();
            }
        }
        else
        {
            if (classicScrollOffset > 0)
            {
                classicScrollOffset--;
                drawClassicTable();
            }
        }
    }
}

void handleEncoderButton()
{
    if (inPetPopup) 
    {
        if (selectedPopupButton == 0) 
        {
            showPetScreen();
        } 
        else 
        {
            drawMainScreen();
            updateDisplay(); 
        }
        inPetPopup = false;
        return;
    }

    if (inMainScreen)
    {
        switch (selectedButton)
        {
            case 0: showTemperatureScreen(); break;
            case 1: showHumidityScreen(); break;
            case 2: showGraphScreen(); break;
            case 3: showWifiScreen(); break;
            case 4: showBleScreen(); break;
            case 5: showPetScreen(); break;
        }
    } 
    else if (inWifiScreen)
    {
        switch (selectedWifiButton)
        {
            case 0: showWifiScanScreen(); break;
            case 1: showWaterfallScreen(); break;
            case 2: drawMainScreen(); break;
        }
    } 
    else if (inWifiScanScreen)
    {
        inWifiScanScreen = false;
        showWifiScreen();
    } 
    else if (inWaterfallScreen)
    {
        switch (selectedWaterfallButton)
        {
            case 0:
                setPromiscuousMode(false);
                scanNetworks();
                maxPps = 0;
                setPromiscuousMode(true, currentChannel);
                break;
            case 1:
                currentChannel = (currentChannel - 2 + NUM_CHANNELS) % NUM_CHANNELS + 1;
                maxPps = 0;
                setPromiscuousMode(true, currentChannel);
                drawWaterfallHeader();
                break;
            case 2:
                currentChannel = (currentChannel % NUM_CHANNELS) + 1;
                maxPps = 0;
                setPromiscuousMode(true, currentChannel);
                drawWaterfallHeader();
                break;
            case 3:
                setPromiscuousMode(false);
                inWaterfallScreen = false;
                showWifiScreen();
                break;
        }
    }
    else if (inBleScreen)
    {
        switch(selectedBleButton)
        {
            case 0: showBleScanScreen(); break;
            case 1: showClassicScanScreen(); break;
            case 2: showBleGraphScreen(); break;
            case 3: drawMainScreen(); break;
        }
    }
    else if (inBleWaterfallScreen)
    {
        switch(selectedBleWaterfallButton)
        {
            case 0:
                blePacketCount = 0;
                lastBleCount = 0;
                maxBlePps = 0;
                break;
            case 1:
                stopBleGraphScan();
                inBleWaterfallScreen = false;
                showBleScreen();
                break;
        }
    }
    else if (inBleScanScreen)
    {
        inBleScanScreen = false;
        showBleScreen();
    }
    else if (inClassicScanScreen)
    {
        inClassicScanScreen = false;
        showBleScreen();
    }
    else
    {
        drawMainScreen();
        lastSensorRead = 0;
        lastBatteryCheck = 0;
        lastTime = "";
        lastTemp = "";
        lastHumidity = "";
        lastWifi = "";
        lastBattery = "";
        lastHeap = "";
        lastUptime = "";
        updateDisplay();
    }
}
*/
void updateDisplay()
{
    if (!inMainScreen) return;
    if (WiFi.status() == WL_CONNECTED)
    {
        timeClient.update();
    }
    String newTime = timeClient.getFormattedTime();
    if (millis() - lastSensorRead > 2000)
    {
        lastSensorRead = millis();
        readDHTData();
    }
    String newTemp = String(temperature, 1) + "C";
    String newHumidity = String(humidity, 0) + "%";
    if (millis() - lastBatteryCheck > BATTERY_CHECK_INTERVAL)
    {
        lastBatteryCheck = millis();
        updateBatteryReadings();
    }
    String newBattery = getBatteryStatus();
    String newHeap = String(ESP.getFreeHeap() / 1024) + "KB";
    String newUptime = String(millis() / 60000) + "m";
    String newWifi = WiFi.status() == WL_CONNECTED ? "Connected" : "Offline";
    uint16_t wifiColor = WiFi.status() == WL_CONNECTED ? TERMINAL_GREEN : ILI9341_RED;
    int valueColumn = 15 + LABEL_WIDTH;
    if (newTime != lastTime)
    {
        clearArea(valueColumn, 40, 100, FONT_HEIGHT);
        drawText(newTime.c_str(), valueColumn, 40, TERMINAL_AMBER);
        lastTime = newTime;
    }
    if (newTemp != lastTemp)
    {
        clearArea(valueColumn, 40 + LINE_SPACING, 60, FONT_HEIGHT);
        drawText(newTemp.c_str(), valueColumn, 40 + LINE_SPACING);
        lastTemp = newTemp;
    }
    if (newHumidity != lastHumidity)
    {
        clearArea(valueColumn, 40 + LINE_SPACING * 2, 60, FONT_HEIGHT);
        drawText(newHumidity.c_str(), valueColumn, 40 + LINE_SPACING * 2);
        lastHumidity = newHumidity;
    }
    if (newWifi != lastWifi)
    {
        clearArea(valueColumn, 40 + LINE_SPACING * 3, 100, FONT_HEIGHT);
        drawText(newWifi.c_str(), valueColumn, 40 + LINE_SPACING * 3, wifiColor);
        lastWifi = newWifi;
    }
    if (newBattery != lastBattery)
    {
        clearArea(valueColumn, 40 + LINE_SPACING * 4, 120, FONT_HEIGHT);
        drawText(newBattery.c_str(), valueColumn, 40 + LINE_SPACING * 4, getBatteryColor());
        lastBattery = newBattery;
    }
    if (newHeap != lastHeap)
    {
        clearArea(valueColumn, 40 + LINE_SPACING * 5, 80, FONT_HEIGHT);
        drawText(newHeap.c_str(), valueColumn, 40 + LINE_SPACING * 5);
        lastHeap = newHeap;
    }
    if (newUptime != lastUptime)
    {
        clearArea(valueColumn, 40 + LINE_SPACING * 6, 80, FONT_HEIGHT);
        drawText(newUptime.c_str(), valueColumn, 40 + LINE_SPACING * 6);
        lastUptime = newUptime;
    }
}

#endif