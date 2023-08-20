#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <HardwareSerial.h>

HardwareSerial Serial2(PA3,PD5);

String UID = "";

/* Network */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttClientId = "myClientIDs";


void connectToMQTTServer() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT server...");
    if (mqttClient.connect(mqttClientId)) {
      Serial.println("Connected");
      mqttClient.subscribe(UID);
    } else {
      Serial.print("Failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void reconnectToMQTTServer() {
  if (!mqttClient.connected()) {
    Serial.println("MQTT connection lost. Reconnecting...");
    connectToMQTTServer();
  }
}

void print_network_info(void) {
  byte print_mac[] = { 0, };
  Serial.println("\r\n-------------------------------------------------");
  Serial.printf("MAC        : ");
  Ethernet.MACAddress(print_mac);
  for (byte i = 0; i < 6; i++) {
    Serial.print(print_mac[i], HEX);
    if (i < 5) {
      Serial.print(":");
    }
  }
  Serial.println();
  Serial.printf("IP         : ");
  Serial.print(Ethernet.localIP());
  Serial.println("-------------------------------------------------");
}


void setup() {
  Serial.begin(115200);
  Serial3.setRx(PC11);
  Serial3.setTx(PC10);
  Serial2.begin(9600);

  Ethernet.begin(mac);
  IPAddress localIP = Ethernet.localIP();
  IPAddress modifiedIP(localIP[0], localIP[1], localIP[2], 5);
  Ethernet.begin(mac, modifiedIP);
  print_network_info();

  mqttClient.setServer(mqttServer, mqttPort);
  //mqttClient.setCallback(subscribeReceive);
  mqttClient.subscribe(UID);
  connectToMQTTServer();
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectToMQTTServer();
  }
  mqttClient.loop();
  
  if (Serial2.available() > 0) {
        String sensorData = Serial2.readStringUntil('\n');
        sensorData.trim(); // Remove leading/trailing whitespace characters
        String topic = UID;
        String datum = sensorData;
        
        if (sensorData.length() > 0) {
          if (mqttClient.publish(topic.c_str(), datum.c_str())) {
            Serial.println(datum.c_str());
            Serial.println("Publish message success");
          } else {
            Serial.println("Publish message failed");
          }
        }
      }
    delay(1);
  }
