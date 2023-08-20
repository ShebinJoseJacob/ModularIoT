/*
 WizFi360 example: WebClientRepeating

 This sketch connects to a web server and makes an HTTP request
 using an Arduino WizFi360 module.
 It repeats the HTTP call each 10 seconds.
*/

#include "WizFi360.h"

// setup according to the device you use
#define WIZFI360_EVB_PICO

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
#if defined(ARDUINO_MEGA_2560)
SoftwareSerial Serial1(6, 7); // RX, TX
#elif defined(WIZFI360_EVB_PICO)
SoftwareSerial Serial2(6, 7); // RX, TX
#endif
#endif

/* Baudrate */
#define SERIAL_BAUDRATE   115200
#if defined(ARDUINO_MEGA_2560)
#define SERIAL1_BAUDRATE  115200
#elif defined(WIZFI360_EVB_PICO)
#define SERIAL2_BAUDRATE  115200
#endif

/* Wi-Fi info */
char ssid[] = "DARKMATTER";       // your network SSID (name)
char pass[] = "8848191317";   // your network password

int status = WL_IDLE_STATUS;  // the Wifi radio's status

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 1000L; // delay between updates, in milliseconds

int pir = 0;

// Initialize the Ethernet client object
WiFiClient client;

void setup() {
  pinMode(pir, INPUT);
  // initialize serial for debugging
  Serial.begin(SERIAL_BAUDRATE);
  // initialize serial for WizFi360 module
  #if defined(ARDUINO_MEGA_2560)
    Serial1.begin(SERIAL1_BAUDRATE);
  #elif defined(WIZFI360_EVB_PICO)
    Serial2.begin(SERIAL2_BAUDRATE);
  #endif
    // initialize WizFi360 module
  #if defined(ARDUINO_MEGA_2560)
    WiFi.init(&Serial1);
  #elif defined(WIZFI360_EVB_PICO)
    WiFi.init(&Serial2);
  #endif

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  
  printWifiStatus();
}

void loop() {
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}

// this method makes a HTTP connection to the server
void httpRequest() {
  int pirStatus = digitalRead(pir);
  IPAddress server = WiFi.localIP();
  server[3]=6;
  Serial.println(server);
  Serial.println();
    
  if (client.connect(server, 5000)) {
    Serial.println("Connecting...");
    String msg;
    if (pirStatus == 1){
      msg = "PIR1:TRUE";
    }
    else{
      msg = "PIR1:FALSE";
    }
    
    Serial.println(String(pirStatus)+msg);
    // send the HTTP PUT request
    client.println(msg);
    client.stop();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
    delay(100);
  }
  
}

void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
