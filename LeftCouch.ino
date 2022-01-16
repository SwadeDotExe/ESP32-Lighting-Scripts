//#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "RHIT-OPEN";
const char* password = "";

String hostname = "leftCouchESP";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "monitoringserver.reshall.rose-hulman.edu";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

#define RELAY 0

void setup() {
  Serial.begin(115200);
  setup_wifi();

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.setHostname(hostname.c_str()); //define hostname
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "LeftCouchLight") {
    Serial.print("Changing output to ");

     if (messageTemp == "true") {
      Serial.println("Turning off Red LEDs");
       digitalWrite(RELAY, LOW);
    }
    else if (messageTemp == "false") {
      Serial.println("Turning off Red LEDs");
       digitalWrite(RELAY, HIGH);
    }
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("leftCouchESP")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("LeftCouchLight");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  delay(25);
  client.loop();
}
