#ifndef UI_H
#define UI_H

#include "config.h"
#include "sensors.h"

void setCurrentFont();
int getTextWidth(const char* text);
void drawText(const char* text, int x, int y, uint16_t color = TERMINAL_GREEN);
void clearArea(int x, int y, int width, int height);
void drawButton(const char* text, int x, int y, int width, bool selected = false);
void drawButtons();
void drawTerminalHeader();
void drawMainScreen();
void showTemperatureScreen();
void showHumidityScreen();
void showGraphScreen();
void handleButtonPress(int button);
void updateDisplay();

void setCurrentFont()
{
    tft.setFont(&FreeMono9pt7b);
    tft.setTextSize(1);
    tft.setTextColor(TERMINAL_GREEN);
}

int getTextWidth(const char* text)
{
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    return w;
}

void drawText(const char* text, int x, int y, uint16_t color)
{
    setCurrentFont();
    tft.setTextColor(color);
    tft.setCursor(x, y + FONT_BASELINE);
    tft.print(text);
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
    
    int textWidth = getTextWidth(text);
    int textX = x + (width - textWidth) / 2;
    drawText(text, textX, y + 4, textColor);
}

void drawButtons()
{
    int buttonY = 40 + LINE_SPACING * 7 + 10;
    int buttonWidth = (tft.width() - 20 - BUTTON_SPACING * 2) / 3;
    
    tft.fillRect(8, buttonY - 2, tft.width() - 16, BUTTON_HEIGHT + 4, TERMINAL_BG);
    
    drawButton("Temp", 10, buttonY, buttonWidth, selectedButton == 0);
    drawButton("Humi", 10 + buttonWidth + BUTTON_SPACING, buttonY, buttonWidth, selectedButton == 1);
    drawButton("Graph", 10 + (buttonWidth + BUTTON_SPACING) * 2, buttonY, buttonWidth, selectedButton == 2);
}

void drawTerminalHeader()
{
    tft.fillRect(0, 0, tft.width(), 30, TERMINAL_HEADER);
    const char* title = "HackOchi v0.6";
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
}

void showTemperatureScreen()
{
    inMainScreen = false;
    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    
    drawText("TEMPERATURE DETAILS", 40, 40, TERMINAL_AMBER);
    tft.drawFastHLine(10, 60, tft.width() - 20, TERMINAL_GREEN);
    
    char tempStr[20];
    snprintf(tempStr, sizeof(tempStr), "Current: %.1f C", temperature);
    drawText(tempStr, 20, 80);
    
    snprintf(tempStr, sizeof(tempStr), "Min: 20.0 C");
    drawText(tempStr, 20, 110);
    
    snprintf(tempStr, sizeof(tempStr), "Max: 35.0 C");
    drawText(tempStr, 20, 140);
    
    snprintf(tempStr, sizeof(tempStr), "Avg: 27.5 C");
    drawText(tempStr, 20, 170);
    
    drawText("Press SELECT to return", 20, 220, TERMINAL_AMBER);
}

