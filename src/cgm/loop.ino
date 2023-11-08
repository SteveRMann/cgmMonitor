/********** LOOP **********/
void loop() {
  ArduinoOTA.handle();

  // Is WiFi connected?
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected. Rebooting in 5 seconds"));
    Serial.print(F("WiFi.status= "));
    Serial.println(WiFi.status());
    delay(5000);
    ESP.restart();
  }

  if (rssiTimer.ding()) publishRssi();

  //Re-connect the pubsub client to the mqtt broker
  if (!Client.connected()) {
    reconnect();
  }
  if (!Client.loop()) {
    Client.connect(hostName);
  }

  if (millis() - ledMillis > ledTime) {     //If the led has been on for a while, turn it off.
    digitalWrite(ledPin, ledOFF);
  }

  if (now() - crashTimer > crashTimeout) {    //If the time has not been updated, assume the worst.
    ESP.restart();
  }

  //staleFlag is set if the stale tick timer times out
  if (staleFlag) ESP.reset();



  /*
    if (now() - bgTimestamp > staleTime) {    //If the BG reading is stale..
      Serial.println(F("DEBUG- staleTime timeout"));
      bgDisplay.writeDigitAscii(3, '*');
      bgDisplay.writeDisplay();
    }
  */

  //Now, we just wait for a callback

  // Reset the ESP8266 WDT timer.
  yield();
}
