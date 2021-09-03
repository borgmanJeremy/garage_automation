#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "passwords.h"

const char ssid[] = "its_a_trap";
const char pass[] = WIFI_PASSWORD;

WiFiClient net;
MQTTClient client;

int id4_door_gpio = 5;
int volt_door_gpio = 4;

void connect()
{
  Serial.print("Connecting to WiFi");

  WiFi.forceSleepWake();
  delay(1);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");

  Serial.println("Connecting to MQTT");
  client.begin("192.168.1.219", net);
  while (!client.connect("garage_sensor", "jeremy", HA_PASSWORD))
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");
}

void setup()
{

  Serial.begin(115200);
  pinMode(id4_door_gpio, INPUT_PULLUP);
  pinMode(volt_door_gpio, INPUT_PULLUP);

  WiFi.mode(WIFI_OFF);

  WiFi.forceSleepBegin();
  delay(1);
}

int door_1 = 0;
int door_2 = 0;

void loop()
{
  door_1 = digitalRead(id4_door_gpio);
  door_2 = digitalRead(volt_door_gpio);

  connect();
  Serial.print("Door 1: ");
  if (door_1)
  {
    client.publish("/garage/id4/door/state", String("Open"));
    Serial.println("open");
  }
  else
  {
    client.publish("/garage/id4/door/state", String("Closed"));
    Serial.println("closed");
  }

  Serial.print("Door 2: ");
  if (door_2)
  {
    client.publish("/garage/volt/door/state", String("Open"));
    Serial.println("open");
  }
  else
  {
    client.publish("/garage/volt/door/state", String("Closed"));
    Serial.println("closed");
  }

  delay(5000);
}
