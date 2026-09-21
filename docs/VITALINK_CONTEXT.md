# VITALINK — AI AGENT STARTUP PROTOCOL

Before modifying anything:

1. Read this entire document.
2. Inspect the actual workspace.
3. Check Git status.
4. Identify the current project phase.
5. Identify current blockers.
6. Confirm what is implemented versus planned.
7. Read the relevant source files.
8. Do not modify unrelated components.
9. Make the smallest safe change.
10. Build/test.
11. Record the result.
12. Update this document only when project state genuinely changes.

If the requested task conflicts with this document, stop and explain the conflict before making architectural changes.

## Project Identity

- **Project:** VitaLink
- **SIH problem ID:** SIH26181
- **Organization:** Qualcomm Inc
- **Category:** Hardware
- **Theme:** MedTech / BioTech / HealthTech
- **Context:** SIH 2026
- **Current phase:** Phase 2 — Verified/Frozen Baseline Restored

The current workspace is a small ESP32-S3 Wokwi prototype. The implemented product surface is motion and prototype fall detection with a local SSD1306 status display. It is not a medical device, clinical monitor, emergency system, or validated fall detector.

### Objective and Intended Use

The long-term objective is a wearable health and safety platform that combines physiological, motion, environmental, location, edge-analysis, local-alert, logging, and communication capabilities. Intended users and use cases include a wearable safety aid for an individual, offline-first monitoring, activity/fall demonstrations, and future emergency notification workflows. None of the long-term health, risk, or emergency claims should be presented as implemented in the current workspace.

## Status Vocabulary

- **IMPLEMENTED:** present in the current source/configuration and exercised by a successful build or direct inspection.
- **PARTIALLY IMPLEMENTED:** some files or behavior exist, but integration or verification is incomplete.
- **PLANNED:** described as future work only.
- **EXPERIMENTAL:** prototype behavior with known limitations.
- **BROKEN:** currently fails a directly observed check.
- **DEFERRED:** intentionally postponed to a later phase.
- **UNKNOWN:** cannot be established from the current workspace.

## Current Truth at a Glance

| Area | Status | Evidence / note |
|---|---|---|
| ESP32-S3 Arduino firmware | IMPLEMENTED | `src/main.cpp`, PlatformIO configuration |
| SSD1306 OLED | IMPLEMENTED / PRIMARY SIMULATION UI | Native Wokwi part, I2C address `0x3C`, firmware rendering; current runtime screenshot unavailable |
| MPU6050 | IMPLEMENTED | Native Wokwi part, address `0x68`, Adafruit driver |
| Motion classification | IMPLEMENTED / EXPERIMENTAL | Hysteresis and three-sample confirmation |
| Fall detection | IMPLEMENTED / EXPERIMENTAL | Explicit deterministic prototype state machine |
| MAX30102 | PARTIALLY PRESENT, NOT INTEGRATED | `max30102.chip.wasm` exists, but no diagram part or firmware use |
| DS3231, GPS/GNSS, SIM800L | PLANNED / DEFERRED | No current source or diagram integration |
| Serial monitor | DEFERRED | Secondary/optional telemetry channel; not a current project blocker |
| Physical prototype | PLANNED | No physical hardware is represented by this repository state |
| Compound Risk Index | PLANNED | No implementation |
| AI/ML | PLANNED | No implementation |
| Dashboard/backend/authentication | PLANNED | No implementation |

## Repository Tree

Current non-generated project files:

```text
VitaLink/
├── .vscode/
│   └── mcp.json
├── docs/
│   ├── HANDOFF.md
│   └── VITALINK_CONTEXT.md
├── .agents/
│   └── rules/
│       └── vitalink.md
├── diagram.json
├── max30102.chip.wasm
├── platformio.ini
├── src/
│   └── main.cpp
├── wokwi-api.h
└── wokwi.toml
```

Generated/local directories also exist:

- `.pio/`: PlatformIO build output and downloaded dependencies. Do not edit as source.
- `.venv/`: local Python environment. Do not edit as project source.

The parent Git repository resolves to `C:\Users\hp`; the VitaLink directory itself has no `.git` directory. At documentation time the parent repository reports `Documents/VitaLink/` as untracked, so there is no tracked VitaLink diff to use as historical proof.

## Important Files

