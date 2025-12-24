#define SKETCH __FILE__
#define VERSION "6.00"           // Four characters
#define hostPrefix "CGM-"        // Six characters max

#define DEBUG true               // set to true for debug output, false for no debug ouput
#define Serial if(DEBUG)Serial

/*****
  Github Repository local location: E:\cgmMonitor

  Version 6.00 (12/22/25) Added topics for battery charge level and state
  
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
const char *chargeTopic = NODENAME "/charge";           // Phone battery, charge= true or false


//---------------- setup_wifi vars ----------------
char macBuffer[24];       // Holds the last three digits of the MAC, in hex.
char hostNamePrefix[] = hostPrefix;
char hostName[12];        // Holds hostNamePrefix + the last three bytes of the MAC address.


//---------------- LED Blink ------------------------------
const int ledPin = D6;
const int ledON  = HIGH;
const int ledOFF = LOW;
bool charging = false;                // Set in MQTT callback if the phone battery is charging
unsigned long chargeBlinkTimer = 0;
bool chargeBlinkState = false;   // false = LED OFF, true = LED ON
int batteryPercent = 0;               // Battery charge in percent


//---------------- Global Vars -------------------------

#include "segmentMap.h"               // Get the segment definitions 

int lastDim = HIGH;
int lastBright = HIGH;
byte brightness = 5;                  // Display brightness, initially dim

byte tempI;



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
