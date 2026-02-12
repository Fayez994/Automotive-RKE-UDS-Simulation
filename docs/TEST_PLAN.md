<!-- docs/TEST_PLAN.md -->
# Test Plan — Automotive-RKE-UDS-Simulation (Bench / Vehicle-Style Integration)

**Owner:** Fayez Alallaf  
**Doc ID:** TP-RKE-001  
**Version:** 1.0  
**Last Updated:** 2026-02-11

---

## 1. Objective

Validate end-to-end behavior of the RKE / PEPS demo across:

- BLE connection + unlock trigger (App write + touch “handle”)
- CAN transport of UDS RoutineControl (ISO-TP single-frame)
- Door ECU actuation + positive UDS response
- Evidence collection (logs + traces + video)

---

## 2. Scope

### In Scope
- Node A sends UDS unlock request on **0x7E0**
- Node B responds on **0x7E8** and actuates servo latch
- Evidence capture is complete and reviewable by a third party

### Out of Scope
- UDS SecurityAccess (seed/key), crypto, anti-relay protections
- ISO-TP multi-frame segmentation/flow-control
- OEM session control (0x10), DTC services, extended addressing
- Environmental validation (EMC/temp/vibration)

---

## 3. Test Levels

- **Integration Test (Primary):** Node A ↔ CAN ↔ Node B
- **System Test:** user scenario (phone/touch → unlock)
- **Regression Check:** rerun core cases after code changes

---

## 4. Test Environment

### Hardware
- 2× ESP32 dev boards
- 2× MCP2515 CAN modules (SPI)
- SSD1306 OLED (Node A)
- Servo actuator (Node B)
- USB-to-CAN adapter for capture (SavvyCAN / python-can)

### Network / Bus
- CAN bitrate: **500 kbps**
- Correct termination (target ~60Ω total)

### Tools
- Serial Monitor @ 115200 baud
- SavvyCAN and/or python-can logger

---

## 5. Evidence & Storage

All required evidence is stored under:
- `assets/evidence/`

Required artifacts:
- `assets/evidence/hil.jpeg`
- `assets/evidence/canlog.md`
- `assets/evidence/serial_nodeA.txt`
- `assets/evidence/serial_nodeB.txt`
- Demo video link in `README.md` (YouTube)

Pass rule: a test is **PASS** only if expected behavior is observed **and** evidence is captured.

---

## 6. Entry / Exit Criteria

### Entry
- Correct firmware flashed to both nodes
- CAN wiring validated (CANH/CANL + common ground + termination)
- Servo powered reliably (recommended separate 5V + shared ground)
- BLE client app available

### Exit
- All **Core** test cases (TC-001..TC-006) PASS
- Evidence committed under `assets/evidence/`
- `docs/TRACEABILITY.md` 

---

## 7. Defect Handling

Defects are documented with:
- steps to reproduce
- expected vs observed behavior
- logs/traces
- environment details (adapter + bitrate + firmware version)

Template:
- `docs/ISSUE_TEMPLATE.md`

---

## 8. References
UDS services (RoutineControl / positive response): https://www.csselectronics.com/pages/uds-protocol-tutorial-unified-diagnostic-services  
ISO-TP concept: https://docs.kernel.org/networking/iso15765-2.html  
SavvyCAN: https://github.com/collin80/SavvyCAN  
python-can: https://python-can.readthedocs.io/  
MCP2515 library: https://github.com/coryjfowler/MCP_CAN_lib
