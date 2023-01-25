#ifndef _LCD_H
#define _LCD_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "font.h"
#include <string.h>
void LCD_init(void);
void LCD_showstring(String TEXT);
#endif