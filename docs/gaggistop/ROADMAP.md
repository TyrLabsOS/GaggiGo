# GaggiStop — Project Roadmap

## Current State

Project status:

```text
PHASE 1 RAW TELEMETRY PROOF ACHIEVED
```

Current completed work:

- project architecture defined
- repository structure created
- firmware direction established
- telemetry protocol direction established
- GaggiMate integration path analysed
- scope discipline established
- PlatformIO environment prepared
- upstream integration strategy established
- ESP32 powered successfully
- ESP32 detected on COM3
- firmware built successfully
- firmware uploaded successfully
- serial monitor confirmed working
- HX711 headers soldered
- HX711 connected to ESP32
- load cell connected to HX711
- raw HX711 data confirmed scrolling in serial monitor
- load cell pressure confirmed to change readings

Current focus:

- clean up temporary hardware wiring
- improve connection reliability
- keep setup low-voltage and USB-powered
- maintain upstream-friendly architecture
- preserve deterministic behaviour

Important current constraint:

```text
Do not advance to WiFi telemetry until raw readings are physically stable and repeatable.
```

---

# Phase 1 — Hardware Bring-Up

## Goal

Stable repeatable live weight readings from HX711 on ESP32.

## Current Result

Raw telemetry path confirmed:

```text
Load cell
↓
HX711
↓
ESP32
↓
serial monitor
```

The system currently produces live serial data, and pressure applied to the load cell changes the readings.

## Remaining Tasks

- clean temporary load-cell connections
- reduce loose-wire movement
- confirm readings remain live after reconnect
- confirm readings respond consistently to pressure
- document working pinout
- begin basic tare/calibration only after wiring is physically stable

## Success Criteria

```text
stable repeatable live weight readings
```

Phase 1 is not complete until the physical wiring is stable enough that readings are repeatable without constant hand-adjustment.

---

# Phase 2 — Calibration & Filtering

## Goal

Create usable espresso-grade weight telemetry.

## Entry Requirement

Phase 1 stable wiring must be complete first.

Do not tune calibration while the load-cell wiring is still loose or intermittent.

## Tasks

- calibration factor
- tare refinement
- vibration testing
- smoothing/filtering
- latency observation

## Success Criteria

```text
repeatable espresso-grade readings
```

Target:
approximately ±0.5g practical repeatability.

---

# Phase 3 — WiFi Telemetry

## Goal

Transmit live weight telemetry over WiFi.

## Entry Requirement

Raw serial readings must be stable and repeatable first.

## Tasks

- WiFi connection
- WebSocket implementation
- packet structure
- reconnect handling
- telemetry validation

Current preferred packet philosophy:

```text
minimal
lightweight
deterministic
```

Example:

```json
{"w":31.2}
```

## Success Criteria

```text
stable low-latency telemetry stream
```

---

# Phase 4 — GaggiMate Integration

## Goal

Inject telemetry into existing volumetric pipeline.

## Current Expected Architecture

```text
ESP32 scale
↓
WiFi/WebSocket telemetry
↓
WiFiScalePlugin
↓
controller->onVolumetricMeasurement(...)
```

## Important Discovery

GaggiMate already contains:

- volumetric logic
- BLE scale abstraction
- telemetry fields
- WebSocket infrastructure
- brew-by-weight logic

Meaning:

GaggiStop is not a full rewrite.
It is a new telemetry source.

## Success Criteria

```text
GaggiMate displays live weight telemetry
```

---

# Phase 5 — Auto-Stop Tuning

## Goal

Reliable brew-by-weight stopping.

## Current Philosophy

Do not chase perfect drip elimination.

Expected v1 behaviour:

```text
target = 32g
stop = 31g
user removes cup
```

Likely future configurable setting:

```text
stop_offset_g
```

Probable default:

```text
1.0g
```

## Main Engineering Challenge

```text
flow continuation after stop
```

because the machine does not use a 3-way solenoid.

---

# Immediate Cleanup Plan

Current installation is valid for proof-of-life, but not stable enough for calibration.

Cleanup target:

```text
secure temporary wiring without finalising permanent build
```

Allowed cleanup:

- reseat HX711 firmly in breadboard
- shorten or organise jumper routes where possible
- secure load-cell wire joins temporarily
- avoid strain on HX711 pins
- keep USB power only
- keep all wiring visible for inspection

Do not yet:

- solder load-cell wires permanently
- hide wiring inside enclosure
- add WiFi logic
- add GaggiMate integration
- start auto-stop tuning

---

# Important Rules

Do NOT add:

- OLED displays
- batteries
- cloud systems
- mobile apps
- Home Assistant
- analytics platforms
- smart ecosystem features

Single project objective only:

```text
weight telemetry → brew auto-stop
```
