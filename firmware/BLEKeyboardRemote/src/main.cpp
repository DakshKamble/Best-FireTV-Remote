#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard blekeyboard;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Remote!");
  blekeyboard.begin();
}

void loop() {
  if(blekeyboard.isConnected()) {
    Serial.println("Sending 'UP DOWN LEFT RIGHT'");

    blekeyboard.write(KEY_UP_ARROW);
    delay(1000);

    blekeyboard.write(KEY_DOWN_ARROW);
    delay(1000);

    blekeyboard.write(KEY_LEFT_ARROW);
    delay(1000);

    blekeyboard.write(KEY_RIGHT_ARROW);
    delay(1000);

    blekeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    delay(1000);
    blekeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    delay(1000);

    Serial.println("Commands sent");
  }

  Serial.println("Waiting 5 seconds...");
  delay(5000);
}