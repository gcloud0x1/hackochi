#ifndef NETWORKS_H
#define NETWORKS_H

#include "config.h"
#include "credentials.h"

void setupNetwork();
void scanNetworks();
String getEncryptionTypeString(uint8_t encryptionType);
void updatePacketWaterfall();

void setupNetwork()
{
    WiFi.begin(ssid, password);
    if (WiFi.status() == WL_CONNECTED)
    {
        timeClient.begin();
        timeClient.forceUpdate();
    }
}

void updatePacketWaterfall()
{
    for (int ch = 0; ch < NUM_CHANNELS; ch++)
    {
        for (int i = MAX_PACKET_HISTORY - 1; i > 0; i--)
        {
            packetHistory[ch][i] = packetHistory[ch][i-1];
        }
        packetHistory[ch][0] = 0;
    }

    if (promiscuousModeActive)
    {
        uint32_t currentPackets = packetCount;
        uint32_t pps = currentPackets - lastCount;

        if (monitorChannel >= 1 && monitorChannel <= NUM_CHANNELS)
        {
            packetHistory[monitorChannel - 1][0] = pps;
        }

        Serial.printf("REAL PACKETS - Channel %d: %d PPS | Total since start: %u\n",
                     monitorChannel, pps, currentPackets);

        lastCount = currentPackets;
    }
}

void scanNetworks()
{
    wifiScanResultCount = 0;

    Serial.println("Starting WiFi scan...");
    int foundNetworks = WiFi.scanNetworks();
    Serial.printf("WiFi.scanNetworks returned: %d\n", foundNetworks);

    if (foundNetworks > 0)
    {
        wifiScanResultCount = min(foundNetworks, 20);
        for (int i = 0; i < wifiScanResultCount; i++)
        {
            wifiNetworks[i].ssid = WiFi.SSID(i);
            wifiNetworks[i].rssi = WiFi.RSSI(i);
            wifiNetworks[i].encryptionType = WiFi.encryptionType(i);
            wifiNetworks[i].channel = WiFi.channel(i);
            wifiNetworks[i].bssid = WiFi.BSSIDstr(i);
            wifiNetworks[i].isHidden = (WiFi.SSID(i).length() == 0);
        }
    }
    else
    {
        Serial.println("No networks found or scan failed.");
        wifiScanResultCount = 0;
    }
}

String getEncryptionTypeString(uint8_t encryptionType)
{
    switch (encryptionType)
    {
        case WIFI_AUTH_OPEN: return "Open";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA";
        case WIFI_AUTH_WPA2_PSK: return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
        case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-E";
        case WIFI_AUTH_WPA3_PSK: return "WPA3";
        case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
        default: return "Unknown";
    }
}

#endif