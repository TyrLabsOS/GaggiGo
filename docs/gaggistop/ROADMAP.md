# GaggiStop — Project Roadmap

## Current State

Project status:

```text
WAITING FOR HARDWARE DELIVERY
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

Current focus:

- avoid speculative complexity
- wait for physical telemetry
- maintain upstream-friendly architecture
- preserve deterministic behaviour

---

# Phase 1 — Hardware Bring-Up

## Goal

Stable repeatable live weight readings from HX711 on ESP32.

## Tasks

- wire HX711
- wire load cell
- connect ESP32
- verify serial output
- confirm repeatability
- verify tare behaviour

## Success Criteria

```text
stable repeatable live weight readings
```

---

# Phase 2 — Calibration & Filtering

## Goal

Create usable espresso-grade weight telemetry.

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
