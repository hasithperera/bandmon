

void read_EEPROM_wifi(){

  EEPROM.begin(512);
  // dump eeprom data
  EEPROM.get(0, wifi_data); 
  EEPROM.get(sizeof(wifi_data)+1,user_data);

  Serial.println();
  Serial.printf("i user_call:%s\n",user_data.user_call);
  Serial.printf("i repeater_call:%s\n",user_data.rptr_call);
  Serial.printf("i state:%s\n",user_data.state);
  //Serial.printf("i mqtt server:%s:%s\n",user_data.mqtt_svr,user_data.port);
  Serial.printf("i mqtt svr:%s\n",user_data.mqtt_svr);
  Serial.printf("i mqtt port:%d\n",user_data.port);
  Serial.printf("i timeout:%d\n",user_data.timeout);
  Serial.printf("i audio cutoff:%f\n",user_data.audio_cutoff);


}


void write_EEPROM_wifi(){
  //write user and wifi data to EEPROM

  EEPROM.begin(512);
  EEPROM.put(0,wifi_data);
  EEPROM.put(sizeof(wifi_data)+1,user_data);
  EEPROM.commit();
}



/*

void write_EEPROM_repeater(int rep_id, struct s_rptr_data rep){
  EEPROM.begin(512);
  EEPROM.put(sizeof(wifi_data)+5+sizeof(rep)*rep_id+1,rep);

}
void read_EEPROM_repeater(){
  struct s_rptr_data rep;
  EEPROM.begin(512);
  for (int i=0;i<5;i++){
    EEPROM.get(sizeof(wifi_data)+5+sizeof(rep)*i+1,rep);
    repeaters[i] = rep;
  }
}

void save_bglevel(){

// update


}

*/