| File | Purpose | Owner | Status | Do not modify unless |
|---|---|---|---|---|
| `src/main.cpp` | Current ESP32-S3 firmware entry point, initialization, sampling, motion, fall, OLED, diagnostics | Firmware integration | IMPLEMENTED / EXPERIMENTAL | The focused firmware task explicitly requires it |
| `diagram.json` | Wokwi board, OLED, MPU6050, wiring, layout, serial monitor configuration | Simulation integration | IMPLEMENTED / PARTIALLY CONFIGURED | A simulation or visual-layout task requires it |
| `platformio.ini` | ESP32-S3 Arduino build target and declared libraries | Build configuration | IMPLEMENTED | Build/dependency task requires it |
| `wokwi.toml` | Firmware and ELF paths used by Wokwi | Simulation configuration | IMPLEMENTED | Artifact path changes are required |
| `.vscode/mcp.json` | Wokwi MCP stdio server declaration | Tooling | IMPLEMENTED | MCP setup explicitly requires it |
| `wokwi-api.h` | Wokwi custom-chip API declarations | Simulation tooling | PRESENT, NOT AN INTEGRATED DEVICE | Custom-chip development explicitly requires it |
| `max30102.chip.wasm` | Compiled custom-chip binary artifact | Future/custom-chip work | PRESENT, NOT INTEGRATED | MAX30102 integration explicitly requires it |
| `.pio/` | Generated firmware/dependency output | PlatformIO | GENERATED | Never edit manually |
| `.venv/` | Local Python environment | Tooling | LOCAL ENVIRONMENT | Never treat as application source |

## Project Vision

The intended complete system is:

```text
Wearable
  -> physiological sensing
  -> motion sensing
  -> environmental/location sensing
  -> edge processing
  -> personal baseline
  -> anomaly detection
  -> Compound Risk Index
  -> local alerts
  -> offline logging
  -> emergency communication
  -> companion application
```

The layers exist for these intended reasons:

- **Wearable sensing:** gather physiological, motion, and contextual signals.
- **Edge processing:** continue operating with low latency and limited connectivity.
- **Personal baseline:** compare an individual against their own normal patterns rather than a universal threshold alone.
- **Anomaly detection:** identify unusual combinations or sequences.
- **Compound Risk Index:** combine validated signals into an early-warning score.
- **Local alerts:** notify the wearer without network dependency.
- **Offline logging:** preserve events during connectivity loss.
- **Emergency communication:** use GNSS/GSM when a configured emergency workflow is triggered.
- **Companion application:** provide configuration, history, and shared processed results.

This is the proposal, not the current implementation. The current workspace only implements the ESP32-S3, SSD1306, and MPU6050 slice.

## Hardware Inventory

### Current Simulation Hardware

| Component | Purpose | Interface | Address/Pins | Current Status | Notes |
|---|---|---|---|---|---|
| ESP32-S3 DevKitC-1 | MCU and edge firmware | Arduino / I2C / USB serial configuration | GPIO8 SDA, GPIO9 SCL | IMPLEMENTED | Native Wokwi board |
| SSD1306 OLED | Local status display | I2C | `0x3C`; `DATA`/SDA and `CLK`/SCL | IMPLEMENTED | Native `wokwi-ssd1306` |
| MPU6050 | 3-axis acceleration and 3-axis gyro | I2C | `0x68`; SDA GPIO8, SCL GPIO9 | IMPLEMENTED | Native `wokwi-mpu6050` |

### Intended Full/Physical PoC Hardware

| Component | Purpose | Interface | Address/Pins | Current Status | Notes |
|---|---|---|---|---|---|
| ESP32-S3 | Main controller | I2C, UART, GPIO, USB | GPIO8/9 are current I2C pins | IMPLEMENTED in simulation | Physical board not verified here |
| MAX30102 | Heart rate / SpO2 sensing | I2C | Commonly `0x57` | DEFERRED | Custom Wokwi artifact exists but is not integrated |
| MPU6050 | Motion/fall sensing | I2C | `0x68` | IMPLEMENTED in simulation | Prototype only |
| DS3231 | RTC/timekeeping | I2C | Commonly `0x68` | PLANNED | Address collision with MPU6050 must be resolved explicitly |
| GPS/GNSS | Location/time | UART | Board/module-dependent | PLANNED | No current module or code |
| SIM800L/SIM868 | GSM/SMS emergency communication | UART/power/GPIO | Board/module-dependent | PLANNED | Power-burst risk |
| OLED | Local status | I2C | `0x3C` | IMPLEMENTED in simulation | Not a full dashboard |
| Buzzer | Local audible alert | GPIO/PWM | Not assigned | PLANNED | No current hardware |
| Vibration motor | Local haptic alert | GPIO/driver | Not assigned | PLANNED | No current hardware |
| MicroSD | Offline logging | SPI | Not assigned | PLANNED | No current hardware |
| Battery/power management | Portable power | Power subsystem | Not assigned | PLANNED | No current hardware |

