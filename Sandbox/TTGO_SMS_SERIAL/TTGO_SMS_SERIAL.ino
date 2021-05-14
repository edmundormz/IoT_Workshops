//Connections between SIM800L <--> ESP32
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

#define SerialAT  Serial1

void setup()
{
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
  SerialAT.println("AT");            //Send AT command it will says "OK" if everything is fine
  Serialcom();                      //description below
  SerialAT.println("AT+CMGF=1");     // Configuring TEXT mode
  Serialcom();
  SerialAT.println("AT+CNMI=2,2,0,0,0"); //The way to handle the SMS received, check the module manual, to sum up: store the text or transfert it to serial or other number....
  Serialcom();
}



void loop() {
  
Serialcom();                //The only function to run in loop, this function just check if the user has sent something using the Serial monitor and transfers it to the module
                            //and if the module has something to say it will be displayed on the serial monitor, here used to show the feedback of the AT commands sent in the setup
                            //and shows the received SMS

}


void Serialcom()
{
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
