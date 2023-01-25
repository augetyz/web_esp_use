
#include<Arduino.h>

int incomedate = 0;
void setup() {

  Serial.begin(9600); 

  Serial.println(78, BIN);// "1001110"
  Serial.println(78, OCT);// "116"
  Serial.println(78, DEC);// "78"
  Serial.println(78, HEX);// "4E"
  Serial.println(1.23456, 0);// "1"
  Serial.println(1.23456, 2);// "1.23"
  Serial.println(1.23456, 4);// "1.2346"
  Serial.println('N');// "N"
  Serial.println("Hello world.");// "Hello world."
}
void loop() {

  if (Serial.available() > 0)
  {
    incomedate = Serial.read();
    if (incomedate == 'H')
    {
      Serial.println("Good Job!");
    }
  }
  delay(1000);
}
