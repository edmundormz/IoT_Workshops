#define timeBlueLedOn 1
#define timeAlarmPeriod 5

// Input-Output variables
const int blueLed = 13;
const int pushButton = 27;

// Control variables
volatile bool ledControl = false;

//Time variables
unsigned long timeNow = 0;
volatile unsigned long lastTrigger = 0;

//Interrupts shared variables
volatile boolean invoked_isr_blueled = false;

//Timer alarm pointers
hw_timer_t * ledTimer = NULL;

//portMUX_TYPE Controls
portMUX_TYPE bpMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE ledTimerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR turn_on_blueled(){
  portENTER_CRITICAL_ISR(&bpMux);
  invoked_isr_blueled = true;
  ledControl = true;
  lastTrigger = millis();
  portEXIT_CRITICAL_ISR(&bpMux);
}

void setup() {
  pinMode(pushButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pushButton),turn_on_blueled,RISING);
  pinMode(blueLed, OUTPUT);
  digitalWrite(blueLed, LOW);

  //Set an alarm timer to invoke turn_on_blueled every timeAlarmPeriod
  ledTimer = timerBegin(0,80,true);
  timerAttachInterrupt(ledTimer,&turn_on_blueled,true);
  timerAlarmWrite(ledTimer,timeAlarmPeriod * 1000000,true);
  timerAlarmEnable(ledTimer);

}

void loop() {
  timeNow = millis();

  //Turn-on blueLed if ISR is invoked either by push-button or timer alarm
  if(invoked_isr_blueled && ledControl){
    digitalWrite(blueLed, ledControl);
    ledControl = false;
  }

  //Turn-off blueLed after "timeBlueLedOn" elapsed
  if(invoked_isr_blueled && (timeNow - lastTrigger > (timeBlueLedOn*1000))){
    digitalWrite(blueLed,ledControl);
    invoked_isr_blueled = false;
  }
  
}
