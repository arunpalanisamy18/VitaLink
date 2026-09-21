# VitaLink Agent Rules

## 1. Mandatory Startup

Before making any change:

1. Read `docs/VITALINK_CONTEXT.md` completely.
2. Read `docs/HANDOFF.md`.
3. Inspect the actual workspace.
4. Check Git status.
5. Identify the current project phase.
6. Identify current blockers.
7. Confirm IMPLEMENTED vs PARTIALLY IMPLEMENTED vs PLANNED vs DEFERRED vs UNKNOWN.
8. Read the relevant source/configuration files.
9. Do not modify unrelated components.
10. Make the smallest safe change.
11. Build/test the affected subsystem.
12. Record meaningful verification results.
13. Update project documentation only when project state genuinely changes.

If a requested task conflicts with these rules or the documented architecture, stop and explain the conflict before making architectural changes.

---

# 2. Agent Ownership

## Primary and sole development agent

Antigravity is the current primary and sole active development agent for VitaLink.

Do not assume that GitHub Copilot, Codex, Claude Code, Gemini CLI, or another agent is simultaneously modifying this workspace.

Only one agent may actively modify the VitaLink working tree at a time.

Do not ask another agent to modify files while Antigravity is actively working.

If another agent is detected as actively editing the same workspace, stop before making conflicting changes.

---

# 3. Source of Truth

The actual workspace is the source of truth.

Documentation describes intended/current state but must not override what is actually present in:

- source files
- configuration files
- `diagram.json`
- custom-chip artifacts
- build output
- Git state
- directly observed simulation behavior

Never claim that a feature is implemented merely because documentation says it should exist.

Distinguish clearly between:

- IMPLEMENTED
- PARTIALLY IMPLEMENTED
- PLANNED
- EXPERIMENTAL
- BROKEN
- DEFERRED
- UNKNOWN

---

# 4. Phase Discipline

## Current baseline

Phase 2 is the stable frozen baseline.

Current implemented simulation hardware:

- ESP32-S3 DevKitC-1
- SSD1306 OLED
- MPU6050

Current implemented behavior:

- I2C initialization
- OLED initialization/rendering
- MPU6050 sampling
- acceleration magnitude
- gyro magnitude
- motion classification
- deterministic prototype fall-detection state machine
- OLED status/alert display

Do not modify working Phase 2 logic unless the requested task specifically requires a focused Phase 2 defect fix.

Do not begin Phase 3 features without explicit approval.

---

# 5. Current Hardware Baseline

Preserve:

- ESP32-S3
- SSD1306 OLED
- MPU6050
- GPIO8 = SDA
- GPIO9 = SCL
- OLED address = `0x3C`
- MPU6050 address = `0x68`

Do not silently replace intended hardware because Wokwi lacks a native component.

If a hardware architecture must change, document the decision before implementation.

---

# 6. OLED Policy

The SSD1306 OLED is the primary simulation UI and observability interface.

The OLED must remain part of the current simulation.

Do not remove or replace the OLED merely to solve Serial Monitor visibility.

Serial Monitor is secondary/optional and currently deferred.

Do not repeatedly spend development cycles debugging Serial Monitor while the main system integration is progressing.

---

# 7. Wokwi Policy

Wokwi is the primary embedded simulation environment for VitaLink.

Prefer local Wokwi simulation whenever practical.

The Wokwi MCP server may be used for:

- simulation status
- start/stop/restart/resume
- component controls
- pin reads
- serial interaction
- screenshots
- VCD export
- focused simulation verification

However:

- Do not repeatedly retry a failing MCP operation.
- Do not repeatedly run expensive cloud simulations.
- Do not waste simulation quota on redundant tests.
- One focused verification is preferred over repeated identical runs.
- A successful tool call or accepted control value is not proof that firmware behavior changed.
- Behavior must be directly observed before being claimed as verified.
- A screenshot that cannot be interpreted reliably is not sufficient evidence of behavior.

The local VS Code Wokwi simulator should be preferred over unnecessary cloud runs.

---

# 8. Wokwi MCP and Credentials

Never expose:

- Wokwi tokens
- API keys
- passwords
- authentication credentials
- secrets from environment variables

Do not request the user's Wokwi token in chat.

Use the existing local environment configuration where available.

The Wokwi MCP configuration for Antigravity is separate from historical/editor-specific MCP configuration.

Do not assume `.vscode/mcp.json` automatically establishes Antigravity's workspace MCP configuration.

