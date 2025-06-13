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
    bool pwmEnabled;
    int pwmThreshold;
    int currentPwmValue;
    int currentStep;

    // Default constructor (required for dynamic allocation)
public:
    RelayController() {}

public:
    RelayController(int relayPin, int togglePin, char command, bool defaultState = LOW, 
                   unsigned long delayMs = 300, bool pwm = false, int pwmThreshold = 128)
        : relayPin(relayPin), togglePin(togglePin), btCommand(command), 
          defaultState(defaultState), debounceDelay(delayMs), lastToggleTime(0),
          pwmEnabled(pwm), pwmThreshold(pwmThreshold), currentPwmValue(0), currentStep(0) {}

    void begin()
    {
        if (pwmEnabled) {
            ledcSetup(0, 5000, 8);  // Channel 0, 5kHz, 8-bit resolution
            ledcAttachPin(relayPin, 0);
            ledcWrite(0, 0);  // Start with 0 duty cycle
        } else {
            pinMode(relayPin, OUTPUT);
            digitalWrite(relayPin, defaultState);
        }
        
        if (togglePin >= 0) {
            pinMode(togglePin, INPUT_PULLUP); // active LOW button
        }
    }

    void handleButton()
    {
        if (togglePin < 0)
            return;

        bool pressed = digitalRead(togglePin) == HIGH;
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
            if (pwmEnabled) {
                SerialBT.printf("Relay '%c' PWM: %d%%\n", btCommand, (currentPwmValue * 100) / 255);
                Serial.printf("Relay '%c' PWM: %d%%\n", btCommand, (currentPwmValue * 100) / 255);
            } else {
                SerialBT.printf("Relay '%c' Toggled\n", btCommand);
                Serial.printf("Relay '%c' Toggled\n", btCommand);
            }
        }
    }

    void toggle()
    {
        if (pwmEnabled) {
            if (currentStep >= pwmThreshold) {
                // Reset to 0 when we reach the threshold
                currentStep = 0;
                currentPwmValue = 0;
            } else {
                // Increment step and calculate new PWM value
                currentStep++;
                currentPwmValue = (255 * currentStep) / pwmThreshold;
            }
            ledcWrite(0, currentPwmValue);
        } else {
            digitalWrite(relayPin, !digitalRead(relayPin));
        }
        lastToggleTime = millis();
    }

    bool debouncePassed()
    {
        return (millis() - lastToggleTime) > debounceDelay;
    }

    bool isPwmEnabled() const { return pwmEnabled; }
    int getPwmThreshold() const { return pwmThreshold; }
    int getCurrentPwmValue() const { return currentPwmValue; }
    int getCurrentStep() const { return currentStep; }
};

#endif
