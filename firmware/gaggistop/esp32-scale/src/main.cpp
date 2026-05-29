#include <Arduino.h>
#include "HX711.h"

#define HX711_DOUT 4
#define HX711_SCK  5

namespace {

HX711 scale;

constexpr uint8_t READ_SAMPLES = 10;
constexpr uint8_t TARE_SAMPLES = 25;
constexpr uint16_t LOOP_DELAY_MS = 500;
constexpr uint16_t HX711_READY_TIMEOUT_MS = 3000;
constexpr long STABLE_SPREAD_RAW = 1500;
constexpr size_t COMMAND_BUFFER_SIZE = 32;

// Temporary Phase 2 starting point.
// If weight reads negative when loaded, this value should remain negative.
// Use serial command "c 8.0" with an 8g 50p coin fitted to calculate a better value.
float rawUnitsPerGram = -10000.0f;
long tareOffset = 0;
bool tareValid = false;
char commandBuffer[COMMAND_BUFFER_SIZE] = {0};
size_t commandIndex = 0;

bool waitForHx711Ready(uint16_t timeoutMs = HX711_READY_TIMEOUT_MS) {
    const unsigned long start = millis();

    while (!scale.is_ready()) {
        if (millis() - start >= timeoutMs) {
            return false;
        }
        delay(10);
    }

    return true;
}

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
    Serial.println("Taring. Keep platform empty and still...");

    if (!waitForHx711Ready()) {
        tareValid = false;
        Serial.println("Tare failed: HX711 not ready after timeout");
        return;
    }

    delay(1000);

    if (!waitForHx711Ready()) {
        tareValid = false;
        Serial.println("Tare failed: HX711 not ready before read");
        return;
    }

    tareOffset = readAverage(TARE_SAMPLES);
    tareValid = true;

    Serial.print("Tare offset: ");
    Serial.println(tareOffset);
}

void calibrate(float knownWeightGrams) {
    if (knownWeightGrams <= 0.0f) {
        Serial.println("Calibration failed: known weight must be > 0g");
        return;
    }

    if (!tareValid) {
        Serial.println("Calibration failed: tare first with empty platform using command: t");
        return;
    }

    if (!waitForHx711Ready()) {
        Serial.println("Calibration failed: HX711 not ready after timeout");
        return;
    }

    Serial.print("Calibrating with ");
    Serial.print(knownWeightGrams, 2);
    Serial.println("g. Keep weight fitted and still...");
    delay(1000);

    if (!waitForHx711Ready()) {
        Serial.println("Calibration failed: HX711 not ready before read");
        return;
    }

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

void processCommand(const char* command) {
    String line(command);
    line.trim();
    line.toLowerCase();

    if (line.length() == 0) {
        return;
    }

    Serial.print("Command received: ");
    Serial.println(line);

    if (line == "h" || line == "help") {
        printHelp();
        return;
    }

    if (line == "t" || line == "tare") {
        tareScale();
        return;
    }

    if (line.startsWith("c") || line.startsWith("cal")) {
        line.replace("cal", "c");
        line.remove(0, 1);
        line.trim();

        const float knownWeight = line.toFloat();
        calibrate(knownWeight);
        return;
    }

    Serial.println("Unknown command. Use h, t, or c 8.0");
}

void flushCommandBuffer() {
    commandBuffer[commandIndex] = '\0';
    processCommand(commandBuffer);
    commandIndex = 0;
    commandBuffer[0] = '\0';
}

void handleSerialCommand() {
    while (Serial.available() > 0) {
        const char incoming = static_cast<char>(Serial.read());

        if (incoming == '\r' || incoming == '\n') {
            if (commandIndex > 0) {
                flushCommandBuffer();
            }
            continue;
        }

        if (!isPrintable(incoming)) {
            continue;
        }

        if (commandIndex < COMMAND_BUFFER_SIZE - 1) {
            commandBuffer[commandIndex++] = incoming;
            commandBuffer[commandIndex] = '\0';
        } else {
            flushCommandBuffer();
        }
    }
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
    Serial.print(" tare=");
    Serial.print(tareValid ? "yes" : "no");
    Serial.print(" cal=");
    Serial.println(rawUnitsPerGram, 2);
}

}  // namespace

void setup() {
    Serial.begin(115200);

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
