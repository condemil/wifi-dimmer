#pragma once

#include <Arduino.h>

namespace encoder {
static uint8_t _pinA;
static uint8_t _pinB;
std::function<void(int8_t)> _onRotate;
volatile int8_t _position;
volatile byte _aFlag;   // when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte _bFlag;   // when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent
                        // (opposite direction to when aFlag is set)
volatile int _readingA; // somewhere to store the direct values we read from our interrupt pins before checking to see
                        // if we have moved a whole detent
volatile int _readingB; // somewhere to store the direct values we read from our interrupt pins before checking to see
                        // if we have moved a whole detent

void ICACHE_RAM_ATTR PinA() {
    cli(); // stop interrupts happening before we read pin values
    _readingA = digitalRead(_pinA);
    _readingB = digitalRead(_pinB);
    if (_readingA && _readingB && _aFlag) { // check that we have both pins at detent (HIGH) and that we are expecting
                                            // detent on this pin's rising edge
        _position++;
        _aFlag = 0;
        _bFlag = 0;
    } else if (_readingA && !_readingB) {
        _bFlag = 1; // signal that we're expecting pinB to signal the transition to detent from free rotation
    }
    sei(); // restart interrupts
}

void ICACHE_RAM_ATTR PinB() {
    cli(); // stop interrupts before we read pin values
    _readingA = digitalRead(_pinA);
    _readingB = digitalRead(_pinB);
    if (_readingA && _readingB && _bFlag) { // check that we have both pins at detent (HIGH) and that we are expecting
                                            // detent on this pin's rising edge
        _position--;
        _aFlag = 0;
        _bFlag = 0;
    } else if (!_readingA && _readingB) {
        _aFlag = 1; // signal that we're expecting pinA to signal the transition to detent from free rotation
    }
    sei(); // restart interrupts
}

void setup(uint8_t pinA, uint8_t pinB, std::function<void(int8_t)> onRotate) {
    _pinA = pinA;
    _pinB = pinB;
    _onRotate = onRotate;

    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pinA), PinA, RISING);
    attachInterrupt(digitalPinToInterrupt(pinB), PinB, RISING);
}

void handle() {
    if (_position != 0) {
        _onRotate(_position);
        _position = 0;
    }
}
} // namespace encoder
