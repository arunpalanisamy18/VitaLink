# VitaLink — Complete A→Z Developer & Agent Setup

**SIH 2026 — SIH26181**  
**Project:** Disaster-Resilient Wearable Health Monitoring and Early Warning System

This README is the operational setup guide for every teammate and AI development agent working on VitaLink.

It is intentionally detailed. A new developer/agent should be able to clone the repository, prepare the environment, build the firmware, run the Wokwi simulation, configure Wokwi CLI/MCP, understand the current Phase 2 baseline, diagnose common failures, and continue development without asking the original developer to recreate the environment.

---

# 0. Golden Rules

Before doing anything else:

1. **VitaLink's GitHub repository is the project source of truth.**
2. Work only inside the cloned `VitaLink` directory.
3. Never initialize Git in a parent directory such as `C:\Users\<user>`.
4. Never commit API keys, Wokwi tokens, passwords, certificates, or other secrets.
5. Each developer uses their **own Wokwi account/token**.
6. Do not copy another developer's Wokwi token.
7. Do not let two AI agents edit the same VitaLink working tree simultaneously.
8. Do not rewrite the existing Phase 2 architecture merely because a tool reports a missing feature.
9. Read the project context before making architectural changes.
10. Build and verify after every meaningful firmware change.
11. Use feature branches. Protect `main`.
12. Treat the prototype as a research/safety-support system, **not a medical diagnostic device**.

---

# 1. Repository

GitHub:

```text
https://github.com/arunpalanisamy18/VitaLink.git
```

Clone:

```powershell
cd C:\Users\<YOUR_USERNAME>\Documents
git clone https://github.com/arunpalanisamy18/VitaLink.git
cd VitaLink
```

Verify:

```powershell
git remote -v
git rev-parse --show-toplevel
git branch --show-current
```

The repository root must be the `VitaLink` directory.

Expected:

```text
C:/Users/<YOUR_USERNAME>/Documents/VitaLink
```

---

# 2. Repository Structure

Current important structure:

```text
VitaLink/
│
├── .agents/
│   └── rules/
│       └── vitalink.md
│
├── .vscode/
│   └── mcp.json
│
├── docs/
│   ├── HANDOFF.md
│   └── VITALINK_CONTEXT.md
│
├── src/
│   └── main.cpp
│
├── diagram.json
├── platformio.ini
├── wokwi.toml
├── wokwi-api.h
├── max30102.chip.wasm
├── .gitignore
└── README.md
```

The repository may gain additional directories as custom Wokwi chips and physical-prototype firmware are added.

---

# 3. Read These Files First

Before editing anything, read these in order:

```text
docs/VITALINK_CONTEXT.md
docs/HANDOFF.md
.agents/rules/vitalink.md
```

## `VITALINK_CONTEXT.md`

This is the long-form A→Z project context.

It records:

- project identity
- architecture
- hardware inventory
- simulation architecture
- firmware
- I2C configuration
- motion/fall detection
- OLED behavior
- MAX30102 state
- future DS3231/GPS/SIM800L integration
- Wokwi configuration
- MCP workflow
- known risks
- verification history
- phase decisions
- current next action

## `HANDOFF.md`

This is the shorter operational handoff.

Use it to understand:

- current frozen phase
- current blockers
- current hardware
- immediate next task
- important decisions

## `.agents/rules/vitalink.md`

This contains agent-specific rules.

AI agents must read it before modifying the project.

---

# 4. Current Phase — Phase 2 Frozen

The current baseline is:

```text
VitaLink Phase 2 — FROZEN
```

Implemented:

```text
ESP32-S3
    │
    ├── MPU6050
    │
    └── SSD1306 OLED
```

Current functionality:

- ESP32-S3 firmware
- I2C communication
- MPU6050 motion sensing
- motion classification
- multi-stage fall detection
- OLED status display
- Wokwi simulation
- PlatformIO build
- project documentation

Motion states:

```text
RESTING
MOVING
HIGH ACTIVITY
FALL DETECTED
```

Fall state machine:

```text
NORMAL
   ↓
POSSIBLE_FREE_FALL
   ↓
IMPACT_DETECTED
   ↓
POST_IMPACT_CHECK
   ↓
FALL_CONFIRMED
   ↓
NORMAL
```

Do not replace this with a simplistic single-threshold fall detector without first reviewing the project context and discussing the architectural change.

