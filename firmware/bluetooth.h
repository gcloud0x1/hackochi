#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "config.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <esp_bt.h>
#include <esp_gap_bt_api.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>

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
        blePacketCount++;
        
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

static void gap_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_BT_GAP_DISC_RES_EVT: 
        {
            char addr_str[18];
            sprintf(addr_str, "%02X:%02X:%02X:%02X:%02X:%02X",
                param->disc_res.bda[0], param->disc_res.bda[1], param->disc_res.bda[2],
                param->disc_res.bda[3], param->disc_res.bda[4], param->disc_res.bda[5]);
            String name = "";
            for (int i = 0; i < param->disc_res.num_prop; i++) 
            {
                esp_bt_gap_dev_prop_t *prop = &param->disc_res.prop[i];
                if (prop->type == ESP_BT_GAP_DEV_PROP_EIR) 
                {
                    uint8_t *eir = (uint8_t *)prop->val;
                    uint8_t *p = eir;
                    while (p < eir + prop->len) 
                    {
                        uint8_t len = p[0];
                        if (len == 0) break;
                        uint8_t type = p[1];
                        if (type == 0x08 || type == 0x09) 
                        {  
                            name = String((char*)(p + 2), len - 1);
                            break;
                        }
                        p += len + 1;
                    }
                }
            }
            if (name == "") name = "[No Name]";
            bool duplicate = false;
            for (int j = 0; j < classicScanResultCount; j++) 
            {
                if (classicNetworks[j].address == addr_str) 
                {
                    duplicate = true;
                    if (classicNetworks[j].name == "[No Name]" && name != "[No Name]") 
                    {
                        classicNetworks[j].name = name;
                    }
                    break;
                }
            }
            if (!duplicate && classicScanResultCount < 20) 
            {
                classicNetworks[classicScanResultCount].address = addr_str;
                classicNetworks[classicScanResultCount].rssi = -100;
                classicNetworks[classicScanResultCount].name = name;
                classicScanResultCount++;
            }
            break;
        }
        case ESP_BT_GAP_DISC_STATE_CHANGED_EVT: 
        {
            if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) 
            {
                Serial.println("Classic BT discovery stopped.");
            } 
            else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED) 
            {
                Serial.println("Classic BT discovery started.");
            }
            break;
        }
        default: break;
    }
}

void bt_init()
{
    Serial.println("Initializing Bluetooth (BLE + Classic)...");
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    bt_cfg.mode = ESP_BT_MODE_BTDM;
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BTDM);
    esp_bluedroid_init();
    esp_bluedroid_enable();
    esp_bt_gap_register_callback(gap_callback);
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    Serial.println("Bluetooth (BLE + Classic) Initialized.");
}

void startBleGraphScan() 
{
    Serial.println("Starting continuous BLE scan for graph...");
    blePacketCount = 0;
    lastBleCount = 0;
    pBLEScan->start(0, nullptr, false); 
}

void stopBleGraphScan() 
{
    Serial.println("Stopping continuous BLE scan.");
    pBLEScan->stop();
    pBLEScan->clearResults();
}

void scanBLE()
{
    stopBleGraphScan();
    
    Serial.println("Starting BLE scan...");
    bleScanResultCount = 0;
    bleScrollOffset = 0;
    pBLEScan->start(5, false);
    delay(5100);
    Serial.printf("BLE Scan finished. Found %d devices.\n", bleScanResultCount);
    pBLEScan->clearResults();
}

void scanClassicBT()
{
    stopBleGraphScan();

    Serial.println("Starting Classic BT scan...");
    classicScanResultCount = 0;
    classicScrollOffset = 0;
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
    delay(13000);
    esp_bt_gap_cancel_discovery();
    Serial.printf("Classic BT Scan finished. Found %d devices.\n", classicScanResultCount);
}

#endif