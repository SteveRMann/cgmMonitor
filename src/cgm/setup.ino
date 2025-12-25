
// **************************************** SETUP *************************************
void setup() {
  beginSerial();
  Serial.println();
  Serial.println(__FILE__);

  pinMode(ledPin5, OUTPUT);                  // ledPin5 is an indicator that an MQTT message was received.
  pinMode(ledPin6, OUTPUT);                  // ledPin6 is an indicator that an MQTT message was received.  analogWrite(ledPin6, 0);

  setup_wifi();
  ///start_OTA();

  // Set the MQTT server and keep-alive interval
  //Client.setServer(myMQTT_SERVER, 1883);    //Server details for pubsub.
  Client.setServer(HA_MQTT_SERVER, 1883);    //Server details for pubsub.
  Client.setKeepAlive(60);  // Set the keep-alive interval (in seconds)

  Client.setCallback(callback);

  Serial.println("...............");
  Serial.print(F("Node name=   "));
  Serial.println(NODENAME);
  Serial.print(F("statusTopic= "));
  Serial.println((String(statusTopic)));
  Serial.print(F("rssiTopic=   "));
  Serial.println((String(rssiTopic)));
  Serial.print(F("bgTopic=     "));
  Serial.println((String(bgTopic)));
  Serial.print(F("trendTopic=  "));
  Serial.println((String(trendTopic)));
  Serial.print(F("timeTopic=   "));
  Serial.println((String(timeTopic)));
  Serial.print(F("batteryTopic=   "));
  Serial.println((String(batteryTopic)));
  Serial.print(F("chargeTopic=   "));
  Serial.println((String(chargeTopic)));
  Serial.println(F("..............."));

  bgDisplay.begin(0x70);                             // bgDisplay is on I2C address 70
  timeDisplay.begin(0x71);                           // timeDisplay is on I2C address 71
  bgDisplay.setBrightness(brightness);
  timeDisplay.setBrightness(brightness);


  analogWrite(ledPin5, ledBrightness);
  analogWrite(ledPin6, ledBrightness);
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
  analogWrite(ledPin5, 0);
  analogWrite(ledPin6, 0);

   randomSeed(micros());
}
