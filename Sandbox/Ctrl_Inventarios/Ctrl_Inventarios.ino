/// Global Variables
// GPIO where the DS18B20 temperature sensor is connected to
const int oneWireBus = 15;
// Ultrasonic sensors pins  
#define TRIGGER_PIN_1 32
#define ECHO_PIN_1 33
#define TRIGGER_PIN_2 27 // ***CAMBIAR POR OTRO, PUEDE SER 36,2 
#define ECHO_PIN_2 14
//Temperature sensor variable
float temperatureC;
// Door sensor (push button)
int door_state = 0;
String door_is;
const int door_pin = 35;  // the number of the door sensor pin
const int led_pin =  0;    // the number of the indicator LED pin
// Push buttons array for can lanes
const int cans_lane_1 = 34; // pin where lane 1 is connected to
const int cans_lane_2 = 39; // pin where lane 2 is connected to
int switches_value = 0;
int cant_latas = 4;
int cant_latas_sonar_1 = 0;
int cant_latas_sonar_2 = 0;
int dist_latas = 0;
//Location variables
String gsm_location;
//GSM modem definitions
const char simPIN[]   = "1111";     //SIM Card Pin
// GPRS Connection credentials
const char apn[] = "internet.itelcel.com";
const char gprsUser[] = "webgprs";
const char gprsPass[] = "webgprs2002";

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


/// Libraries
// DS18B20 OneWire Temperature Sensor Libraries
#include <OneWire.h>
#include <DallasTemperature.h>
// HC-SR04 Ultrasonic Sensors Library
#include <NewPing.h>
// GSM Modem libraries
#include <TinyGsmClient.h>  //GSM library
#include <Wire.h>           //Intended to use with power save function

///GSM Serial debug definitions (to be removed)
// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT  Serial1

/// This is supposed to work as a battery saver when using GSM (To be Studied)
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

/// Instances Declarations
// OneWire protocol instance and temperature sensor declaration
OneWire oneWire(oneWireBus);
DallasTemperature tmp_sensor(&oneWire);
// Ultrasonic Sensors 
NewPing sonar_1(TRIGGER_PIN_1, ECHO_PIN_1);
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2);
TinyGsm modem(SerialAT);

/// Configuraciones Iniciales
void setup() {
  SerialMon.begin(115200); // SerialMon 
  tmp_sensor.begin(); // Start the DS18B20 sensor
  pinMode(door_pin, INPUT); // door sensor as an input
  pinMode(led_pin, OUTPUT); // indicator led pin as an output
  digitalWrite(led_pin, LOW); // initialize indicator led in off

  //GSM: Keep power when running from battery
  Wire.begin(I2C_SDA, I2C_SCL);
  bool isOk = setPowerBoostKeepOn(1);
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));
  //GSM Set modem reset, enable, power pins
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

///Funciones
void read_temperature(){
  tmp_sensor.requestTemperatures(); 
  temperatureC = tmp_sensor.getTempCByIndex(0);
  SerialMon.print(temperatureC);
  SerialMon.println("ºC");
}

void read_distances(){
  // ***AGREGAR SU CÓDIGO DEL SEGUNDO CARRIL DE LATAS usando cant_latas_sonar_2
/*   SerialMon.print(sonar_1.ping_cm());
   SerialMon.println("cm 1");
   SerialMon.print(sonar_2.ping_cm());
   SerialMon.println("cm 2");
*/
   dist_latas = sonar_1.ping_cm();
   if(dist_latas < 40){
    cant_latas_sonar_1 = 0;
   }
   if(dist_latas < 35){
    cant_latas_sonar_1 = 1;
   }
   if(dist_latas < 27){
    cant_latas_sonar_1 = 2;
   }
   if(dist_latas < 19){
    cant_latas_sonar_1 = 3;
   }
   if(dist_latas < 16){
    cant_latas_sonar_1 = 4;
   }
   if(dist_latas < 6){
    cant_latas_sonar_1 = 5;
   }
   SerialMon.print(cant_latas_sonar_1);
   SerialMon.println(" latas carril 4");
}

