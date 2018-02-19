//#include ""
//#include <>


void setup(){
  Serial.begin(9600);
}
void loop(){
  unsigned long currentMillis = millis();
  
  Serial.print("Time: ");
  Serial.println(currentMillis);    //prints time since program started
  delay(1000);             // wait a second so as not to send massive amounts of data
}
