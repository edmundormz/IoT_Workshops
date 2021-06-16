//Connections between SIM800L <--> ESP32
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

#define SerialAT  Serial1

char data_received_sms;
String data_to_sms;
short on_relay= -1;
short off_relay= -1;

#define RELAY 14

void set_receive_mode(){
  SerialAT.println("AT");            //Send AT command it will says "OK" if everything is fine
  Serialcom();
  SerialAT.println("AT+CMGF=1");     // Configuring TEXT mode
  Serialcom();
  SerialAT.println("AT+CNMI=2,2,0,0,0"); //The way to handle the SMS received, check the module manual, to sum up: store the text or transfert it to serial or other number....
  Serialcom();
}

void send_data(){
  Serial.println("Sending Data...");     //Displays on the serial monitor...Optional
  SerialAT.print("AT+CMGF=1\r");          //Set the module to SMS mode
  SerialAT.print("AT+CMGS=\"+***********\"\r");  //Your phone number don't forget to include your country code example +212xxxxxxxxx"
  SerialAT.print(data_to_sms);  //This string is sent as SMS
  SerialAT.print((char)26);//Required to tell the module that it can send the SMS
  SerialAT.println();
  Serial.println("Data Sent.");
}

void setup(){
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY,LOW);
  
  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
  
   
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(1000);
  Serial.begin(9600);               //Begin the Serial communication with PC
  Serial.println("Starting ...");
  delay(5000);                      //Delay to let the module to connect to network, if turned on before you can remove this
}



void loop() {
  String RSMS;

  while(SerialAT.available()>0){
    data_received_sms=SerialAT.read();
    Serial.println(data_received_sms);
    RSMS.concat(data_received_sms);     //Maybe String(data_received_sms)???
    on_relay=RSMS.indexOf("RELAY ON");
    off_relay=RSMS.indexOf("RELAY OFF");
  }

  if(on_relay!=-1){
    Serial.println("Relay ON");
    digitalWrite(RELAY,HIGH);
    data_to_sms = "Relay turned on";
    send_data();
    on_relay=-1;
  }
  if(off_relay!=-1){
    Serial.println("Relay OFF");
    digitalWrite(RELAY,LOW);
    data_to_sms = "Relay turned off";
    send_data();
    off_relay=-1;
  }
  else{
    Serial.println("Invalid message");
    data_to_sms = "Invalid message received";
    send_data();
  }
  set_receive_mode();
}


void Serialcom(){
  delay(500);
  while(Serial.available())                                                                      // IDE serial l serial  SerialAT
  {
    SerialAT.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(SerialAT.available())                                                                      //serialSerialAT l serial dial IDE
  {
    Serial.write(SerialAT.read());//Forward what Software Serial received to Serial Port
  }
}
