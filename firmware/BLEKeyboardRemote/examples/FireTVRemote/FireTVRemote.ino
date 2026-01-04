/**
 * Fire TV Remote Example using ESP32-BLE-Keyboard
 * 
 * This example demonstrates how to use the ESP32 as a Bluetooth remote control
 * for Amazon Fire TV, including the newly added Fast Forward and Rewind functions.
 */
#include <BleKeyboard.h>

// Create a BLE Keyboard instance with a custom name
BleKeyboard bleKeyboard("FireTV Remote", "ESP32", 100);

// Define button pins (adjust these to match your hardware)
#define BUTTON_UP     12
#define BUTTON_DOWN   14
#define BUTTON_LEFT   27
#define BUTTON_RIGHT  26
#define BUTTON_SELECT 25
#define BUTTON_BACK   33
#define BUTTON_HOME   32
#define BUTTON_PLAY   13
#define BUTTON_FF     15  // Fast Forward button
#define BUTTON_REW    4   // Rewind button

// Variables to track button states
bool lastButtonStates[10] = {false, false, false, false, false, false, false, false, false, false};
bool currentButtonStates[10] = {false, false, false, false, false, false, false, false, false, false};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Fire TV Remote");
  
  // Initialize button pins as inputs with pull-up resistors
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_BACK, INPUT_PULLUP);
  pinMode(BUTTON_HOME, INPUT_PULLUP);
  pinMode(BUTTON_PLAY, INPUT_PULLUP);
  pinMode(BUTTON_FF, INPUT_PULLUP);
  pinMode(BUTTON_REW, INPUT_PULLUP);
  
  // Start the BLE keyboard service
  bleKeyboard.begin();
}

void loop() {
  // Only send commands if connected to a device
  if (bleKeyboard.isConnected()) {
    
    // Read all button states (LOW means pressed with pull-up resistors)
    currentButtonStates[0] = !digitalRead(BUTTON_UP);
    currentButtonStates[1] = !digitalRead(BUTTON_DOWN);
    currentButtonStates[2] = !digitalRead(BUTTON_LEFT);
    currentButtonStates[3] = !digitalRead(BUTTON_RIGHT);
    currentButtonStates[4] = !digitalRead(BUTTON_SELECT);
    currentButtonStates[5] = !digitalRead(BUTTON_BACK);
    currentButtonStates[6] = !digitalRead(BUTTON_HOME);
    currentButtonStates[7] = !digitalRead(BUTTON_PLAY);
    currentButtonStates[8] = !digitalRead(BUTTON_FF);
    currentButtonStates[9] = !digitalRead(BUTTON_REW);
    
    // Check for button presses and send corresponding commands
    
    // Navigation buttons
    if (currentButtonStates[0] && !lastButtonStates[0]) {
      Serial.println("UP pressed");
      bleKeyboard.write(KEY_UP_ARROW);
    }
    
    if (currentButtonStates[1] && !lastButtonStates[1]) {
      Serial.println("DOWN pressed");
      bleKeyboard.write(KEY_DOWN_ARROW);
    }
    
    if (currentButtonStates[2] && !lastButtonStates[2]) {
      Serial.println("LEFT pressed");
      bleKeyboard.write(KEY_LEFT_ARROW);
    }
    
    if (currentButtonStates[3] && !lastButtonStates[3]) {
      Serial.println("RIGHT pressed");
      bleKeyboard.write(KEY_RIGHT_ARROW);
    }
    
    if (currentButtonStates[4] && !lastButtonStates[4]) {
      Serial.println("SELECT pressed");
      bleKeyboard.write(KEY_RETURN);
    }
    
    if (currentButtonStates[5] && !lastButtonStates[5]) {
      Serial.println("BACK pressed");
      bleKeyboard.write(KEY_ESC);
    }
    
    if (currentButtonStates[6] && !lastButtonStates[6]) {
      Serial.println("HOME pressed");
      bleKeyboard.write(KEY_HOME);
    }
    
    if (currentButtonStates[7] && !lastButtonStates[7]) {
      Serial.println("PLAY/PAUSE pressed");
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    }
    
    // New Fast Forward and Rewind buttons
    if (currentButtonStates[8] && !lastButtonStates[8]) {
      Serial.println("FAST FORWARD pressed");
      bleKeyboard.write(KEY_MEDIA_FAST_FORWARD);
    }
    
    if (currentButtonStates[9] && !lastButtonStates[9]) {
      Serial.println("REWIND pressed");
      bleKeyboard.write(KEY_MEDIA_REWIND);
    }
    
    // Update last button states
    for (int i = 0; i < 10; i++) {
      lastButtonStates[i] = currentButtonStates[i];
    }
  } else {
    // If not connected, try to reconnect
    Serial.println("Not connected. Waiting for connection...");
  }
  
  // Small delay to avoid bouncing
  delay(50);
}
