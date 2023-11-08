// ******** dbugs ********
// Function to display a string for debugging.
void dbugs(const char *s, const char *v) {
  //Show a string variable. Enter with the string description and the string.
  //Example dbugs("My String= ",myString);
  Serial.print(s);
  Serial.print (F("\""));
  Serial.print(v);
  Serial.println(F("\""));
}



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



// ********** Function to publish RSSI over MQTT **********
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



// ********** Callback Function to display "Stale Data" **********
void staleTick()
{
  staleFlag = true;
  Serial.println(F("Staleflag set"));
}

void restartStaleTimer() {
  lastSensorDate = sensorDate;          // Save the new sensor date and
  staleFlag = false;                    // Clear the stale flag if it's on.
  staleTicker.detach();                 // Restart the stale timer
  staleTicker.attach(120, staleTick);   // Call the staleTick function after two minutes.
}
