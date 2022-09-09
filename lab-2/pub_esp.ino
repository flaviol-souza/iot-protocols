#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "GF_Wifi";
const char* password =  "asbanga2020";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  client.setServer("192.168.0.5", 1883);

  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    //Serial.printfln("The client ID : %s", client_id.c_str());
    if (client.connect(client_id.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}


void loop() {
  client.publish("topic/esp32", "Hello MQTT");
  delay(10000);
}
