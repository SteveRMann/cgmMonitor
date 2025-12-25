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

  //Re-connect the pubsub client to the mqtt broker
  if (!Client.connected()) {
    reconnect();
  }
  if (!Client.loop()) {
    Client.connect(hostName);
  }


  // If charging, blink the LED every five seconds.
  static unsigned long lastBlink = 0;
  if (charging) {
    if (millis() - lastBlink >= 5000) {
      lastBlink = millis();
      runChargingBlink();
    }
  }



  // Reset the ESP8266 WDT timer.
  yield();
}
