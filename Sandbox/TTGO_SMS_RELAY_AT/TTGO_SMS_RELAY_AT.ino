//Connections between SIM800L <--> ESP32
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

#define SerialAT  Serial1

String textMessage;
String lampState = "HIGH";
const int relay = 13;

void setup(){
  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
   
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(1000);
  Serial.begin(115200);               //Begin the Serial communication with PC
  Serial.println("Starting ...");
  delay(5000);                      //Delay to let the module to connect to network, if turned on before you can remove this
  SerialAT.println("AT");            //Send AT command it will says "OK" if everything is fine
  Serialcom();                      //description below
  SerialAT.print("AT+CMGF=1\r");     // Configuring TEXT mode
  Serialcom();
  SerialAT.print("AT+CNMI=2,2,0,0,0\r"); // Set module to send SMS data to serial out upon receipt
  Serialcom();
}



void loop() {
  
//Serialcom();                //The only function to run in loop, this function just check if the user has sent something using the Serial monitor and transfers it to the module
                            //and if the module has something to say it will be displayed on the serial monitor, here used to show the feedback of the AT commands sent in the setup
                            //and shows the received SMS
  
  if(SerialAT.available()>0){
    textMessage = SerialAT.readString();
    Serial.print("textMessage: ");
    Serial.println(textMessage);
    delay(10);
  }
  if(textMessage.indexOf("ON")>=0){
    // Turn on relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "on";
    Serial.println("Relay set to ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0){
    // Turn off relay and save current state
    digitalWrite(relay, LOW);
    lampState = "off"; 
    Serial.println("Relay set to OFF");
    textMessage = ""; 
  }
  if(textMessage.indexOf("STATE")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
  
}

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SerialAT to SMS mode
  SerialAT.print("AT+CMGF=1\r");
  delay(100);
  SerialAT.print("AT + CMGS = \"+523314173023\""); 
  delay(100);
  SerialAT.println(message); 
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  SerialAT.println((char)26); 
  delay(100);
  SerialAT.println();
  delay(5000);  
}

void Serialcom(){
  //delay(500);
  while(Serial.available())                                                                      // IDE serial l serial  SerialAT
  {
    SerialAT.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(SerialAT.available())                                                                      //serialSerialAT l serial dial IDE
  {
    Serial.write(SerialAT.read());//Forward what Software Serial received to Serial Port
  }
}
