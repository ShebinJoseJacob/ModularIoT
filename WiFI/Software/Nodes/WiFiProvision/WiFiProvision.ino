#include "WizFi360.h"
#include "provision.h"
#include <EEPROM.h>

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

char ap_ssid[] = "wiznet";
char ap_pass[] = "0123456789";

int status = WL_IDLE_STATUS;

WiFiServer server(80);

char ssid[32]; // Max SSID length
char pass[64]; // Max password length
const int ssidAddress = 0;
const int passAddress = ssidAddress + sizeof(ssid);


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


  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  EEPROM.get(ssidAddress, ssid);
  EEPROM.get(passAddress, pass);

  Serial.print("Attempting to start AP ");
  Serial.println(ssid);

  if (ssid[0] == '\0' || pass[0] == '\0') {
    Serial.println(ssid);
    startAPMode();
  } else {
    connectToWiFi();
  }
  printWifiStatus();

  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        if (request.endsWith("\r\n\r\n")) {
          if (request.indexOf("GET /submit") != -1) {
            handleFormSubmission(client, request);
          } else {
            sendHtmlForm(client);
          }
          break;
        }
      }
    }

    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}

void sendHtmlForm(WiFiClient client) {
  client.print(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"
    "\r\n");
  client.println(home_0);
  client.println(home_1);
  client.println(home_2);
  client.println(home_3);

}

void handleFormSubmission(WiFiClient client, String request) {
  String ssidValue, passwordValue;
  getSsidAndPasswordFromRequest(request, ssidValue, passwordValue);

  Serial.println("Entered SSID: " + ssidValue);
  Serial.println("Entered Password: " + passwordValue);

  // Store credentials in EEPROM
  ssidValue.toCharArray(ssid, sizeof(ssid));
  passwordValue.toCharArray(pass, sizeof(pass));
  Serial.println(ssid);
  Serial.println(pass);
  EEPROM.put(ssidAddress, ssid);
  EEPROM.put(passAddress, pass);
  EEPROM.commit();

  Serial.println("Values in EEPROM:");
  Serial.println("SSID: " + String(ssid));
  Serial.println("Password: " + String(pass));

  client.print(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"
    "\r\n");
  client.print(response_0);

  //resetWizFi360();
  delay(5000);

resetWiFiModule();
  delay(1000);
  connectToWiFi();
}

void startAPMode() {
  // Start access point and set up web server
  WiFi.beginAP(ap_ssid, 10, ap_pass, ENC_TYPE_WPA2_PSK);
  IPAddress localIp(192, 168, 2, 100);
  WiFi.configAP(localIp);
  
  // Set up the web server for configuration
  server.begin();
  Serial.println("Access point started");
  Serial.println("Server started");
}

void connectToWiFi() {
  WiFi.disconnect(); // Disconnect from any previous connections
  Serial.println("SSID: "+String(ssid));
  Serial.println("Password: "+String(pass));
  WiFi.begin(ssid, pass);
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 10) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi");
    startAPMode(); // Enter AP mode if failed to connect
  }
}

void getSsidAndPasswordFromRequest(String request, String &ssidValue, String &passwordValue) {
  int ssidIndex = request.indexOf("ssid=");
  int passwordIndex = request.indexOf("password=");
  int spaceIndex = request.indexOf("HTTP/1.1");

  if (ssidIndex != -1 && passwordIndex != -1 && spaceIndex != -1) {
    ssidValue = request.substring(ssidIndex + 5, passwordIndex - 1); // 5 is the length of "ssid="
    passwordValue = request.substring(passwordIndex + 9, spaceIndex-1); // 9 is the length of "password="
  } else {
    ssidValue = "";
    passwordValue = "";
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

void resetWiFiModule() {
  WiFi.disconnect();
  delay(1000); // Wait for module to fully disconnect
  WiFi.init(&Serial2); // Initialize WiFi module again

  Serial.println("WiFi module reset");
}
