/*
 WizFi360 example: WebClientRepeating

 This sketch connects to a web server and makes an HTTP request
 using an Arduino WizFi360 module.
 It repeats the HTTP call each 10 seconds.
*/

#include "WizFi360.h"
#include <DHT.h>

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

// Sensor
#define DHT_PIN 28
DHT dht(DHT_PIN, DHT11);

// Initialize the Ethernet client object
WiFiClient client;

void setup() {
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
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

  printWifiStatus();

  dht.begin();
}

void loop() {
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}

// this method makes an HTTP connection to the server
void httpRequest() {
  IPAddress server = WiFi.localIP();
  server[3] = 6;
  Serial.println(server);
  Serial.println();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.println();

  // close any connection before sending a new request
  // this will free the socket on the WiFi shield
  //client.stop();

  // if there's a successful connection
  if (client.connect(server, 5000)) {
    Serial.println("Connected");
    String TempData = "TEMP:"+String(temperature);
    String HumidityData = "HUMIDITY:"+String(humidity);
    Serial.println(TempData);
    // send the HTTP PUT request
    client.print(TempData);
    client.println();
    client.print(HumidityData);
    Serial.println("Message Sent");
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
