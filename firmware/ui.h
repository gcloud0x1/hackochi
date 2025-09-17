#ifndef UI_H
#define UI_H

#include "config.h"
#include "sensors.h"
#include "networks.h"

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
void handleButtonPress(int button);
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
    int buttonWidth = (tft.width() - 20 - BUTTON_SPACING * 2) / 3;
    tft.fillRect(8, buttonY - 2, tft.width() - 16, BUTTON_HEIGHT + 4, TERMINAL_BG);
    int startButton = 0;
    if (selectedButton >= 3)
    {
        startButton = 1;
    }
    const char* buttonTexts[] = {"Temp", "Humi", "Graph", "WiFi"};
    for (int i = 0; i < 3; i++)
    {
        int buttonIndex = startButton + i;
        if (buttonIndex < 4)
        {
            int xPos = 10 + (buttonWidth + BUTTON_SPACING) * i;
            drawButton(buttonTexts[buttonIndex], xPos, buttonY, buttonWidth, selectedButton == buttonIndex);
        }
    }
}

void drawTerminalHeader()
{
    tft.fillRect(0, 0, tft.width(), 30, TERMINAL_HEADER);
    const char* title = "HackOchi v0.7";
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
}

void showTemperatureScreen()
{
    inMainScreen = false;
    inWifiScreen = false;
    inWifiScanScreen = false;
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
}

void showWifiScreen()
{
    inMainScreen = false;
    inWifiScreen = true;
    inWifiScanScreen = false;
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
    if (!inWifiScreen)
    {
        return;
    }
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
            selectedButton = (selectedButton - 1 + 4) % 4;
            drawButtons();
        }
        else if (button == BUTTON_RIGHT)
        {
            selectedButton = (selectedButton + 1) % 4;
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
                case 3:
                    showWifiScreen();
                    break;
            }
        }
    }
    else if (inWifiScreen)
    {
        if (button == BUTTON_LEFT)
        {
            selectedWifiButton = (selectedWifiButton - 1 + 3) % 3;
            drawWifiButtons();
        }
        else if (button == BUTTON_RIGHT)
        {
            selectedWifiButton = (selectedWifiButton + 1) % 3;
            drawWifiButtons();
        }
        else if (button == BUTTON_SELECT)
        {
            switch (selectedWifiButton)
            {
                case 0:
                    showWifiScanScreen();
                    break;
                case 1:
                    break;
                case 2:
                    drawMainScreen();
                    break;
            }
        }
    }
    else if (inWifiScanScreen)
    {
        if (button == BUTTON_LEFT)
        {
            if (wifiScrollOffset > 0)
            {
                wifiScrollOffset--;
                updateWifiScanScreen();
            }
        }
        else if (button == BUTTON_RIGHT)
        {
            int maxOffset = max(0, wifiScanResultCount - MAX_VISIBLE_WIFI_ROWS);
            if (wifiScrollOffset < maxOffset)
            {
                wifiScrollOffset++;
                updateWifiScanScreen();
            }
        }
        else if (button == BUTTON_SELECT)
        {
            inWifiScanScreen = false;
            showWifiScreen();
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