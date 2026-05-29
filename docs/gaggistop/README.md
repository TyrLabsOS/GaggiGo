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
PHASE 2 CALIBRATION & FILTERING VALIDATED
```

Current confirmed working chain:

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

Confirmed:

- ESP32 flashing works
- PlatformIO environment works
- serial monitor works
- HX711 detected successfully
- load cell produces live changing readings
- pressure and known weights produce stable telemetry deltas
- tare works
- calibration works using a UK 50p coin as an 8.0g reference
- median filtering works
- filtered output returns to zero quickly after unload
- practical repeatability target is achievable
- mechanical mounting is viable
- espresso-scale sensitivity appears achievable

Validated test references:

```text
UK 50p = 8.0g
UK 10p = 6.5g
50p + 10p = 14.5g
```

Observed behaviour after filter tuning:

```text
50p fitted  → filtered output approx 8.0g
50p removed → filtered output snaps back to 0.00g when near zero
empty scale  → filtered output remains near 0.00g
```

Current known reality:

```text
mechanical assembly and wiring quality affect readings more than electronics complexity
```

The largest improvements in telemetry stability came from:

- correcting plate preload
- reducing mechanical binding
- improving load-cell flex behaviour
- stabilising the mounting geometry
- improving temporary wire strain relief
- tuning filter behaviour for fast unload / zero recovery

Current weakest point:

```text
temporary jumper wiring
```

The firmware and scale path are now good enough to justify permanent wiring cleanup before WiFi telemetry work.

Detailed validated wiring and hardware findings live in:

```text
docs/gaggistop/HARDWARE_VALIDATION.md
```

---

## Current Immediate Priorities

Current focus:

- secure or solder permanent wiring
- add strain relief
- remove temporary jumper instability
- repeat empty / 50p / unload validation after wiring cleanup
- freeze Phase 2 firmware behaviour before WiFi

Do NOT yet:

- add WiFi telemetry before wiring cleanup validation
- integrate GaggiMate control logic
- optimise enclosure aesthetics
- implement brew auto-stop
- chase perfect laboratory metrology

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
