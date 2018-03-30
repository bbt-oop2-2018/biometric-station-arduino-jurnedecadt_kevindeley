double x = 2.45;
int y = 9;

void setup() {
  Serial.begin(115200);
  
  
  //Serial.println(y);

}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.println("mensen");
Serial.println(String("[") + x + String("|") + y + String("]")); 
delay(500);
}
