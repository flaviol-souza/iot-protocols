#include <DHT.h>
#define DHT11PIN 27

DHT dht(DHT11PIN, DHT11);
void setup()
{
  Serial.begin(115200);
  dht.begin();
}

void loop()
{
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.print("ºC ");
  Serial.print("Umidade: "); 
  Serial.println(humi);
  delay(1000);
}
