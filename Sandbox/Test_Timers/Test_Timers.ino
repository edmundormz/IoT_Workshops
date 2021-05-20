volatile int interruptCounter = -1;

int totalInterruptsCounter;
 
hw_timer_t * timer0 = NULL;
hw_timer_t * timer1 = NULL;
hw_timer_t * timer2 = NULL;
hw_timer_t * timer3 = NULL;

portMUX_TYPE timer0Mux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timer1Mux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timer2Mux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timer3Mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR ontimer0() {
  portENTER_CRITICAL_ISR(&timer0Mux);
  interruptCounter=0;
  portEXIT_CRITICAL_ISR(&timer0Mux);
}

void IRAM_ATTR ontimer1() {
  portENTER_CRITICAL_ISR(&timer1Mux);
  interruptCounter=1;
  portEXIT_CRITICAL_ISR(&timer1Mux);
}

void IRAM_ATTR ontimer2() {
  portENTER_CRITICAL_ISR(&timer2Mux);
  interruptCounter=2;
  portEXIT_CRITICAL_ISR(&timer2Mux);
}

void IRAM_ATTR ontimer3() {
  portENTER_CRITICAL_ISR(&timer3Mux);
  interruptCounter=3;
  portEXIT_CRITICAL_ISR(&timer3Mux);
}

void setup() {
  Serial.begin(115200);
 
  timer0 = timerBegin(0, 80, true);                //Initialize timer. Timer#, Prescaler,true=rising count
  timerAttachInterrupt(timer0, &ontimer0, true);    //Relates timer & ISR. Timer pointer,function pointer,edge
  timerAlarmWrite(timer0, 1000000, true);          //Configures timer as an alarm. Timer pointer, uS value, reboot
  timerAlarmEnable(timer0);                        //Enables the alarm (timer). Is it possible to disable an alarm?

  timer1 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer1, &ontimer1, true);
  timerAlarmWrite(timer1, 2000000, true);
  timerAlarmEnable(timer1);

  timer2 = timerBegin(2, 80, true);
  timerAttachInterrupt(timer2, &ontimer2, true);
  timerAlarmWrite(timer2, 3000000, true);
  timerAlarmEnable(timer2);

  timer3 = timerBegin(3, 80, true);
  timerAttachInterrupt(timer3, &ontimer3, true);
  timerAlarmWrite(timer3, 4000000, true);
  timerAlarmEnable(timer3);
}
 
void loop() {

if(interruptCounter != -1){
   switch(interruptCounter){
    case 0:
      Serial.print("0 Alarm ");
      Serial.print(interruptCounter);
      Serial.print(" , Total interrupts: ");
      Serial.println(totalInterruptsCounter);
      Serial.println("");
      portENTER_CRITICAL(&timer0Mux);
      interruptCounter = -1;
      portEXIT_CRITICAL(&timer0Mux);
      break;
    case 1:
      Serial.print("1 Alarm ");
      Serial.print(interruptCounter);
      Serial.print(" , Total interrupts: ");
      Serial.println(totalInterruptsCounter);
      Serial.println("");
      portENTER_CRITICAL(&timer1Mux);
      interruptCounter = -1;
      portEXIT_CRITICAL(&timer1Mux);
      break;
    case 2:
      Serial.print("2 Alarm ");
      Serial.print(interruptCounter);
      Serial.print(" , Total interrupts: ");
      Serial.println(totalInterruptsCounter);
      Serial.println("");
      portENTER_CRITICAL(&timer2Mux);
      interruptCounter = -1;
      portEXIT_CRITICAL(&timer2Mux);
      break;
    case 3:
      Serial.print("3 Alarm ");
      Serial.print(interruptCounter);
      Serial.print(" , Total interrupts: ");
      Serial.println(totalInterruptsCounter);
      Serial.println("");
      portENTER_CRITICAL(&timer3Mux);
      interruptCounter = -1;
      portEXIT_CRITICAL(&timer3Mux);
      break;
    default:
      Serial.print("\n Interrupt counter out of range: ");
      Serial.print(interruptCounter);
      Serial.println("");
   }
   totalInterruptsCounter++;
}

switch(totalInterruptsCounter){
  case 2:
    timerAlarmDisable(timer0);
    Serial.println("Alarm 0 Disabled");
    break;
  case 4:
    timerAlarmDisable(timer1);
    Serial.println("Alarm 1 Disabled");
    break;
  case 6:
    timerAlarmDisable(timer2);
    Serial.println("Alarm 2 Disabled");
    break;
  case 8:
    timerAlarmDisable(timer3);
    Serial.println("Alarm 3 Disabled");
    break;
}
if(totalInterruptsCounter == 8){
  while(1){
    
  }
}
}
