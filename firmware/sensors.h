#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"

float readBatteryVoltage();
int calculateBatteryPercentage(float voltage);
String getBatteryStatus();
uint16_t getBatteryColor();
void readDHTData();
void updateBatteryReadings();
void setupSensors();
void printDebugInfo();

float readBatteryVoltage()
{
    int analogValue = analogRead(BATTERY_PIN);
    float voltageAtPin = (analogValue / 4095.0) * 3.3;
    float actualVoltage = voltageAtPin * VOLTAGE_DIVIDER;
    return actualVoltage;
}

int calculateBatteryPercentage(float voltage)
{
    if (voltage >= MAX_BATTERY_VOLTAGE)
    {
        return 100;
    }
    if (voltage <= MIN_BATTERY_VOLTAGE)
    {
        return 0;
    }
    
    int percentage = map(voltage * 100, MIN_BATTERY_VOLTAGE * 100, MAX_BATTERY_VOLTAGE * 100, 0, 100);
    return percentage;
}

String getBatteryStatus()
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.1fV (%d%%)", batteryVoltage, batteryPercentage);
    return String(buffer);
}

uint16_t getBatteryColor()
{
    if (batteryPercentage < 10)
    {
        return ILI9341_RED;
    }
    if (batteryPercentage < 30)
    {
        return TERMINAL_AMBER;
    }
    return TERMINAL_GREEN;
}

void readDHTData()
{
    float newTemp = dht.readTemperature();
    float newHumidity = dht.readHumidity();
    
    if (!isnan(newTemp))
    {
        temperature = newTemp;
    }
    if (!isnan(newHumidity))
    {
        humidity = newHumidity;
    }
}

void updateBatteryReadings()
{
    batteryVoltage = readBatteryVoltage();
    batteryPercentage = calculateBatteryPercentage(batteryVoltage);
}

void setupSensors()
{
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
    pinMode(BATTERY_PIN, INPUT);
    dht.begin();
}

void printDebugInfo()
{
    Serial.print("Battery: ");
    Serial.print(batteryVoltage, 2);
    Serial.print("V (");
    Serial.print(batteryPercentage);
    Serial.println("%)");
    
    Serial.print("Temperature: ");
    Serial.print(temperature, 1);
    Serial.print("C, Humidity: ");
    Serial.print(humidity, 0);
    Serial.println("%");
}

#endif