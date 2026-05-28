# GaggiStop — Hardware Validation Notes

## Current Validated State

```text
PHASE 1 RAW TELEMETRY VALIDATED
```

Confirmed working telemetry path:

```text
Load cell
↓
HX711
↓
ESP32
↓
serial telemetry
```

This confirms the core GaggiStop hardware concept is viable.

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

Raw HX711 values visibly changed when:

- finger pressure was applied
- a UK 50p coin was applied
- platform load changed

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

---

# Validated Behaviour

## Idle Stability

Raw telemetry settled into stable ranges rather than random uncontrolled noise.

## Load Response

Applying an 8g UK 50p coin produced a clear stable telemetry delta.

This strongly suggests:

```text
espresso-scale-grade sensitivity is achievable
```

with filtering and calibration.

---

# Current Known Limitations

Current prototype still uses:

- temporary wiring
- exposed electronics
- no filtering
- no averaging
- no tare system
- no calibration factor
- no WiFi telemetry yet

Current firmware state is raw HX711 telemetry only.

---

# Current Correct Development Order

```text
1. Stable raw readings
2. Tare
3. Calibration factor
4. Averaging/filtering
5. Repeatability validation
6. WiFi telemetry
7. GaggiMate integration
8. Auto-stop tuning
```

Do NOT skip ahead.

---

# Current Engineering Priority

Current focus:

```text
stable deterministic telemetry
```

NOT:

- aesthetics
- cloud features
- analytics
- ecosystem expansion
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

have been achieved.

---

# Key Validation Outcome

```text
GaggiStop is no longer theoretical.
The hardware telemetry path has been physically validated.
```
