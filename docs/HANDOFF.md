# VitaLink Handoff

## Current Phase

**Phase 2 — Verified/Frozen Baseline**

Do not start Phase 3 without explicit approval.

---

## Current Development Agent

**Antigravity is the sole active development agent for VitaLink.**

Do not use another coding agent simultaneously on this working tree.

The project should maintain one agent, one workspace, and one source of truth.

---

## Current Primary Interface

**SSD1306 OLED**

The OLED is the primary simulation UI and observability channel.

Serial Monitor is secondary/deferred and must not block development.

---

## Current Working Hardware

- ESP32-S3 DevKitC-1
- SSD1306 I2C OLED
- MPU6050 I2C accelerometer/gyroscope

### I2C

- GPIO8 = SDA
- GPIO9 = SCL
- OLED = `0x3C`
- MPU6050 = `0x68`

---

## Current Firmware

`src/main.cpp`

Current functionality:

- ESP32-S3 boot
- I2C initialization
- SSD1306 initialization
- MPU6050 initialization
- acceleration magnitude
- gyro magnitude
- motion classification
- motion hysteresis
- three-sample motion confirmation
- deterministic prototype fall state machine
- OLED status rendering
- rate-limited serial diagnostics

The current source uses:

- free-fall threshold: `< 3.0 m/s²`
- impact threshold: `>= 18.0 m/s²`
- impact gyro support: `>= 2.0 rad/s`
- post-impact observation: `1000 ms`
- post-impact motion: `>= 13.0 m/s²` or gyro `>= 0.55 rad/s`
- alert duration: `3000 ms`

These are prototype/demo thresholds and are not clinically validated.

---

## Phase 2 State

**FROZEN**

Do not change the Phase 2 fall/motion architecture unless a focused defect is identified.

Historical manual testing demonstrated:

- resting
- normal movement
- high activity
- fall sequence
- fall alert
- recovery

Current documentation/runtime verification may not have fresh Wokwi evidence.

---

## Current Wokwi Simulation

Current intended diagram contains only:

1. ESP32-S3 DevKitC-1
2. SSD1306 OLED
3. MPU6050

Do not add future hardware until explicitly approved.

---

## Current Wokwi Configuration

`diagram.json` contains:

- ESP32-S3 with `USB_SERIAL_JTAG`
- OLED at `0x3C`
- MPU6050 at `0x68`
- GPIO8/GPIO9 shared I2C wiring
- persistent Wokwi serial monitor configuration
- no `$serialMonitor` physical connections

`wokwi.toml` points to the PlatformIO firmware and ELF artifacts.

---

## Wokwi MCP

Existing `.vscode/mcp.json` contains the Wokwi stdio MCP declaration.

Antigravity's own workspace MCP configuration must be treated separately.

Target Antigravity workspace configuration:

`.agents/mcp_config.json`

Expected Wokwi server:

`wokwi-cli mcp`

Do not expose the Wokwi token.

Do not claim Antigravity/Wokwi MCP integration is verified until Antigravity itself successfully discovers and uses the Wokwi server.

---

## Current MAX30102 State

`max30102.chip.wasm` exists.

Current status:

**PARTIALLY PRESENT / NOT INTEGRATED**

There is currently no confirmed:

- MAX30102 diagram part
- `max30102.chip.json`
- `max30102.chip.c`
- firmware integration
- I2C transaction path
- HR/SpO2 controls
- verified FIFO behavior

Do not claim the MAX30102 is currently implemented.

---

## Current DS3231 State

**PLANNED / DEFERRED**

The planned DS3231 commonly uses `0x68`, which conflicts with the active MPU6050.

Do not integrate it until the bus architecture is explicitly decided.

---

## Other Future Hardware

Planned:

- GPS/GNSS
- SIM800L/SIM868
- buzzer
- vibration motor
- MicroSD
- battery/power management

No current implementation.

---

## Explicitly Deferred

- Serial Monitor debugging
- MAX30102 integration until Phase 3 approval
- DS3231 until address architecture is resolved
- GPS/GNSS
- GSM/SMS
- Compound Risk Index
- AI/ML
- BLE companion
- dashboard/backend
- authentication
- cloud functionality
- physical prototype

---

## Important Engineering Rule

Simulation and physical prototype are separate engineering phases.

Wokwi can validate:

- firmware logic
- interfaces
- deterministic sensor models
- integration paths
- observable prototype behavior

Wokwi cannot prove:

- physical calibration
- electrical safety
- clinical accuracy
- medical validity
- real-world GSM/GNSS performance

---

## Current Next Action

### Tooling

Complete and verify:

**Antigravity → Wokwi MCP**

without changing application code.

After MCP connectivity is verified:

1. inspect the actual workspace;
2. verify the frozen Phase 2 baseline;
3. perform one focused local Wokwi verification;
4. only then begin the next approved subsystem.

### Next Feature

**MAX30102 integration**

Only after explicit Phase 3 approval.

Integrate the actual sensor path rather than injecting HR/SpO2 values into firmware.

---

## Do Not

- Do not modify Phase 2 unnecessarily.
- Do not replace intended hardware because Wokwi lacks native support.
- Do not inject fake sensor values directly into firmware.
- Do not repeatedly debug Serial Monitor.
- Do not repeatedly run cloud simulations.
- Do not expose credentials.
- Do not add multiple future subsystems at once.
- Do not allow two agents to edit the workspace simultaneously.
- Do not claim runtime behavior without evidence.