---

# 5. Physical Hardware Target

The intended physical prototype currently includes:

```text
1. ESP32-S3
2. MAX30102
3. MPU6050
4. DS3231
5. GPS/GNSS module
6. OLED
7. SIM800L GSM
```

Current Phase 2 simulation only uses:

```text
ESP32-S3
MPU6050
SSD1306 OLED
```

The other sensors are introduced incrementally.

---

# 6. Development Environment

## Required

Install:

### Git

Verify:

```powershell
git --version
```

### Visual Studio Code

Install VS Code and open the `VitaLink` folder.

### PlatformIO

Install the **PlatformIO IDE** extension in VS Code.

Verify PlatformIO from the terminal:

```powershell
python -m platformio --version
```

If that command is unavailable, use PlatformIO's integrated VS Code environment or install PlatformIO according to the current PlatformIO documentation.

### Wokwi for VS Code

Install:

```text
Wokwi for VS Code
```

This is the preferred local simulation workflow.

---

# 7. Python Environment

Python is useful for PlatformIO and project tooling.

Verify:

```powershell
python --version
```

If the repository uses a virtual environment, activate it:

```powershell
.\.venv\Scripts\Activate.ps1
```

If PowerShell blocks activation because of execution policy, do not permanently weaken the machine's security policy without understanding the consequence.

A temporary process-level option may be used when appropriate:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
```

Then:

```powershell
.\.venv\Scripts\Activate.ps1
```

Do not commit `.venv/`.

---

# 8. PlatformIO Project

The project is configured by:

```text
platformio.ini
```

Main firmware:

```text
src/main.cpp
```

Build:

```powershell
python -m platformio run
```

Expected successful result:

```text
========================= [SUCCESS] =========================
```

If PlatformIO reports missing libraries, do not manually copy random libraries into the repository.

First inspect:

```text
platformio.ini
```

Then allow PlatformIO to resolve the dependencies specified by the project.

---

# 9. Firmware Libraries

The exact dependencies are controlled by `platformio.ini`.

Do not assume that libraries available in the web version of Wokwi are automatically available locally.

If a library is missing:

1. Open `platformio.ini`.
2. Identify the declared dependency.
3. Run:

```powershell
python -m platformio run
```

4. Read the actual error.
5. Fix the dependency in the project configuration if necessary.
6. Rebuild.

Never silently replace the intended sensor library with a different library merely to make compilation succeed.

---

# 10. Wokwi Local Simulation

VitaLink uses:

```text
diagram.json
wokwi.toml
```

These files describe the simulation.

The Wokwi VS Code extension and Wokwi CLI use the same project configuration.

## Before simulation

Always build first:

```powershell
python -m platformio run
```

Then open:

```text
diagram.json
```

and start the Wokwi simulator.

---

# 11. Current I2C Wiring

Current Phase 2 I2C:

```text
ESP32-S3 GPIO 8 → SDA
ESP32-S3 GPIO 9 → SCL
```

Devices:

```text
SSD1306 OLED → 0x3C
MPU6050      → 0x68
```

Do not casually change GPIO 8/9.

If a future sensor shares I2C, confirm its address and electrical compatibility before adding it.

---

# 12. Current OLED Role

The OLED is the **primary simulation observability interface**.

Current simulation philosophy:

```text
Sensor
  ↓
ESP32-S3
  ↓
Processing
  ↓
