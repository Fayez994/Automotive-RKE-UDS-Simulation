# Automotive-RKE-UDS-Simulation

**Automotive Remote Keyless Entry (RKE) + Passive Entry (PEPS) demo using BLE (Phone-as-a-Key) → UDS over CAN → Door Actuator (HIL bench).**

This repository demonstrates a **2-ECU architecture**:
- **Node A (Gateway / BCM)**: BLE GATT server receives unlock intent (mobile app or capacitive touch), then sends a **UDS RoutineControl (0x31)** request over **High-Speed CAN**.
- **Node B (Door ECU / DCU)**: UDS server listens for routine ID **0x01 (Unlock)** and actuates a **servo latch**, then returns a **positive response (0x71)**.

> Scope: bench / hardware-in-the-loop demonstration of integration + validation workflows (logs, evidence, requirements, test cases, traceability).

---

## Architecture

![RKE & PEPS Architecture](assets/architecture.png)

**Topology:** Mobile Device ⇄(BLE GATT)⇄ Node A (Gateway ECU) ⇄(CAN HS 500 kbps)⇄ Node B (Door ECU) ⇄(PWM)⇄ Servo Latch

---

## Key Protocol Details

### BLE (Phone-as-a-Key)
- Node A advertises a BLE service and characteristic (GATT Server).
- Mobile client writes a value (`'1'`) to trigger unlock intent.

### UDS over CAN (DoCAN / ISO-TP single-frame)
**Request (Gateway → Door)**
- CAN ID: `0x7E0` (Functional Request)
- Data: `02 31 01 00 00 00 00 00`
  - `02` = PCI length (2 bytes payload)
  - `31` = UDS Service: RoutineControl
  - `01` = Routine ID: Unlock (project-defined)

**Response (Door → Gateway)**
- CAN ID: `0x7E8` (Positive Response)
- Data: `02 71 01 00 00 00 00 00`
  - `71` = Positive Response SID (0x31 + 0x40)
  - `01` = Routine ID echo

---

## Hardware

- 2× ESP32 DevKit
- 2× MCP2515 + TJA1050 CAN modules (8 MHz oscillator, 500 kbps)
- 2× 120Ω termination (end-to-end)
- Servo motor (door latch simulation)
- OLED SSD1306 (I2C) on Node A
- Capacitive touch wire on Node A (ESP32 touch pin)

---

## Repository Layout

```
assets/
  architecture.png
  evidence/                
docs/
  REQUIREMENTS.md
  TEST_PLAN.md
  TEST_CASES.md
  TRACEABILITY.md
  ISSUE_TEMPLATE.md
  TEST_REPORT_TEMPLATE.md
firmware/
  nodeA_gateway/nodeA_gateway.ino
  nodeB_door/nodeB_door.ino
```

---

## Quick Start (Bench)

1. Wire CAN:
   - MCP2515/TJA1050 to each ESP32 (SPI + INT as needed)
   - CANH↔CANH, CANL↔CANL
   - Add 120Ω termination at each end
2. Flash firmware:
   - Node A: `firmware/nodeA_gateway/nodeA_gateway.ino`
   - Node B: `firmware/nodeB_door/nodeB_door.ino`
3. Open Serial Monitor (115200) for both nodes.
4. Connect phone to BLE device: `Fayez_Car_Pro`.
5. Trigger unlock:
   - Option A: write `'1'` to characteristic
   - Option B: touch capacitive wire (PEPS intent)
6. Observe evidence:
   - Node A OLED: `UNLOCKING...` → `UNLOCKED!`
   - Node B servo: moves to 90° then returns to 0°
   - Serial logs show request + positive response

---

## Systems Engineering Artifacts (Requirements + Verification)

- Requirements: **docs/REQUIREMENTS.md**
- Test Plan: **docs/TEST_PLAN.md**
- Test Cases: **docs/TEST_CASES.md**
- Traceability Matrix (Req ↔ Test): **docs/TRACEABILITY.md**
- Issue Report template: **docs/ISSUE_TEMPLATE.md**

---

## Evidence (what to upload)

Put artifacts in **assets/evidence/**:
- `video_demo.mp4` 
- `serial_gateway.log`, `serial_door.log`
- CAN trace screenshot (logic analyzer / CAN tool)

See **docs/TEST_CASES.md** for exact evidence mapping per test case.

---

## Notes / Limitations

- This demo uses **single-frame ISO-TP** formatting (8-byte CAN frame with PCI length).
- Security is intentionally simplified for demonstration (production systems require cryptography, challenge-response, and OEM-specific security concepts).
