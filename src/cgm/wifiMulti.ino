void setup_wifiMulti() {
  byte mac[6];                      //// the MAC address of your Wifi shield

  Serial.println(F("Connecting to WiFi"));

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Register multi WiFi networks
  wifiMulti.addAP(MYSSID, MYPASSWORD);
  wifiMulti.addAP("Naboo", NABOOPASSWORD);
  wifiMulti.addAP("Serenity", SERENITYPASSWORD);

  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
    Serial.print(F("WiFi connected: "));
    Serial.println(WiFi.SSID());
    Serial.print(F("MAC Address: "));
    Serial.println(WiFi.macAddress());
    Serial.print(F("IP Address: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("Hostname: "));
    Serial.println(WiFi.hostname());
    long rssi = WiFi.RSSI();
    Serial.print("Signal Strength (RSSI):");
    Serial.println(rssi);
  } else {
    Serial.println(F("WiFi not connected!"));
  }


  // Get the last three numbers of the mac address.
  // "4C:11:AE:0D:83:86" becomes "0D8386" in macBuffer.
  WiFi.macAddress(mac);
  snprintf(macBuffer, sizeof(macBuffer), "%02X%02X%02X", mac[3], mac[4], mac[5]);

  // Build hostNamePrefix + last three bytes of the MAC address.
  strcpy(hostName, hostNamePrefix);
  strcat(hostName, macBuffer);

  //Serial.print(F("hostName = \""));
  //Serial.print(hostName);
  //Serial.println(F("\""));

}
