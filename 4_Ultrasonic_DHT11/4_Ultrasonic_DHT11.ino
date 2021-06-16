#include <NewPing.h>
#include <DHT.h>
 
#define TRIGGER_PIN 22
#define ECHO_PIN 23
#define MAX_DISTANCE 200

#define DHTPIN 32
#define DHTTYPE DHT11


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
   Serial.begin(115200);
   dht.begin();
}
 
void loop() {
   delay(1000);
   
   //unsigned int distance = sonar.ping_cm();
   //Serial.print(distance);
   Serial.print(sonar.ping_cm());
   Serial.println("cm");

   float h = dht.readHumidity();
   float t = dht.readTemperature();
   if (isnan(h) || isnan(t)){
    Serial.println("Failed to read from DHT sensor!");
   }
   float hic = dht.computeHeatIndex(t,h,false);
   Serial.print("Humidity: ");
   Serial.print(h);
   Serial.print("% ,Temperature: ");
   Serial.print(t);
   Serial.print("°C ");
   Serial.print("Heat index: ");
   Serial.print(hic);
   Serial.println("°C");
}
