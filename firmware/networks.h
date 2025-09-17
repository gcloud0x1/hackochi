#ifndef NETWORKS_H
#define NETWORKS_H

#include "config.h"
#include "credentials.h"

void setupNetwork();
void scanNetworks();
String getEncryptionTypeString(uint8_t encryptionType);

void setupNetwork()
{
    WiFi.begin(ssid, password);
    if (WiFi.status() == WL_CONNECTED)
    {
        timeClient.begin();
        timeClient.forceUpdate();
    }
}

void scanNetworks()
{
    wifiScanResultCount = 0;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Starting WiFi scan...");
    int foundNetworks = WiFi.scanNetworks(false, true);
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
            Serial.printf("Network %d: SSID='%s', RSSI=%d, Channel=%d, Encryption=%s, BSSID=%s, Hidden=%s\n",
                          i, wifiNetworks[i].ssid.c_str(), wifiNetworks[i].rssi, wifiNetworks[i].channel,
                          getEncryptionTypeString(wifiNetworks[i].encryptionType).c_str(),
                          wifiNetworks[i].bssid.c_str(), wifiNetworks[i].isHidden ? "Yes" : "No");
        }
    }
    else if (foundNetworks == 0)
    {
        Serial.println("No networks found (0 returned)");
        wifiScanResultCount = 0;
    }
    else
    {
        Serial.printf("Scan failed with error code: %d\n", foundNetworks);
        if (foundNetworks == -1)
        {
            Serial.println("Error: Scan not triggered (radio busy or off)");
        }
        else if (foundNetworks == -2)
        {
            Serial.println("Error: Scan failed to start (possible hardware or mode issue)");
        }
        wifiScanResultCount = 0;
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, password);
        Serial.println("Reconnecting to network...");
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