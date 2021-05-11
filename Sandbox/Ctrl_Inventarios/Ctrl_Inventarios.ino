/// Libraries
// DS18B20 OneWire Temperature Sensor Libraries
#include <OneWire.h>
#include <DallasTemperature.h>
// HC-SR04 Ultrasonic Sensors Library
#include <NewPing.h>

/// Globar Variables
// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;
// Ultrasonic sensors pins  
#define TRIGGER_PIN_1 32
#define ECHO_PIN_1 33
#define TRIGGER_PIN_2 27
#define ECHO_PIN_2 14
// Door sensor (push button)
int door_state = 0;
const int door_pin = 35;  // the number of the door sensor pin
const int led_pin =  0;    // the number of the indicator LED pin
// Push buttons array for can lanes
const int cans_lane_1 = 34; // pin where lane 1 is connected to
const int cans_lane_2 = 39; // pin where lane 2 is connected to
int switches_value = 0;
int cant_latas = 4;

//// Instances Declarations
// OneWire protocol instance and temperature sensor declaration
OneWire oneWire(oneWireBus);
DallasTemperature tmp_sensor(&oneWire);
// Ultrasonic Sensors 
NewPing sonar_1(TRIGGER_PIN_1, ECHO_PIN_1);
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2);

/// Configuraciones Iniciales
void setup() {
  Serial.begin(115200); // Serial 
  tmp_sensor.begin(); // Start the DS18B20 sensor
  pinMode(door_pin, INPUT); // door sensor as an input
  pinMode(led_pin, OUTPUT); // indicator led pin as an output
  digitalWrite(led_pin, LOW); // initialize indicator led in off
}

void read_temperature(){
  tmp_sensor.requestTemperatures(); 
  float temperatureC = tmp_sensor.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
}

void read_distances(){
   Serial.print(sonar_1.ping_cm());
   Serial.println("cm 1");
   Serial.print(sonar_2.ping_cm());
   Serial.println("cm 2");
}

void read_door(){
  door_state = digitalRead(door_pin); // read the state of the door value
  if (door_state == HIGH) {
    digitalWrite(led_pin, LOW);
    Serial.println("Puerta cerrada");
  } else {
    digitalWrite(led_pin, HIGH);
    Serial.println("Puerta abierta");
  }
}

void read_cans(int switches_lane){
  switches_value = analogRead(switches_lane);
  Serial.print("switches_value = ");
  Serial.println(switches_value);
  if (switches_value < 4096){
    cant_latas = 0;
  }
  if (switches_value < 1850){
    cant_latas = 1;
  }
  if (switches_value < 1250){
    cant_latas = 2;
  }
  if (switches_value < 900){
    cant_latas = 3;
  }
  if (switches_value < 700){
    cant_latas = 4;
  }
  Serial.print(cant_latas);
  
}

/// Ciclo de ejecución
void loop() {
  Serial.println("Proyecto Inventarios");
  read_temperature();
  read_distances();
  read_door();
  read_cans(cans_lane_1); Serial.println(" latas carril 1");
  read_cans(cans_lane_2); Serial.println(" latas carril 2");
  Serial.println();
  delay(1000);
}
