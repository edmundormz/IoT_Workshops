//https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/
#include <TinyGPS++.h>

TinyGPSPlus gps;
void setup(){
  Serial.begin(9600);
  //Serial2.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 14, 27);
}

void loop(){
  while (Serial2.available() > 0){
    // get the byte data from the GPS
    gps.encode(Serial2.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
      // Raw date in DDMMYY format (u32)
      Serial.print("Raw date DDMMYY = ");
      Serial.println(gps.date.value());
      // Raw time in HHMMSSCC format (u32)
      Serial.print("Raw time in HHMMSSCC = "); 
      Serial.println(gps.time.value());
      // Speed in kilometers per hour (double)
      Serial.print("Speed in km/h = "); 
      Serial.println(gps.speed.kmph());
      // Number of satellites in use (u32)
      Serial.print("Number os satellites in use = "); 
      Serial.println(gps.satellites.value()); 
    }
  }
}
