#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "RHIT-OPEN";
const char* password = "";

String hostname = "LightingESP";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "monitoringserver.reshall.rose-hulman.edu";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const int RED = 25;
const int GREEN = 26;
const int BLUE = 27;
const int WHITE = 14;


void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(WHITE, OUTPUT);
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
  if (String(topic) == "CouchStrip") {
    Serial.print("Changing output to ");
    if (messageTemp == "Red=true") {
      Serial.println("Turning on Red LEDs");
      digitalWrite(RED, HIGH);
    }
    else if (messageTemp == "Red=false") {
      Serial.println("Turning off Red LEDs");
      digitalWrite(RED, LOW);
    }

    else if (messageTemp == "Blue=true") {
      Serial.println("Turning off Red LEDs");
      digitalWrite(BLUE, HIGH);
    }
    else if (messageTemp == "Blue=false") {
      Serial.println("Turning off Red LEDs");
      digitalWrite(BLUE, LOW);

    }

    else if (messageTemp == "Green=true") {
      Serial.println("Turning off Red LEDs");
      digitalWrite(GREEN, HIGH);
    }
    else if (messageTemp == "Green=false") {
      Serial.println("Turning off Red LEDs");
      digitalWrite(GREEN, LOW);
    }

    else if (messageTemp == "White=true") {
      Serial.println("Turning off Red LEDs");
      digitalWrite(WHITE, HIGH);
    }
    else if (messageTemp == "White=false") {
      Serial.println("Turning off Red LEDs");
      digitalWrite(WHITE, LOW);
    }
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("CouchStrip");
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
