
/*
   In setup()
   // Set the MQTT server and keep-alive interval
   Client.setServer(MQTT_SERVER, 1883);    //Server details for pubsub.
   Client.setKeepAlive(60);  // Set the keep-alive interval (in seconds)
*/
//************************** reconnect to MQTT broker ********************************
void reconnect() {
  // Loop until we're reconnected
  while (!Client.connected())
  {
    Serial.print(F("Attempting MQTT connection..."));


    //Test
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (Client.connect(clientId.c_str(), "mqttuser", "nudist")) {

      ///    // Attempt to connect
      ///    if (Client.connect(hostName, "mqttuser", "nudist")){
      ///      Serial.println(F("connected"));
      //End Test


      // Subscribe or resubscribe to topics
      Serial.println();

      Client.subscribe(cmndTopic);
      Serial.print(F("Subscribing to "));
      Serial.println(cmndTopic);

      Client.subscribe(bgTopic);
      Serial.print(F("Subscribing to "));
      Serial.println(bgTopic);

      Client.subscribe(trendTopic);
      Serial.print(F("Subscribing to "));
      Serial.println(trendTopic);

///      Client.subscribe(dateTopic);
///      Serial.print(F("Subscribing to "));
///      Serial.println(dateTopic);

      Client.subscribe(timeTopic);
      Serial.print(F("Subscribing to "));
      Serial.println(timeTopic);

      Client.subscribe(brightTopic);
      Serial.print(F("Subscribing to "));
      Serial.println(brightTopic);

      Client.subscribe(batteryTopic);
      Serial.print(F("Subscribing to "));
      Serial.println(batteryTopic);

      Client.subscribe(chargeTopic);
      Serial.print(F("Subscribing to "));
      Serial.println(chargeTopic);



      Serial.println(F("====================="));

    } else {
      //digitalWrite(ledPin, ledOFF);
      Serial.print(F("failed, rc="));
      Serial.print(String(Client.state()));
      Serial.println(F(" try again in 5 seconds"));
      delay(5000);

      /* Possible values for client.state()
         #define MQTT_CONNECTION_TIMEOUT     -4
         #define MQTT_CONNECTION_LOST        -3
         #define MQTT_CONNECT_FAILED         -2
         #define MQTT_DISCONNECTED           -1
         #define MQTT_CONNECTED               0
         #define MQTT_CONNECT_BAD_PROTOCOL    1
         #define MQTT_CONNECT_BAD_CLIENT_ID   2
         #define MQTT_CONNECT_UNAVAILABLE     3
         #define MQTT_CONNECT_BAD_CREDENTIALS 4
         #define MQTT_CONNECT_UNAUTHORIZED    5
      */
    }
  }
}
