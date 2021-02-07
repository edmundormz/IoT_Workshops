#include "WiFi.h"
//#include "ESP8266WiFi.h"
// WiFi parameters to be configured
const char* ssid = "Mi_WiFi";
const char* password = "Mi_Password";
void setup(void)
{
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("Conectado a internet, mi IP es:");
  // Print the IP address
  Serial.println(WiFi.localIP());
}
void loop() {
  // Nothing
}