Do not modify MCP configuration unless the task explicitly concerns MCP/tooling setup.

---

# 9. Incremental Sensor Integration

Future sensors must be integrated one subsystem at a time.

Do not add multiple future subsystems in one uncontrolled change.

Do not add any of the following without explicit phase approval:

- MAX30102
- DS3231
- GPS/GNSS
- SIM800L/SIM868
- buzzer
- vibration motor
- MicroSD
- dashboard
- BLE companion
- Compound Risk Index
- AI/ML
- cloud backend
- authentication

When a subsystem is approved:

1. inspect existing artifacts;
2. define the intended interface;
3. integrate the simulation model;
4. integrate firmware;
5. build;
6. perform focused simulation verification;
7. document the result;
8. only then proceed to the next subsystem.

---

# 10. MAX30102 Policy

The current workspace contains a `max30102.chip.wasm` artifact.

This does NOT prove MAX30102 integration.

Do not:

- inject HR/SpO2 values directly into firmware;
- fabricate sensor readings;
- claim clinical accuracy;
- claim MAX30102 integration without an actual sensor path.

A proper integration must model:

Wokwi MAX30102 control
→ custom chip
→ simulated sensor registers/FIFO
→ I2C
→ ESP32-S3
→ firmware library/processing
→ observable output

Any custom-chip source or metadata must be inspected before being treated as recoverable implementation.

---

# 11. DS3231 Policy

The planned DS3231 commonly uses I2C address `0x68`, which conflicts with the active MPU6050.

Do not silently solve this conflict.

Before DS3231 integration, explicitly choose and document an architecture such as:

- alternate device address if supported;
- separate I2C bus;
- multiplexer;
- other documented hardware arrangement.

---

# 12. Firmware Safety

Do not rewrite working fall-detection logic during unrelated work.

Do not change thresholds without a documented reason.

Do not inject simulated sensor values directly into application logic when the purpose is to model an actual sensor.

Do not remove existing functionality during a focused fix.

Do not modify multiple unrelated subsystems during one debugging task.

---

# 13. Build and Verification

A successful compilation proves compilation only.

It does not prove:

- Wokwi runtime behavior
- sensor behavior
- OLED behavior
- fall detection behavior
- custom-chip behavior
- physical hardware behavior
- medical accuracy

Verification claims must identify their evidence.

Use the strongest available evidence:

1. direct physical observation;
2. direct local simulation observation;
3. reproducible automated test;
4. build result;
5. source/configuration inspection.

Do not present weaker evidence as stronger evidence.

---

# 14. Generated Files

Never manually edit:

- `.pio/`
- `.venv/`

Treat them as generated/local environments.

Do not commit generated artifacts unless explicitly required by project architecture.

---

# 15. Documentation

Keep:

- `docs/VITALINK_CONTEXT.md` = canonical detailed project truth
- `docs/HANDOFF.md` = short current-state handoff
- `.agents/rules/vitalink.md` = agent operating rules

Update documentation when project state genuinely changes.

Do not rewrite documentation merely because an agent performed an inspection.

---

# 16. Medical/Safety Claims

VitaLink is currently a prototype/research/safety-support system.

Do not describe it as:

- a medical device;
- a clinically validated fall detector;
- a diagnostic system;
- a clinically accurate SpO2 monitor;
- a medically validated risk score.

Prototype thresholds and simulated behavior must be described as engineering demonstrations unless validated evidence exists.

---

# 17. Agent Startup Checklist

At the beginning of every substantial task:

- [ ] Read `docs/VITALINK_CONTEXT.md`
- [ ] Read `docs/HANDOFF.md`
- [ ] Inspect workspace
- [ ] Check Git status
- [ ] Identify current phase
- [ ] Identify blockers
- [ ] Identify files that will actually change
- [ ] Confirm no second agent is modifying the tree
- [ ] Make smallest safe change
- [ ] Build/test
- [ ] Verify behavior using appropriate evidence
- [ ] Update documentation if state changed

---

# 18. Current Priority

The current priority is:

1. preserve the Phase 2 baseline;
2. establish reliable Antigravity + Wokwi MCP tooling;
3. continue development incrementally;
4. integrate MAX30102 only after explicit Phase 3 approval;
5. resolve the DS3231/MPU6050 address conflict before RTC integration.

Do not allow Serial Monitor debugging to block development.

Do not jump directly to the complete system.

Build VitaLink subsystem by subsystem.