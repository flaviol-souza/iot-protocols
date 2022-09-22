
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <coap-simple.h>
#include <LiquidCrystal.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress dev_ip(192, 168, 40, 110);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
EthernetUDP Udp;
Coap coap(Udp);

String statusLed = "";

void callbackResponse(CoapPacket &packet, IPAddress ip, int port);

void callbackResponse(CoapPacket &packet, IPAddress ip, int port) {
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  Serial.print("[Resp. ");
  Serial.print(packet.type);
  Serial.print("] Status Led: ");
  Serial.println(p);
  statusLed = p;
}

void setup() {
  Serial.begin(9600);

  delay(3000);
  
  lcd.begin(16, 2);

  Ethernet.begin(mac,dev_ip);
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  
  Serial.println("Setup Response Callback");
  coap.response(callbackResponse);

  coap.start();
}

void loop() {
  Serial.print("Send Request Get Light -> ");
  coap.get(IPAddress(192, 168, 40, 101), 5683, "light");
  Serial.println(statusLed);
  delay(3000);
  if(statusLed == "ON"){
    Serial.println("Send Request Put OFF Light");
    coap.put(IPAddress(192, 168, 40, 101), 5683, "light", "OFF");
  } else {
    Serial.println("Send Request Put ON Light");
    coap.put(IPAddress(192, 168, 40, 101), 5683, "light", "ON");
  }
  delay(2000);
  coap.loop();
}
