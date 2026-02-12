import time
import can
import csv

# CONFIGURATION
OUT_FILE = "full_system_trace.csv"
DURATION_S = 15
BITRATE = 500000

# 0x505 (Heartbeat) to the allow-list
FILTER_IDS = {0x7E0, 0x7E8, 0x505}


def run_test_cycle():
    print(f"--- STARTING FULL SYSTEM TRACE ({DURATION_S}s) ---")
    print("Looking for: UDS (0x7E0/0x7E8) AND Heartbeats (0x505)")

    try:
        
        with can.Bus(interface="gs_usb", channel=0, bitrate=BITRATE) as bus:

            start_time = time.time()

            with open(OUT_FILE, "w", newline='') as f:
                writer = csv.writer(f)
                writer.writerow(
                    ["Time_Sec", "ID_Hex", "Type", "Data_Hex", "Details"])

                while (time.time() - start_time) < DURATION_S:
                    msg = bus.recv(timeout=1.0)
                    if not msg:
                        continue

                    if msg.arbitration_id in FILTER_IDS:
                        rel_time = msg.timestamp - start_time if msg.timestamp else time.time() - \
                            start_time

                        # INTERPRET THE HEARTBEAT (0x505)
                        if msg.arbitration_id == 0x505:
                            msg_type = "CYCLIC"
                            # Parse Payload: [State, Angle, ...]
                            state_code = msg.data[0]
                            angle = msg.data[1]

                            state_str = "IDLE"
                            if state_code == 1:
                                state_str = "UNLOCKING"
                            if state_code == 2:
                                state_str = "HOLD"
                            if state_code == 3:
                                state_str = "LOCKING"

                            notes = f"Door Status: {state_str} | Angle: {angle}°"

                        # INTERPRET THE UDS EVENTS (0x7E0 / 0x7E8)
                        elif msg.arbitration_id == 0x7E0:
                            msg_type = "EVENT (REQ)"
                            notes = "UDS Request: Unlock"
                        elif msg.arbitration_id == 0x7E8:
                            msg_type = "EVENT (RES)"
                            notes = "UDS Response: Positive Ack"

                        # 3. LOG IT
                        data_str = " ".join(f"{b:02X}" for b in msg.data)
                        writer.writerow(
                            [f"{rel_time:.4f}", f"0x{msg.arbitration_id:X}", msg_type, data_str, notes])

                        print(f"[{rel_time:.3f}] {msg_type}: {notes}")

    except Exception as e:
        print(f"\nERROR: {e}")


if __name__ == "__main__":
    run_test_cycle()
