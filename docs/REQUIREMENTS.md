<!-- docs/REQUIREMENTS.md -->
# Requirements — Automotive-RKE-UDS-Simulation (System / Software / Interfaces)

**Author:** Fayez Alallaf  
**Doc ID:** REQ-RKE-001  
**Version:** 1.0  
**Last Updated:** 2026-02-11

This project is documented using a lightweight systems-engineering format:

- **SYS-**: system-level requirements
- **SW-**: software requirements
- **IF-**: interface requirements

Each requirement includes a **verification method** (Test / Analysis / Inspection / Demonstration) and maps to one or more test cases in `docs/TRACEABILITY.md`.

---

## Assumptions

- UDS **RoutineControl (0x31)** represents an “Unlock Door” routine in this bench demo.  
- Diagnostics are carried over CAN using **ISO-TP single-frame** formatting (payload ≤ 7 bytes).  
- Bench/HIL environment with two ESP32 nodes and physical CAN wiring (500 kbps).

References: UDS overview and services: https://www.csselectronics.com/pages/uds-protocol-tutorial-unified-diagnostic-services  
ISO-TP concept (single vs multi-frame): https://docs.kernel.org/networking/iso15765-2.html

---

## System Requirements (SYS)

| ID | Requirement | Rationale | Verification |
|---|---|---|---|
| SYS-RKE-001 | The system shall provide an **unlock action** triggered by a mobile device command (BLE). | Demonstrate Phone-as-a-Key flow | Test |
| SYS-RKE-002 | The system shall provide a **passive entry intent** using capacitive touch (handle touch) when BLE is connected. | Demonstrate PEPS-style intent gating | Test |
| SYS-RKE-003 | The gateway shall translate unlock intent into a **UDS RoutineControl request (0x31)** transmitted on CAN. | Demonstrate diagnostics-based command transport | Test |
| SYS-RKE-004 | The door ECU shall actuate the latch when it receives a valid unlock routine request. | Demonstrate endpoint actuation | Test |
| SYS-RKE-005 | The door ECU shall respond with a **positive UDS response (0x71)** to the gateway after successful routine execution. | Demonstrate request/response verification | Test |
| SYS-RKE-006 | The system shall operate on a high-speed CAN physical layer at **500 kbps** with proper termination. | Realistic bench environment | Inspection + Test |

---

## Software Requirements — Node A (Gateway / BCM)

| ID | Requirement | Verification |
|---|---|---|
| SW-GW-001 | Node A shall advertise a BLE GATT server named `Fayez_Car_Pro`. | Test |
| SW-GW-002 | Node A shall set `unlockRequest=true` when the BLE characteristic is written with value `'1'`. | Test |
| SW-GW-003 | Node A shall set `unlockRequest=true` when capacitive touch value is below the configured threshold (edge-detected/debounced). | Test |
| SW-GW-004 | When `unlockRequest=true`, Node A shall transmit an 8-byte CAN frame with ID `0x7E0` and payload `02 31 01 00 00 00 00 00`. | Test |
| SW-GW-005 | Node A shall display system state on OLED (DISCONNECTED/SCAN, LOCKED, UNLOCKING/SENT, OK/UNLOCKED). | Demonstration |

---

## Software Requirements — Node B (Door ECU / DCU)

| ID | Requirement | Verification |
|---|---|---|
| SW-DOOR-001 | Node B shall listen for CAN frames on ID `0x7E0` and parse bytes `[1]=0x31` and `[2]=0x01`. | Test |
| SW-DOOR-002 | On receiving a valid unlock request, Node B shall drive the servo to **~70–90°** for **~3 seconds** then return to **~0–20°** (bench-safe angles allowed). | Test |
| SW-DOOR-003 | After executing the routine, Node B shall transmit a positive response on CAN ID `0x7E8` with payload `02 71 01 00 00 00 00 00`. | Test |

---

## Interface Requirements (IF)

| ID | Requirement | Verification |
|---|---|---|
| IF-CAN-001 | CAN bus shall use **CANH/CANL** wiring and 120Ω termination at each end (≈60Ω total). | Inspection |
| IF-CAN-002 | Node A shall transmit on CAN ID `0x7E0` and Node B shall respond on CAN ID `0x7E8`. | Test |
| IF-BLE-001 | BLE client shall connect and write to the specified characteristic UUID to request unlock. | Test |
| IF-ACT-001 | Servo actuator shall be controlled via PWM on Node B **GPIO13** (configurable via `SERVO_PIN`). | Inspection + Test |
| IF-DISP-001 | OLED SSD1306 shall communicate over I2C on Node A (SDA=GPIO4, SCL=GPIO16). | Inspection + Demonstration |

References: ESP32 touch API (`touchRead`): https://docs.espressif.com/projects/arduino-esp32/en/latest/api/touch.html  
MCP2515 Arduino library: https://github.com/coryjfowler/MCP_CAN_lib
