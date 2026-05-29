# GaggiStop — Hardware Validation Notes

## Current Validated State

```text
PHASE 2 CALIBRATION & FILTERING VALIDATED
CURRENT PHASE: HARDWARE STABILISATION BEFORE WIFI
```

Confirmed working telemetry path:

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

This confirms the core GaggiStop hardware concept is viable and that the Phase 2 firmware path is good enough for prototype validation.

Do not advance to WiFi telemetry until the physical wiring has been stabilised and the Phase 2 validation checks are repeatable after that cleanup.

---

# Validated Hardware

## Components

- ESP32 DevKit V1
- HX711 amplifier board
- 2kg load cell
- acrylic platform kit
- PlatformIO firmware environment

---

# Confirmed Working Wiring

## HX711 → ESP32

```text
VCC → 3V3
GND → GND
DT  → D4
SCK → D5
```

## Load Cell → HX711

```text
RED   → E+
BLACK → E-
WHITE → A-
GREEN → A+
```

Unused:

```text
B+
B-
```

---

# Firmware Validation State

Confirmed:

- ESP32 firmware upload successful
- PlatformIO environment functional
- serial monitor functional
- HX711 detected successfully
- live raw readings confirmed
- pressure changes produce live telemetry changes
- telemetry updates continuously
- tare implemented and validated
- calibration implemented and validated
- median filtering implemented
- adaptive filter tuning implemented
- rapid unload / near-zero recovery implemented
- UK 50p, UK 10p, and combined coin tests performed
- practical espresso-scale repeatability demonstrated

Current firmware state:

```text
Phase 2 filtered diagnostic firmware
```

Implemented behaviour:

- serial command handling
- tare command
- calibration command using known gram weight
- raw average output
- raw median output
- raw delta output
- median delta output
- raw grams output
- median grams output
- filtered grams output
- spread reporting
- stability flag
- tare status
- calibration factor reporting

Validated test references:

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

---

# Mechanical Discoveries

## Major Discovery

```text
mechanical assembly quality affects stability more than electronics complexity
```

The largest stability improvements came from:

- reducing plate preload
- reducing binding
- improving flex behaviour
- adjusting acrylic alignment
- avoiding over-tightening
- improving load-cell mounting geometry
- reducing strain from temporary wiring

---

# Important Mechanical Rules

## Load Cell Flex Region

The centre cutout / white epoxy region must NOT be clamped rigidly.

Only the mounting ends should be fixed.

The load cell requires microscopic flex to operate correctly.

Over-tightening introduces:

- kick-back
- binding
- unstable readings
- mechanical preload

## Wiring

Temporary jumper wiring is now the weakest known part of the prototype.

The current readings are good enough to justify moving from loose jumper wiring to a more mechanically stable connection path before WiFi work begins.

Priority wiring rules:

- avoid loose HX711 ↔ ESP32 connections
- avoid wire movement loading the scale platform
- add strain relief
- keep low-voltage wiring visible during validation
- do not hide the electronics before repeatability is proven
- avoid unnecessary splice chains if soldering temporary wire

---

# Validated Behaviour

## Idle Stability

Filtered telemetry can remain near zero on an empty platform after tare and filtering.

## Load Response

A UK 50p coin produces a clear and repeatable calibrated response around 8.0g.

## Unload Recovery

After filter tuning, removing the 50p returns the filtered output quickly back to 0.00g when near zero.

This strongly suggests:

```text
espresso-scale-grade sensitivity is achievable
```

with the current hardware path, once physical wiring is stabilised.

---

# Current Known Limitations

Current prototype still uses:

- temporary jumper wiring
- exposed electronics
- prototype mechanical mounting
- serial telemetry only
- no WiFi telemetry yet
- no GaggiMate integration yet
- no auto-stop testing yet

Current weakest component:

```text
temporary jumper wiring between HX711 and ESP32
```

Firmware is no longer the primary concern for the current phase.

The immediate engineering problem is physical stability, not more software complexity.

---

# Current Correct Development Order

```text
1. Stable raw readings — complete
2. Tare — complete
3. Calibration factor — complete
4. Median filtering — complete
5. Adaptive unload / zero recovery — complete
6. Repeatability validation with UK 50p / 10p — complete
7. Wiring stabilisation — current phase
8. Repeat Phase 2 validation after wiring cleanup
9. WiFi telemetry
10. GaggiMate integration
11. Auto-stop tuning
```

Do NOT skip ahead.

---

# Current Engineering Priority

Current focus:

```text
hardware stabilisation before WiFi
```

Immediate validation loop after wiring cleanup:

```text
empty       → stable near 0.00g
50p fitted  → stable near 8.0g
50p removed → fast return to 0.00g
10p fitted  → stable near 6.5g
50p + 10p   → stable near 14.5g
```

NOT current priorities:

- WiFi telemetry
- GaggiMate integration
- auto-stop tuning
- aesthetics
- cloud features
- analytics
- smart-scale ecosystem expansion
- premature enclosure optimisation

---

# Current Prototype Philosophy

Current prototype state is intentionally:

```text
functional before beautiful
```

External ESP32 mounting is acceptable during validation.

Permanent internal packaging should only occur AFTER:

- stable telemetry
- calibration
- repeatability validation
- filtering
- reliable connections

have been achieved and repeated after wiring cleanup.

---

# Key Validation Outcome

```text
GaggiStop is no longer theoretical.
Phase 1 is complete.
Phase 2 is complete / prototype validated.
The current phase is hardware stabilisation before WiFi.
```
