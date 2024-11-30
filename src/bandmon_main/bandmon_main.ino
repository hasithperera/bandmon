// Author KE8TJE
// Bandmon - a project by WVU ARC - W8CUL
// intended as a 2m repeater activity monitor

//#include <WiFi.h> - old library from 2024 (removed)

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <EEPROM.h>

#define SQL_pin 0
#define analog_in A0

//#define SOUND_IN // uncomment if audio is provided

#define adc_count 100

// data formats to read data
struct s_wifi_data {
  char ssid[40];
  char psk[40];
  int reconnect_count;
} wifi_data;

struct s_user_data{
  char user_call[10];
  char rptr_call[10];
  char state[2];

  float lng;
  float lat;
  
  char mqtt_svr[50];
  char port[6];
  long timeout;
  float audio_cutoff;

} user_data;

long report_timeout; // used for millis time out
float audio_cutoff =  400;
float dev_avg = 0;

float talk_time = 0; // accumilate audio signature

;WiFiClient espClient;
PubSubClient client(espClient);
long lastReconnectAttempt = 0;


char topic[50];
char cmd_topic[50];
char tmp_value[10];
int value = 0;
uint32_t chipid;

int restart_count = 30;
#define wifi_reconnect_interval 15
int update_count = 0;

// #define inituser_data


void setup() {
  // put your setup code here, to run once:
  
  chipid = ESP.getChipId();//get_chip_id();

  
  Serial.begin(9600);
  //while (!Serial) { delay(100); } // removed to work without serial


  #ifdef inituser_data
    sprintf(wifi_data.ssid,"ahe");
    sprintf(wifi_data.psk, "Morgantown");
    wifi_data.reconnect_count = 50;

    sprintf(user_data.user_call,"KE8TJE");
    sprintf(user_data.state,"WV");
    sprintf(user_data.rprtr_call,"W8CUL-2");

    user_data.lng = 0;
    user_data.lat = 0;
  
    sprintf(user_data.mqtt_svr,"mqtt.eclipseprojects.io");
    sprintf(user_data.port,"1883");
    user_data.timeout = 60000;
    user_data.audio_cutoff = 700;

    write_EEPROM_wifi();
    Serial.println("EEPROM Write done");
    
  #endif
  

  //EEPROM data dump
  read_EEPROM_wifi();


  Serial.println("\ni Init bandmon - KE8TJE/W8CUL");
  Serial.print("i Chipid:");
  Serial.println(ESP.getChipId());
  



  //set topic for regular updates
  sprintf(topic,"%s/%s/%d","bandmon",user_data.state,chipid);

  //set command topic
  sprintf(cmd_topic,"bandmon/cmd/%s/%d",user_data.user_call,chipid);
  Serial.print("i CMD topic:");
  Serial.println(cmd_topic);
  //Serial.println("Chipid stored:",chipid);
  Serial1.begin(9600);

 

  Serial.println();
  Serial.print("i Connecting to ");
  Serial.print(wifi_data.ssid);
  Serial.print(" with psk starting with:");
  Serial.print(wifi_data.psk[0]);

  WiFi.begin(wifi_data.ssid, wifi_data.psk);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");

    // handel no internet for 100 s
    if (restart_count--<0){
    //restart ESP command
      ESP.restart();
    }

  }

  restart_count = 100;
  

  Serial.println("");
  Serial.println("i WiFi connected");
  Serial.print("i IP address: ");
  Serial.println(WiFi.localIP());
  
  delay(500);

  client.setServer("mqtt.eclipseprojects.io", 1883);


   //set topic for stats update
  sprintf(topic,"bandmon/%s/%s",user_data.state,user_data.rptr_call);

  // update report time out
  report_timeout = millis();
}

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
 
  dev_avg=0;
  for (int i=0;i<50;i++){
    dev_avg += abs(analogRead(analog_in)-analogRead(analog_in));
  }
  
  
  if(dev_avg>audio_cutoff){
    talk_time +=1.0;
    Serial.println(talk_time);
  }


  if((millis()-report_timeout)>60000){
    
    report_timeout = millis();
    //client.publish("bandmon/test/events","report time out");
    if(talk_time>2){
      sprintf(tmp_value,"%f",talk_time/4.0);
      client.publish(topic,tmp_value);
    }
    //clear talk timer
    talk_time = 0 ;
    
    if(update_count > wifi_reconnect_interval){
      ESP.restart();
    }

    Serial.printf("i update %d\n",update_count++);
    
  }

  delay(300); 

}
