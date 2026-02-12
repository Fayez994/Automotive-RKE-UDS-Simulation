# python can log
 Heartbeats (0x505)
| Timestamp | Type | Message | Door Status | Angle (°) | Notes |
|---:|---|---|---|---:|---|
| -1770836694.862 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770836694.362 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770836693.862 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770836693.362 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770836692.861 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770836692.361 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770836691.861 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770836691.569 | EVENT (REQ) | UDS Request: Unlock |  |  | UDS 0x7E0 |
| -1770836691.569 | EVENT (RES) | UDS Response: Positive Ack |  |  | UDS 0x7E8 |
| -1770836691.341 | CYCLIC | Door Status: UNLOCKING \| Angle: 27° | UNLOCKING | 27 |  |
| -1770836690.821 | CYCLIC | Door Status: UNLOCKING \| Angle: 40° | UNLOCKING | 40 |  |
| -1770836690.301 | CYCLIC | Door Status: UNLOCKING \| Angle: 53° | UNLOCKING | 53 |  |
| -1770836689.781 | CYCLIC | Door Status: UNLOCKING \| Angle: 66° | UNLOCKING | 66 |  |
| -1770836689.281 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836688.781 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836688.280 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836687.780 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836687.280 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836686.780 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836686.280 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836685.780 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836685.280 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836684.779 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836684.279 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836683.779 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836683.279 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836682.779 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836682.279 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836681.779 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836681.279 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836680.779 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836680.279 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770836679.779 | CYCLIC | Door Status: HOLD \| Angle: 70° | HOLD | 70 |  |
| -1770865674.245 | CYCLIC | Door Status: LOCKING \| Angle: 69° | LOCKING | 69 |  |
| -1770865673.705 | CYCLIC | Door Status: LOCKING \| Angle: 55° | LOCKING | 55 |  |
| -1770865673.186 | CYCLIC | Door Status: LOCKING \| Angle: 42° | LOCKING | 42 |  |
| -1770865672.666 | CYCLIC | Door Status: LOCKING \| Angle: 29° | LOCKING | 29 |  |
| -1770865672.166 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770865671.666 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770865671.166 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |
| -1770865670.667 | CYCLIC | Door Status: IDLE \| Angle: 20° | IDLE | 20 |  |

---
---
# Looking for: UDS (0x7E0/0x7E8)
| Timestamp | Direction | ID_Hex | DLC | Data |Interpretation|
|---|---|---|---|---|---|
|-1770836691|	REQ| (Gw->Door)|	0x7E0	|8|	02 31 01 00 00 00 00 00|	Unlock Command|
|-1770836691	|RES| (Door->Gw)|	0x7E8	|8	|02 71 01 00 00 00 00 00	|Positive Ack|

