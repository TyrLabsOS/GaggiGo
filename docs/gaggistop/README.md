# GaggiStop

GaggiStop is a deterministic WiFi espresso scale telemetry node for GaggiMate brew-by-weight auto-stop.

## Core Objective

```text
Load Cell
↓
HX711
↓
ESP32
↓
WiFi/WebSocket
↓
GaggiMate firmware
↓
brew-by-weight auto-stop
```

Single mission only:

```text
live weight telemetry → GaggiMate volumetric system → brew auto-stop
```

---

## Project Philosophy

GaggiStop is a deterministic telemetry node.

It is not:

- a Bluetooth product
- a cloud platform
- a smart scale ecosystem
- a Home Assistant project
- an analytics platform
- a consumer gadget project

Critical machine stop logic belongs inside GaggiMate firmware.

GaggiGo may later visualise telemetry and scale status, but must not own live machine stop control.

---

## Branch Purpose

This branch exists to keep GaggiStop aligned with the GaggiMate/GaggiGo upstream structure.

The standalone `TyrLabsOS/GaggiStop` repo acted as the initial proving ground for ESP32 firmware and hardware experimentation, but upstream-ready project state now lives inside the GaggiGo structure.

This avoids:

- fork confusion
- duplicated histories
- awkward upstream merges
- repo fragmentation

---

## Expected Integration Path

```text
ESP32 scale
↓
WiFi/WebSocket telemetry
↓
WiFiScalePlugin
↓
controller->onVolumetricMeasurement(...)
↓
existing volumetric auto-stop logic
```

Key discovery:
GaggiMate already contains most of the volumetric infrastructure required.

GaggiStop is not a full control rewrite.
It is a new telemetry source injected into the existing volumetric system.

---

## Current Hardware

Validated hardware:

- ESP32 DevKit V1
- HX711
- 2kg load cell
- acrylic load cell platform kit

Development hardware:

- jumper wires
- breadboard
- soldering kit
- ABS enclosure

Approximate total cost:

```text
~£29
```

---

## Current Development State

Current status:

```text
PHASE 1 RAW TELEMETRY VALIDATED
```

Current confirmed working chain:

```text
Load cell
↓
HX711
↓
ESP32
↓
serial telemetry
```

Confirmed:

- ESP32 flashing works
- PlatformIO environment works
- serial monitor works
- HX711 detected successfully
- load cell produces live changing readings
- pressure changes produce stable telemetry deltas
- mechanical mounting is viable
- espresso-scale sensitivity appears achievable

Current known reality:

```text
mechanical assembly quality matters more than electronics complexity
```

The largest improvements in telemetry stability came from:

- correcting plate preload
- reducing mechanical binding
- improving load-cell flex behaviour
- stabilising the mounting geometry

---

## Current Immediate Priorities

Current focus:

- improve temporary wire reliability
- stabilise prototype mounting
- add tare support
- add averaging/filtering
- convert raw HX711 counts into grams
- validate repeatability using known weights

Do NOT yet:

- add WiFi telemetry
- integrate GaggiMate control logic
- optimise enclosure aesthetics
- implement brew auto-stop
- chase perfect metrology

Current objective remains:

```text
stable deterministic weight telemetry
```

---

## Current Auto-Stop Philosophy

Do not chase perfect drip elimination.

Expected v1 behaviour:

```text
target weight = 32g
machine stop = 31g
user removes cup
remaining drips ignored
```

Likely future configurable setting:

```text
stop_offset_g
```

Probable sensible default:

```text
1.0g
```
