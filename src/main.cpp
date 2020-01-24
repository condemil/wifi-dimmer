#include "button.hpp"
#include "config.hpp"
#include "dimmer.hpp"
#include "encoder.hpp"
#include <Arduino.h>

void onRotate(int8_t);
void onClick();

Button button;
unsigned long secondElapsed = millis();
unsigned long fakeFreq = millis();

void setup() {
    Serial.begin(115200);
    Serial.println("\nBooting");
    Serial.println(US_TO_RTC_TIMER_TICKS(65));

    dimmer::setup();
    encoder::setup(ENCODER_PINA, ENCODER_PINB, onRotate);
    button.setup(ButtonType::pulldown, BUTTON_PIN, onClick);

    Serial.println("Ready");
}

void loop() {
    encoder::handle();
    button.handle();
    // if (millis() - fakeFreq >= 10) {
    //     fakeFreq = millis(); // reset timer
    //     dimmer::zeroCrossed();
    // }
    if (millis() - secondElapsed >= 1000) {
        secondElapsed = millis(); // reset timer
        Serial.printf("power: %s; brightness: %d; cross count: %d; low count: %d; timer interrupt count: %d; max "
                      "interrupt count: %d\n",
            dimmer::powerOn ? "on" : "off", dimmer::brightness, dimmer::debugCrossCount, dimmer::debugLowCount,
            dimmer::debugTimerInterrupt, dimmer::maxInterruptCount);
        dimmer::debugCrossCount = 0;
        dimmer::debugLowCount = 0;
        dimmer::debugTimerInterrupt = 0;
        dimmer::maxInterruptCount = 0;
    }
}

void onRotate(int8_t position) {
    dimmer::setBrightness(position * 1);
    // Serial.println(position);
}

void onClick() { dimmer::togglePower(); }
