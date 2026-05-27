#include <Arduino.h>

// GaggiStop
// Initial firmware skeleton only.
// Real HX711 integration begins after hardware validation.

void setup() {
    Serial.begin(115200);

    Serial.println();
    Serial.println("GaggiStop boot");
}

void loop() {
    delay(1000);
}
