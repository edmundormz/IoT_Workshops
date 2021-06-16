#include <NewPing.h>
#include <DHT.h>
 
#define TRIGGER_PIN 32
#define ECHO_PIN 33
#define TRIGGER_PIN_2 34
#define ECHO_PIN_2 35
//#define MAX_DISTANCE 200

#define DHTPIN 25
#define DHTTYPE DHT11


//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
NewPing sonar(TRIGGER_PIN, ECHO_PIN);
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2); 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
   Serial.begin(115200);
   dht.begin();
}
 
void loop() {
   delay(2000);
   
   //unsigned int distance = sonar.ping_cm();
   //Serial.print(distance);
   Serial.print(sonar.ping_cm());
   Serial.println("cm");
   Serial.print(sonar_2.ping_cm());
   Serial.println("cm 2");

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
