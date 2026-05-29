#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include "HX711.h"

#define HX711_DOUT 4
#define HX711_SCK  5

namespace {

HX711 scale;
WebSocketsServer webSocket(81);

constexpr uint8_t READ_SAMPLES = 10;
constexpr uint8_t TARE_SAMPLES = 25;
constexpr uint8_t MEDIAN_SAMPLES = 9;
constexpr uint16_t LOOP_DELAY_MS = 250;
constexpr uint16_t HX711_READY_TIMEOUT_MS = 3000;
constexpr long STABLE_SPREAD_RAW = 1500;
constexpr float FILTER_ALPHA = 0.25f;
constexpr float MAX_VALID_GRAMS = 2000.0f;
constexpr size_t COMMAND_BUFFER_SIZE = 32;
constexpr char WIFI_AP_SSID[] = "GaggiStop-Scale";
constexpr char WIFI_AP_PASSWORD[] = "gaggistop";

float rawUnitsPerGram = -10000.0f;
long tareOffset = 0;
bool tareValid = false;
char commandBuffer[COMMAND_BUFFER_SIZE] = {0};
size_t commandIndex = 0;
float filteredGrams = 0.0f;
bool filterReady = false;
uint32_t packetSeq = 0;

bool waitForHx711Ready(uint16_t timeoutMs = HX711_READY_TIMEOUT_MS) {
    const unsigned long start = millis();
    while (!scale.is_ready()) {
        if (millis() - start >= timeoutMs) return false;
        delay(10);
    }
    return true;
}

long readAverage(uint8_t samples) {
    return scale.read_average(samples);
}

void sortValues(long* values, uint8_t count) {
    for (uint8_t i = 1; i < count; i++) {
        long key = values[i];
        int8_t j = i - 1;
        while (j >= 0 && values[j] > key) {
            values[j + 1] = values[j];
            j--;
        }
        values[j + 1] = key;
    }
}

long readMedian() {
    long values[MEDIAN_SAMPLES] = {0};
    for (uint8_t i = 0; i < MEDIAN_SAMPLES; i++) values[i] = scale.read();
    sortValues(values, MEDIAN_SAMPLES);
    return values[MEDIAN_SAMPLES / 2];
}

long readSpread(uint8_t samples) {
    long minReading = 0;
    long maxReading = 0;

    for (uint8_t i = 0; i < samples; i++) {
        const long reading = scale.read();
        if (i == 0 || reading < minReading) minReading = reading;
        if (i == 0 || reading > maxReading) maxReading = reading;
    }

    return maxReading - minReading;
}

float rawToGrams(long rawReading) {
    return static_cast<float>(rawReading - tareOffset) / rawUnitsPerGram;
}

void resetFilter(float grams) {
    filteredGrams = grams;
    filterReady = true;
}

float updateFiltered(float grams, bool accept) {
    if (!accept) return filteredGrams;

    if (!filterReady) {
        resetFilter(grams);
        return filteredGrams;
    }

    filteredGrams += FILTER_ALPHA * (grams - filteredGrams);
    return filteredGrams;
}

void sendTelemetry(float grams, long rawMedian, long spread, bool stable, bool accepted) {
    String payload = "{";
    payload += "\"type\":\"gaggistop_weight\",";
    payload += "\"seq\":" + String(packetSeq++) + ",";
    payload += "\"ms\":" + String(millis()) + ",";
    payload += "\"weight_g\":" + String(grams, 2) + ",";
    payload += "\"raw\":" + String(rawMedian) + ",";
    payload += "\"spread\":" + String(spread) + ",";
    payload += "\"stable\":" + String(stable ? "true" : "false") + ",";
    payload += "\"accepted\":" + String(accepted ? "true" : "false") + ",";
    payload += "\"tare\":" + String(tareValid ? "true" : "false");
    payload += "}";

    Serial.println(payload);

    if (accepted) {
        webSocket.broadcastTXT(payload);
    }
}

void printHelp() {
    Serial.println();
    Serial.println("GaggiStop Phase 3A WiFi telemetry firmware");
    Serial.println("Commands:");
    Serial.println("  h       help");
    Serial.println("  t       tare empty platform");
    Serial.println("  c 8.0   calibrate using known weight in grams");
    Serial.println();
    Serial.println("WiFi AP:");
    Serial.print("  SSID: ");
    Serial.println(WIFI_AP_SSID);
    Serial.print("  PASS: ");
    Serial.println(WIFI_AP_PASSWORD);
    Serial.println("  WS:   ws://192.168.4.1:81/");
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
    resetFilter(0.0f);

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
    resetFilter(knownWeightGrams);

    Serial.print("Loaded raw: ");
    Serial.println(loadedRaw);
    Serial.print("Raw delta: ");
    Serial.println(deltaRaw);
    Serial.print("Calibration factor raw units/g: ");
    Serial.println(rawUnitsPerGram, 4);

    if (abs(deltaRaw) < 1000) Serial.println("Warning: raw delta is very small. Check mechanics and wiring.");
}

void processCommand(const char* command) {
    String line(command);
    line.trim();
    line.toLowerCase();

    if (line.length() == 0) return;

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
        calibrate(line.toFloat());
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
            if (commandIndex > 0) flushCommandBuffer();
            continue;
        }

        if (!isPrintable(incoming)) continue;

        if (commandIndex < COMMAND_BUFFER_SIZE - 1) {
            commandBuffer[commandIndex++] = incoming;
            commandBuffer[commandIndex] = '\0';
        } else {
            flushCommandBuffer();
        }
    }
}

void readAndSendWeight() {
    if (!scale.is_ready()) {
        Serial.println("{\"type\":\"gaggistop_status\",\"hx711\":\"not_ready\"}");
        return;
    }

    const long rawMedian = readMedian();
    const long spread = readSpread(READ_SAMPLES);
    const float gramsMedian = rawToGrams(rawMedian);
    const bool stable = spread <= STABLE_SPREAD_RAW;
    const bool plausible = abs(gramsMedian) <= MAX_VALID_GRAMS;
    const bool accepted = tareValid && stable && plausible;
    const float gramsFiltered = updateFiltered(gramsMedian, accepted);

    sendTelemetry(gramsFiltered, rawMedian, spread, stable, accepted);
}

void startWifiTelemetry() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);
    webSocket.begin();

    Serial.println("WiFi telemetry AP started");
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("WebSocket: ws://192.168.4.1:81/");
}

}  // namespace

void setup() {
    Serial.begin(115200);

    delay(500);
    Serial.println();
    Serial.println("GaggiStop HX711 Phase 3A");

    scale.begin(HX711_DOUT, HX711_SCK);
    Serial.println("HX711 started");

    startWifiTelemetry();
    printHelp();
    tareScale();
}

void loop() {
    webSocket.loop();
    handleSerialCommand();
    readAndSendWeight();
    delay(LOOP_DELAY_MS);
}
