#define time_seconds 10

const int led = 13;
const int push_button = 27;
unsigned long now = millis();
unsigned long last_trigger = 0;
boolean start_timer = false;

void IRAM_ATTR buttonPressed(){
  Serial.println("Button pressed!");
  digitalWrite(led, HIGH);
  start_timer = true;
  last_trigger = millis();
}

void setup() {
  Serial.begin(115200);
  pinMode(push_button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(push_button),buttonPressed,RISING);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
}

void loop() {
  now =millis();
  if(start_timer &&(now - last_trigger > (time_seconds*1000))){
    Serial.println("LED turned off...");
    digitalWrite(led,LOW);
    start_timer = false;
  }
}
