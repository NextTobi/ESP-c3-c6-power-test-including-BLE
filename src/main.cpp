#include <Arduino.h>

// Test only one feature at a time to save space
// Uncomment only ONE of these defines:
#define TEST_WIFI_ONLY
//#define TEST_BLE_ONLY

#ifdef TEST_WIFI_ONLY
  #include <WiFi.h>
#endif

#ifdef TEST_BLE_ONLY
  #include "BLEDevice.h"
  #include "BLEServer.h"
  #include "BLEUtils.h"
#endif

#define LED_PIN 8

// Function declarations
#ifdef TEST_WIFI_ONLY
void testWiFi();
#endif

#ifdef TEST_BLE_ONLY
void testBLE();
#endif

void setup() {
  Serial.begin(115200);
  delay(3000);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  #ifdef TEST_WIFI_ONLY
    Serial.println("=== ESP32-C6 WiFi ONLY Test ===");
    testWiFi();
  #endif
  
  #ifdef TEST_BLE_ONLY
    Serial.println("=== ESP32-C6 BLE ONLY Test ===");
    testBLE();
  #endif
  
  Serial.println("Test completed - starting blink loop");
  Serial.flush();
}

void loop() {
  // Simple blink to show it's alive
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  if (millis() - lastBlink >= 1000) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.println("ESP32-C6 alive - LED: " + String(ledState ? "ON" : "OFF"));
    lastBlink = millis();
  }
  
  delay(100);
}

// WiFi test implementation
#ifdef TEST_WIFI_ONLY
void testWiFi() {
  Serial.println("Testing WiFi features...");
  
  // Test 1: MAC Address
  Serial.println("\n1. Testing WiFi MAC address:");
  try {
    WiFi.mode(WIFI_STA);
    delay(500);
    String mac = WiFi.macAddress();
    Serial.println("✓ SUCCESS: WiFi MAC: " + mac);
  } catch (...) {
    Serial.println("✗ ERROR: WiFi MAC failed");
  }
  
  // Test 2: Network Scanning
  Serial.println("\n2. Testing WiFi network scanning:");
  try {
    Serial.println("Starting scan...");
    int networks = WiFi.scanNetworks();
    Serial.println("✓ SUCCESS: Found " + String(networks) + " networks");
    
    for (int i = 0; i < networks && i < 3; i++) {
      Serial.println("  - " + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + " dBm)");
    }
  } catch (...) {
    Serial.println("✗ ERROR: WiFi scan failed");
  }
  
  // Test 3: Power Settings
  Serial.println("\n3. Testing WiFi power settings:");
  try {
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    Serial.println("✓ SUCCESS: Max power set");
    
    WiFi.setSleep(false);
    Serial.println("✓ SUCCESS: Sleep disabled");
  } catch (...) {
    Serial.println("✗ ERROR: WiFi power settings failed");
  }
  
  // Test 4: Connection Attempt (will fail but tests TX)
  Serial.println("\n4. Testing WiFi connection attempt:");
  try {
    WiFi.begin("ESP32Test", "password123");
    delay(2000);
    Serial.println("✓ SUCCESS: Connection attempt completed");
    Serial.println("  Status: " + String(WiFi.status()));
  } catch (...) {
    Serial.println("✗ ERROR: WiFi connection attempt failed");
  }
  
  WiFi.mode(WIFI_OFF);
  Serial.println("\n✓ WiFi tests completed");
}
#endif

// BLE test implementation
#ifdef TEST_BLE_ONLY
void testBLE() {
  Serial.println("Testing BLE features...");
  
  // Test 1: BLE Initialization
  Serial.println("\n1. Testing BLE initialization:");
  try {
    BLEDevice::init("ESP32-C6-BLE-Test");
    delay(500);
    Serial.println("✓ SUCCESS: BLE initialized");
  } catch (...) {
    Serial.println("✗ ERROR: BLE init failed");
    return;
  }
  
  // Test 2: MAC Address
  Serial.println("\n2. Testing BLE MAC address:");
  try {
    String bleMac = BLEDevice::getAddress().toString().c_str();
    Serial.println("✓ SUCCESS: BLE MAC: " + bleMac);
  } catch (...) {
    Serial.println("✗ ERROR: BLE MAC failed");
  }
  
  // Test 3: Server Creation
  Serial.println("\n3. Testing BLE server:");
  try {
    BLEServer* pServer = BLEDevice::createServer();
    Serial.println("✓ SUCCESS: BLE server created");
    
    BLEService* pService = pServer->createService("180F");
    pService->start();
    Serial.println("✓ SUCCESS: BLE service started");
  } catch (...) {
    Serial.println("✗ ERROR: BLE server failed");
  }
  
  // Test 4: Advertising
  Serial.println("\n4. Testing BLE advertising:");
  try {
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("180F");
    pAdvertising->start();
    Serial.println("✓ SUCCESS: BLE advertising started");
    Serial.println("  Device should be visible as 'ESP32-C6-BLE-Test'");
  } catch (...) {
    Serial.println("✗ ERROR: BLE advertising failed");
  }
  
  Serial.println("\n✓ BLE tests completed");
}
#endif