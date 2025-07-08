
boolean reconnect() {
  itoa(chipid,tmp_value,10);
  if (client.connect(tmp_value)) {
    // Once connected, publish an announcement...
    client.publish("bandmon/alive",tmp_value);

    // ... and resubscribe
    client.subscribe("bandmon/cmd");

  }
  return client.connected();
}

//added sub function
void sub_callback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.
  Serial.print("topic:");
  Serial.println(topic);


  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }

  if((char)payload[0]=='d'){
    debug_print = 1;
  }


  if((char)payload[0]=='r'){
    ESP.restart();
  }

  // Allocate the correct amount of memory for the payload copy
  
  
  //byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer
  //memcpy(p,payload,length);
  //client.publish("outTopic", p, length);
  // Free the memory
  //free(p);
}