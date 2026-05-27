# GaggiStop

GaggiStop is a proposed WiFi espresso scale integration for GaggiMate brew-by-weight auto-stop.

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

The standalone `TyrLabsOS/GaggiStop` repo can remain a proving ground for ESP32 firmware experiments, but upstream-ready project state belongs here.

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

Purchased:

- ESP32 DevKit V1
- HX711
- 2kg load cell
- acrylic load cell platform kit
- jumper wires
- breadboard
- soldering kit
- ABS enclosure

Approximate total cost:

~£29

---

## Current Development State

Current status:

```text
WAITING FOR HARDWARE DELIVERY
```

Current focus:

- keep architecture simple
- avoid speculative filtering logic
- validate physical telemetry first
- keep implementation upstream-friendly

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
