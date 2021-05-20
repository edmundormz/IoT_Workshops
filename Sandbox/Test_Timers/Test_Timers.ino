volatile int interruptCounter1 = 0;
int totalInterruptCounter;
 
hw_timer_t * timer1 = NULL;

portMUX_TYPE timer1Mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer1() {
  portENTER_CRITICAL_ISR(&timer1Mux);
  interruptCounter1++;
  portEXIT_CRITICAL_ISR(&timer1Mux);
}
 
void setup() {
  Serial.begin(115200);
 
  timer1 = timerBegin(3, 80, true);                //Initialize timer. Timer#, Prescaler,true=rising count
  timerAttachInterrupt(timer1, &onTimer1, true);    //Relates timer & ISR. Timer pointer,function pointer,edge
  timerAlarmWrite(timer1, 1000000, true);          //Configures timer as an alarm. Timer pointer, uS value, reboot
  timerAlarmEnable(timer1);                        //Enables the alarm (timer). Is it possible to disable an alarm?
 
}
 
void loop() {
 
  if (interruptCounter1 > 0) {
 
    portENTER_CRITICAL(&timer1Mux);
    interruptCounter1--;
    portEXIT_CRITICAL(&timer1Mux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
  if(totalInterruptCounter > 2){
    timerAlarmDisable(timer1);
    Serial.println("Alarm 1 Disabled");
    while(1){
      
    }
  }
}