void read_door(){
  door_state = digitalRead(door_pin); // read the state of the door value
  if (door_state == HIGH) {
    digitalWrite(led_pin, LOW);
    door_is = String("cerrada");
    SerialMon.println("Puerta cerrada");
  } else {
    digitalWrite(led_pin, HIGH);
    door_is = String("abierta");
    SerialMon.println("Puerta abierta");
  }
}

void read_cans(int switches_lane){
  switches_value = analogRead(switches_lane);
  //SerialMon.print("switches_value = ");
  //SerialMon.println(switches_value);
  if (switches_value < 4096){
    cant_latas = 0;
  }
  if (switches_value < 1850){
    cant_latas = 1;
  }
  if (switches_value < 1250){
    cant_latas = 2;
  }
  if (switches_value < 900){
    cant_latas = 3;
  }
  if (switches_value < 700){
    cant_latas = 4;
  }
  SerialMon.print(cant_latas);
  
}
void send_sms(){
  String sms_message = String(" Temp: ") + String(temperatureC);
  sms_message+= String("\n Latas1: ") + String(cant_latas_sonar_1);
  sms_message+= String("\n Latas2: ") + String(cant_latas_sonar_1);
  sms_message+= String("\n Puerta: ") + door_is;
  sms_message+= String("\n Latas3: ") + String(cant_latas);
  if(modem.sendSMS(SMS_TARGET,sms_message)){
    SerialMon.println(sms_message);
  }
  else{
    SerialMon.println("SMS failed to send");
  }
}

void get_gsm_location(){
  // Unlock your SIM card with a PIN if needed
  if (simPIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
  SerialMon.println("Waiting for network...");
  if (!modem.waitForNetwork(600000L)) {
    delay(10000);
    return;
  }
  if (modem.isNetworkConnected()) {
    SerialMon.println("Network connected");
  }
  //SerialMon.println("Connecting to", String(apn));
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    delay(10000);
    return;
  }
  bool res = modem.isGprsConnected();
  if(res==true){
    SerialMon.println("Connected");
  }else{
    SerialMon.println("Not Connected");
  }
  IPAddress local = modem.localIP();
  SerialMon.println(String(local));
  
  //SerialMon.println("GPRS status:", res ? "connected" : "not connected");

  float lat      = 0;
  float lon      = 0;
  float accuracy = 0;
  int   year     = 0;
  int   month    = 0;
  int   day      = 0;
  int   hour     = 0;
  int   minn      = 0;
  int sec = 0;
  for (int8_t i = 3; i; i--) {
    SerialMon.println("Requesting current GSM location");
    if (modem.getGsmLocation(&lat, &lon, &accuracy, &year, &month, &day, &hour,
                             &minn, &sec)) {
      SerialMon.println("Success location");
      SerialMon.print(String(lat));
      SerialMon.println(String(lon));
      break;
    } else {
      SerialMon.println("Couldn't get GSM location, retrying in 15s.");
      delay(15000L);
    }
  }
   
  SerialMon.println("Triying to get gsm location as a string: ");
  String location = modem.getGsmLocation();
  delay(1000);
  //gsm_location = modem.getGsmLocation();
  SerialMon.print("String GSM location: "); 
  SerialMon.println(location);
  modem.gprsDisconnect();
  delay(5000L);
  if (!modem.isGprsConnected()) {
    SerialMon.println("GPRS disconnected");
  } else {
    SerialMon.println("GPRS disconnect: Failed.");
  }
}

/// Ciclo de ejecución
void loop() {
  SerialMon.println("Proyecto Inventarios");
  /*
  read_temperature();
  read_distances();
  read_door();
  read_cans(cans_lane_1); SerialMon.println(" latas carril 1");
  read_cans(cans_lane_2); SerialMon.println(" latas carril 2");
  */
  get_gsm_location();
  //SerialMon.println();
  delay(3000);
  //SerialMon.println("Triying to send SMS");
  //send_sms();
  /*while(1){
    
  }*/
}
