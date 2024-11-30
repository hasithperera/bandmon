
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