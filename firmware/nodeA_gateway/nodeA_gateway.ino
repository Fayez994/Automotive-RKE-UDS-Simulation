/* NODE A: GATEWAY + OLED + SMART TOUCH (PEPS)
   - Role: Validates Key (BLE/Touch), Sends UDS Request, Updates HMI.
   - Pin 15: Touch Sensor (Jumper Wire)
   - Pins 4/16: OLED SDA/SCL
*/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------------------- CONFIG --------------------
#define CAN_CS_PIN 5
#define MCP_CLOCK  MCP_8MHZ

// Touch Config
#define TOUCH_PIN       15
#define TOUCH_THRESHOLD 40
#define TOUCH_DEBOUNCE  350 // ms

// OLED Config
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// CAN Config
MCP_CAN CAN0(CAN_CS_PIN);
#define CAN_ID_REQ  0x7E0
#define CAN_ID_RESP 0x7E8

// BLE UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// -------------------- GLOBALS --------------------
bool deviceConnected = false;
bool unlockRequest = false;

// -------------------- BLE CALLBACKS --------------------
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) { deviceConnected = true; };
    void onDisconnect(BLEServer* pServer) { 
      deviceConnected = false; 
      pServer->getAdvertising()->start(); 
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String value = pCharacteristic->getValue(); 
      if (value.length() > 0 && value[0] == '1') unlockRequest = true;
    }
};

// -------------------- DISPLAY HELPER --------------------
void updateDisplay(String status, String action) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(">> GATEWAY ECU <<");
  display.drawLine(0, 10, 128, 10, WHITE);
  
  display.setCursor(0, 20);
  display.print("BLE: "); display.println(status);
  
  display.setCursor(0, 40);
  display.setTextSize(2);
  display.println(action);
  display.display();
  display.setTextSize(1);
}

// -------------------- MAIN --------------------
void setup() {
  Serial.begin(115200);
  
  // Init OLED (Custom Pins)
  Wire.begin(4, 16);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) Serial.println("OLED Failed");
  display.setTextColor(WHITE);
  updateDisplay("WAITING...", "BOOTING");

  // Init CAN
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_CLOCK) == CAN_OK) Serial.println("CAN Init OK");
  else Serial.println("CAN Init Failed");
  CAN0.setMode(MCP_NORMAL);

  // Init BLE
  BLEDevice::init("Fayez_Car_Pro"); 
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEDevice::getAdvertising()->start();
  BLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID); // Ensure visibility
}

void loop() {
  // 1. CONNECTION CHECK
  if(!deviceConnected) {
     updateDisplay("DISCONNECTED", "SCAN ME");
     delay(200); return;
  }

  // 2. TOUCH CHECK (Passive Entry)
  // touchRead drops below threshold when touched
  if(touchRead(TOUCH_PIN) < TOUCH_THRESHOLD) {
     unlockRequest = true;
     delay(TOUCH_DEBOUNCE); 
  }

  // 3. PROCESS UNLOCK
  if(unlockRequest) {
    updateDisplay("CONNECTED", "SENDING...");
    
    // Send UDS Request: 0x31 (Routine Control)
    byte udsFrame[8] = {0x02, 0x31, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    if(CAN0.sendMsgBuf(CAN_ID_REQ, 0, 8, udsFrame) == CAN_OK) {
      Serial.println(">> TX: UDS Unlock Request");
      
      // Wait for Response (0x7E8)
      unsigned long startWait = millis();
      bool responseReceived = false;
      
      while(millis() - startWait < 1000) {
        if(CAN0.checkReceive() == CAN_MSGAVAIL) {
           long unsigned int rxId;
           unsigned char len;
           unsigned char rxBuf[8];
           CAN0.readMsgBuf(&rxId, &len, rxBuf);
           
           // Check for Positive Response (0x71)
           if(rxId == CAN_ID_RESP && rxBuf[1] == 0x71) {
             responseReceived = true;
             break;
           }
        }
      }

      if(responseReceived) {
        updateDisplay("CONNECTED", "SUCCESS!");
        Serial.println("<< RX: Positive Response (0x71)");
        delay(2000); // Hold Success Screen
      } else {
        updateDisplay("CONNECTED", "TIMEOUT");
        Serial.println("<< Error: ECU Timeout");
        delay(2000);
      }
    }
    unlockRequest = false; 
  } else {
    // Idle Screen
    static long lastUpdate = 0;
    if(millis() - lastUpdate > 1000) {
       updateDisplay("CONNECTED", "LOCKED(Tch)");
       lastUpdate = millis();
    }
  }
}