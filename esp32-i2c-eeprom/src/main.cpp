
#include <Arduino.h>
#include <Wire.h>
#include <WireData.h>

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
void setup()
{

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.begin(115200);

  // TESTS FOR EACH FUNCTION BEGIN HERE
  char write_date[64] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  char read_date[64];
  uint8_t i;
  /* i2c_eeprom_read_date(1,read_date);
  Serial.println(read_date);
  i2c_eeprom_write_date(1,write_date);
  Serial.println(write_date);
  i2c_eeprom_read_date(1,read_date);
  Serial.println(read_date); */
  for (i = 0; i < 512; i++)
  {
    i2c_eeprom_write_date(i, write_date);
  }
}

void loop()

{
  char *read_date;
  int i;
  for (i = 0; i < 512; i++)
  {
    i2c_eeprom_read_date(i, read_date);
    Serial.println(read_date);
    delay(500);
  }
}
int i2c_eeprom_write_date(int page_address, char *date)
{

  int page = strlen(date);
  if (page > 64)
    page = 64;

  i2c_eeprom_write_page(EEPROM_ADDR, 0x40 * page_address, date, page);
  return 1;
}
void i2c_eeprom_read_date(int page_address, char *buffer)
{
  i2c_eeprom_read_buffer(EEPROM_ADDR, 0x40 * page_address, buffer, 64);
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

char *get_string(char *date, int x, int num)
{
  char *return_date;
  int i;
  for (i = 0; i < num; i++)
  {
    return_date[i] = date[x + i - 1];
  }
  return return_date;
}