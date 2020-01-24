#pragma once

#include <ESP8266WiFi.h>

#include "config.hpp"

namespace wifi {
WiFiClient espClient;

void setup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        Serial.println(WIFI_SSID);
        Serial.println(WIFI_PASSWORD);
        delay(5000);
        ESP.restart();
    }
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
}
} // namespace wifi
