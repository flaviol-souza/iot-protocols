#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <coap-simple.h>
#include <LiquidCrystal.h>
 
#define LEDP 9

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress dev_ip(192, 168, 40, 110);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void callback_response(CoapPacket &packet, IPAddress ip, int port);

EthernetUDP Udp;
Coap coap(Udp);

bool LEDSTATE;

void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
  lcd.clear();
  lcd.print("IFSP - CTD ");
  lcd.clear();
  lcd.print(p);
  lcd.setCursor(3, 0);
}

void setup() {
  Serial.begin(9600);

  delay(3000);
  
  lcd.begin(16, 2);

  Ethernet.begin(mac,dev_ip);
  Serial.print("My IP address: ");
  Serial.print(Ethernet.localIP());
  Serial.println();

  Serial.println("Setup Response Callback");
  coap.response(callback_response);

  coap.start();
}

void loop() {
  Serial.println("Send Request");
  int msgid = coap.get(IPAddress(192, 168, 40, 100), 5683, "time");

  delay(1000);
  coap.loop();
}
