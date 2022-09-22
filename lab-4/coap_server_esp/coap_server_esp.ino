#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

#define PINLED 14

const char* ssid = "ifsp-ibge-1";
const char* password = "ifspcatanduva";

WiFiUDP udp;
Coap coap(udp);
char* statusLed = NULL;

void callback_light(CoapPacket &packet, IPAddress ip, int port);

void callback_light(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Light] ON/OFF");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  String message(p);   
  Serial.println(message);   
  if (message.equals("ON")){
    digitalWrite(PINLED, HIGH) ; 
    statusLed = "ON";
  } else if(message.equals("OFF")){ 
    digitalWrite(PINLED, LOW) ; 
    statusLed = "OFF";
  }

  coap.sendResponse(ip, port, packet.messageid, statusLed);
}

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(PINLED, OUTPUT);
  digitalWrite(PINLED, HIGH);
  statusLed = "ON";
    
  Serial.println("Setup Callback Light");
  coap.server(callback_light, "light");

  coap.start();
}

void loop() {
  delay(1000);
  coap.loop();
}
