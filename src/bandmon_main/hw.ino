
// Radio pins

// COR pin attach as an interupt to get events and use milsto export the data

void init_io(){

  //attachInterrupt(digitalPinToInterrupt(SQL_pin), ISR,CHANGE);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SQL_pin,INPUT);

}