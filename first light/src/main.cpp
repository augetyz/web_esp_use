#include <Arduino.h>

void setup()
 {
   pinMode(2,OUTPUT);
  // put your setup code here, to run once:
}

void loop() 
{
  digitalWrite(2,LOW);
  delay(1000);
  digitalWrite(2,HIGH);
  delay(1000);
  for(int i=0;i<20;i++)
  {
    digitalWrite(2,LOW);
    delay(100);
    digitalWrite(2,HIGH);
    delay(100);
  }
  // put your main code here, to run repeatedly:
}