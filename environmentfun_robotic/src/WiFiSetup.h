#ifndef WIFISETUP_H
#define WIFISETUP_H

#include <WiFi.h>

extern const char* ssid;
extern const char* password;

IPAddress setupWiFi();

#endif // WIFISETUP_H