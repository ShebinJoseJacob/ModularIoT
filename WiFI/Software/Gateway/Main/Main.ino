#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SPI.h>

/* Network */
#define SERVER_PORT 5000
#define MAX_CLIENT  8
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

const char* UID="KIT ID";

EthernetServer server(SERVER_PORT);
void print_network_info(void);

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttClientId = "myClientIDs";

// Array to store connected clients
EthernetClient clients[MAX_CLIENT];
EthernetClient client;

void setup() {
  Serial.begin(115200);
  Serial3.setRx(PC11);
  Serial3.setTx(PC10);

  Ethernet.begin(mac);
  IPAddress localIP = Ethernet.localIP();
  IPAddress modifiedIP(localIP[0], localIP[1], localIP[2], 6);
  Ethernet.begin(mac, modifiedIP);
  print_network_info();

  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(subscribeReceive);

  connectToMQTTServer();
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectToMQTTServer();
  }
  mqttClient.loop();

  // Handle new client connections
  EthernetClient newClient = server.accept();
  if (newClient) {
    for (byte i = 0; i < MAX_CLIENT; i++) {
      if (!clients[i]) {
        Serial.printf("We have a new client # %d\r\n", i);
        clients[i] = newClient;
        break;
      }
    }
  }

  // Handle client data and publish to MQTT
  for (byte i = 0; i < MAX_CLIENT; i++) {
    if (clients[i] && clients[i].available() > 0) {
      byte buffer[80];
      int count = clients[i].read(buffer, 80);
      //Serial.write(buffer, count);

      if (count>0) {
        String topic = UID;
        String datum = String((char*)buffer);
        datum.trim();
        Serial.println(datum);
        if (mqttClient.publish(topic.c_str(), datum.c_str())) {
            //Serial.println(String((char*)buffer)+":");
            Serial.println("Publish message success");
            //break;
          } else {
            Serial.println("Publish message failed");
            //break;
          }
      }
    }

  }


  // Check for disconnected clients and remove them
  for (byte i = 0; i < MAX_CLIENT; i++) {
    if (clients[i] && !clients[i].connected()) {
      Serial.printf("Client #%d disconnected\r\n", i);
      clients[i].stop();
    }
  }
}

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
  Serial.printf(": %d\r\n", SERVER_PORT);
  Serial.println("-------------------------------------------------");
}

void subscribeReceive(char* topic, byte* payload, unsigned int length) {
    for (byte i = 0; i < MAX_CLIENT; i++) {
      client = clients[i];
      if (client && client.connected()) {
        client.write(payload, length);
      }
    }
}
