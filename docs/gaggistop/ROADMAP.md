# GaggiStop — Project Roadmap

## Current State

Project status:

```text
PHASE 2 CALIBRATION & FILTERING VALIDATED
CURRENT PHASE: HARDWARE STABILISATION BEFORE WIFI
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
- firmware built successfully
- firmware uploaded successfully
- serial monitor confirmed working
- HX711 headers soldered
- HX711 connected to ESP32
- load cell connected to HX711
- raw HX711 data confirmed in serial monitor
- load cell pressure confirmed to change readings
- tare implemented and validated
- calibration implemented and validated
- median filtering implemented
- adaptive filter tuning implemented
- rapid unload / near-zero recovery implemented
- UK 50p, UK 10p, and combined coin tests performed
- practical espresso-scale repeatability demonstrated

Current focus:

- replace or improve temporary jumper wiring
- add strain relief
- remove intermittent connection risk
- repeat empty / 50p / unload validation after wiring cleanup
- keep setup low-voltage and USB-powered
- maintain upstream-friendly architecture
- preserve deterministic behaviour

Important current constraint:

```text
Do not advance to WiFi telemetry until the physical wiring has been stabilised and Phase 2 validation is repeatable after that cleanup.
```

---

# Phase 1 — Hardware Bring-Up

## Status

```text
COMPLETE
```

## Goal

Stable repeatable live weight readings from HX711 on ESP32.

## Result

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

The system produces live serial data, and pressure applied to the load cell changes the readings.

## Completed Tasks

- ESP32 powered successfully
- PlatformIO build/upload works
- serial monitor works
- HX711 detected
- load cell connected
- raw readings confirmed
- pressure/load response confirmed
- working pinout documented

## Success Criteria

```text
stable repeatable live weight readings
```

Phase 1 is complete.

---

# Phase 2 — Calibration & Filtering

## Status

```text
COMPLETE / VALIDATED FOR PROTOTYPE
```

## Goal

Create usable espresso-grade weight telemetry.

## Result

Confirmed working chain:

```text
Load cell
↓
HX711
↓
ESP32
↓
tare
↓
calibration
↓
median filtering
↓
adaptive filtered grams output
↓
serial telemetry
```

## Completed Tasks

- tare support
- calibration factor calculation
- raw-to-grams conversion
- median filtering
- filtered grams output
- fast response for large weight changes
- near-zero snap-back for unload behaviour
- repeatability testing using UK coins
- return-to-zero behaviour improved

## Validated Test Weights

```text
UK 50p = 8.0g
UK 10p = 6.5g
50p + 10p = 14.5g
```

Observed after filter tuning:

```text
50p fitted  → filtered output approx 8.0g
50p removed → filtered output returns to 0.00g quickly
empty scale  → filtered output remains near 0.00g
```

## Success Criteria

```text
repeatable espresso-grade readings
```

Target:
approximately ±0.5g practical repeatability.

Prototype result:

```text
PASS — good enough to proceed after hardware wiring stabilisation.
```

## Remaining Limitation

Temporary wiring is still a known instability source.

Large occasional spikes are likely caused by jumper/breadboard/interconnect disturbance rather than firmware logic.

---

# Phase 3 — Hardware Stabilisation

## Status

```text
CURRENT PHASE
```

## Goal

Turn the temporary proof-of-life build into a physically reliable telemetry prototype before adding WiFi.

## Tasks

- secure or replace temporary jumper wires
- solder permanent connections where sensible
- avoid unnecessary temporary splice chains
- strain-relieve HX711 and ESP32 wiring
- keep low-voltage wiring away from movement/load paths
- ensure no wire touches or loads the platform/load-cell flex path
- repeat empty stability test
- repeat 50p calibration test
- repeat 50p unload / return-to-zero test

## Success Criteria

```text
Phase 2 behaviour remains repeatable after wiring cleanup.
```

Expected post-cleanup checks:

```text
empty       → stable near 0.00g
50p fitted  → stable near 8.0g
50p removed → fast return to 0.00g
```

Do not add WiFi until this is confirmed.

---

# Phase 4 — WiFi Telemetry

## Goal

Transmit live weight telemetry over WiFi.

## Entry Requirement

Physical wiring must be stable and repeatable after Phase 3 cleanup.

## Tasks

- WiFi connection
- WebSocket implementation
- packet structure
- reconnect handling
- telemetry validation
- latency observation

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

# Phase 5 — GaggiMate Integration

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

# Phase 6 — Auto-Stop Tuning

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

Current installation is valid for proof-of-life and Phase 2 firmware validation, but temporary wiring is still the weakest link.

Cleanup target:

```text
secure physical interconnects before WiFi
```

Allowed cleanup:

- secure HX711 and ESP32 jumper routes
- solder permanent connections where sensible
- add strain relief
- shorten or organise jumper routes where possible
- avoid strain on HX711 pins
- keep USB power only
- keep all wiring visible for inspection until stable

Do not yet:

- hide wiring inside enclosure
- add WiFi logic before repeat validation
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
