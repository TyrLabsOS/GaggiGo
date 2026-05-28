#include <Arduino.h>
#include "HX711.h"

#define HX711_DOUT 4
#define HX711_SCK  5

namespace {

HX711 scale;

constexpr uint8_t READ_SAMPLES = 10;
constexpr uint8_t TARE_SAMPLES = 25;
constexpr uint16_t LOOP_DELAY_MS = 500;
constexpr long STABLE_SPREAD_RAW = 1500;

// Temporary Phase 2 starting point.
// If weight reads negative when loaded, this value should remain negative.
// Use serial command "c 8.0" with an 8g 50p coin fitted to calculate a better value.
float rawUnitsPerGram = -10000.0f;
long tareOffset = 0;

long readAverage(uint8_t samples) {
    return scale.read_average(samples);
}

long readSpread(uint8_t samples) {
    long minReading = 0;
    long maxReading = 0;

    for (uint8_t i = 0; i < samples; i++) {
        const long reading = scale.read();

        if (i == 0 || reading < minReading) {
            minReading = reading;
        }

        if (i == 0 || reading > maxReading) {
            maxReading = reading;
        }
    }

    return maxReading - minReading;
}

float rawToGrams(long rawReading) {
    return static_cast<float>(rawReading - tareOffset) / rawUnitsPerGram;
}

void printHelp() {
    Serial.println();
    Serial.println("GaggiStop Phase 2 diagnostic firmware");
    Serial.println("Commands:");
    Serial.println("  h       help");
    Serial.println("  t       tare empty platform");
    Serial.println("  c 8.0   calibrate using known weight in grams");
    Serial.println();
    Serial.println("Known test weights:");
    Serial.println("  UK 10p = 6.5g");
    Serial.println("  UK 50p = 8.0g");
    Serial.println("  both   = 14.5g");
    Serial.println();
}

void tareScale() {
    if (!scale.is_ready()) {
        Serial.println("Tare failed: HX711 not ready");
        return;
    }

    Serial.println("Taring. Keep platform empty and still...");
    delay(1000);

    tareOffset = readAverage(TARE_SAMPLES);

    Serial.print("Tare offset: ");
    Serial.println(tareOffset);
}

void calibrate(float knownWeightGrams) {
    if (knownWeightGrams <= 0.0f) {
        Serial.println("Calibration failed: known weight must be > 0g");
        return;
    }

    if (!scale.is_ready()) {
        Serial.println("Calibration failed: HX711 not ready");
        return;
    }

    Serial.print("Calibrating with ");
    Serial.print(knownWeightGrams, 2);
    Serial.println("g. Keep weight fitted and still...");
    delay(1000);

    const long loadedRaw = readAverage(TARE_SAMPLES);
    const long deltaRaw = loadedRaw - tareOffset;
    rawUnitsPerGram = static_cast<float>(deltaRaw) / knownWeightGrams;

    Serial.print("Loaded raw: ");
    Serial.println(loadedRaw);
    Serial.print("Raw delta: ");
    Serial.println(deltaRaw);
    Serial.print("Calibration factor raw units/g: ");
    Serial.println(rawUnitsPerGram, 4);

    if (abs(deltaRaw) < 1000) {
        Serial.println("Warning: raw delta is very small. Check load cell mechanics and wiring.");
    }
}

void handleSerialCommand() {
    if (!Serial.available()) {
        return;
    }

    const String command = Serial.readStringUntil('\n');
    const String trimmed = command.substring(0, command.length());

    if (trimmed.length() == 0) {
        return;
    }

    const char action = tolower(trimmed.charAt(0));

    if (action == 'h') {
        printHelp();
        return;
    }

    if (action == 't') {
        tareScale();
        return;
    }

    if (action == 'c') {
        const float knownWeight = trimmed.substring(1).toFloat();
        calibrate(knownWeight);
        return;
    }

    Serial.println("Unknown command. Type h for help.");
}

void printReading() {
    if (!scale.is_ready()) {
        Serial.println("HX711 not ready");
        return;
    }

    const long rawAverage = readAverage(READ_SAMPLES);
    const long rawDelta = rawAverage - tareOffset;
    const long spread = readSpread(READ_SAMPLES);
    const float grams = rawToGrams(rawAverage);
    const bool stable = spread <= STABLE_SPREAD_RAW;

    Serial.print("raw_avg=");
    Serial.print(rawAverage);
    Serial.print(" raw_delta=");
    Serial.print(rawDelta);
    Serial.print(" grams=");
    Serial.print(grams, 2);
    Serial.print(" spread=");
    Serial.print(spread);
    Serial.print(" stable=");
    Serial.print(stable ? "yes" : "no");
    Serial.print(" cal=");
    Serial.println(rawUnitsPerGram, 2);
}

}  // namespace

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(100);

    delay(500);
    Serial.println();
    Serial.println("GaggiStop HX711 Phase 2");

    scale.begin(HX711_DOUT, HX711_SCK);

    Serial.println("HX711 started");
    printHelp();
    tareScale();
}

void loop() {
    handleSerialCommand();
    printReading();
    delay(LOOP_DELAY_MS);
}
