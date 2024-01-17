int setup_wifiMulti() {
  /*
    In the main program, include:
      //--------------- WiFiMulti declarations ---------------
      #include <ESP8266WiFiMulti.h>
      ESP8266WiFiMulti wifiMulti;

      // WiFi connect timeout per AP. Increase when connecting takes longer.
      const uint32_t connectTimeoutMs = 5000;

      // setup_wifi vars
      char macBuffer[24];       // Holds the last three digits of the MAC, in hex.
      char hostNamePrefix[] = hostPrefix;
      char hostName[12];        // Holds hostNamePrefix + the last three bytes of the MAC address.

    And in setup:
      setup_wifiMulti();
  */

  const uint32_t connectTimeoutMs = 5000;
  byte mac[6];                      //// the MAC address of your Wifi shield

  Serial.println(F("Connecting to WiFi"));

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);

  // Register multi WiFi networks
  // Defined in kaywinnet.h
#ifndef Kaywinnet
#include "Kaywinnet.h"
#endif
  wifiMulti.addAP(MY_SSID, MY_PASSWORD);
//  wifiMulti.addAP(IOT_SSID, IOT_PASSWORD);
//  wifiMulti.addAP(GUEST_SSID, GUEST_PASSWORD);
    

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
    //showNoWifi();
    timeDisplay.writeDigitRaw(0, letterN);
    timeDisplay.writeDigitRaw(1, letterO);
    timeDisplay.writeDigitRaw(2, 0x0);
    timeDisplay.writeDigitRaw(3, 0x0);
    timeDisplay.writeDisplay();

    bgDisplay.writeDigitRaw(0, letterW);
    bgDisplay.writeDigitRaw(1, letterI);
    bgDisplay.writeDigitRaw(2, letterF);
    bgDisplay.writeDigitRaw(3, letterI);
    bgDisplay.writeDisplay();
    delay(2500);

    return 1;
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

  return 0;
}