OLED
```

Serial output is useful for diagnostics, but do not block the entire project waiting for a perfect Serial Monitor integration.

The current OLED should remain part of the simulation.

---

# 13. Wokwi VS Code License

Wokwi for VS Code requires the appropriate Wokwi license/account activation for local use.

Typical setup:

1. Install the Wokwi for VS Code extension.
2. Open the Command Palette.
3. Run:

```text
Wokwi: Request a new License
```

4. Sign in to the developer's Wokwi account.
5. Complete the browser/VS Code authorization.
6. Restart the simulator if required.

If a teammate can view but not edit the visual diagram, check their Wokwi plan/license. The diagram can still be edited as JSON text.

---

# 14. Wokwi CLI

The CLI is useful for:

- command-line simulation
- CI
- automated tests
- serial logging
- screenshots
- VCD export
- linting
- MCP integration

It is not required for a teammate who only wants to build and run the local VS Code simulation.

Check installation:

```powershell
wokwi-cli --version
```

If not installed, install the current Wokwi CLI using Wokwi's official installation method.

On Windows, Wokwi documents:

```powershell
iwr https://wokwi.com/ci/install.ps1 -useb | iex
```

After installation, restart the terminal and verify:

```powershell
wokwi-cli --version
```

---

# 15. Wokwi CLI Authentication

Wokwi CLI/MCP uses a Wokwi CLI token.

Environment variable:

```text
WOKWI_CLI_TOKEN
```

A valid token begins with:

```text
wok_
```

## IMPORTANT

Never put the token in:

```text
README.md
diagram.json
wokwi.toml
.vscode/mcp.json
source code
Git history
GitHub Issues
chat messages
screenshots
```

Each developer should configure their **own** token.

Do not copy the original developer's token.

---

# 16. Configure WOKWI_CLI_TOKEN on Windows

For the current PowerShell session only:

```powershell
$env:WOKWI_CLI_TOKEN="wok_REPLACE_WITH_YOUR_TOKEN"
```

For a persistent user-level environment variable:

```powershell
[Environment]::SetEnvironmentVariable(
    "WOKWI_CLI_TOKEN",
    "wok_REPLACE_WITH_YOUR_TOKEN",
    "User"
)
```

Then close and reopen:

- PowerShell
- VS Code
- Antigravity

Verify that the environment variable exists without printing the secret:

```powershell
if ($env:WOKWI_CLI_TOKEN) {
    "WOKWI_CLI_TOKEN is configured"
} else {
    "WOKWI_CLI_TOKEN is NOT configured"
}
```

Do **not** run:

```powershell
echo $env:WOKWI_CLI_TOKEN
```

because that exposes the token.

---

# 17. Testing Wokwi CLI Authentication

From the VitaLink root:

```powershell
wokwi-cli --version
```

Then, if CLI simulation is intentionally being used:

```powershell
wokwi-cli .
```

For controlled runs:

```powershell
wokwi-cli . --timeout 10000
```

For serial logging:

```powershell
wokwi-cli . --serial-log-file wokwi-serial.log
```

Do not commit generated logs unless the team explicitly wants them.

---

# 18. Wokwi Lint

Before debugging a complex simulation, validate the diagram:

```powershell
wokwi-cli lint
```

This can catch:

- unknown components
- invalid connections
- missing components
- duplicate IDs
- diagram errors

Fix diagram errors before blaming the firmware.

---

# 19. Wokwi `wokwi.toml`

`wokwi.toml` tells Wokwi which firmware to load.

Typical concept:

```toml
[wokwi]
version = 1
firmware = "..."
elf = "..."
```

Use the repository's existing `wokwi.toml` as the source of truth.

Do not replace it with a generic example unless the current configuration is genuinely broken.

Use forward slashes in paths:

```text
build/firmware.elf
```

rather than Windows backslashes:

```text
build\firmware.elf
```

---

# 20. Wokwi Custom Chips

VitaLink may use custom Wokwi chips because some intended hardware is not represented by a native Wokwi component.

Current repository artifact:

```text
max30102.chip.wasm
```

Important:

A `.wasm` file alone does not automatically mean that the complete custom-chip integration is ready.

Wokwi custom-chip configurations normally require:

- WASM binary
- matching chip definition JSON
- `wokwi.toml` registration
- `diagram.json` component entry
- firmware-side driver integration

Therefore, before claiming a custom sensor is integrated, verify the complete path:

```text
Wokwi control/model
        ↓
custom chip
        ↓
I2C/UART/GPIO
        ↓
ESP32-S3
        ↓
firmware library/driver
        ↓
application logic
        ↓
OLED/log
```

---

# 21. MAX30102 Status

Intended physical sensor:

```text
MAX30102
```

Purpose:

```text
Heart Rate
SpO2
```

The repository contains a custom MAX30102 WASM artifact.

However, do not assume that the complete firmware integration is already part of Phase 2.

The next integration phase should explicitly verify:

```text
MAX30102 custom chip
        ↓
I2C 0x57
        ↓
ESP32-S3
        ↓
MAX3010x library
        ↓
HR / SpO2 processing
        ↓