### Proposed Additional Sensors

MAX30205, GSR, AD5933, and BME680 belong to the broader proposal. They are PLANNED/UNKNOWN for this workspace and are not in the current diagram or firmware.

## Current Wokwi Simulation Architecture

The current `diagram.json` contains exactly three native Wokwi parts:

1. `board-esp32-s3-devkitc-1`, id `esp`, with `serialInterface: USB_SERIAL_JTAG`.
2. `wokwi-ssd1306`, id `oled`, with `i2cAddress: 0x3C`.
3. `wokwi-mpu6050`, id `mpu`, using its default I2C address `0x68`.

Visual layout:

- ESP32-S3: left, `top: 0`, `left: 0`.
- OLED: upper right, `top: -10`, `left: 360`.
- MPU6050: lower right, `top: 220`, `left: 360`.

Electrical connections:

```text
ESP 3V3.1 -> OLED 3V3
ESP GND.1 -> OLED GND
ESP GPIO8 -> OLED DATA/SDA
ESP GPIO9 -> OLED CLK/SCL
ESP 3V3.2 -> MPU VCC
ESP GND.2 -> MPU GND
ESP GPIO8 -> MPU SDA
ESP GPIO9 -> MPU SCL
```

There are no `$serialMonitor` physical connections in `diagram.json`.

The `max30102.chip.wasm` file is present, but there is no MAX30102 custom-chip part in the current diagram, no `max30102.chip.json`, no `max30102.chip.c`, and no MAX30102 firmware integration. Its controls/FIFO/HR/SpO2/finger-contact behavior are therefore UNKNOWN from the current workspace, not implemented claims.

Simulation and physical prototype are separate engineering phases. Wokwi is for reproducible software validation, deterministic controls, observable outputs, and integration experiments; it is not a substitute for physical sensor calibration or electrical validation.

## Firmware Architecture

`src/main.cpp` is the only application source file.

### Boot and initialization

`setup()` performs:

1. `Serial.begin(115200)`.
2. Prints `[VitaLink] ESP32 boot`.
3. Calls `initializeI2c()`, which calls `Wire.setPins(8, 9)` and `Wire.begin()`.
4. Calls `initializeDisplay()`, which calls `display.begin(SSD1306_SWITCHCAPVCC, 0x3C)`.
5. Calls `initializeMpu6050()`, which calls `mpu.begin(0x68, &Wire)` and configures an 8 g accelerometer range, 500 degree-range gyro, and 21 Hz filter bandwidth.
6. Renders the initial OLED status.
7. Prints the Phase 2 status message.

Initialization failure enters an infinite delay loop after printing an error. There is no recovery/reinitialization routine.

### Main loop

The loop samples every 250 ms. It:

1. Reads `sensors_event_t` acceleration, gyroscope, and temperature via `mpu.getEvent()`; temperature is read into a local structure but is not used or displayed.
2. Calculates acceleration magnitude and gyro magnitude.
3. Updates the fall state machine.
4. Updates the motion state.
5. Renders the OLED status.
6. Prints rate-limited diagnostics every 500 ms.

Diagnostics use `[Motion]` and `[Fall]` prefixes and report SI units, feature magnitudes, motion state, and fall state.

### Error handling and alerts

An MPU6050 read failure prints `[VitaLink] MPU6050 read failed` and skips that sample. Fall confirmation changes the motion state to `FALL_DETECTED`, which changes the OLED to an alert layout. There is no buzzer, vibration motor, SD log, GSM, GNSS, or companion application.

## Motion and Fall Detection

### Motion states

The supported motion states are:

- `RESTING`
- `MOVING`
- `HIGH ACTIVITY`
- `FALL DETECTED`

The Adafruit Unified Sensor values are used in SI units: acceleration in `m/s^2`, gyro in `rad/s`. The Wokwi controls may display g and degrees per second, but the firmware receives converted library values.

### Motion hysteresis

The firmware calculates:

```text
accelerationMagnitude = sqrt(x^2 + y^2 + z^2)
gyroMagnitude = sqrt(gx^2 + gy^2 + gz^2)
```

Thresholds in the current source:

- RESTING entry: acceleration `<= 11.5 m/s^2` and gyro `<= 0.35 rad/s`.
- MOVING entry: acceleration `>= 12.5 m/s^2` or gyro `>= 0.55 rad/s`.
- HIGH ACTIVITY entry: acceleration `>= 19.0 m/s^2` or gyro `>= 2.5 rad/s`.
- HIGH ACTIVITY retention: acceleration `>= 17.0 m/s^2` or gyro `>= 2.0 rad/s`.
- A candidate transition requires `3` consecutive samples.

The motion candidate logic is transparent threshold/hysteresis logic, not machine learning.

### Fall state machine

```text
NORMAL
  -> POSSIBLE_FREE_FALL
  -> IMPACT_DETECTED
  -> POST_IMPACT_CHECK
  -> FALL_CONFIRMED
  -> NORMAL
```

Current source thresholds:

- Free fall: acceleration magnitude `< 3.0 m/s^2`.
- Impact: acceleration magnitude `>= 18.0 m/s^2`.
- Free-fall-to-impact maximum interval: `1500 ms`.
- Impact gyro support: stored impact gyro magnitude `>= 2.0 rad/s`.
- Post-impact observation: `1000 ms`.
- Post-impact motion: acceleration `>= 13.0 m/s^2` OR gyro `>= 0.55 rad/s`.
- Alert duration: `3000 ms`.

The impact threshold comment identifies `18.0 m/s^2` as a prototype/Wokwi demonstration threshold selected to allow approximately 2 g simulated input while retaining the required sequence. It is not clinically validated.

A suspected fall is cancelled when free fall times out, free fall ends without reaching impact, or post-impact confirmation has neither impact gyro support nor post-impact movement. A confirmed alert returns to `NORMAL` after 3000 ms. This is EXPERIMENTAL prototype logic and is not medical-grade or clinically validated.

## OLED

The firmware uses a 128x64 SSD1306 buffer with fixed short rows:

```text
VITALINK
SIH26181
----------------
STATUS: ONLINE / ACTIVE / ALERT
MOTION: RESTING / MOVING / HIGH ACT / FALL
FALL: NO / FALL DETECTED!
```

Raw sensor values are not rendered on the OLED. The OLED is a local status display, not a dashboard.

## MAX30102 and Custom-Chip Roadmap

### MAX30102

- **Purpose:** future heart-rate and SpO2 sensing.
- **Current files:** `max30102.chip.wasm` and `wokwi-api.h` are present.
- **Missing files:** no `max30102.chip.c` or `max30102.chip.json` is present in the current workspace.
- **Current integration:** no diagram part, no firmware include/object, no I2C transaction path, no controls in the current diagram.
- **Address:** proposed/common address `0x57`, not active in current simulation.
- **HR, SpO2, finger contact, FIFO behavior:** UNKNOWN from current source; do not claim verified.
- **SparkFun MAX3010x integration:** not present in `src/main.cpp`; any dependency cache is generated and not proof of integration.
- **Calibration:** no clinical accuracy claim is valid.
- **Next step:** inspect/restore the custom-chip source and metadata, define its behavior, add it to a dedicated simulation task, then integrate firmware only under explicit Phase 3 approval.

### DS3231

- **Purpose:** RTC.
- **Required interface:** I2C, commonly `0x68`.
- **Files:** none in current workspace.
- **Status:** PLANNED/DEFERRED.
- **Risk:** address collision with MPU6050 at `0x68` on the same bus. Options must be an explicit engineering decision: alternate address capability (if applicable), bus multiplexer, separate bus, or another documented architecture. Do not silently solve it.

### GPS/GNSS

- **Purpose:** location and time.
- **Interface:** typically UART.
- **Files:** none.
- **Status:** PLANNED/DEFERRED.
- **Risks:** signal availability, antenna placement, power, cold start, indoor testing.

### SIM800L/SIM868

- **Purpose:** GSM/SMS emergency communication.
- **Interface:** UART plus high-current power path and control pins.
- **Files:** none.
- **Status:** PLANNED/DEFERRED.
- **Risks:** burst current, brownout, network availability, SIM/carrier behavior.

## I2C Bus Map

