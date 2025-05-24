# 🔌 ESP32 Relay Controller

A scalable and dynamic relay control system using an ESP32 microcontroller. Supports relay toggling via Bluetooth and optional physical buttons. Relay configuration is fully customizable via a JSON file stored in SPIFFS.

---

## 🚀 Features

- 📱 Bluetooth control using a custom command per relay  
- 🔘 Optional physical toggle buttons per relay  
- 📁 Configurable relays via `config.json` (no need to recompile)  
- 🧠 Clean object-oriented design with `RelayController` class  
- ⚡ Supports unlimited relays using `std::vector`  
- 🗂️ Uses SPIFFS for internal file storage  

---

## 🔧 Hardware Setup

| Component      | Description                           |
|----------------|---------------------------------------|
| ESP32          | Any dev board (e.g. DevKit V1)        |
| Relay Module   | 1 to N channel relay board            |
| Push Buttons   | Optional, connected to GPIO pins      |

> Each relay is assigned:
> - a GPIO output pin for switching  
> - an optional GPIO input pin for button  
> - a Bluetooth command character (e.g. `'1'`, `'2'`, `'A'`, etc.)  
> - a default ON/OFF state on startup  

---

## 🗂 Project Structure

```
esp32-relay-controller/
├── data/                      # SPIFFS files
│   └── config.json            # Relay configuration
├── src/
│   ├── main.cpp               # Main application logic
│   ├── RelayController.h      # Relay behavior abstraction
│   └── ConfigLoader.h         # JSON parser and relay loader
├── platformio.ini             # PlatformIO config
└── README.md                  # Project documentation
```

---

## 🧾 `config.json` Example

```json
[
  { "pin": 15, "button": 2, "command": "1", "default": true },
  { "pin": 4,  "button": -1, "command": "2", "default": false },
  { "pin": 16, "button": 0, "command": "3", "default": true }
]
```

- `pin`: GPIO controlling the relay  
- `button`: GPIO for optional toggle button (`-1` = unused)  
- `command`: Single character Bluetooth command  
- `default`: `true` = relay starts ON, `false` = OFF  

---

## 📲 Bluetooth Usage

1. Connect to **ESP32_VDS** via any serial Bluetooth app.
2. Send a single character (e.g. `'1'`) to toggle the corresponding relay.

---

## 🛠 How to Build & Upload

1. Install [PlatformIO](https://platformio.org/)
2. Clone the project:
   ```bash
   git clone https://github.com/alex-oderdenge/esp32-bluetooth-relay-controller
   cd esp32-bluetooth-relay-controller
   ```
3. Upload code and filesystem:
   ```bash
   pio run --target upload
   pio run --target uploadfs
   (OR use the PlatformIO IDE upload button)
   ```

---

## 🧱 Dependencies

- [ArduinoJson](https://arduinojson.org/) `^6.21.3`  
- ESP32 Arduino framework with SPIFFS support  

All dependencies are auto-managed by `platformio.ini`.

---

## ✨ Future Improvements

- Add functionality to receive and overwrite `config.json` via Bluetooth, updating the relay configuration dynamically in real time.
- Web-based config editor (using ESPAsyncWebServer)  

---

## 🧑‍💻 Credits

Developed by Alex Oderdenge
- [GitHub](https://github.com/alex-oderdenge)
- [LinkedIn](https://www.linkedin.com/in/alex-oderdenge/)  

---

## 🛡 License

This project is licensed under the MIT License.  
You are free to use, modify, and distribute this software, even commercially,  
**as long as attribution is given to** [Alex Oderdenge](https://github.com/alex-oderdenge).

See [LICENSE](LICENSE) for full details.

