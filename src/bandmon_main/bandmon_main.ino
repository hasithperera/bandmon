// Author KE8TJE
// Bandmon - a project by WVU ARC - W8CUL
// intended as a 2m repeater activity monitor

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "ahe-IoT";       // Change this to your WiFi SSID
const char* password = "Ta9suaq8";  // Change this to your WiFi password


WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char str1[50];
char str2[50];
int value = 0;
uint32_t chipid;


void setup() {
  // put your setup code here, to run once:
  
  chipid = get_chip_id();

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) { delay(100); }
  Serial.println("\nInit bandmon - KE8TJE/W8CUL");


  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer("mqtt.eclipseprojects.io", 1883);

  //set topic for regular updates
  sprintf(str1,"%s/%s/%d","bandmon","KE8TJE",chipid);  
  //client.setCallback(callback);
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000); 
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();                     // wait for a second
  client.publish(str1, "test");

}