| Device | Address | Current bus status |
|---|---:|---|
| SSD1306 | `0x3C` | Active |
| MPU6050 | `0x68` | Active |
| MAX30102 | `0x57` | Proposed, inactive |
| DS3231 | `0x68` | Proposed, collides with MPU6050 |

Current bus pins are GPIO8 SDA and GPIO9 SCL. The DS3231/MPU6050 collision is intentionally documented and unresolved.

## Serial Monitor

Current firmware calls `Serial.begin(115200)` and emits boot, initialization, motion, and fall diagnostics. The current `diagram.json` configures the ESP32-S3 part with:

```json
"serialInterface": "USB_SERIAL_JTAG"
```

It also contains:

```json
"serialMonitor": {
  "display": "always",
  "collapse": false,
  "convertEol": true,
  "newline": "lf"
}
```

There are no `$serialMonitor` TX/RX connections. `wokwi.toml` points to the current PlatformIO binary and ELF. Serial Monitor is SECONDARY/OPTIONAL and DEFERRED until the complete VitaLink system has been integrated and verified through the OLED. Historical MCP serial visibility problems are not current project blockers and should not drive architecture or repeated simulation work.

The VS Code/PowerShell terminal is not the Wokwi embedded Serial Monitor. Their output streams must not be conflated.

## Wokwi Configuration and MCP

### Configuration

- `diagram.json`: circuit parts, layout, connections, USB serial interface, and serial monitor settings.
- `wokwi.toml`: firmware `.pio/build/esp32-s3-devkitc-1/firmware.bin`; ELF `.pio/build/esp32-s3-devkitc-1/firmware.elf`.
- `platformio.ini`: ESP32-S3 DevKitC-1, Arduino framework, SSD1306 and MPU6050 dependencies.
- `.vscode/mcp.json`: Wokwi stdio server using `wokwi-cli mcp`.
- Wokwi token configuration may exist in the environment; do not expose it. **[SECRET PRESENT — DO NOT EXPOSE]**


### Available Wokwi MCP workflow

The tool surface supports:

- start, stop, restart, and resume simulation;
- status checks;
- serial read and write;
- pin reads;
- component controls;
- display screenshots;
- VCD export.

Known limitations encountered:

- MCP serial reads have returned no serial output despite firmware diagnostics.
- Screenshot results may be returned as base64 rather than a directly readable visual.
- Control acceptance is not proof that firmware state changed; behavior must be observed.
- Prefer the local Wokwi VS Code simulator. Cloud Wokwi quota has previously been exhausted; avoid repeated cloud runs and unnecessary screenshots.
### Antigravity MCP Status

The repository currently contains `.vscode/mcp.json`, which declares the Wokwi stdio MCP server.

This configuration should not automatically be assumed to be Antigravity's workspace MCP configuration.

Antigravity workspace MCP configuration should be maintained separately at:

`.agents/mcp_config.json`

Expected server command:

`wokwi-cli mcp`

Current status:

**CONFIGURATION IN PROGRESS / NOT YET VERIFIED THROUGH ANTIGRAVITY**

Do not mark Wokwi MCP as fully integrated until Antigravity itself successfully discovers the Wokwi server and can invoke an appropriate Wokwi operation.

Wokwi authentication credentials must remain in the local environment and must never be exposed in project documentation or chat.
## Physical Prototype vs Simulation

**Simulation** is for software validation, sensor models, deterministic control input, serial telemetry, and integration tests.

**Physical prototype** requires real sensors, OLED, buzzer, vibration driver, battery/power management, GSM, GNSS, enclosure, calibration, and electrical testing. A Wokwi component does not prove physical accuracy, safety, calibration, or clinical behavior.

## Compound Risk Index

The intended concept is a 0–100 early-warning score combining validated inputs such as HR, SpO2, temperature, environmental conditions, activity, GSR, and other signals. It is explicitly not a medical diagnosis. No risk score exists in the current firmware. Status: PLANNED.

## Emergency Flow

Intended future architecture:

```text
Sensor -> ESP32-S3 -> local analysis -> local alert -> MicroSD
Wearable -> GNSS -> GSM/SMS SOS
```

Manual SOS, dead-man's-switch concepts, offline behavior, and intermittent-connectivity handling are PLANNED. No corresponding hardware or code exists in the current workspace.

## Privacy Architecture

The intended design is offline-first: raw physiological data should primarily remain on the wearable; processed parameters, risk indicators, and alerts may be shared through a future BLE companion. No cloud dependency, authentication, or companion application is implemented.

