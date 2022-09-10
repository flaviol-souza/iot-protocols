#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <tinyxml2.h>

using namespace tinyxml2;

#define DHT11PIN 27

DHT dht(DHT11PIN, DHT11);

char * text;
const char* ssid = "ifsp-ibge-1";
const char* password = "ifspcatanduva";

void setup() {
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }  
  Serial.println("Connected to the WiFi network");
  dht.begin();
}
 
void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {   
    HTTPClient http;
    String payload;
  
    http.begin("http://servicos.cptec.inpe.br/XML/cidade/1407/previsao.xml"); 
    int httpCode = http.GET();
  
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
    else {
      Serial.println("Error on HTTP request");
      exit(1);
    }
    http.end();
    text = new char [payload.length()+1];
    strcpy(text, payload.c_str());
  }  
  XMLDocument xml;
 
  if(xml.Parse(text)!= XML_SUCCESS){
    Serial.println("Error parsing"); 
    return; 
  };
  
  const char* maxima = xml.FirstChildElement( "cidade" )->FirstChildElement( "previsao" )->FirstChildElement( "maxima" )->GetText();
  const char* minimum = xml.FirstChildElement( "cidade" )->FirstChildElement( "previsao" )->FirstChildElement( "minima" )->GetText();
  printf( "Temp. Max. INPE: %s\n", maxima);
  printf( "Temp. Min. INPE: %s\n", minimum);

  float temp = dht.readTemperature();
  printf( "Temp. Local: %f\n", temp);
  
  delay(10000);
 }
