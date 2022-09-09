#include <DHT.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define DHTPIN A1
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 88);      

EthernetClient ethClient;
PubSubClient client(ethClient);

void setup()
{
  Serial.begin(9600);
  delay(3000);
  Ethernet.begin(mac, ip);
  delay(3000);
  client.setClient(ethClient);
  client.setServer("192.168.0.5", 1883);
  dht.begin();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT");
  }
  else
  {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }

  if (client.connect("arduinoClient")) {
      Serial.println("connected");
      char temp[8];
      dtostrf(t, 6, 2, temp);
      int length = strlen(temp);
      client.publish("topic/temp", (byte*)temp, length);

      char hum[8];
      dtostrf(h, 6, 2, hum);
      length = strlen(hum);
      client.publish("topic/hum", (byte*)hum, length);
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");

  }
  delay(5000);
}
