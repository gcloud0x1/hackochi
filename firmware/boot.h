#ifndef BOOT_H
#define BOOT_H

#include "config.h"
#include "ui.h"
#include "sensors.h"
#include "networks.h"
#include "credentials.h"

void setupDisplay();
void setupEncoder();
void runBootSequence();

void setupDisplay()
{
    tft.begin();
    tft.setRotation(1);
    setCurrentFont();
    tft.setTextWrap(false);
}

void setupEncoder()
{
    pinMode(ENCODER_CLK, INPUT_PULLUP);
    pinMode(ENCODER_DT, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
}

void runBootSequence()
{
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
    drawText("Sensors: Initializing...", 15, yPos);
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
    while (WiFi.status() != WL_CONNECTED && attempts < 10)
    {
        delay(500);
        tft.print(".");
        attempts++;
    }
    yPos += LINE_SPACING;
    clearArea(15, yPos, 200, FONT_HEIGHT);
    if (WiFi.status() == WL_CONNECTED)
    {
        drawText("Network: Connected", 15, yPos, TERMINAL_GREEN);
        delay(300);
        yPos += LINE_SPACING;
        drawText("Time: Syncing with NTP", 15, yPos);
        timeClient.begin();
        timeClient.forceUpdate();
        yPos += LINE_SPACING;
        clearArea(15, yPos, 200, FONT_HEIGHT);
        drawText("Time: Synchronized", 15, yPos, TERMINAL_GREEN);
    }
    else
    {
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

#endif