OLED
```

Do not inject fake HR/SpO2 values directly into application logic merely to make the OLED display numbers.

---

# 22. Wokwi MCP — What It Does

The Wokwi MCP server allows an AI agent to interact with the embedded simulation.

It can provide workflows such as:

- starting/stopping simulations
- interacting with simulated hardware
- observing serial output
- running tests
- automating simulation tasks

The Wokwi MCP server is provided through:

```text
wokwi-cli mcp
```

---

# 23. VS Code MCP Integration

The repository currently contains:

```text
.vscode/mcp.json
```

Current basic configuration:

```json
{
  "servers": {
    "wokwi": {
      "type": "stdio",
      "command": "wokwi-cli",
      "args": ["mcp"]
    }
  },
  "inputs": []
}
```

This intentionally does **not** contain a Wokwi token.

If the environment already provides `WOKWI_CLI_TOKEN`, the Wokwi MCP process can inherit it.

For a more explicit token-input setup, Wokwi documents a configuration pattern using an input variable:

```json
{
  "servers": {
    "Wokwi": {
      "type": "stdio",
      "command": "wokwi-cli",
      "args": ["mcp"],
      "env": {
        "WOKWI_CLI_TOKEN": "${input:wokwi-cli-token}"
      }
    }
  }
}
```

Do not commit an actual token into this file.

---

# 24. VS Code MCP Setup Checklist

After cloning:

1. Install VS Code.
2. Install Wokwi extension.
3. Install Wokwi CLI.
4. Configure your own `WOKWI_CLI_TOKEN`.
5. Restart VS Code.
6. Open the VitaLink workspace.
7. Open:

```text
.vscode/mcp.json
```

8. Check that the Wokwi server points to:

```text
wokwi-cli mcp
```

9. Use VS Code's MCP tools/server UI to inspect the configured Wokwi server.
10. If VS Code asks whether you trust the MCP server, review the configuration and approve it only if it matches the expected local Wokwi command.

---

# 25. VS Code MCP Troubleshooting

## Error: `wokwi-cli` not found

Check:

```powershell
wokwi-cli --version
```

If it fails:

- install Wokwi CLI
- ensure its installation directory is in PATH
- restart VS Code

---

## Error: authorization/token error

Check:

```powershell
if ($env:WOKWI_CLI_TOKEN) {
    "Token variable exists"
} else {
    "Token variable missing"
}
```

Then verify that the token is valid.

Do not print the token.

If the token was exposed publicly, revoke/regenerate it.

---

## MCP server starts but has no useful simulation

Check:

```text
VitaLink/
├── diagram.json
├── wokwi.toml
└── built firmware
```

Build first:

```powershell
python -m platformio run
```

Then validate:

```powershell
wokwi-cli lint
```

Then run the simulation independently:

```powershell
wokwi-cli .
```

Only after the CLI works should you debug the MCP layer.

---

## MCP server works but agent cannot see serial output

Check:

1. firmware actually builds
2. simulation actually starts
3. correct firmware is referenced by `wokwi.toml`
4. simulator is not paused
5. serial configuration is correct
6. the agent is connected to the correct MCP server

Do not immediately rewrite firmware.

---

# 26. Antigravity Integration

Antigravity is the current primary AI development agent for VitaLink.

The project must follow:

```text
one agent
one workspace
one source of truth
```

Do not run Copilot Agent and Antigravity simultaneously against the same working tree.

Antigravity may use its own MCP configuration rather than relying exclusively on:

```text
.vscode/mcp.json
```

If Antigravity does not automatically discover the VS Code MCP configuration, configure Wokwi MCP in the Antigravity workspace/global MCP configuration according to the installed Antigravity version.

The important server command remains:

```text
wokwi-cli mcp
```

and the token remains an environment/secret configuration, not source code.

---

# 27. Antigravity Agent Startup Protocol

When an AI agent starts work on VitaLink:

### Step 1

Read:

```text
docs/VITALINK_CONTEXT.md
```

### Step 2

Read:

```text
docs/HANDOFF.md
```

### Step 3

Read:

```text
.agents/rules/vitalink.md
```

### Step 4

Inspect:

```text
git status
git branch --show-current
```

### Step 5

Inspect the relevant source/configuration.

### Step 6

Identify:

- current phase
- current blocker
- intended next action

### Step 7

Make the smallest safe change.

### Step 8

Build:

```powershell
python -m platformio run
```

### Step 9

If simulation is relevant, run the appropriate Wokwi verification.

### Step 10

Report:

- files changed
- why
- build result
- simulation result
- remaining limitations

---

# 28. Agent Must Not Do These Things

An AI agent must not:

- delete the project architecture without evidence
- replace physical hardware because Wokwi lacks a native part
- fake sensor readings directly in `main.cpp`
- silently change I2C pins
- silently change sensor addresses
- remove the OLED because Serial Monitor is easier
- expose Wokwi credentials
- commit secrets
- rewrite the entire repository for a small bug
- run destructive Git commands without explicit authorization
- force-push `main`
- reset or discard another developer's work
- claim a simulation passed without actually running/observing it
- claim a sensor is integrated when only a stub/custom chip exists
- claim medical accuracy from simulated or prototype values

---

# 29. Git Branching

Never develop directly on `main`.

Create a branch:

```powershell
git switch -c feature/<feature-name>
```

Examples:

```powershell
git switch -c feature/max30102-integration
git switch -c feature/ds3231
git switch -c feature/gps
git switch -c feature/gsm-sos
```

Check:

```powershell
git branch --show-current
```

---

# 30. Git Commit Workflow

Before changing:

```powershell
git status
```

After changing:

```powershell
git diff
```

Build:

```powershell
python -m platformio run
```

Stage:

```powershell
git add .
```

Review:

```powershell
git diff --cached --stat
git diff --cached --name-only
git diff --cached --check
```

Commit:

```powershell
git commit -m "Describe the actual change"
```

Push:

```powershell
git push -u origin feature/<feature-name>
```

Create a Pull Request.

---

# 31. Before Merging to Main

Check:

```powershell
git status
python -m platformio run
```

If Wokwi is affected:

```powershell
wokwi-cli lint
```

Run the appropriate simulation verification.

Review the diff.

Then merge through the team's normal Pull Request process.

---

# 32. Common Error: `.pio` Appears in Git

`.pio/` is generated PlatformIO build data.

It should be ignored.

Check:

```powershell
git status --short
```

If `.pio/` appears:

```text
?? .pio/
```

check `.gitignore`.

Do not commit `.pio/`.

---

# 33. Common Error: Repository Root Is Wrong

If:

```powershell
git rev-parse --show-toplevel
```

returns:

```text
C:/Users/<username>
```

instead of:

```text
C:/Users/<username>/Documents/VitaLink
```

STOP.

Do not run:

```powershell
git add .
```

The parent repository may contain unrelated personal files.

Correct the repository structure before proceeding.

---

# 34. Common Error: PlatformIO Build Fails

First run:

```powershell
python -m platformio run
```

Read the first meaningful compiler error.

Then check:

```text
platformio.ini
src/main.cpp
```

Typical causes:

- missing library
- incorrect board
- syntax error
- wrong include
- incompatible library version
- stale PlatformIO environment

Do not respond to a compiler error by replacing the complete firmware.

---

# 35. Common Error: Wokwi Simulation Does Not Start

Check:

```powershell
python -m platformio run
```

Then:

```powershell
wokwi-cli lint
```

Then inspect:

```text
wokwi.toml
diagram.json
```

Common causes:

- firmware file path is wrong
- firmware was not built
- invalid diagram JSON
- unknown part
- invalid connection
- missing custom-chip registration
- Wokwi license/authentication issue

---

# 36. Common Error: Wokwi CLI Works but VS Code Does Not

Check:

```powershell
wokwi-cli --version
```

Then verify VS Code can find the same executable.

On Windows, use:

```powershell
Get-Command wokwi-cli
```

If PowerShell finds it but VS Code does not:

1. close VS Code
2. restart VS Code
3. reopen the VitaLink workspace
4. retry

---

# 37. Common Error: Wokwi MCP Does Not Start

Debug in this order:

```text
1. Is wokwi-cli installed?
2. Does wokwi-cli --version work?
3. Is WOKWI_CLI_TOKEN configured?
4. Does normal Wokwi CLI simulation work?
5. Does wokwi-cli lint work?
6. Does mcp.json contain the expected command?
7. Does VS Code/Antigravity trust the MCP server?
8. Is the correct workspace open?
```

Do not debug all layers simultaneously.

---

# 38. Common Error: Serial Monitor Shows Nothing

Remember:

```text
Serial output ≠ proof that firmware is not running.
```

Check:

- simulation is running
- simulator tab is visible
- firmware was rebuilt
- serial configuration
- USB serial interface configuration
- baud rate
- firmware actually reaches `Serial.begin(...)`

For the current project, OLED is the primary simulation observability channel.

---

# 39. Common Error: Custom Wokwi Chip Does Not Work

Verify the complete chain:

```text
wokwi.toml
   ↓
