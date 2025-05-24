# ESP32 Relay Controller

A scalable relay controller for ESP32 that supports:

- Physical buttons (optional)
- Bluetooth toggle via serial commands
- Modular design using C++ classes

## Example Bluetooth Commands

- Send `'1'` to toggle relay 1
- Send `'2'` to toggle relay 2
- ...

Each relay is independently mapped to a GPIO and optional button.

pio run --target uploadfs

