#define SKETCH __FILE__
#define VERSION "5.05"           // Four characters
#define hostPrefix "CGM-"        // Six characters max

#define DEBUG true               // set to true for debug output, false for no debug ouput
#define Serial if(DEBUG)Serial

/*****
  Github Repository local location: E:\cgmMonitor

  Version 3.30 (11/10/21) Added rssi display on startup
  Version 3.31 (11/11/21) Moved functions from the main ino file
  Version 4.00 (03/01/22) Changed WiFi to WiFiMulti
  Version 4.01 (03/20/22) Added 'miranda' to WiFimulti
  Version 4.20 (12/20/22) Added test to setup_wifiMulti (success or fail)
  Version 5.00 (12/13/23) Removed ticker.
  Version 5.01 (12/13/23) Removed crashtimer
  Version 5.02 (12/05/25) Reverted back to MQTT broker at 192.168.1.124
  Version 5.03 (12/13/25) Added topics for battery charge level and state
  Version 5.04 (12/18/25) MQTT broker is 192.168.1.57
  Version 5.05 (12/18/25) Using a single "WiFiClient espClientx;" for all boards
  
  IDE settings:
    NodeMCU 1.0 (ESP-12E Module), 4M, 1M SPIFFS
    Wemos D1 R1 Mini

  ---------------------------------------------------------------------------
  The CGM data comes from a Node-Red flow.
  The instructions are here: https://community.home-assistant.io/t/dexcom-api/94730/5
  There are some details in the comments at the top of 'sensors.yaml'
  --------------------------------------------------------------------
*****/


// ****************************** Includes  ******************************
#include <ArduinoOTA.h>
#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <TimeLib.h>
#include "Kaywinnet.h"


//--------------- timeObj declarations ---------------
#include <dlay.h>
const long int SECONDS = 1000;        //ms per second


#include <Wire.h>                     // Wire.h is the library for communicating with I2C devices
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"



// ****************************** Globals  ******************************
#define NODENAME "cgm"
const char *statusTopic = NODENAME "/status";
const char *cmndTopic = NODENAME "/cmnd";
const char *rssiTopic = NODENAME "/rssi";
const char *bgTopic = NODENAME "/bg";
const char *trendTopic = NODENAME "/trend";
const char *timeTopic = NODENAME "/time";               // Time from Node-Red, hh:mm every ten seconds.
const char *brightTopic = NODENAME "/bright";           // Allows the setting of the display brightness
const char *batteryTopic = NODENAME "/battery";         // Charge percent of phone battery
const char *chargeTopic = NODENAME "/charge";           // Charge or discharge of phone battery


//---------------- setup_wifi vars ----------------
char macBuffer[24];       // Holds the last three digits of the MAC, in hex.
char hostNamePrefix[] = hostPrefix;
char hostName[12];        // Holds hostNamePrefix + the last three bytes of the MAC address.


//---------------- GPIO ------------------------------
const int ledPin = D5;
#define LEDPIN            //When defined, the LED is flashed while WiFi is connecting.




//---------------- Global Vars -------------------------

#include "segmentMap.h"               // Get the segment definitions 

int lastDim = HIGH;
int lastBright = HIGH;
byte tempI;
const bool ledOFF = LOW;
const bool ledON = HIGH;
unsigned long ledMillis;              // Used to time the LED on period
unsigned long ledTime = 25;           // How long the LED should be on in ms. Resets with each incoming MQTT message.

const int staleTime = 120;            // Number of seconds in two minutes.
String sensorDate;
String lastSensorDate;                // The last sendor date.  If this is unchanged after ten minutes, then the data is stale.
bool staleFlag = false;               // Flag is set if no new sensor data is received in two minutes
byte brightness = 5;                  // Display brightness, initially dim




//----------------------------------- Initialize stuff ----------------------------------------
//
//************* Initialize the Alphanumeric displays *************
// Make two instances of the Adafruit_AlphaNum4 object
// There's no arguments or pins because the first display uses the fixed I2C pins and address 0x70
// The I2C address used in timeDisplay will be set with timeDisplay.begin
Adafruit_AlphaNum4 bgDisplay = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 timeDisplay = Adafruit_AlphaNum4();



// ************* Initialize the espClient ***********************
// 'espClientx' is the name of a variable. It disappears at compile time.
WiFiClient espClientx;
PubSubClient Client(espClientx);
