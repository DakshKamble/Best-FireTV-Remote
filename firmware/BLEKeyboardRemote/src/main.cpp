#include <Arduino.h>
#include <BleKeyboard.h>

// Create BLE keyboard instance with custom name
BleKeyboard bleKeyboard("Fire TV Remote", "ESP32", 100);

// Button configuration
const int HOME_BUTTON_PIN = 4;   // D4 on ESP32
const int UP_BUTTON_PIN = 19;    // D19 on ESP32
const int DOWN_BUTTON_PIN = 22;  // D22 on ESP32
const int LEFT_BUTTON_PIN = 18;  // D18 on ESP32
const int RIGHT_BUTTON_PIN = 21; // D21 on ESP32
const int DEBOUNCE_DELAY = 50;   // 50ms debounce

// Button state tracking for all buttons
struct Button {
  int pin;
  bool lastState;
  bool currentState;
  unsigned long lastDebounceTime;
  const char* name;
};

Button buttons[] = {
  {HOME_BUTTON_PIN, HIGH, HIGH, 0, "HOME"},
  {UP_BUTTON_PIN, HIGH, HIGH, 0, "UP"},
  {DOWN_BUTTON_PIN, HIGH, HIGH, 0, "DOWN"},
  {LEFT_BUTTON_PIN, HIGH, HIGH, 0, "LEFT"},
  {RIGHT_BUTTON_PIN, HIGH, HIGH, 0, "RIGHT"}
};

const int NUM_BUTTONS = sizeof(buttons) / sizeof(buttons[0]);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Fire TV BLE Remote!");
  
  // Configure all button pins
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
    Serial.print("Configured ");
    Serial.print(buttons[i].name);
    Serial.print(" button on GPIO ");
    Serial.println(buttons[i].pin);
  }
  
  // Initialize BLE keyboard
  bleKeyboard.begin();
  
  Serial.println("BLE keyboard initialized. Waiting for connection...");
}

void loop() {
  // Check if BLE keyboard is connected
  if (bleKeyboard.isConnected()) {
    // Check all buttons
    for (int i = 0; i < NUM_BUTTONS; i++) {
      // Read button state
      int reading = digitalRead(buttons[i].pin);
      
      // Check if button state changed (for debouncing)
      if (reading != buttons[i].lastState) {
        buttons[i].lastDebounceTime = millis();
      }
      
      // If enough time has passed since last state change
      if ((millis() - buttons[i].lastDebounceTime) > DEBOUNCE_DELAY) {
        // If button state has actually changed
        if (reading != buttons[i].currentState) {
          buttons[i].currentState = reading;
          
          // Button pressed (LOW because of INPUT_PULLUP)
          if (buttons[i].currentState == LOW) {
            Serial.print(buttons[i].name);
            Serial.println(" button pressed");
            
            // Send appropriate command based on button
            switch (i) {
              case 0: // HOME button
                Serial.println("Sending Fire TV home command");
                bleKeyboard.write(KEY_MEDIA_WWW_HOME);
                break;
              case 1: // UP button
                Serial.println("Sending UP arrow");
                bleKeyboard.write(KEY_UP_ARROW);
                break;
              case 2: // DOWN button
                Serial.println("Sending DOWN arrow");
                bleKeyboard.write(KEY_DOWN_ARROW);
                break;
              case 3: // LEFT button
                Serial.println("Sending LEFT arrow");
                bleKeyboard.write(KEY_LEFT_ARROW);
                break;
              case 4: // RIGHT button
                Serial.println("Sending RIGHT arrow");
                bleKeyboard.write(KEY_RIGHT_ARROW);
                break;
            }
            
            delay(200); // Prevent multiple rapid presses
          }
        }
      }
      
      // Save button state for next iteration
      buttons[i].lastState = reading;
    }
    
    // Optional: Send test commands every 30 seconds (comment out for production)
    // Uncomment the block below if you want automatic testing
    /*
    static unsigned long lastTestTime = 0;
    if (millis() - lastTestTime > 30000) {
      lastTestTime = millis();
      Serial.println("Auto test disabled - use physical buttons");
    }
    */
    
  } else {
    // Not connected - show status
    static unsigned long lastStatusTime = 0;
    if (millis() - lastStatusTime > 5000) {
      lastStatusTime = millis();
      Serial.println("Waiting for BLE connection...");
      Serial.println("Make sure to pair with your Fire TV:");
      Serial.println("Settings > Controllers & Bluetooth Devices > Other Bluetooth Devices > Add Bluetooth Devices");
    }
  }
  
  // Small delay to prevent excessive CPU usage
  delay(10);
}

/*
 * FIRE TV REMOTE FUNCTIONALITY:
 * 
 * Hardware Setup:
 * - HOME button: Connect between GPIO 4 (D4) and GND
 * - UP button: Connect between GPIO 19 (D19) and GND  
 * - DOWN button: Connect between GPIO 22 (D22) and GND
 * - LEFT button: Connect between GPIO 18 (D18) and GND
 * - RIGHT button: Connect between GPIO 21 (D21) and GND
 * - All buttons use internal pull-up resistors
 * 
 * BLE Pairing:
 * 1. Upload this code to your ESP32
 * 2. Open Serial Monitor to see connection status
 * 3. On Fire TV: Settings > Controllers & Bluetooth Devices > Other Bluetooth Devices
 * 4. Select "Add Bluetooth Devices"
 * 5. Look for "Fire TV Remote" in the list and select it
 * 6. Once connected, the home button will work
 * 
 * Button Functions:
 * - Home Button (GPIO 5): Sends Fire TV home command
 * - Test sequence runs every 10 seconds when connected (remove for production)
 * 
 * Media Keys Available:
 * - KEY_MEDIA_WWW_HOME (Fire TV home)
 * - KEY_MEDIA_PLAY_PAUSE
 * - KEY_MEDIA_VOLUME_UP
 * - KEY_MEDIA_VOLUME_DOWN
 * - KEY_MEDIA_NEXT_TRACK
 * - KEY_MEDIA_PREVIOUS_TRACK
 * - KEY_MEDIA_MUTE
 * 
 * Navigation Keys:
 * - KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW
 * - KEY_RETURN (select/OK)
 * - KEY_ESC (back)
 * 
 * Customization:
 * - Change HOME_BUTTON_PIN to use different GPIO
 * - Modify DEBOUNCE_DELAY for button sensitivity
 * - Add more buttons by copying the button handling code
 * - Remove test sequence for production use
 */