void showHumidityScreen()
{
    inMainScreen = false;
    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    
    drawText("HUMIDITY DETAILS", 40, 40, TERMINAL_BLUE);
    tft.drawFastHLine(10, 60, tft.width() - 20, TERMINAL_GREEN);
    
    char humStr[20];
    snprintf(humStr, sizeof(humStr), "Current: %.0f %%", humidity);
    drawText(humStr, 20, 80);
    
    snprintf(humStr, sizeof(humStr), "Min: 30 %%");
    drawText(humStr, 20, 110);
    
    snprintf(humStr, sizeof(humStr), "Max: 80 %%");
    drawText(humStr, 20, 140);
    
    snprintf(humStr, sizeof(humStr), "Avg: 55 %%");
    drawText(humStr, 20, 170);
    
    drawText("Press SELECT to return", 20, 220, TERMINAL_AMBER);
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

    if (firstRun) {
        inMainScreen = false;
        tft.fillScreen(TERMINAL_BG);
        drawTerminalHeader();
        for (int i = 0; i < numPoints; i++) {
            tempData[i] = 25.0;
            humData[i] = 50.0;
        }
        readDHTData();
        if (!isnan(temperature) && temperature >= 0 && temperature <= 100) {
            tempData[0] = temperature;
        }
        if (!isnan(humidity) && humidity >= 0 && humidity <= 100) {
            humData[0] = humidity;
        }
        dataCount = 1;
        firstRun = false;
    }

    if (millis() - lastUpdate >= 3000) {
        readDHTData();
        float newTemp = temperature;
        float newHum = humidity;
        if (isnan(newTemp) || newTemp < 0 || newTemp > 100) {
            newTemp = tempData[(dataCount - 1) % numPoints];
        }
        if (isnan(newHum) || newHum < 0 || newHum > 100) {
            newHum = humData[(dataCount - 1) % numPoints];
        }
        if (dataCount < numPoints) {
            tempData[dataCount] = newTemp;
            humData[dataCount] = newHum;
            dataCount++;
        } else {
            for (int i = 0; i < numPoints - 1; i++) {
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
    for (int i = 1; i < dataCount && i < numPoints; i++) {
        if (tempData[i] < tempMin) tempMin = tempData[i];
        if (tempData[i] > tempMax) tempMax = tempData[i];
    }
    tempMin -= 5.0;
    tempMax += 5.0;
    if (tempMax - tempMin < 10.0) {
        tempMax = tempMin + 10.0;
    }

    tft.drawFastHLine(0, tempGraphY - 1, tft.width(), TERMINAL_GREEN);
    tft.drawFastHLine(0, tempGraphY + graphHeight, tft.width(), TERMINAL_GREEN);
    tft.drawFastVLine(0, tempGraphY - 1, graphHeight + 2, TERMINAL_GREEN);
    tft.drawFastVLine(tft.width() - 1, tempGraphY - 1, graphHeight + 2, TERMINAL_GREEN);

    for (int i = 1; i <= 3; i++) {
        int y = tempGraphY + (graphHeight * i) / 4;
        tft.drawFastHLine(1, y, graphWidth, gridColor);
    }
    for (int i = 1; i <= 4; i++) {
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
    for (int i = 0; i < dataCount - 1 && i < numPoints - 1; i++) {
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
    for (int i = 1; i < dataCount && i < numPoints; i++) {
        if (humData[i] < humMin) humMin = humData[i];
        if (humData[i] > humMax) humMax = humData[i];
    }
    humMin -= 10.0;
    humMax += 10.0;
    if (humMax - humMin < 20.0) {
        humMax = humMin + 20.0;
    }

    tft.drawFastHLine(0, humGraphY - 1, tft.width(), TERMINAL_GREEN);
    tft.drawFastHLine(0, humGraphY + graphHeight, tft.width(), TERMINAL_GREEN);
    tft.drawFastVLine(0, humGraphY - 1, graphHeight + 2, TERMINAL_GREEN);
    tft.drawFastVLine(tft.width() - 1, humGraphY - 1, graphHeight + 2, TERMINAL_GREEN);

    for (int i = 1; i <= 3; i++) {
        int y = humGraphY + (graphHeight * i) / 4;
        tft.drawFastHLine(1, y, graphWidth, gridColor);
    }
    for (int i = 1; i <= 4; i++) {
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

    for (int i = 0; i < dataCount - 1 && i < numPoints - 1; i++) {
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

    if (digitalRead(BUTTON_SELECT) == LOW) {
        firstRun = true;
    }
}

void handleButtonPress(int button)
{
    if (millis() - lastButtonPress < buttonDebounce)
    {
        return;
    }
    lastButtonPress = millis();
    
    if (inMainScreen)
    {
        if (button == BUTTON_LEFT)
        {
            selectedButton = (selectedButton - 1 + 3) % 3;
            drawButtons();
        }
        else if (button == BUTTON_RIGHT)
        {
            selectedButton = (selectedButton + 1) % 3;
            drawButtons();
        }
        else if (button == BUTTON_SELECT)
        {
            switch (selectedButton)
            {
                case 0:
                    showTemperatureScreen();
                    break;
                case 1:
                    showHumidityScreen();
                    break;
                case 2:
                    showGraphScreen();
                    break;
            }
        }
    }
    else
    {
        if (button == BUTTON_SELECT)
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
}

void updateDisplay()
{
    if (!inMainScreen)
    {
        return;
    }
    
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
