#include <Arduino.h>
#include "button.hpp"

Button::Button() {
}

void Button::setup(ButtonType buttonType, uint8_t buttonPin, std::function<void()> onClick) {
    pinMode(buttonPin, INPUT);
    this->buttonPin = buttonPin;
    this->onClick = onClick;

    pressedState = buttonType == ButtonType::pullup ? LOW : HIGH;
    previousButtonRead = !pressedState;
    buttonPressHandled = false;
    buttonPressTimeElapsed = millis(); // reset timer
}

void Button::handle() {
    int buttonRead = digitalRead(buttonPin);

    if (buttonRead == pressedState && !buttonPressHandled) {
        if (previousButtonRead != pressedState) {
            buttonPressTimeElapsed = millis(); // button press started, reset timer
        } else if (millis() - buttonPressTimeElapsed >= debounceDelay) {
            onClick(); // button pressed long enough witout bouncing, threat is as actual press
            buttonPressHandled = true;
        }
    } else if (buttonRead != pressedState && buttonPressHandled) {
        buttonPressHandled = false; // button is released
    }

    previousButtonRead = buttonRead;
}
