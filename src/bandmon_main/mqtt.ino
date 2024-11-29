void reconnect() {
  itoa(chipid,str2,10);
  while (!client.connected()) {
    Serial.print("-");
    // Attempt to connect
    if (client.connect(str2)) {
      Serial.println(".");

      // Subscribe
      client.subscribe("bandmon/cmd");
      
      //alive packet
      //sprintf(str2,"%d",chipid)
      client.publish("bandmon/alive",str2);


    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}