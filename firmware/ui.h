#ifndef UI_H
#define UI_H

#include "config.h"
#include "sensors.h"
#include "networks.h"
#include "bluetooth.h"
#include <Arduino.h>

int moveMacTextUpSukiCriesHard = 8;

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
void showBleGraphScreen();
void updateBleScanScreen();

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
    int textWidth = getTextWidth(text);
    int textX = x + (width - textWidth) / 2;
    drawText(text, textX, y + 4, textColor);
}

void drawButtons()
{
    int buttonY = 40 + LINE_SPACING * 7 + 10;
    int buttonWidth = (tft.width() - 20 - BUTTON_SPACING * 3) / 4;
    tft.fillRect(8, buttonY - 2, tft.width() - 16, BUTTON_HEIGHT + 4, TERMINAL_BG);

    const char* buttonTexts[] = {"Temp", "Humi", "Graph", "WiFi", "BLE"};
    int numButtons = 5;
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
    const char* title = "HackOchi v0.9";
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
    inGraphScreen = false;
}

void showTemperatureScreen()
{
    inMainScreen = false;
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = false;
    inBleScanScreen = false;
    inGraphScreen = false;
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
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = false;
    inBleScanScreen = false;
    inGraphScreen = false;
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

    if (firstRun)
    {
        inMainScreen = false;
        inWifiScreen = false;
        inWifiScanScreen = false;
        inWaterfallScreen = false;
        inBleScreen = false;
        inBleScanScreen = false;
        inGraphScreen = true;
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
    inGraphScreen = false;
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
        String ipStr   = WiFi.localIP().toString();
        String macStr  = WiFi.macAddress();
        int32_t rssi   = WiFi.RSSI();
        int channel    = WiFi.channel();
        int leftX = 10, leftY = 35, leftW = 140, leftH = 140;
        int cornerRadius = 8;
        tft.fillRoundRect(leftX, leftY, leftW, leftH, cornerRadius, TERMINAL_BG);
        tft.drawRoundRect(leftX, leftY, leftW, leftH, cornerRadius, TERMINAL_GREEN);
        tft.drawRoundRect(leftX + 1, leftY + 1, leftW - 2, leftH - 2, cornerRadius - 1, TERMINAL_GREEN);
        drawTinyText("SSID:", leftX + 5, leftY + 15, TERMINAL_AMBER);
        drawSmallText(ssidStr.c_str(), leftX + 50, leftY + 15);
        drawTinyText("IP:", leftX + 5, leftY + 35, TERMINAL_AMBER);
        drawSmallText(ipStr.c_str(), leftX + 50, leftY + 35);
        drawTinyText("MAC:", leftX + 5, leftY + 55, TERMINAL_AMBER);
        drawTinyText(macStr.c_str(), leftX + 50, leftY + 55, TERMINAL_BLUE);
        drawTinyText("CH:", leftX + 5, leftY + 75, TERMINAL_AMBER);
        char chStr[6]; snprintf(chStr, sizeof(chStr), "%d", channel);
        drawSmallText(chStr, leftX + 50, leftY + 75);
        int topRightX = 160, topRightY = 35, topRightW = 150, topRightH = 80;
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
        drawText("WiFi Disconnected", 80, 120, ILI9341_RED);
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
    inGraphScreen = false;
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
        int macTextWidth  = getTinyTextWidth(macDisplay.c_str());
        int macBoxHeight  = 14;
        int macBoxWidth   = macTextWidth + 10;
        int macX = 10;
        int macY = y + 12;
        tft.fillRoundRect(macX, macY, macBoxWidth, macBoxHeight, 6, TERMINAL_BG);
        tft.drawRoundRect(macX, macY, macBoxWidth, macBoxHeight, 6, TERMINAL_BLUE);
        int textX = macX + (macBoxWidth - macTextWidth) / 2;
        int textY = macY + (macBoxHeight / 2) - moveMacTextUpSukiCriesHard;
        drawTinyText(macDisplay.c_str(), textX, textY, TERMINAL_BLUE);
        int32_t cappedRssi   = net.rssi > -100 ? net.rssi : -100;
        int rssiStrength     = map(cappedRssi, -100, -30, 0, RSSI_BAR_MAX_WIDTH);
        uint16_t barColor    = (net.rssi > -60) ? TERMINAL_GREEN : (net.rssi > -80) ? TERMINAL_AMBER : ILI9341_RED;
        tft.fillRect(160, y, RSSI_BAR_MAX_WIDTH, 7, TERMINAL_BG);
        tft.fillRect(160, y, rssiStrength, 7, barColor);
        tft.drawRect(160, y, RSSI_BAR_MAX_WIDTH, 7, TERMINAL_GREEN);
        String enc = getEncryptionTypeString(net.encryptionType).substring(0, 3);
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
    inGraphScreen = false;
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

void showBleScreen()
{
    inMainScreen = false;
    inWifiScreen = false;
    inWifiScanScreen = false;
    inWaterfallScreen = false;
    inBleScreen = true;
    inBleScanScreen = false;
    inGraphScreen = false;
    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    drawText("Bluetooth LE", 80, 100, TERMINAL_BLUE);
    drawBleButtons();
}

void drawBleButtons()
{
    int buttonY = tft.height() - BUTTON_HEIGHT - 10;
    int buttonWidth = (tft.width() - 20 - BUTTON_SPACING * 2) / 3;
    clearArea(8, buttonY - 2, tft.width() - 16, BUTTON_HEIGHT + 4);
    drawButton("Scan", 10, buttonY, buttonWidth, selectedBleButton == 0);
    drawButton("Graph", 10 + (buttonWidth + BUTTON_SPACING), buttonY, buttonWidth, selectedBleButton == 1);
    drawButton("Back", 10 + 2 * (buttonWidth + BUTTON_SPACING), buttonY, buttonWidth, selectedBleButton == 2);
}

void showBleScanScreen()
{
    inBleScreen = false;
    inBleScanScreen = true;
    inGraphScreen = false;
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
    inGraphScreen = false;
    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    drawText("BLE Graph", 90, 80, TERMINAL_AMBER);
    drawText("Coming Soon!", 75, 120);
    drawText("Press SELECT to return", 20, 220, TERMINAL_AMBER);
}

void handleEncoderRotation(int direction)
{
    if (millis() - lastEncoderPress < encoderDebounce) return;
    if (inMainScreen)
    {
        selectedButton = (selectedButton + direction + 5) % 5;
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
        selectedBleButton = (selectedBleButton + direction + 3) % 3;
        drawBleButtons();
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
}

void handleEncoderButton()
{
    if (millis() - lastEncoderPress < encoderDebounce) return;
    lastEncoderPress = millis();
    if (inMainScreen)
    {
        switch (selectedButton)
        {
            case 0: showTemperatureScreen(); break;
            case 1: showHumidityScreen(); break;
            case 2: showGraphScreen(); break;
            case 3: showWifiScreen(); break;
            case 4: showBleScreen(); break;
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
            case 1: showBleGraphScreen(); break;
            case 2: drawMainScreen(); break;
        }
    }
    else if (inBleScanScreen)
    {
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