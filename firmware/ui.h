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
    const char* title = "HackOchi v0.5";
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
    inMainScreen = false;
    tft.fillScreen(TERMINAL_BG);
    drawTerminalHeader();
    
    drawText("GRAPH VIEW", 80, 40, TERMINAL_GREEN);
    tft.drawFastHLine(10, 60, tft.width() - 20, TERMINAL_GREEN);
    
    drawText("Temperature Graph", 20, 80);
    for (int i = 0; i < 10; i++)
    {
        int height = random(5, 50);
        tft.fillRect(20 + i * 30, 150 - height, 20, height, TERMINAL_GREEN);
    }
    
    drawText("Humidity Graph", 20, 170);
    for (int i = 0; i < 10; i++)
    {
        int height = random(5, 50);
        tft.fillRect(20 + i * 30, 240 - height, 20, height, TERMINAL_BLUE);
    }
    
    drawText("Press SELECT to return", 20, 250, TERMINAL_AMBER);
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