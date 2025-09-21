#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "config.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;

String getNameFromPayload(std::string payload)
{
    unsigned int i = 0;
    while (i < payload.length())
    {
        uint8_t length = strtol(payload.substr(i, 2).c_str(), NULL, 16);
        if (length == 0)
        {
            break;
        }
        i += 2;
        uint8_t type = strtol(payload.substr(i, 2).c_str(), NULL, 16);
        i += 2;
        if (type == 0x09 || type == 0x08)
        {
            std::string name_hex = payload.substr(i, (length - 1) * 2);
            String deviceName = "";
            for (unsigned int j = 0; j < name_hex.length(); j += 2)
            {
                char chr = (char)strtol(name_hex.substr(j, 2).c_str(), NULL, 16);
                deviceName += chr;
            }
            return deviceName;
        }
        i += (length - 1) * 2;
    }
    return "";
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        String foundName = "";
        if (advertisedDevice.haveName())
        {
            foundName = advertisedDevice.getName().c_str();
        }
        else
        {
            std::string raw_payload_str = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getPayload(), advertisedDevice.getPayloadLength());
            foundName = getNameFromPayload(raw_payload_str);
        }
        if (foundName == "")
        {
            foundName = "[No Name]";
        }
        if (bleScanResultCount >= 20)
        {
            return;
        }
        for (int i = 0; i < bleScanResultCount; i++)
        {
            if (bleNetworks[i].address == advertisedDevice.getAddress().toString().c_str())
            {
                bleNetworks[i].rssi = advertisedDevice.getRSSI();
                if (bleNetworks[i].name == "[No Name]" && foundName != "[No Name]")
                {
                    bleNetworks[i].name = foundName;
                }
                return;
            }
        }
        BLENetworkInfo newDevice;
        newDevice.name = foundName;
        newDevice.address = advertisedDevice.getAddress().toString().c_str();
        newDevice.rssi = advertisedDevice.getRSSI();
        bleNetworks[bleScanResultCount] = newDevice;
        bleScanResultCount++;
    }
};

void ble_init()
{
    Serial.println("Initializing BLE...");
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    Serial.println("BLE Initialized.");
}

void scanBLE()
{
    Serial.println("Starting BLE scan...");
    bleScanResultCount = 0;
    bleScrollOffset = 0;
    pBLEScan->start(5, false);
    delay(5100);
    Serial.printf("BLE Scan finished. Found %d devices.\n", bleScanResultCount);
    pBLEScan->clearResults();
}

#endif