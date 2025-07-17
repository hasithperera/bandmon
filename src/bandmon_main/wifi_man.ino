
//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void init_file_system(){
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);
        if ( ! deserializeError ) {
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
#endif
          Serial.println("\nparsed json");
          //strcpy(mqtt_server, json["mqtt_server"]);
          //strcpy(mqtt_port, json["mqtt_port"]);
          //strcpy(api_token, json["api_token"]);
        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
}


void wifi_manager_reset(){
    WiFiManager wm;
    wm.resetSettings();
}

void wifi_manager_config(){

  // Parameter list with defaults 

  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "phys.cmb.ac.lk", 30);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "1883", 6);
  WiFiManagerParameter custom_user_call("u_c", "User Call", "N0CALL", 10);
  WiFiManagerParameter custom_user_rcall("u_rc", "Repeater Call", "N0RCALL", 10);
  WiFiManagerParameter custom_user_state("u_state", "State", "NA", 2);
  WiFiManagerParameter custom_user_cutoff("u_audio_cut", "Audio cutoff", "200", 4);

  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_user_call);
  wifiManager.addParameter(&custom_user_rcall);
  wifiManager.addParameter(&custom_user_state);
  wifiManager.addParameter(&custom_user_cutoff);


  if (!wifiManager.autoConnect("Bandmon")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }

  // connected to WIFI
  Serial.println("i Connected to Wifi");

  //Update user_data with wifiman data
  strcpy(user_data.mqtt_svr, custom_mqtt_server.getValue());
  strcpy(user_data.user_call, custom_user_call.getValue());
  strcpy(user_data.rptr_call, custom_user_rcall.getValue());
  strcpy(user_data.state, custom_user_state.getValue());
  user_data.port = atoi(custom_mqtt_port.getValue());
  user_data.audio_cutoff = atoi(custom_user_cutoff.getValue());

  //save the custom parameters to EEPROM

  if (shouldSaveConfig) {
    Serial.println("[i] Save conig settings to EEPROM");
    write_EEPROM_wifi();

  }
}