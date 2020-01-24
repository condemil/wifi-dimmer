# Wi-Fi Dimmer

ESP8266-based dimmer to switch and dim AC lightbulbs though MQTT and rotary encoder.

## How to use it

You need to do the following to compile the firmware:

* Open the project with PlatformIO
* Copy `config.example.hpp` to `config.hpp`
* Modify config according to your setup
* Specify proper board and upload port in `platformio.ini`
* Build and upload the firmware to your board

## How it works

The software implements trailing-edge phase cut that allows control brightness of dimmable LED lightbulbs.

## Wishlist

- [] Add MQTT support
- [] Use WiFiManager to set up Wi-Fi and MQTT

## Disclaimer

Working with high voltage is dangerous.
I am not taking any liability for risk you taking by using this project.
