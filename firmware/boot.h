#ifndef BOOT_H
#define BOOT_H

#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <esp_task_wdt.h>
#include "config.h"
#include "ui.h"
#include "sensors.h"
#include "networks.h"
#include "credentials.h"

extern Adafruit_ILI9341 tft;
extern NTPClient timeClient;
extern float batteryVoltage;
extern int batteryPercentage;

void setupDisplay();
void setupEncoder();
void runBootSequence();
void drawTypingText(const char* text, int x, int y, uint16_t color, int delayMs = 40);

void setupDisplay()
{
    Serial.println("Initializing display...");
    tft.begin();
    tft.setRotation(1);
    setCurrentFont();
    tft.setTextWrap(false);
    tft.fillScreen(TERMINAL_BG);
    Serial.println("Display initialized");
}

void setupEncoder()
{
    pinMode(ENCODER_CLK, INPUT_PULLUP);
    pinMode(ENCODER_DT, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    Serial.println("Encoder initialized");
}

void fadeScreen(uint16_t color, int steps = 20, int delayMs = 50)
{
    Serial.println("Fading screen...");
    for (int i = steps; i >= 0; i--)
    {
        tft.fillScreen(tft.color565(i * (color >> 11) / steps, i * ((color >> 5) & 0x3F) / steps, i * (color & 0x1F) / steps));
        delay(delayMs);
        esp_task_wdt_reset();
    }
}

void drawTypingText(const char* text, int x, int y, uint16_t color, int delayMs)
{
    Serial.println("Drawing typing text: " + String(text));
    clearArea(x, y, getTextWidth(text), FONT_HEIGHT);
    String currentText = "";
    for (int i = 0; text[i] != '\0'; i++)
    {
        currentText += text[i];
        clearArea(x, y, getTextWidth(text), FONT_HEIGHT);
        drawText(currentText.c_str(), x, y, color);
        delay(delayMs);
        esp_task_wdt_reset();
    }
    Serial.println("Typing text complete: " + String(text));
}

void drawBootLogo()
{
    Serial.println("Drawing boot logo with neural pulse...");
    tft.fillScreen(TERMINAL_BG);
    setCurrentFont();
    const char* logo = "HackOchi";
    int textWidth = getTextWidth(logo);
    int x = (tft.width() - textWidth) / 2;
    int y = tft.height() / 2 - FONT_HEIGHT / 2;
    
    for (int brightness = 0; brightness <= 255; brightness += 15)
    {
        tft.fillScreen(TERMINAL_BG);
        for (int angle = 0; angle < 360; angle += 30)
        {
            int len = 30 + 10 * sin(millis() / 500.0);
            int xEnd = x + textWidth / 2 + len * cos(angle * PI / 180);
            int yEnd = y + FONT_HEIGHT / 2 + len * sin(angle * PI / 180);
            tft.drawLine(x + textWidth / 2, y + FONT_HEIGHT / 2, xEnd, yEnd, tft.color565(0, brightness / 4, 0));
        }
        uint16_t glowColor = tft.color565(0, brightness / 4, 0);
        tft.drawRect(x - 2, y - 2, textWidth + 4, FONT_HEIGHT + 4, glowColor);
        drawText(logo, x, y, tft.color565(0, brightness, 0));
        delay(100);
        esp_task_wdt_reset();
    }
    delay(1500);
    fadeScreen(TERMINAL_BG);
    Serial.println("Boot logo complete");
}

void runNeuralNetwork(int durationMs = 2000)
{
    Serial.println("Running neural network nodes...");
    const int numNodes = 8;
    int nodeX[numNodes], nodeY[numNodes];
    for (int i = 0; i < numNodes; i++)
    {
        nodeX[i] = random(20, tft.width() - 20);
        nodeY[i] = random(20, tft.height() - 20);
    }
    
    unsigned long startTime = millis();
    while (millis() - startTime < durationMs)
    {
        tft.fillScreen(TERMINAL_BG);
        for (int i = 0; i < numNodes; i++)
        {
            for (int j = i + 1; j < numNodes; j++)
            {
                int brightness = constrain(batteryPercentage * 2.55 + 50 * sin(millis() / 500.0), 0, 255);
                tft.drawLine(nodeX[i], nodeY[i], nodeX[j], nodeY[j], tft.color565(0, brightness, 0));
            }
            tft.fillCircle(nodeX[i], nodeY[i], 3, ILI9341_RED);
        }
        delay(50);
        esp_task_wdt_reset();
        Serial.println("Drawing neural network frame...");
    }
    fadeScreen(TERMINAL_BG);
    Serial.println("Neural network complete");
}

void runMatrixRain(int durationMs = 2000)
{
    Serial.println("Running faster denser matrix rain with fade...");
    const int numColumns = 45;
    int positions[numColumns] = {0};
    int speeds[numColumns] = {0};
    for (int i = 0; i < numColumns; i++)
    {
        positions[i] = random(-tft.height(), 0);
        speeds[i] = random(10, 25);
    }
    
    unsigned long startTime = millis();
    while (millis() - startTime < durationMs)
    {
        tft.fillScreen(TERMINAL_BG);
        float fadeProgress = (millis() - startTime) / (float)durationMs;
        int brightness = (fadeProgress > 0.85) ? 255 * (1 - (fadeProgress - 0.85) / 0.15) : 255;
        for (int col = 0; col < numColumns; col++)
        {
            int x = col * (tft.width() / numColumns);
            int y = positions[col];
            char ch = random(0, 3) == 0 ? random(0x30A0, 0x30FF) : random(33, 127);
            drawTinyText(String(ch).c_str(), x, y, tft.color565(0, brightness, 0));
            positions[col] += speeds[col];
            if (positions[col] > tft.height()) positions[col] = 0;
        }
        delay(40);
        esp_task_wdt_reset();
        Serial.println("Drawing matrix rain frame...");
    }
    fadeScreen(TERMINAL_BG);
    Serial.println("Matrix rain complete");
}

void runBootSequence()
{
    Serial.println("Running Iteration 19 boot sequence...");
    tft.fillScreen(TERMINAL_BG);
    setCurrentFont();
    
    tft.fillScreen(ILI9341_BLACK);
    delay(500);
    Serial.println("Black screen complete");
    
    drawBootLogo();
    
    runNeuralNetwork(2000);
    
    runMatrixRain(2000);
    
    tft.fillScreen(TERMINAL_BG);
    struct BootLine
    {
        const char* text;
        uint16_t color;
        int yPos;
    };
    char battMsg[30];
    snprintf(battMsg, sizeof(battMsg), "Battery: %.1fV (%d%%)", batteryVoltage, batteryPercentage);
    BootLine lines[10] = 
    {
        {"HackOchi BIOS v2.1", TERMINAL_GREEN, 0},
        {"CPU: ESP32 @ 240MHz", TERMINAL_GREEN, LINE_SPACING},
        {"RAM: 320KB Test OK", TERMINAL_GREEN, 2 * LINE_SPACING},
        {"FLASH: 16MB Ready", TERMINAL_GREEN, 3 * LINE_SPACING},
        {"Display: ILI9341 320x240", TERMINAL_GREEN, 4 * LINE_SPACING},
        {"Sensors: Initializing...", TERMINAL_GREEN, 5 * LINE_SPACING},
        {battMsg, batteryVoltage >= 3.7 ? TERMINAL_GREEN : ILI9341_RED, 6 * LINE_SPACING},
        {nullptr, 0, 7 * LINE_SPACING},
        {nullptr, 0, 8 * LINE_SPACING},
        {nullptr, 0, 9 * LINE_SPACING}
    };
    
    for (int i = 0; i < 7; i++)
    {
        drawTypingText(lines[i].text, 15, lines[i].yPos, lines[i].color);
        for (int j = 0; j <= i; j++)
        {
            drawText(lines[j].text, 15, lines[j].yPos, lines[j].color);
        }
        delay(i == 0 ? 500 : i == 5 ? 500 : 300);
    }
    
    int yPos = 7 * LINE_SPACING;
    String scanningText = "Network: Scanning";
    drawTypingText(scanningText.c_str(), 15, yPos, TERMINAL_GREEN);
    lines[7] = {scanningText.c_str(), TERMINAL_GREEN, yPos};
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 5)
    {
        clearArea(15 + getTextWidth(scanningText.c_str()), yPos, getTextWidth("....."), FONT_HEIGHT);
        for (int i = 0; i < 7; i++)
        {
            drawText(lines[i].text, 15, lines[i].yPos, lines[i].color);
        }
        drawText(scanningText.c_str(), 15, yPos, TERMINAL_GREEN);
        String dots = "";
        for (int i = 0; i <= attempts; i++)
        {
            dots += ".";
        }
        drawText(dots.c_str(), 15 + getTextWidth(scanningText.c_str()), yPos, TERMINAL_GREEN);
        delay(600);
        esp_task_wdt_reset();
        Serial.println("Drawing scanning dots: " + dots);
        attempts++;
    }
    
    clearArea(15, 7 * LINE_SPACING, getTextWidth("Network: Scanning....."), FONT_HEIGHT);
    for (int i = 0; i < 7; i++)
    {
        drawText(lines[i].text, 15, lines[i].yPos, lines[i].color);
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        drawTypingText("Network: Connected", 15, yPos, TERMINAL_GREEN);
        lines[7] = {"Network: Connected", TERMINAL_GREEN, 7 * LINE_SPACING};
        for (int i = 0; i <= 7; i++)
        {
            drawText(lines[i].text, 15, lines[i].yPos, lines[i].color);
        }
        delay(300);
        yPos += LINE_SPACING;
        drawTypingText("Time: Syncing with NTP", 15, yPos, TERMINAL_GREEN);
        lines[8] = {"Time: Syncing with NTP", TERMINAL_GREEN, 8 * LINE_SPACING};
        timeClient.begin();
        timeClient.forceUpdate();
    }
    else
    {
        drawTypingText("Network: Failed", 15, yPos, ILI9341_RED);
        lines[7] = {"Network: Failed", ILI9341_RED, 7 * LINE_SPACING};
        for (int i = 0; i <= 7; i++)
        {
            drawText(lines[i].text, 15, lines[i].yPos, lines[i].color);
        }
        delay(300);
        yPos += LINE_SPACING;
        clearArea(15, yPos, getTextWidth("Offline mode"), FONT_HEIGHT);
        drawTypingText("Offline mode", 15, yPos, TERMINAL_AMBER);
        lines[8] = {"Offline mode", TERMINAL_AMBER, 8 * LINE_SPACING};
        for (int i = 0; i <= 8; i++)
        {
            drawText(lines[i].text, 15, lines[i].yPos, lines[i].color);
        }
        delay(300);
    }
    
    yPos += LINE_SPACING;
    clearArea(15, yPos, getTextWidth("System Ready >"), FONT_HEIGHT);
    drawTypingText("System Ready >", 15, yPos, TERMINAL_GREEN);
    lines[9] = {"System Ready >", TERMINAL_GREEN, yPos};
    for (int i = 0; i < 10; i++)
    {
        if (lines[i].text)
        {
            drawText(lines[i].text, 15, lines[i].yPos, lines[i].color);
        }
    }
    delay(1000);
    Serial.println("Text boot sequence complete");
    Serial.println("Boot sequence finished");
}

#endif