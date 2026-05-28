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
Phase 1 — Hardware bring-up
```

Current status:

```text
parts received / physical validation starting
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

GaggiStop must not become:

- a standalone control authority
- a smart scale ecosystem
- a cloud product
- a Bluetooth product
- a Home Assistant project
- a separate consumer app

GaggiGo must not own live machine stop control.

---

## Current Engineering Target

Work only on Phase 1 until complete.

Phase 1 target:

```text
ESP32 boots
Serial monitor works
HX711 produces live readings
Load cell responds to weight changes
Readings are repeatable enough to continue
```

Do not advance to WiFi telemetry until raw serial weight readings are stable.

Do not advance to GaggiMate integration until WiFi telemetry is proven.

Do not tune auto-stop until GaggiMate receives live weight reliably.

---

## Method Rules

One variable at a time.

Correct order:

```text
1. ESP32 boot
2. serial monitor
3. HX711 wiring
4. raw readings
5. tare/calibration
6. basic filtering
7. WiFi telemetry
8. GaggiMate integration
9. auto-stop tuning
```

Never debug multiple layers at once.

If something fails, reduce back to the last known working layer.

---

## Hardware Safety Rules

Before powering hardware:

- verify voltage pins
- verify ground
- verify HX711 pinout
- verify load cell wire colours
- check for loose jumpers
- avoid late-night angry rewiring

No mains-voltage work belongs in GaggiStop Phase 1.

USB-powered low-voltage prototype only.

---

## Software Rules

Use PlatformIO for firmware work.

Initial firmware goal is only:

```text
print boot message
read HX711
print readings over serial
```

Do not add advanced logic before physical readings exist.

Do not add:

- prediction engines
- adaptive filtering
- complex protocols
- UI work
- cloud/MQTT/Home Assistant integrations
- battery/power optimisation

---

## Stop Strategy Rules

Do not chase perfect drip elimination.

Current working assumption:

```text
target = 32g
stop trigger = 31g
offset = 1.0g
```

The offset must remain configurable later.

0.5g may be tested only after the full signal path is proven.

---

## Repo Rules

Canonical integration work lives in:

```text
TyrLabsOS/GaggiGo branch GaggiStop
```

The old standalone GaggiStop repo is no longer the canonical project home.

Do not create more repos for this project.

Do not copy unrelated experimental clutter into this branch.

Keep commits small and upstream-friendly.

---

## Decision Rule

Before any new feature is proposed, ask:

```text
Does this help Phase 1 physical validation?
```

If no, defer it.

Before any architecture change is proposed, check:

- README.md
- ROADMAP.md
- this runtime rules file
- current GitHub branch state

---

## Current Hard Stop

Until Phase 1 succeeds, the only valid engineering work is:

```text
ESP32 boot
serial output
HX711 wiring
raw readings
basic calibration
```

Everything else is later.