## Engineering Risks

Known proposal risks include:

- GSM burst current and brownout.
- I2C address conflict between MPU6050 and DS3231.
- Sensor placement and calibration.
- False fall detection and false alarms.
- Signal quality and connectivity loss.
- Battery life, thermal behavior, and enclosure constraints.
- Environmental interference.
- Difference between simulation models and physical sensors.

## Current Bugs and Blockers

| Issue | Evidence | Impact | Current hypothesis | Next diagnostic step | Status |
|---|---|---|---|---|---|
| Wokwi embedded Serial Monitor visibility | Prior MCP reads returned `No serial output` | Secondary telemetry is unavailable through that path | Tool/session visibility issue | Revisit only after complete system integration | DEFERRED / NOT A PROJECT BLOCKER |
| MAX30102 custom-chip integration | `max30102.chip.wasm` exists, but no diagram part, metadata, source, or firmware use | Physiological sensing is not available | Artifact was staged without integration | Decide whether to restore/document custom-chip source under Phase 3 | DEFERRED |
| DS3231 address collision | Proposed DS3231 and active MPU6050 both use `0x68` | Future shared-bus integration cannot proceed silently | Requires documented alternate-bus/address decision | Choose a bus architecture before adding DS3231 | DEFERRED |

Only real workspace-supported issues are listed. No OLED/I2C compile failure is currently evidenced in the source/configuration audit.

## Verification Record

### Manually verified from current workspace

| Test | Expected | Actual | Result | Date/phase | Evidence |
|---|---|---|---|---|---|
| Source/configuration audit | Phase 2 files and values present | ESP32-S3, OLED, MPU6050, GPIO8/9, addresses, state machine, and `18.0 m/s^2` threshold present | VERIFIED by inspection | 2026-09-21 / Phase 2 checkpoint | Current files listed above |
| Diagram inventory | Only current parts in diagram | Three native parts: ESP32-S3, SSD1306, MPU6050 | VERIFIED by inspection | 2026-09-21 / Phase 2 | `diagram.json` |
| Serial configuration | USB serial settings present | `USB_SERIAL_JTAG`, persistent serial monitor settings, no physical monitor wiring | VERIFIED by inspection | 2026-09-21 / Phase 2 refinement | `diagram.json` |

### Agent-reported historical checks

Earlier work reported successful PlatformIO builds, Wokwi starts, Wokwi connection status, and OLED screenshot capture. Those reports are historical agent output, not fresh evidence from this documentation pass. The MCP serial reader was historically reported empty.

### Not verified in this documentation pass

- Current runtime serial monitor visibility after the `USB_SERIAL_JTAG` configuration.
- MAX30102 controls, FIFO, HR, SpO2, or finger-contact behavior.
- Physical hardware behavior, calibration, medical accuracy, or clinical validity.
- Any Phase 3 feature.

## Project Phases

| Phase | Goal | Status | Completed / remaining |
|---|---|---|---|
| 1 — Foundation | ESP32-S3, I2C, SSD1306 status | IMPLEMENTED | Current firmware and native Wokwi circuit |
| 2 — Motion/Fall Detection | MPU6050 motion and prototype fall sequence | VERIFIED/FROZEN BASELINE | Historically manually verified through OLED; current Wokwi runtime was unavailable in this check |
| 3 — Physiological sensing | MAX30102 heart rate and SpO2 | DEFERRED | Artifact present but not integrated |
| 4 — RTC/location | DS3231 and GNSS | PLANNED | Address and interface decisions required |
| 5 — GSM emergency communication | GSM/SMS SOS | PLANNED | Power and network design required |
| 6 — Compound Risk Index | 0–100 early-warning score | PLANNED | Validated feature definitions required |
| 7 — Edge intelligence | Baselines/anomaly detection | PLANNED | Data and validation required |
| 8 — BLE companion application | Local companion UI | PLANNED | Protocol and privacy design required |
| 9 — Physical prototype | Real wearable hardware | PLANNED | Calibration, power, enclosure, safety |
| 10 — Final integration/testing | End-to-end validation | PLANNED | All previous phases and test evidence |

## Decision Log

