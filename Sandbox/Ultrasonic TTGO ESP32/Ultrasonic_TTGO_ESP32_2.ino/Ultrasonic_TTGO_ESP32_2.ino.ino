#include <NewPing.h>
 
#define TRIGGER_PIN 32
#define ECHO_PIN 33
#define TRIGGER_PIN_2 34
#define ECHO_PIN_2 35
//#define MAX_DISTANCE 200

//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
NewPing sonar(TRIGGER_PIN, ECHO_PIN);
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2); 

void setup() {
   Serial.begin(115200);
}
 
void loop() {
   delay(500);
   Serial.print(sonar.ping_cm());
   Serial.println("cm");
   Serial.print(sonar_2.ping_cm());
   Serial.println("cm 2");
}
