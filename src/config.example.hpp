#pragma once

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define DIMMER_ZERO_CROSS_PIN 5
#define DIMMER_DRIVE_PIN 12
#define DIMMER_AC_FREQ 50 // can be 60 Hz or 50 Hz depending on country, US - 60 Hz, Europe - 50 Hz, check your country

#define BUTTON_PIN 0 // D3

#define ENCODER_PINA 14 // D5; don't connect to GPIO16, it doesn't have INPUT_PULLUP
#define ENCODER_PINB 13 // D7; don't connect to GPIO16, it doesn't have INPUT_PULLUP
