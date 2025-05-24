#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include <Arduino.h>
#include <BluetoothSerial.h>

class RelayController
{
private:
    int relayPin;
    int togglePin;
    char btCommand;
    bool defaultState;
    unsigned long debounceDelay;
    unsigned long lastToggleTime;

    // Default constructor (required for dynamic allocation)
public:
    RelayController() {}

public:
    RelayController(int relayPin, int togglePin, char command, bool defaultState = LOW, unsigned long delayMs = 300)
        : relayPin(relayPin), togglePin(togglePin), btCommand(command), defaultState(defaultState), debounceDelay(delayMs), lastToggleTime(0) {}

    void begin()
    {
        pinMode(relayPin, OUTPUT);
        digitalWrite(relayPin, defaultState); // default OFF
        if (togglePin >= 0)
        {
            pinMode(togglePin, INPUT_PULLUP); // active LOW button
        }
    }

    void handleButton()
    {
        if (togglePin < 0)
            return;

        bool pressed = digitalRead(togglePin) == LOW;
        if (pressed && debouncePassed())
        {
            toggle();
        }
    }

    void handleBluetooth(char received, BluetoothSerial &SerialBT)
    {
        if (received == btCommand && debouncePassed())
        {
            toggle();
            SerialBT.printf("Relay '%c' Toggled\n", btCommand);
            Serial.printf("Relay '%c' Toggled\n", btCommand);
        }
    }

    void toggle()
    {
        digitalWrite(relayPin, !digitalRead(relayPin));
        lastToggleTime = millis();
    }

    bool debouncePassed()
    {
        return (millis() - lastToggleTime) > debounceDelay;
    }
};

#endif
