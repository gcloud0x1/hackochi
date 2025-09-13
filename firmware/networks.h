#ifndef NETWORKS_H
#define NETWORKS_H

#include "config.h"
#include "credentials.h"

void setupNetwork();

void setupNetwork()
{
    WiFi.begin(ssid, password);
    if (WiFi.status() == WL_CONNECTED)
    {
        timeClient.begin();
        timeClient.forceUpdate();
    }
}

#endif