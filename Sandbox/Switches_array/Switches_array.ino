// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int switches_pin = 34;

// variable for storing the potentiometer value
int switches_value = 0;
int cant_latas = 4;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // Reading potentiometer value
  switches_value = analogRead(switches_pin);
  Serial.println(switches_value);
  delay(500);
  /*if (switches_value < 4096){
    cant_latas = 0;
  }
  if (switches_value < 1800){
    cant_latas = 1;
  }
  if (switches_value < 1200){
    cant_latas = 2;
  }
  if (switches_value < 850){
    cant_latas = 3;
  }
  if (switches_value < 650){
    cant_latas = 4;
  }
  Serial.print(cant_latas);
  Serial.println(" latas carril 1");
  */
}


/*
4095 0
 1800 1
 1200 2
 850 3
 650 4
 */