| Date/Phase | Decision | Reason | Alternatives rejected | Current validity |
|---|---|---|---|---|
| Phase 1 | Use Wokwi for primary simulation | Reproducible embedded prototyping and controls | Unverified physical-only development | Valid |
| Phase 1/2 | Keep simulation separate from physical prototype | Simulation cannot establish calibration or clinical behavior | Treating Wokwi as physical proof | Valid |
| Phase 2 | Use native Wokwi SSD1306 and MPU6050 | Directly observable I2C prototype | Inventing alternate hardware | Valid |
| Phase 2 | Use explicit deterministic fall state machine | Explainable prototype behavior | Opaque ML or high-acceleration-only trigger | Valid |
| Phase 2 | Use `18.0 m/s^2` impact threshold | Allows approximately 2 g Wokwi demonstration while preserving sequence | Removing free-fall/post-impact requirements | Experimental and not clinically validated |
| Phase 2 refinement | Prefer local Wokwi simulation | Cloud quota and observability concerns | Repeated cloud runs | Valid |
| Current | Keep agent ownership boundaries | Prevent conflicting edits | Multiple agents modifying one worktree | Valid |

Historical decisions are preserved here only when supported by the workspace or prior explicit project instructions. They do not make planned features current.

## Agent Workflow and Ownership

Antigravity is currently the sole active development agent for VitaLink.

The project intentionally uses:

- one active agent;
- one workspace;
- one source of truth.

GitHub Copilot is not the active VitaLink development agent during this phase.

Antigravity is responsible for:

- workspace inspection;
- firmware integration;
- architecture work;
- custom-chip work;
- Wokwi integration;
- build/test;
- documentation updates;
- final system integration.

Do not allow another agent to modify the VitaLink worktree simultaneously.

The agent must read:

1. `docs/VITALINK_CONTEXT.md`
2. `docs/HANDOFF.md`
3. `.agents/rules/vitalink.md`

before substantial work.

Prefer focused tasks and incremental subsystem integration.

Avoid repeated builds when no source/build input changed.

Prefer local Wokwi simulation.

Avoid repeated cloud simulation and unnecessary screenshots.

Use stronger reasoning for architecture, debugging, and integration; use faster models for simple mechanical tasks where appropriate.

Never ask a future agent to rediscover facts already documented without first checking the actual workspace.

## NEVER DO WITHOUT EXPLICIT APPROVAL

- Replace intended hardware merely because Wokwi lacks a native component.
- Rewrite working fall-detection logic during unrelated work.
- Inject sensor values directly into firmware instead of modeling the sensor path.
- Remove existing functionality during a focused fix.
- Modify multiple subsystems during a focused debugging task.
- Expose secrets, tokens, or credentials.
- Run expensive or repeated cloud simulations unnecessarily.
- Change architecture without recording the decision.
- Claim a behavior works without direct evidence.
- Allow two agents to edit the same worktree simultaneously.
- Add MAX30102, DS3231, GNSS, SIM800L, buzzer, vibration motor, dashboard, risk scoring, AI/ML, cloud, or authentication without explicit phase approval.


## CURRENT NEXT ACTION

### Stage A — Tooling Ownership

Establish and verify Antigravity as the sole development agent with access to the local Wokwi MCP server.

Do not modify application code during this tooling step.

### Stage B — Phase 2 Acceptance

After Wokwi MCP connectivity is confirmed:

1. inspect the actual workspace;
2. verify the frozen ESP32-S3 + OLED + MPU6050 baseline;
3. perform one focused local simulation verification;
4. preserve the baseline unless a real defect is observed.

### Stage C — Phase 3

After explicit Phase 3 approval:

Begin MAX30102 integration incrementally.

Do not begin DS3231, GPS, GSM, risk scoring, AI/ML, dashboard, or other later features simultaneously.

## NEXT 5 ACTIONS

1. Continue OLED-first integration and verification of currently implemented behavior.
2. Keep the Phase 2 OLED + MPU6050 baseline unchanged unless a focused defect is found.
3. Begin MAX30102 integration only after explicit approval and stable Phase 2 acceptance.
4. Resolve the MPU6050/DS3231 `0x68` bus conflict before RTC integration.
5. Revisit Serial Monitor telemetry after complete system integration.

## NEXT 5 ACTIONS

1. Complete Antigravity workspace Wokwi MCP configuration.
2. Verify Antigravity can discover and invoke Wokwi MCP.
3. Perform one focused local Wokwi verification of the frozen Phase 2 baseline.
4. Obtain explicit Phase 3 approval and begin MAX30102 integration.
5. Resolve the MPU6050/DS3231 `0x68` conflict before RTC integration.