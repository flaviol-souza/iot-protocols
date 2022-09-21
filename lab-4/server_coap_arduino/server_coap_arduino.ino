#include <Ethernet.h>
#include <EthernetUdp.h>
#include <coap-simple.h>

#define LEDP 10

bool LEDSTATE;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 88);

EthernetUDP Udp;
Coap coap(Udp);

void callback_light(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Light] ON/OFF");
  
  // send response
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  String message(p);

  if (message.equals("0"))
    LEDSTATE = false;
  else if(message.equals("1"))
    LEDSTATE = true;
      
  if (LEDSTATE) {
    digitalWrite(LEDP, HIGH) ; 
    coap.sendResponse(ip, port, packet.messageid, "1");
  } else { 
    digitalWrite(LEDP, LOW) ; 
    coap.sendResponse(ip, port, packet.messageid, "0");
  }
}

void setup() {
  Serial.begin(9600);
  delay(3000);
  
  Ethernet.begin(mac, ip);
  Serial.print("Server IP address: ");
  Serial.print(Ethernet.localIP());

  pinMode(LEDP, OUTPUT);
  digitalWrite(LEDP, HIGH);
  LEDSTATE = true;
  
  Serial.println("Setup Callback Light");
  coap.server(callback_light, "light");

  coap.start();
}

void loop() {
  delay(1000);
  coap.loop();
}
