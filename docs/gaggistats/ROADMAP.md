# GaggiStats Roadmap

## Core Philosophy

Instrumentation first.
Operator in control.
No espresso autopilot.

The software should expose machine behaviour clearly and deterministically.

---

# Phase 1 — Foundation Stability

Goals:
- stable telemetry pipeline
- stable analyser
- deterministic offline cache
- fast graph rendering
- unified data paths

---

# Phase 2 — Live Telemetry

Metrics:
- pressure
- boiler temperature
- flow
- shot timer
- weight
- pump state
- valve state

Features:
- low-latency live graphs
- event markers
- telemetry overlays

---

# Phase 3 — Analyzer 2.0

Features:
- shot comparison
- pressure variance analysis
- temperature drift analysis
- profile overlays
- exportable telemetry

---

# Phase 4 — Diagnostics

Engineering-focused diagnostics only.

Examples:
- boiler overshoot
- recovery instability
- pressure instability
- decompression anomalies
- sensor noise

---

# Explicit Non-Goals

No:
- adaptive extraction AI
- autonomous brew correction
- flavour prediction
- puck compensation loops
- hidden machine intervention