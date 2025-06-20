#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define TRIG_PIN 5
#define ECHO_PIN 18
#define ALERT_LED 2

/********* WiFi Settings *********/
char ssid[] = "Wokwi-GUEST";
char password[] = "";

/********* Adafruit IO Settings *********/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "bharath23"
#define AIO_KEY         ""

// WiFi and MQTT clients
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feeds
Adafruit_MQTT_Publish waterLevelFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/water-level");
Adafruit_MQTT_Publish floodStatusFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/flood-status");

const int floodThreshold = 20;

long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void MQTT_connect() {
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    Serial.print("MQTT connect failed, retrying in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println("Connected to Adafruit IO");
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ALERT_LED, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected");
}

void loop() {
  MQTT_connect();

  long distance = measureDistance();
  Serial.print("Measured Distance (cm): ");
  Serial.println(distance);

  // Send water level (0-100)
  waterLevelFeed.publish(distance);

  if (distance < floodThreshold) {
    digitalWrite(ALERT_LED, HIGH);
    floodStatusFeed.publish("ON");
    Serial.println("⚠️ Flood Alert Sent!");
  } else {
    digitalWrite(ALERT_LED, LOW);
    floodStatusFeed.publish("OFF");
    Serial.println("✅ Status Safe");
  }

  delay(5000); // every 5 seconds
}
