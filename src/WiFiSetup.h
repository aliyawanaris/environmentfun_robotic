#ifndef WIFISETUP_H
#define WIFISETUP_H

#include <WiFi.h>

extern const char* ssid;
extern const char* password;

/**
 * @brief Fungsi untuk inisialisasi koneksi WiFi.
 * @return IPAddress Alamat IP yang diperoleh setelah koneksi berhasil.
 */
IPAddress setupWiFi();

#endif // WIFISETUP_H