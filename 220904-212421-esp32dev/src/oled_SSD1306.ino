#include "SSD1306.h"

SSD1306 display(0x3c, 21, 22);  //address sda scl

void setup() 
{
  display.init();
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, "Hello World");
  display.display();
  delay(1000);
  
}

void loop(void) 
{
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, "Hello World");
  display.display();
  delay(1000);  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Hello World");
  display.display();
  delay(1000);display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello World");
  display.display();
  delay(1000);display.clear();
}
