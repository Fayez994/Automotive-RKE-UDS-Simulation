<!-- docs/TEST_CASES.md -->
# Test Cases — Automotive-RKE-UDS-Simulation (Execution + Evidence)

**Owner:** Fayez Alallaf  
**Doc ID:** TC-RKE-001  
**Version:** 1.0  
**Last Updated:** 2026-02-11

Conventions:
- **TC-xxx** = Test Case ID
- Evidence must be saved under `assets/evidence/`

Required evidence filenames:
- `assets/evidence/serial_nodeA.txt`
- `assets/evidence/serial_nodeB.txt`
- `assets/evidence/canlog.md`
- `assets/evidence/hil.jpeg`
- Demo video link in `README.md`

---

## Core Tests

| TC ID | Title | Preconditions | Steps | Expected Result | Evidence |
|---|---|---|---|---|---|
| TC-001 | BLE Advertising + Connect | Node A flashed + powered | Scan BLE; connect to `Fayez_Car_Pro` | BLE connects; OLED indicates CONNECTED/READY | `serial_nodeA.txt`, optional OLED photo |
| TC-002 | Manual Unlock via BLE Write | BLE connected | Write `'1'` to characteristic | Node A transmits CAN `0x7E0` with `02 31 01 ...` and shows UNLOCKING/SENT | `serial_nodeA.txt`, demo video |
| TC-003 | Door ECU Actuation | Node B flashed; servo powered | Trigger unlock (TC-002) | Node B receives request and executes servo cycle (unlock → hold → lock) | `serial_nodeB.txt`, demo video |
| TC-004 | Positive UDS Response | CAN bus up | Trigger unlock (TC-002) | Node B transmits `0x7E8` with `02 71 01 ...` and Node A receives it | `canlog.md` (preferred) or capture screenshot |
| TC-005 | PEPS Touch Unlock | BLE connected; touch wire installed | Touch capacitive wire | Unlock routine triggers without BLE write | `serial_nodeA.txt`, demo video |
| TC-006 | No Actuation on Non-Matching Routine | CAN bus up | Send `0x7E0` with routine ID != `0x01` | Node B does not actuate; no positive response | `canlog.md` and/or `serial_nodeB.txt` |

---

## Extras

| TC ID | Title | Steps | Expected Result | Evidence |
|---|---|---|---|---|
| TC-007 | BLE Disconnect Recovery | Disconnect phone; wait 5–10s | Node A re-advertises and OLED indicates DISCONNECTED/SCAN | `serial_nodeA.txt`,  photo |
| TC-008 | CAN Termination Sensitivity (Observation) | Remove one termination; observe | Increased errors/instability (document observation) | notes + photo |
| TC-009 | Timing Check (Unlock Duration) | Measure hold time | Servo hold is ~3 seconds (± tolerance) | demo video timestamps |

---

## References
UDS positive response rule (service + 0x40): https://www.csselectronics.com/pages/uds-protocol-tutorial-unified-diagnostic-services  
ISO-TP concept: https://docs.kernel.org/networking/iso15765-2.html  
SavvyCAN: https://github.com/collin80/SavvyCAN
