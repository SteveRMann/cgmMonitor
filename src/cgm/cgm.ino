#define SKETCH "cgm.ino"
#define VERSION "4.01"           // Four characters
#define hostPrefix "CGM-"        // Six characters max

// The espClient name must be unique. Uncomment one
// Make sure the same port is selected in tools.
// MAC Addresses:
//#define Mac0C6E00  //FamilyRoom
//#define Mac5B668A  //Office
//#define Mac69789C  //Desktop
#define MacADA75B  //MBR
//#define MacD010E9  //Kitchen ??


/*****

  Github Repository local location: E:\cgmMonitor

  Version 3.30 (11/10/21) Added rssi display on startup
  Version 3.31 (11/11/21) Moved functions from the main ino file
  Version 4.00 (03/01/22) Changed WiFi to WiFiMulti
  Version 4.01 (03/20/22) Added 'miranda' to WiFimulti

**PROBLEMS**
  I am using millis to test for stale data timeout, but the code also includes the ticker.h library.
  Which is more useful?

  IDE settings:
    NodeMCU 1.0 (ESP-12E Module), 4M, 1M SPIFFS
    Wemos D1 R1 Mini

  ---------------------------------------------------------------------------
  The CGM data comes from a Node-Red flow.  A 'poll state' node reads the sensor.yaml 'platform:rest' which gets my CGM data from
  https://my-g6.herokuapp.com/api/v1/entries/current.json.  My notes are here: 'Heroku APIs.txt'

  The instructions are here: https://community.home-assistant.io/t/dexcom-api/94730/5
  There are some details in the comments at the top of 'sensors.yaml'

  --------------------------------------------------------------------
  This is the API endpoint I am using in Home Assistant (sensors.yaml).
  https://my-g6.herokuapp.com/api/v1/entries/current.json

  It returns:
  [
     {
        "_id":"5e45b2b70ba41f5790624cc0",
        "sgv":173,
        "date":1581626007000,
        "dateString":"2020-02-13T20:33:27.000Z",
        "trend":4,
        "direction":"Flat",
        "device":"share2",
        "type":"sgv"
     }
  ]


*****/



// ****************************** Includes  ******************************
#include <ArduinoOTA.h>
#include <ESP8266WiFiMulti.h>
//#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <TimeLib.h>
//#include <Time.h>
#include "Kaywinnet.h"

//--------------- WiFiMulti declarations ---------------
ESP8266WiFiMulti wifiMulti;

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 5000;




//--------------- timeObj declarations ---------------
#include <dlay.h>
const long int SECONDS = 1000;        //ms per second
int rssiTime = 15 * SECONDS;          //How often to publish RSSI
dlay rssiTimer(rssiTime, true);       //How often in ms to publish RSSI
//--------------- end timeObj declarations ---------------


#include <Ticker.h>
Ticker staleTicker;                   //Ticker object for the stale ticker flag.

#define DEBUG true                    //set to true for debug output, false for no debug ouput
#define Serial if(DEBUG)Serial

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
const char *dateTopic = NODENAME "/date";               // Sent as dateString from Node Red
const char *timeTopic = NODENAME "/time";               // Time from Node-Red, hh:mm every ten seconds.
const char *brightTopic = NODENAME "/bright";           // Allows the setting of the display brightness

//String lastTime = "";                                   //Last time displayed


// setup_wifi vars
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
unsigned long ledTime = 250;          // How long the LED should be on. Resets with each incoming MQTT message.

int crashTimer;                       // Used to reboot if no new time value is received in timeTime ms.
unsigned long crashTimeout = 120;     // How long to wait for new data before rebooting, in seconds.

int bgTimestamp = 0;                  // Used to detect if bg reading is not received in ten minutes
const int staleTime = 600;            // Number of seconds in ten minutes.
String sensorDate;
String lastSensorDate;                // The last sendor date.  If this is unchanged after ten minutes, then the data is stale.
bool staleFlag = false;               // Flag is set if no new sensor data is received in ten minutes
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
// Initializes the espClient. The espClient name must be unique
#ifdef MacD010E9 //Kitchen
WiFiClient espCGM_10E9;
PubSubClient client(espCGM_10E9);
#endif

#ifdef Mac5B668A  //Office
WiFiClient espCGM_668A;
PubSubClient client(espCGM_668A);
#endif

#ifdef MacADA75B  //MBR
WiFiClient espCGM_A75B;
PubSubClient client(espCGM_A75B);
#endif

#ifdef Mac0C6E00  //FamilyRoom
WiFiClient espCGM_6E00;
PubSubClient client(espCGM_6E00);
#endif

#ifdef Mac69789C  //Desktop
WiFiClient espCGM_789C;
PubSubClient client(espCGM_789C);
#endif

/*
   /MAC Addresses:
   D010E9  Kitchen
   5B668A  Office
   ADA75B  MBR
   0C6E00  FamilyRoom
   69789C  Desktop
*/
