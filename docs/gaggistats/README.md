# GaggiStats

GaggiStats is the instrumentation-first analytics track for GaggiMate-compatible machines.

It is not an espresso autopilot, adaptive brew controller, or AI extraction system.

The aim is simple: expose what the machine is actually doing so the operator can make better decisions.

## Scope

GaggiStats focuses on:

- live pressure telemetry
- live boiler temperature telemetry
- shot history
- analyser overlays
- diagnostics
- event markers
- profile comparison
- exportable data

## Non-goals

GaggiStats does not aim to:

- automatically correct shots
- compensate for bad puck prep
- run adaptive pressure loops
- predict flavour
- replace operator judgement

## Project status

This branch is a planning and documentation foundation only. No implementation work should begin until the existing GaggiGo/GaggiMate data paths have been audited.

## Upstream target

All work must remain compatible with upstream GaggiMate history and should be kept in clean, reviewable commits.