chip registration
   ↓
chip JSON
   ↓
chip WASM
   ↓
diagram.json
   ↓
pin wiring
   ↓
firmware driver
   ↓
sensor address
```

For I2C devices verify:

```text
SDA
SCL
VCC
GND
I2C address
```

For UART devices verify:

```text
TX → RX
RX → TX
GND
baud rate
```

Do not assume that a compiled WASM file alone proves integration.

---

# 40. Common Error: I2C Sensor Not Detected

Check:

```text
GPIO 8 = SDA
GPIO 9 = SCL
```

Then verify the sensor address.

Known Phase 2 addresses:

```text
OLED    0x3C
MPU6050 0x68
```

MAX30102 intended address:

```text
0x57
```

If adding another I2C sensor, verify that its address does not conflict.

---

# 41. Common Error: AI Agent Makes Large Unwanted Changes

Stop the agent.

Run:

```powershell
git status
git diff --stat
git diff
```

If the changes are unwanted and are not yours:

```text
DO NOT blindly run reset/clean commands.
```

First preserve or review the work.

The correct response is to inspect the diff and determine which changes belong to the current task.

---

# 42. Common Error: Agent Says "Simulation Passed" Without Evidence

Require evidence.

A valid verification should identify:

- command/tool used
- simulation actually started
- relevant output/behavior
- what was tested
- what remains untested

For example:

```text
Build: PASS
Wokwi lint: PASS
Simulation: RUN
Fall test: OBSERVED
MAX30102 clinical accuracy: NOT VALIDATED
```

Do not equate compilation with hardware verification.

---

# 43. Current Phase 2 Verification

Historically verified:

```text
RESTING        → PASS
NORMAL MOVING  → PASS
HIGH ACTIVITY  → PASS
FALL           → PASS
RECOVERY       → PASS
```

Fall detection uses a multi-stage state machine rather than one acceleration threshold.

The current threshold and state-machine details are maintained in:

```text
src/main.cpp
docs/VITALINK_CONTEXT.md
```

Those files are authoritative for the current implementation.

---

# 44. Simulation vs Physical Prototype

Keep these concepts separate.

## Simulation

Optimized for:

- observability
- repeatability
- measurable sensor behavior
- debugging
- firmware validation

## Physical prototype

Must account for:

- real sensor noise
- calibration
- wiring
- power
- battery
- GSM current bursts
- antenna
- enclosure
- sensor placement
- thermal effects
- real-world communication failures

Passing Wokwi simulation does not prove physical hardware reliability.

---

# 45. Planned Hardware Integration Order

Recommended incremental order:

```text
Phase 2
ESP32-S3 + MPU6050 + OLED
        ↓
