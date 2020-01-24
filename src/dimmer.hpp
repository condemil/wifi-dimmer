#pragma once

#include "config.hpp"
#include <Arduino.h>

#define US_TO_RTC_TIMER_TICKS(t) (t * (APB_CLK_FREQ >> 4) / 1000000)

namespace dimmer {
uint32_t freqToTicks(uint8_t);
void timerInterrupt();
void ICACHE_RAM_ATTR zeroCrossed();

const uint8_t max_brightness = 100;
// const uint8_t max_brightness = 154;
// const uint8_t max_brightness = 255;

uint8_t brightness = max_brightness;
volatile uint16_t interruptCounter = 0;
volatile bool zeroCross = 0;
bool powerOn = false;

volatile uint32_t debugCrossCount = 0;
volatile uint32_t debugLowCount = 0;
volatile uint32_t debugTimerInterrupt = 0;
volatile uint16_t maxInterruptCount = 0;

void setup() {
    pinMode(DIMMER_ZERO_CROSS_PIN, INPUT);
    pinMode(DIMMER_DRIVE_PIN, OUTPUT);
    digitalWrite(DIMMER_DRIVE_PIN, LOW);

    delay(1000);
    noInterrupts();
    timer1_isr_init();
    timer1_attachInterrupt(timerInterrupt);
    timer1_write(US_TO_RTC_TIMER_TICKS(65));
    // timer1_write(freqToTicks(50));
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP); // TIM_DIV16 = 1:16 = 80/16 = 5MHz = 5 ticks/microseconds
    attachInterrupt(digitalPinToInterrupt(DIMMER_ZERO_CROSS_PIN), zeroCrossed, RISING);
    interrupts();
}

uint32_t freqToTicks(uint8_t frequency) {
    // we need to specify how many ticks timer should count down before calling interrupt function
    // we want to divide AC frequency half-period to 100 levels (0 - 100 brightness percentage) to do trailing-edge
    // half-period cut half-period frequency is 10000 microseconds for 50 Hz (100000/50/2) and 8333 microseconds for 60
    // Hz (100000/60/2) to handle one single brightness level we need 10000/100=100 microseconds for 50 Hz and
    // 8333/100=83 microseconds for 60 Hz we use TIM_DIV16 and it goes 5 ticks / microsecond, that means we need to
    // multiply microseconds on 5 ticks
    uint32_t halfPeriodMicroseconds = 100000 / frequency / 2;
    return halfPeriodMicroseconds / 100 * 5;

    // result should be 325
}

void togglePower() {
    powerOn = !powerOn;

    if (!powerOn) {
        digitalWrite(DIMMER_DRIVE_PIN, LOW);
    }
}

void setBrightness(uint8_t b) {
    brightness += b;
    brightness = max(brightness, (uint8_t)0);
    brightness = min(brightness, (uint8_t)max_brightness);
}

void ICACHE_RAM_ATTR timerInterrupt() {
    debugTimerInterrupt++;

    maxInterruptCount = max(maxInterruptCount, interruptCounter);

    interruptCounter++;

    if (zeroCross == 0) {
        interruptCounter = 0;
        return;
    }

    // if (brightness == max_brightness) {
    //     return;
    // }

    if (interruptCounter < brightness) {
        return;
    }

    digitalWrite(DIMMER_DRIVE_PIN, LOW);
    // interruptCounter = 0;
    zeroCross = 0;
    debugLowCount++;
}

void ICACHE_RAM_ATTR zeroCrossed() {
    if (!powerOn || brightness == 0) {
        return;
    }

    digitalWrite(DIMMER_DRIVE_PIN, HIGH);
    zeroCross = 1;
    // interruptCounter = 0;
    debugCrossCount++;
}
} // namespace dimmer
