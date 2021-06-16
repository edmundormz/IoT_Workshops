//https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/
#include <TinyGPS++.h>

//Testing definitions
String latitude;
String longitude;

TinyGPSPlus gps;
void setup(){
  Serial.begin(115200);
  //Serial2.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 15, 2); //RX,TX
  Serial.println("Beginning...");
}

void read_gps(){
  while (Serial2.available() > 0){
    // get the byte data from the GPS
    gps.encode(Serial2.read());
    if (gps.location.isUpdated()){
      latitude = String(gps.location.lat(),6);
      longitude = String(gps.location.lng(),6);
      //String.substring(,);
      Serial.print("Latitude= "); 
      Serial.println(latitude);
      //Serial.print(gps.location.lat(), 6);
      Serial.print("Longitude= "); 
      Serial.println(longitude);
      //Serial.println(gps.location.lng(), 6);
      Serial.print("Speed in Km/h: ");
      Serial.println(gps.speed.kmph());
    }
  }
}

void loop(){
  read_gps();
}
