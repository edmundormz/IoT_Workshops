#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "Totalplay-8195"; // Your ssid
const char* password = "81956A81qN3nEKbx"; // Your Password 

const char* mqtt_server = "broker.hivemq.com";
//To Do: usar broker local
const char* clientid= "ESP2805";

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);Serial.print(".");}
  Serial.println("");
  Serial.println("WiFi connected");}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);}
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientid)) {
      Serial.println("connected");
      client.subscribe("CIATEQ/IOT/Cafe");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);}}}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  }

void loop() {
  if (!client.connected()) {
    reconnect();}
  client.loop();
  delay(1000);
}
