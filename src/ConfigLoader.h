#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <vector>
#include "RelayController.h"

class ConfigLoader {
public:
    static bool loadRelays(std::vector<RelayController>& relaysOut) {
        if (!SPIFFS.begin(true)) {
            Serial.println("[SPIFFS] Mount failed");
            return false;
        }

        File file = SPIFFS.open("/config.json", "r");
        if (!file) {
            Serial.println("[SPIFFS] Failed to open /config.json");
            return false;
        }

        StaticJsonDocument<2048> doc;
        DeserializationError err = deserializeJson(doc, file);
        if (err) {
            Serial.printf("[JSON] Parse error: %s\n", err.c_str());
            return false;
        }

        relaysOut.clear();
        for (JsonObject obj : doc.as<JsonArray>()) {
            int pin = obj["pin"] | -1;
            int button = obj["button"] | -1;
            const char* command = obj["command"] | "";
            bool defState = obj["default"] | false;

            if (strlen(command) == 1) {
                relaysOut.emplace_back(pin, button, command[0], defState);
            } else {
                Serial.println("[Warning] Skipped invalid relay entry with no valid command");
            }
        }

        file.close();
        return true;
    }
};

#endif
