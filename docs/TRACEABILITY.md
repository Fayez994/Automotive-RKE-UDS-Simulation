<!-- docs/TRACEABILITY.md -->
# Traceability Matrix — Automotive-RKE-UDS-Simulation

**Owner:** Fayez Alallaf  
**Doc ID:** TRC-RKE-001  
**Version:** 1.0  
**Last Updated:** 2026-02-11

Mapping: **Requirement → Test Case(s) → Evidence**

---

## SYS Requirements → Test Cases

| Requirement ID | Verified By (Test Cases) | Primary Evidence |
|---|---|---|
| SYS-RKE-001 | TC-001, TC-002 | `serial_nodeA.txt`, demo video |
| SYS-RKE-002 | TC-005 | `serial_nodeA.txt`, demo video |
| SYS-RKE-003 | TC-002, TC-004 | `canlog.md`, `serial_nodeA.txt` |
| SYS-RKE-004 | TC-003 | `serial_nodeB.txt`, demo video |
| SYS-RKE-005 | TC-004 | `canlog.md` |
| SYS-RKE-006 | TC-004 (behavior) + inspection | `hil.jpeg`, `canlog.md` |

---

## SW Requirements (Gateway) → Test Cases

| Requirement ID | Verified By (Test Cases) | Primary Evidence |
|---|---|---|
| SW-GW-001 | TC-001 | `serial_nodeA.txt` |
| SW-GW-002 | TC-002 | `serial_nodeA.txt`, demo video |
| SW-GW-003 | TC-005 | `serial_nodeA.txt`, demo video |
| SW-GW-004 | TC-002, TC-004 | `canlog.md`, `serial_nodeA.txt` |
| SW-GW-005 | TC-001, TC-002, TC-007 | demo video, optional OLED photo |

---

## SW Requirements (Door ECU) → Test Cases

| Requirement ID | Verified By (Test Cases) | Primary Evidence |
|---|---|---|
| SW-DOOR-001 | TC-003 | `serial_nodeB.txt` |
| SW-DOOR-002 | TC-003, TC-009 | `serial_nodeB.txt`, demo video |
| SW-DOOR-003 | TC-004 | `canlog.md` |

---

## Interface Requirements → Test Cases

| Requirement ID | Verified By (Test Cases) | Primary Evidence |
|---|---|---|
| IF-CAN-001 | TC-004 + inspection | `hil.jpeg`, `canlog.md` |
| IF-CAN-002 | TC-004 | `canlog.md` |
| IF-BLE-001 | TC-001, TC-002 | `serial_nodeA.txt` |
| IF-ACT-001 | TC-003 | `serial_nodeB.txt`, demo video |
| IF-DISP-001 | TC-001/TC-002 demo | demo video, optional photo |

---

## Evidence Index

| Evidence File | Location |
|---|---|
| Bench photo | `assets/evidence/hil.jpeg` |
| CAN trace | `assets/evidence/canlog.md` |
| Node A serial | `assets/evidence/serial_nodeA.txt` |
| Node B serial | `assets/evidence/serial_nodeB.txt` |
| Demo video link | `README.md` |

---

## References
UDS: https://www.csselectronics.com/pages/uds-protocol-tutorial-unified-diagnostic-services  
ISO-TP: https://docs.kernel.org/networking/iso15765-2.html
