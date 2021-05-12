/// References
// About GPS:
// https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/
// About SMS:
// https://randomnerdtutorials.com/esp32-sim800l-send-text-messages-sms/


/// Definitions
//GSM modem definitions
const char simPIN[]   = "1111";     //SIM Card Pin
#define SMS_TARGET  "+523314173023" //Target phone number
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb
// TTGO T-Call pins (Do not use this pins for any other thing)
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22
//Location variables
String latitude;
String longitude;

/// Libraries
#include <TinyGPS++.h>      //GPS library
#include <TinyGsmClient.h>  //GSM library
#include <Wire.h>           //Intended to use with power save function

///Serial debug definitions (to be removed)
// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT  Serial1

/// This is supposed to work as a battery saver (To be Studied)
#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00
bool setPowerBoostKeepOn(int en){
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  if (en) {
    Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    Wire.write(0x35); // 0x37 is default reg value
  }
  return Wire.endTransmission() == 0;
}

/// Instances
TinyGPSPlus gps;
TinyGsm modem(SerialAT);

/// Initial configurations
void setup(){
  SerialMon.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 15, 2);     //GPS Module RX TX pins
  
  // Keep power when running from battery
  Wire.begin(I2C_SDA, I2C_SCL);
  bool isOk = setPowerBoostKeepOn(1);
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart SIM800 module, it takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  //modem.init();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
}

void read_gps(){
  while (Serial2.available() > 0){
    gps.encode(Serial2.read());
    if (gps.location.isUpdated()){
      latitude = String(gps.location.lat(),6);
      longitude = String(gps.location.lng(),6);
      SerialMon.print("Latitude= "); 
      SerialMon.println(latitude);
      SerialMon.print("Longitude= "); 
      SerialMon.println(longitude);
    }
  }
}

void send_sms(){
  String sms_message = String("GPS location: ") + latitude + String(", ") + longitude;
  if(modem.sendSMS(SMS_TARGET,sms_message)){
    SerialMon.println(sms_message);
  }
  else{
    SerialMon.println("SMS failed to send");
  }
}

void loop(){
  read_gps();
  delay(1000);
  send_sms();
  while(1){
    
  }
}