MAX30102
        ↓
DS3231
        ↓
GPS/GNSS
        ↓
SIM800L
        ↓
risk engine
        ↓
emergency workflow
        ↓
physical prototype
```

Integrate and verify one major subsystem at a time.

---

# 46. Emergency Communication Architecture

Intended high-level flow:

```text
Sensors
   ↓
ESP32-S3
   ↓
Local analysis
   ↓
Local alert
   ↓
MicroSD / local record
   ↓
GNSS location
   ↓
GSM/SMS
```

Manual SOS should remain possible.

Offline/local functionality must not depend on cloud connectivity.

---

# 47. Privacy Architecture

The intended companion architecture prioritizes local processing.

General direction:

```text
Raw physiological signals
        ↓
Wearable
        ↓
Local processing
        ↓
Processed parameters / alerts / risk
        ↓
BLE
        ↓
Companion application
```

Do not upload raw physiological data merely because cloud connectivity is available.

---

# 48. Medical-Safety Rule

VitaLink is a prototype/research safety-support system.

It is not currently a certified medical diagnostic device.

Do not describe prototype outputs as:

```text
medical diagnosis
clinical diagnosis
clinically validated SpO2
clinically validated hydration
clinically validated stress
medical-grade prediction
```

unless the relevant validation has actually been completed and documented.

---

# 49. Agent Task Template

When asking an AI agent to implement a feature, use this structure:

```text
You are working on VitaLink.

