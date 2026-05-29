# GaggiStop — Runtime Rules

## Locked Project Context

Repo:

```text
TyrLabsOS/GaggiGo
```

Branch:

```text
GaggiStop
```

Project phase:

```text
PHASE 2 COMPLETE
CURRENT PHASE: HARDWARE STABILISATION BEFORE WIFI
```

Current status:

```text
tare validated
calibration validated
median filtering validated
adaptive unload recovery validated
repeatability demonstrated
```

Canonical project folder:

```text
docs/gaggistop/
```

Firmware folder:

```text
firmware/gaggistop/esp32-scale/
```

---

## Core Architecture

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

GaggiMate is the authority.

GaggiStop is a telemetry source.

GaggiGo is visualisation only.

---

## Authority Boundary

Critical stop logic belongs inside GaggiMate firmware.

Do not add:

- OLED displays
- batteries
- Bluetooth scale support
- cloud systems
- Home Assistant
- analytics platforms
- smart-scale ecosystems
- separate consumer apps

GaggiGo must not own live machine stop control.

---

## Current Engineering Target

Current objective:

```text
hardware stabilisation before WiFi
```

Known weakest component:

```text
temporary jumper wiring between HX711 and ESP32
```

Before WiFi work:

- improve or replace temporary jumper wiring
- add strain relief
- eliminate intermittent connection risk
- repeat Phase 2 validation tests

Do not advance to WiFi telemetry until Phase 2 behaviour remains repeatable after wiring cleanup.

---

## Method Rules

One variable at a time.

Correct order:

```text
1. ESP32 boot
2. serial monitor
3. HX711 wiring
4. raw readings
5. tare
6. calibration
7. median filtering
8. adaptive unload recovery
9. repeatability validation
10. wiring stabilisation
11. WiFi telemetry
12. GaggiMate integration
13. auto-stop tuning
```

Never debug multiple layers at once.

If something fails, reduce back to the last known working layer.

---

## Current Validated State

Confirmed:

- ESP32 flashing works
- PlatformIO environment works
- serial monitor works
- HX711 detected successfully
- load cell produces live readings
- tare implemented
- calibration implemented
- median filtering implemented
- adaptive filter tuning implemented
- unload recovery implemented
- UK 50p validation completed
- UK 10p validation completed
- combined coin validation completed
- practical repeatability achieved

Firmware is no longer the primary concern.

Hardware stability is the current concern.

---

## Hardware Safety Rules

Before powering hardware:

- verify voltage pins
- verify ground
- verify HX711 pinout
- verify load cell wire colours
- check for loose jumpers
- inspect strain on temporary connections

USB-powered prototype only.

No mains-voltage work belongs in GaggiStop.

---

## Software Rules

Current firmware scope is complete for this phase.

Do not add:

- WiFi telemetry before wiring validation
- GaggiMate integration before WiFi validation
- auto-stop logic before telemetry validation
- prediction engines
- cloud integrations
- Home Assistant integrations
- battery optimisation work
- ecosystem features

Current goal is to freeze Phase 2 behaviour and verify it survives wiring cleanup.

---

## Stop Strategy Rules

Do not chase perfect drip elimination.

Current working assumption:

```text
target = 32g
stop trigger = 31g
offset = 1.0g
```

The offset remains configurable later.

Critical stop logic belongs inside GaggiMate firmware.

---

## Repo Rules

Canonical project home:

```text
TyrLabsOS/GaggiGo
branch: GaggiStop
```

Do not create additional repos.

Keep commits small and upstream-friendly.

Before proposing architecture changes:

- check README.md
- check ROADMAP.md
- check HARDWARE_VALIDATION.md
- check RUNTIME_RULES.md
- check current firmware state

---

## Decision Rule

Before any new feature is proposed ask:

```text
Does this help hardware stabilisation before WiFi?
```

If no:

```text
defer it
```

---

## Current Hard Stop

Until wiring stabilisation is complete, the only valid engineering work is:

```text
improve jumper wiring
add strain relief
repeat 50p validation
repeat 10p validation
repeat unload validation
confirm repeatability remains stable
```

Everything else is later.
