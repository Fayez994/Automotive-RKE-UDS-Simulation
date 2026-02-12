/* NODE B: DOOR ECU + SERVO ACTUATOR (NON-BLOCKING STATE MACHINE)
   - Role: Receives UDS 0x31 (Unlock), Sends 0x71 (ACK), Moves Servo safely.
   - Pin 27: Servo (Orange Wire)
   - CS Pin: 5
*/

#include <SPI.h>
#include <mcp_can.h>
#include <ESP32Servo.h>

// -------------------- CONFIG --------------------
#define CAN_CS_PIN   5
#define SERVO_PIN    27  

// Check your crystal! (MCP_8MHZ or MCP_16MHZ)
#define MCP_CLOCK    MCP_8MHZ

// Servo Safety Limits
static const int SERVO_LOCK_ANGLE   = 20;  
static const int SERVO_UNLOCK_ANGLE = 70;  

// Animation Physics
static const uint32_t SERVO_STEP_DELAY_MS = 10;   
static const int      SERVO_STEP_DEG      = 1;    
static const uint32_t UNLOCK_HOLD_MS      = 10000; // 10 Seconds Hold

// UDS Protocol
static const uint32_t CAN_ID_REQ  = 0x7E0; 
static const uint32_t CAN_ID_RESP = 0x7E8; 

// Heartbeat Config
static const uint32_t HEARTBEAT_INTERVAL_MS = 500; // Send status every 500ms
static uint32_t lastHeartbeatMs = 0;

// -------------------- GLOBALS --------------------
MCP_CAN CAN0(CAN_CS_PIN);
Servo doorLock;

enum ActState { IDLE, MOVE_TO_UNLOCK, HOLD_UNLOCK, MOVE_TO_LOCK };
static ActState state = IDLE;

static int currentAngle = SERVO_LOCK_ANGLE;
static uint32_t lastStepMs = 0;
static uint32_t stateStartMs = 0;

// -------------------- SERVO LOGIC --------------------
void attachServoIfNeeded() {
  if (!doorLock.attached()) {
    doorLock.setPeriodHertz(50); 
    doorLock.attach(SERVO_PIN, 500, 2400); 
  }
}

// Returns TRUE when target is reached
bool moveServoToward(int targetAngle) {
  uint32_t now = millis();
  if (now - lastStepMs < SERVO_STEP_DELAY_MS) return false;

  lastStepMs = now;

  if (currentAngle < targetAngle) {
    currentAngle += SERVO_STEP_DEG;
    if (currentAngle > targetAngle) currentAngle = targetAngle;
  } else if (currentAngle > targetAngle) {
    currentAngle -= SERVO_STEP_DEG;
    if (currentAngle < targetAngle) currentAngle = targetAngle;
  }

  doorLock.write(currentAngle);
  return (currentAngle == targetAngle);
}

void sendPositiveResponse() {
  byte response[8] = {0x02, 0x71, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
  CAN0.sendMsgBuf(CAN_ID_RESP, 0, 8, response);
  Serial.println(">> TX: Positive Response (0x71)");
}

void sendHeartbeat() {
  // Byte 0: State, Byte 1: Angle
  byte statusData[8] = { (byte)state, (byte)currentAngle, 0, 0, 0, 0, 0, 0 };
  CAN0.sendMsgBuf(0x505, 0, 8, statusData);
}

// -------------------- MAIN LOOP --------------------
void setup() {
  Serial.begin(115200);
  
  attachServoIfNeeded();
  currentAngle = SERVO_LOCK_ANGLE;
  doorLock.write(currentAngle);

  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_CLOCK) == CAN_OK) {
    Serial.println("Door ECU Ready (Non-Blocking Mode)");
  } else {
    Serial.println("CAN Init Failed");
  }
  CAN0.setMode(MCP_NORMAL);
}

void loop() {
  // 1. CAN RECEIVE (Poll)
  if (CAN0.checkReceive() == CAN_MSGAVAIL) {
    long unsigned int rxId = 0;
    unsigned char len = 0;
    unsigned char rxBuf[8];

    CAN0.readMsgBuf(&rxId, &len, rxBuf);

    if (rxId == CAN_ID_REQ && rxBuf[1] == 0x31 && rxBuf[2] == 0x01) {
      Serial.println("<< RX: Unlock Request (0x31)");
      
      if (state == IDLE) {
        sendPositiveResponse(); // ACK immediately
        state = MOVE_TO_UNLOCK; 
        stateStartMs = millis();
      } else {
        Serial.println("   (Ignored: Actuator Busy)");
      }
    }
  }

  // 2. ACTUATOR STATE MACHINE
  switch (state) {
    case IDLE: 
      break;

    case MOVE_TO_UNLOCK:
      if (moveServoToward(SERVO_UNLOCK_ANGLE)) {
        Serial.println("   Status: UNLOCKED");
        state = HOLD_UNLOCK;
        stateStartMs = millis();
      }
      break;

    case HOLD_UNLOCK:
      if (millis() - stateStartMs >= UNLOCK_HOLD_MS) {
        Serial.println("   Status: Auto-Locking...");
        state = MOVE_TO_LOCK;
      }
      break;

    case MOVE_TO_LOCK:
      if (moveServoToward(SERVO_LOCK_ANGLE)) {
        Serial.println("   Status: LOCKED");
        state = IDLE;
      }
      break;
  } 

  // 3. HEARTBEAT LOGIC 
  if (millis() - lastHeartbeatMs >= HEARTBEAT_INTERVAL_MS) {
    sendHeartbeat();
    lastHeartbeatMs = millis();
  }
}