First read:
- docs/VITALINK_CONTEXT.md
- docs/HANDOFF.md
- .agents/rules/vitalink.md

Then:
1. Inspect git status.
2. Inspect the relevant existing source/configuration.
3. Identify the current phase and constraints.
4. Do not modify unrelated files.
5. Implement the smallest safe change.
6. Build with:
   python -m platformio run
7. Run relevant Wokwi verification if applicable.
8. Report:
   - files changed
   - implementation summary
   - build result
   - simulation result
   - known limitations
9. Do not commit or push unless explicitly requested.
```

---

# 50. Agent Task: Sensor Integration Template

For a new sensor:

```text
First inspect the current VitaLink architecture.

Do not fake sensor values in application code.

Implement the complete path:

hardware model
→ Wokwi/custom chip if required
→ wiring/configuration
→ driver/library
→ ESP32-S3 read path
→ signal validation
→ feature extraction
→ application state
→ OLED/log output
→ verification
→ documentation

Preserve existing Phase 2 behavior.
```

---

# 51. Environment Health Check

A new teammate can run this checklist:

```powershell
git --version
python --version
python -m platformio --version
wokwi-cli --version
Get-Command wokwi-cli
git remote -v
git rev-parse --show-toplevel
git status
```

Then:

```powershell
python -m platformio run
```

Then:

```powershell
wokwi-cli lint
```

Then start the local Wokwi simulation.

---

# 52. Expected Healthy Environment

A healthy setup should look conceptually like:

```text
Git
 │
 └── VitaLink repository
      │
      ├── VS Code
      │    ├── PlatformIO
      │    ├── Wokwi extension
      │    └── MCP
      │
      ├── PlatformIO
      │    └── ESP32-S3 firmware
      │
      ├── Wokwi
      │    ├── diagram.json
      │    ├── wokwi.toml
      │    └── custom chips
      │
      └── AI Agent
           └── Wokwi MCP
```

---

# 53. What Requires Internet

Normal PlatformIO builds may need to download dependencies the first time.

Wokwi for VS Code normally requires network connectivity.

Wokwi CLI cloud simulation requires authentication.

MCP authentication requires a valid Wokwi CLI token.

Local source editing does not require the Internet.

Do not assume that an offline machine can run the complete Wokwi workflow.

---

# 54. Secret Handling

Never commit:

```text
WOKWI_CLI_TOKEN
API keys
Firebase service accounts
private keys
SSH private keys
passwords
OAuth tokens
cloud credentials
```

Use environment variables or the relevant secret-management mechanism.

Before committing:

```powershell
git status --short
git diff --cached --name-only
git diff --cached --check
```

If a credential was accidentally committed:

1. Stop.
2. Revoke/rotate the credential immediately.
3. Remove it from the working tree.
4. Assess whether it entered Git history.
5. Rewrite history if necessary.
6. Force-push only through an agreed team recovery process.

Simply deleting the secret from the latest file does not necessarily remove it from Git history.

---

# 55. Do Not Put Tokens in MCP JSON

Bad:

```json
{
  "env": {
    "WOKWI_CLI_TOKEN": "wok_REAL_SECRET"
  }
}
```

Good:

```json
{
  "env": {
    "WOKWI_CLI_TOKEN": "${input:wokwi-cli-token}"
  }
}
```

or use an environment variable already configured in the developer's environment.

The repository should contain configuration, not personal credentials.

---

# 56. Reproducibility Principle

Every teammate should be able to reach:

```text
clone
 ↓
install tools
 ↓
install dependencies
 ↓
build
 ↓
lint
 ↓
simulate
 ↓
inspect OLED
 ↓
create feature branch
 ↓
