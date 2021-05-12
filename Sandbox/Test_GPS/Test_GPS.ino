
void setup(){
  Serial.begin(9600);
  //Serial2.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 14, 27);
}

void loop(){
  while (Serial2.available() > 0){
    // get the byte data from the GPS
    byte gpsData = Serial2.read();
    Serial.write(gpsData);
  }
}
