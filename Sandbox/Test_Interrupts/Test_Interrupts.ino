#define time_seconds 10
#define timer_counter 1

const int led = 13;
const int push_button = 27;
unsigned long now = 0;
unsigned long last_trigger = 0;
unsigned long previous_millis = 0;
boolean start_timer = false;
volatile int counter = 0; //Volatile to be stored in RAM and accesible by ISRs

void IRAM_ATTR buttonPressed(){
  Serial.println("Button pressed!");
  digitalWrite(led, HIGH);
  start_timer = true;
  last_trigger = millis();
  counter++;
  Serial.print("ISR, Counter = ");Serial.println(counter);
}

void setup() {
  Serial.begin(115200);
  pinMode(push_button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(push_button),buttonPressed,RISING);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
}

void loop() {
  now = millis();
  if(start_timer &&(now - last_trigger > (time_seconds*1000))){
    Serial.println("LED turned off...");
    digitalWrite(led,LOW);
    start_timer = false;
    counter++;
    Serial.print("Trigger Loop, counter = ");
    Serial.println(counter);
  }
  if(now - previous_millis > (timer_counter*1000)){
    previous_millis = millis();
    counter++;
    Serial.print("Timer, counter = ");
    Serial.println(counter);
  }
}
