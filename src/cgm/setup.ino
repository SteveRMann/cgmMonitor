
// **************************************** SETUP *************************************
void setup() {
  beginSerial();

  pinMode(ledPin, OUTPUT);                  // LedPin is an indicator that an MQTT message was received.
  digitalWrite(ledPin, ledOFF);

  setup_wifi();
  start_OTA();

  // Set the MQTT server and keep-alive interval
  Client.setServer(MQTT_SERVER, 1883);    //Server details for pubsub.
  Client.setKeepAlive(60);  // Set the keep-alive interval (in seconds)

  Client.setCallback(callback);

  Serial.println("...............");
  Serial.print(F("Node name= "));
  Serial.println(NODENAME);
  Serial.print(F("statusTopic= "));
  Serial.println((String(statusTopic)));
  Serial.print(F("rssiTopic= "));
  Serial.println((String(rssiTopic)));
  Serial.print(F("bgTopic= "));
  Serial.println((String(bgTopic)));
  Serial.print(F("trendTopic= "));
  Serial.println((String(trendTopic)));
  Serial.print(F("dateTopic= "));
  Serial.println((String(dateTopic)));
  Serial.print(F("timeTopic= "));
  Serial.println((String(timeTopic)));
  Serial.println(F("..............."));


  bgDisplay.begin(0x70);                             // bgDisplay is on I2C address 70
  timeDisplay.begin(0x71);                           // timeDisplay is on I2C address 71
  bgDisplay.setBrightness(brightness);
  timeDisplay.setBrightness(brightness);


  digitalWrite(ledPin, ledON);
  //  lightAllSegments();       // Light up all segments
  //  showNumbers();            // Show the digits, 0-9
  //  showLetters();            // Show all letters.
  showVersion();                // Show the software Version number
  showSSID();                   // Show the connected SSID
  showMac();                    // Show the last two byte-pairs of the MAC address.
  showRssi();                   // Show the RSSI

  delay(1000);
  timeDisplay.clear();
  timeDisplay.writeDisplay();
  bgDisplay.clear();
  bgDisplay.writeDisplay();
  digitalWrite(ledPin, ledOFF);

  crashTimer = now();          // Reboot if the data is not updated in crashTimeout seconds.
}