develop
```

without depending on undocumented settings from the original developer's computer.

If a setup requires a machine-specific manual step, document it in:

```text
docs/VITALINK_CONTEXT.md
```

or this README.

---

# 57. When to Update Documentation

Update documentation when the project state genuinely changes.

Examples:

- new sensor integrated
- new custom chip added
- I2C pin changed
- new Wokwi architecture
- fall detector changed
- new build dependency
- new MCP workflow
- major verification result
- new blocker
- phase transition

Do not rewrite documentation on every tiny code change.

---

# 58. Definition of "Integrated"

A component is considered **integrated** only when the relevant path works.

For example:

```text
MAX30102 integrated
```

should mean more than:

```text
MAX30102 file exists
```

It should mean, as applicable:

```text
Wokwi model
→ bus
→ ESP32-S3
→ driver
→ data
→ processing
→ output
→ verification
```

Document partial states explicitly.

---

# 59. Definition of "Done"

A feature is not considered done merely because the code compiles.

A good completion record is:

```text
Implementation: PASS
Build: PASS
Lint: PASS
Simulation: PASS
Relevant behavior: OBSERVED
Documentation: UPDATED
Known limitations: DOCUMENTED
```

Physical hardware requires an additional physical verification stage.

---

# 60. Quick Start for Human Developer

```powershell
git clone https://github.com/arunpalanisamy18/VitaLink.git
cd VitaLink
code .
```

Read:

```text
docs/VITALINK_CONTEXT.md
docs/HANDOFF.md
.agents/rules/vitalink.md
```

Install:

```text
VS Code
PlatformIO
Wokwi for VS Code
Wokwi CLI
```

Configure your own Wokwi token if CLI/MCP is required.

Build:

```powershell
python -m platformio run
```

Lint:

```powershell
wokwi-cli lint
```

Run Wokwi locally.

Create a branch:

```powershell
git switch -c feature/my-feature
```

Develop.

Build/test.

Commit.

Push.

Create Pull Request.

---

# 61. Quick Start for AI Agent

```text
OPEN WORKSPACE: VitaLink

READ:
1. docs/VITALINK_CONTEXT.md
2. docs/HANDOFF.md
3. .agents/rules/vitalink.md

VERIFY:
- git root
- git status
- current branch
- platformio configuration
- Wokwi configuration
- relevant source

BUILD:
python -m platformio run

SIMULATION:
Use Wokwi only when relevant.
Do not claim simulation success without evidence.

EDIT:
Make the smallest safe change.

VERIFY:
Build again.
Run relevant simulation/lint/test.

REPORT:
- changed files
- why
- build result
- simulation result
- limitations

DO NOT:
- expose secrets
- change unrelated files
- destroy existing work
- rewrite architecture unnecessarily
- commit/push unless explicitly instructed
```

---

# 62. Final Architecture Mindset

VitaLink is being developed as an incremental embedded system:

```text
              ┌──────────────────┐
              │   ESP32-S3       │
              │ Edge Intelligence│
              └────────┬─────────┘
                       │
       ┌───────────────┼────────────────┐
       │               │                │
       ▼               ▼                ▼
   Physiological     Motion         Environment
     Sensors         Sensors          Sensors
       │               │                │
       └───────────────┼────────────────┘
                       ▼
               Signal Validation
                       ↓
              Feature Extraction
                       ↓
              Personal Baseline
                       ↓
               Anomaly Detection
                       ↓
             Compound Risk Index
                       ↓
          ┌────────────┴────────────┐
          ▼                         ▼
     Local Alert              Emergency Flow
     OLED/Buzzer              GNSS + GSM/SMS
          │                         │
          └────────────┬────────────┘
                       ▼
                 Local Logging
```

The system should remain:

- local-first
- privacy-conscious
- incrementally testable
- simulation-friendly
- hardware-realistic
- resilient to connectivity loss
- explicit about uncertainty
- safe in its claims

---

# 63. Official Documentation References

For the current Wokwi/VS Code workflow, consult the official documentation when tool behavior changes:

- Wokwi CLI
- Wokwi CLI installation
- Wokwi for VS Code
- Wokwi project configuration
- Wokwi MCP support
- VS Code MCP configuration

The repository's project-specific files remain the source of truth for VitaLink-specific configuration.

---

# 64. Current Next Development Direction

The Phase 2 baseline is frozen.

Next major subsystem:

```text
MAX30102
```

Before implementing it, the agent must:

1. read project context
2. inspect the current repository
3. verify the custom-chip state
4. verify Wokwi configuration
5. verify PlatformIO dependencies
6. preserve the existing MPU6050 + OLED behavior
7. implement incrementally
8. build
9. simulate
10. verify
11. document the resulting state

**Do not skip directly from Phase 2 to the complete final wearable.**

Build the system one verified subsystem at a time.
