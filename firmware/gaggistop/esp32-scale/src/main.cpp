#include <Arduino.h>
#include "HX711.h"

#define HX711_DOUT 4
#define HX711_SCK  5

HX711 scale;

void setup() {
    Serial.begin(115200);

    Serial.println();
    Serial.println("GaggiStop HX711 test");

    scale.begin(HX711_DOUT, HX711_SCK);

    Serial.println("HX711 started");
}

void loop() {

    if (scale.is_ready()) {

        long reading = scale.read();

        Serial.print("Raw reading: ");
        Serial.println(reading);

    } else {

        Serial.println("HX711 not found");

    }

    delay(500);
}