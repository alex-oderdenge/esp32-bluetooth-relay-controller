#include <Arduino.h>
#include <BluetoothSerial.h>
#include <vector>
#include "RelayController.h"
#include "ConfigLoader.h"

BluetoothSerial SerialBT;
std::vector<RelayController> relays;

// ==== Setup Functions ====
void initializeSerial() {
    Serial.begin(9600);
    while (!Serial);  // wait for serial
    Serial.println("\n[System] Serial Initialized");
}

void initializeBluetooth() {
    SerialBT.begin("ESP32_VDS");
    Serial.println("[System] Bluetooth Started: ESP32_VDS");
}

void loadRelayConfiguration() {
    Serial.println("[System] Loading relay configuration from /config.json...");

    if (!ConfigLoader::loadRelays(relays)) {
        Serial.println("[Error] Failed to load relay config. No relays will be active.");
    } else {
        Serial.printf("[System] %d relay(s) loaded successfully\n", relays.size());
    }
}

void initializeRelays() {
    for (RelayController& relay : relays) {
        relay.begin();
    }
}

// ==== Loop Functions ====
void handleButtons() {
    for (RelayController& relay : relays) {
        relay.handleButton();
    }
}

void handleBluetooth() {
    if (!SerialBT.available()) return;

    char receivedCommand = SerialBT.read();
    for (RelayController& relay : relays) {
        relay.handleBluetooth(receivedCommand, SerialBT);
    }
}

// ==== Arduino Setup ====
void setup() {
    initializeSerial();
    initializeBluetooth();
    loadRelayConfiguration();
    initializeRelays();
}

// ==== Arduino Loop ====
void loop() {
    handleButtons();
    handleBluetooth();
}

