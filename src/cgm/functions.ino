
// ********** Initialize the serial port **********
void beginSerial() {
  Serial.begin(115200);
  delay(1);
  Serial.println();
  Serial.println();
  Serial.print(F("Sketch Name: "));
  Serial.println(F(SKETCH));
  Serial.print(F("Sketch Version: "));
  Serial.println(F(VERSION));
  Serial.println();
  delay(2500);
}

/*
  void runChargingBlink() {
  unsigned long now = millis();

  Serial.print("Blink state: ");
  Serial.println(chargeBlinkState);

  if (!chargeBlinkState) {
    // LED is OFF — wait 900ms
    if (now - chargeBlinkTimer >= 900) {
      analogWrite(ledPin6, ledBrightness);
      chargeBlinkState = true;
      chargeBlinkTimer = now;
    }
  } else {
    // LED is ON — wait 100ms
    if (now - chargeBlinkTimer >= 100) {
      analogWrite(ledPin6, 0);
      chargeBlinkState = false;
      chargeBlinkTimer = now;
    }
  }
  }
*/


void runChargingBlink() {
  /*
     Blink LED according to percent battery charge
     batteryPercent
     1 blink → ~0–25% charged
     2 blinks → ~25–50% charged
     3 blinks → ~50–75% charged
     4 blinks → ~75–99% charged
  */

  if (batteryPercent < 25) {
    // 0-24%, Blink once
    blinkr(1);
  }

  else if (batteryPercent < 50) {
    // 25-49%, Blink twice
    blinkr(2);
  }

  else if (batteryPercent < 75) {
    // 40-74%, Blink three times
    blinkr(3);
  }

  else {
    // 75-99%, Blink four times
    blinkr(4);
  }
  analogWrite(ledPin5, 0);   // ensure LED is fully off
  analogWrite(ledPin6, 0);   // ensure LED is fully off
}

/*
  void blinkr(int n) {
  n = constrain(n, 0, 10);        // n must be between 0-10
  for (int i = 0; i < n; i++) {
    analogWrite(ledPin6, ledBrightness);
    delay(15);
    analogWrite(ledPin6, 0);
    delay(300);
  }
  }
*/

void blinkr(int n) {
  n = constrain(n, 0, 10);

  for (int i = 0; i < n; i++) {
    analogWrite(ledPin5, ledBrightness);  // PWM ON
    analogWrite(ledPin6, ledBrightness);  // PWM ON
    delay(15);
    analogWrite(ledPin5, 0);              // PWM OFF
    analogWrite(ledPin6, 0);              // PWM OFF
    delay(350);
  }
}


// ********** Function to publish RSSI over MQTT **********
/*
  void publishRssi() {
  rssiTimer.stop();
  long myRssi;
  String rssi_str;
  char temp[50];
  myRssi = WiFi.RSSI();                               //RSSI rturns a long int.
  rssi_str = String(myRssi);                          //Convert myRssi to a string
  rssi_str.toCharArray(temp, rssi_str.length() + 1);  //packaging up the data to publish to mqtt
  //Serial.print(F("====="));
  //Serial.print(F("RSSI: "));
  //Serial.print(temp);
  //Serial.println(F("====="));
  Client.publish(rssiTopic, temp);
  rssiTimer.start();
  }
*/
