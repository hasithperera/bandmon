// Author KE8TJE
// Bandmon - a project by WVU ARC - W8CUL
// intended as a 2m repeater activity monitor

//#include <WiFi.h> - old library from 2024 (removed)

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <EEPROM.h>

#define SQL_pin 0
#define analog_in A0

#define debug  // uncomment for debug prints
//#define SOUND_IN // uncomment if audio is provided

#define adc_count 100

// data formats to read data
struct s_wifi_data {
  char ssid[40];
  char psk[40];
  int reconnect_count;
} wifi_data;

struct s_user_data {
  char user_call[10];
  char rptr_call[10];
  char state[2];

  float lon;
  float lat;

  char mqtt_svr[30];
  uint16_t port;
  long timeout;
  float audio_cutoff;

} user_data;

long report_timeout;  // used for millis time out
float audio_cutoff = 400;
float dev_avg = 0;

float talk_time = 0;  // accumilate audio signature

;
WiFiClient espClient;
PubSubClient client(espClient);
long lastReconnectAttempt = 0;


char topic[50];
char cmd_topic[50];
char tmp_value[50];
int value = 0;
uint32_t chipid;

float min_val = 0;
float max_val = 0;

float avg_val;



#define wifi_reconnect_interval 15
int update_count = 0;

//#define inituser_data


void setup() {
  // put your setup code here, to run once:

  chipid = ESP.getChipId();  //get_chip_id();


  Serial.begin(9600);
  //while (!Serial) { delay(100); } // removed to work without serial


#ifdef inituser_data
  sprintf(wifi_data.ssid, "MyResNet-2G");
  sprintf(wifi_data.psk, "Saturday-Washington-37@");
  wifi_data.reconnect_count = 50;

  sprintf(user_data.user_call, "KE8TJE");
  sprintf(user_data.state, "WV");
  sprintf(user_data.rptr_call, "W8CUL-2");



  //sprintf(user_data.mqtt_svr,"mqtt.eclipseprojects.io");
  //user_data.port = 1883;
  user_data.timeout = 60000;
  user_data.audio_cutoff = 400;
  user_data.lon = -79.9743287;
  user_data.lat = 39.6458788;


  write_EEPROM_wifi();
  Serial.println("EEPROM Write done");

#endif





  Serial.println("\ni Init bandmon - KE8TJE/W8CUL");
  Serial.print("i Chipid:");
  Serial.println(ESP.getChipId());
  //EEPROM data dump
  read_EEPROM_wifi();



  //set topic for regular updates
  sprintf(topic, "%s/%s/%d", "bandmon", user_data.state, chipid);

  //set command topic
  sprintf(cmd_topic, "bandmon/cmd/%s/%d", user_data.user_call, chipid);
  Serial.print("i CMD topic:");
  Serial.println(cmd_topic);
  //Serial.println("Chipid stored:",chipid);

  Serial.println();
  Serial.print("i Connecting to ");
  Serial.print(wifi_data.ssid);
  Serial.print(" with psk starting with:");
  Serial.print(wifi_data.psk[0]);

  Serial.println("\n\n Noise level calibration");

  for (int i = 0; i < 500; i++) {
    avg_val += analogRead(analog_in);
    delay(10);
  }
  avg_val = avg_val / 500;
  Serial.print("Noice level:");
  Serial.println(avg_val);


  WiFi.begin(wifi_data.ssid, wifi_data.psk);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");

    // handel no internet for 100 s
    if (wifi_data.reconnect_count-- < 0) {
      //restart ESP command
      ESP.restart();
    }
  }

  Serial.println("");
  Serial.println("i WiFi connected");
  Serial.print("i IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);

  client.setServer(user_data.mqtt_svr, user_data.port);
  sprintf(topic, "bandmon/%s/%s", user_data.state, user_data.rptr_call);

  // update report time out
  report_timeout = millis();
}


//audio detection update



void loop() {

  if (!client.connected()) {
    digitalWrite(LED_BUILTIN, LOW);
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;

      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }

  } else {
    // Client connected
    client.loop();
  }

//#define final
#ifdef final
  dev_avg = 0;
  for (int i = 0; i < 50; i++) {
    dev_avg += abs(analogRead(analog_in) - analogRead(analog_in));
  }
#endif

  //reset counter
  min_val = 0;
  max_val = 0;
  for (int i = 0; i < 200; i++) {
    float adc = analogRead(analog_in);

    if (adc > max_val) {
      max_val = adc;
    }

    if (min_val > adc) {
      min_val = adc;
    }
    delay(10);
  }

  //Debug output
  //Serial.println(max_val  - avg_val);

  
  if(max_val - avg_val>user_data.audio_cutoff/2){
    talk_time +=2.0; //measurements take 2 s (currently)
    #ifdef debug
    Serial.println(talk_time);
    #endif
  }


  if ((millis() - report_timeout) > 60000) {

    report_timeout = millis();
    //client.publish("bandmon/test/events","report time out");
    if (talk_time > 2) {
      sprintf(tmp_value, "%f", talk_time);
      client.publish(topic, tmp_value);

      sprintf(tmp_value, "{\"call\":\"%s\",\"activity\":%f}", user_data.rptr_call, talk_time);
      client.publish("bandmon/data", tmp_value);
    }
    //clear talk timer
    talk_time = 0;

    if (update_count > wifi_reconnect_interval) {
      ESP.restart();
    }

    Serial.printf("i update %d\n", update_count++);
  }
}
