#ifndef _I2C_H
#define _I2C_H

#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 4 // SDA引脚，默认gpio4(D2)
#define SCL_PIN 5 // SCL引脚，默认gpio5(D1)

#define EEPROM_ADDR 0x50

void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, char data);
void i2c_eeprom_write_page(int deviceaddress, unsigned int eeaddresspage, char *data, char length);
char i2c_eeprom_read_byte(int deviceaddress, unsigned int eeaddress);
void i2c_eeprom_read_buffer(int deviceaddress, unsigned int eeaddress, char *buffer, int length);
char *get_string(char *date, int x, int num);
int i2c_eeprom_write_date(int page_address, char *date);
void i2c_eeprom_read_date(int page_address, char *buffer);
#endif