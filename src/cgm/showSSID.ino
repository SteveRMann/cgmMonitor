/********************************* showSSID() ********************************/
void showSSID() {
  // Shows the SSID.

  char ssid[16];
  WiFi.SSID().toCharArray(ssid, 16);

  int ssidChars = sizeof(ssid) / sizeof(ssid[0]);
  //ssidChars = ssidChars - 1;                          // Remove one for the line end.

  Serial.println(F("__________________"));
  Serial.print(F("ssid[] = '"));
  Serial.print(ssid);
  Serial.println(F("'"));
  Serial.print(F("ssidChars = "));
  Serial.println(ssidChars);
  Serial.println(F("__________________"));


  timeDisplay.writeDigitAscii(0, ssid[0]);
  timeDisplay.writeDigitAscii(1, ssid[1]);
  timeDisplay.writeDigitAscii(2, ssid[2]);
  timeDisplay.writeDigitAscii(3, ssid[3]);
  timeDisplay.writeDisplay();

  bgDisplay.writeDigitAscii(0, ssid[4]);
  bgDisplay.writeDigitAscii(1, ssid[5]);
  bgDisplay.writeDigitAscii(2, ssid[6]);
  bgDisplay.writeDigitAscii(3, ssid[7]);
  bgDisplay.writeDisplay();

  delay(1500);

}
