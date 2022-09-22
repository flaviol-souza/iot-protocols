#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

#define PINLED 14

const char* ssid = "ifsp-ibge-1";
const char* password = "ifspcatanduva";

void callback_light(CoapPacket &packet, IPAddress ip, int port);

WiFiUDP udp;
Coap coap(udp);

bool LEDSTATE;

void callback_light(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Light] ON/OFF");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  String message(p);

  if (message.equals("0"))
    LEDSTATE = false;
  else if(message.equals("1"))
    LEDSTATE = true;
      
  if (LEDSTATE) {
    digitalWrite(PINLED, HIGH) ; 
    coap.sendResponse(ip, port, packet.messageid, "1");
  } else { 
    digitalWrite(PINLED, LOW) ; 
    coap.sendResponse(ip, port, packet.messageid, "0");
  }
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
  LEDSTATE = true;
  
  Serial.println("Setup Callback Light");
  coap.server(callback_light, "light");

  coap.start();
}

void loop() {
  delay(1000);
  coap.loop();
}
