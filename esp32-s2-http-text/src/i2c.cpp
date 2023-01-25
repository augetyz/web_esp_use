#include "i2c.h"
int i2c_eeprom_write_date(int page_address,char* date )
{
  
  
  i2c_eeprom_write_page(EEPROM_ADDR, 0x40*page_address, date, 64);
  return 1; 
  

}
void i2c_eeprom_read_date(int page_address,char *buffer )
{
  i2c_eeprom_read_buffer(EEPROM_ADDR, 0x40*page_address,buffer, 62);
}

void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, char data)

{

  int rdata = data;

  Wire.beginTransmission(deviceaddress);

  Wire.write((int)(eeaddress >> 8));

  Wire.write((int)(eeaddress & 0xFF));

  Wire.write(rdata);

  Wire.endTransmission();
}

void i2c_eeprom_write_page(int deviceaddress, unsigned int eeaddresspage, char *data, char length)
{

  Wire.beginTransmission(deviceaddress);

  Wire.write((int)(eeaddresspage >> 8));

  Wire.write((int)(eeaddresspage & 0xFF));

  uint8_t c;

  for (c = 0; c < length; c++)

    Wire.write(data[c]);

  Wire.endTransmission();

  delay(10);
}

char i2c_eeprom_read_byte(int deviceaddress, unsigned int eeaddress)

{

  char rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);

  Wire.write((int)(eeaddress >> 8));

  Wire.write((int)(eeaddress & 0xFF));

  Wire.endTransmission();

  Wire.requestFrom(deviceaddress, 1);

  if (Wire.available())
    rdata = Wire.read();

  return rdata;
}

void i2c_eeprom_read_buffer(int deviceaddress, unsigned int eeaddress, char *buffer, int length)

{

  Wire.beginTransmission(deviceaddress);

  Wire.write((int)(eeaddress >> 8));

  Wire.write((int)(eeaddress & 0xFF));

  Wire.endTransmission();

  Wire.requestFrom(deviceaddress, length);

  for (int c = 0; c < length; c++)

    if (Wire.available())
      buffer[c] = Wire.read();
}

char* get_string(char* date,int x,int num)
{
  char *return_date;
  int i;
  for (i = 0; i < num; i++)
  {
    return_date[i] = date[x + i - 1];
  }
  return return_date;
}
