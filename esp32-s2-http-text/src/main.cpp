#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <TFT_eSPI.h>
#include "Lcd.h"
#include "i2c.h"
#include <Wire.h>
//按键都是上拉输入，意思是低电平触发按键
#define Key_read_PIN 34           //按键一，阅读模式的下一页
#define Key_write_PIN 35          //按键二，阅读模式的上一页
const char *AP_SSID = "ESP32";    //热点名字
const char *AP_PASS = "12345678"; //密码，不得低于八位
IPAddress apIP(192, 168, 4, 1);   // ip地址,访问：192.168.4.1
//除了下面的几个引脚，其他的都可以改成按键。
// 4、5不能动，是储存器的引脚
/*
#define TFT_MISO -1
#define TFT_MOSI 11
#define TFT_SCLK 10
#define TFT_CS    9  // Chip select control pin
#define TFT_DC    18  // Data Command control pin
#define TFT_RST   21  // Reset pin (could connect to RST pin)
屏幕引脚，也不能改
 */

const byte DNS_PORT = 53;
const int webPort = 80;
const char *HOST_NAME = "MY_ESP32S";

// 192.168.4.1
WebServer server(80);
DNSServer dnsServer;
extern TFT_eSPI tft;
String text = "世事无常123564879798456498798744894654654154987894564654987498"; //栈空间申请，别动，修改了长度，freertos会断言重启
uint8_t have_text = 0;
void handleRoot();
void handleNotFound();
void handleled();
void handle_pass_right();
void handle_pass_wrong();
void initSoftAP();
void initDNS();
int mode_select(void);

String sendHTML(bool buttonState);
uint8_t sign = 1;
String con_pass = "12345678"; //
int page = 0;
int write_read = 0;

void setup()
{

  LCD_init();
  initSoftAP();
  initDNS();
  Wire.begin(SDA_PIN, SCL_PIN);
  server.begin();
  server.on("/", HTTP_GET, handleRoot);           //
  server.on("/configwifi", HTTP_POST, handleled); //
  server.onNotFound(handleNotFound);              // 404椤甸潰
  pinMode(Key_read_PIN, INPUT_PULLUP);            //上拉，低电平有效
  pinMode(Key_write_PIN, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  write_read = mode_select();
  tft.fillScreen(TFT_BLACK);
  LCD_showstring("连接WiFi    进入网页输入密码  192.68.4.1");
}

void loop(void)
{
  server.handleClient();
  if (sign == 0)
  {
    if (write_read)
    {
      if (have_text)
      {
        tft.fillScreen(TFT_BLACK);
        LCD_showstring(text);
        i2c_eeprom_write_date(page++, &text[0]);
        have_text = 0;
      }
    }
    else
    {
      if (digitalRead(Key_read_PIN) == 0)
      {
        delay(20);
        if (digitalRead(Key_read_PIN) == 0)
        {
          while (digitalRead(Key_read_PIN) == 0)
            ;

          i2c_eeprom_read_date(page++, &text[0]);
          text = text + '\0';
          tft.fillScreen(TFT_BLACK);
          LCD_showstring(text);
        }
      }
      if (digitalRead(Key_write_PIN) == 0)
      {
        delay(20);
        if (digitalRead(Key_write_PIN) == 0)
        {
          while (digitalRead(Key_write_PIN) == 0)
            ;
          i2c_eeprom_read_date(page--, &text[0]);
          text = text + '\0';
          tft.fillScreen(TFT_BLACK);
          LCD_showstring(text);
        }
      }
      delay(10);
    }
    if (digitalRead(12) == 0)
      write_read = ~write_read;
  }
}

void initSoftAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  if (WiFi.softAP(AP_SSID, AP_PASS))
  {
    Serial.println("ESP-32S SoftAP is right.");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
  }
  else
  {

    delay(1000);

    ESP.restart();
  }
}

void initDNS()
{

  if (dnsServer.start(DNS_PORT, "*", apIP))
  {
    tft.fillScreen(TFT_BLACK);
    LCD_showstring("start dnsserver success.");
  }
  else
  {
    tft.fillScreen(TFT_BLACK);
    LCD_showstring("start dnsserver failed.");
  }
}
void handleRoot()
{
  if (sign)
  {
    server.send(200, "text/html",
                "<!DOCTYPE html><html><head><title>WIFI_Text biubiu</title><meta name=\"viewport\" \
    content=\"width=device-width, initial-scale=1\">\
    </head>   \
    <head>    \
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=GBK\" />\
    </head>\
    Please Input pass:\
    <style type=\"text/css\">.input{display: block; margin-top: 10px;}.\
    input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.\
    input input{height: 30px;width: 200px;}.\
    btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}\
    </style><body><form method=\"POST\" action=\"configwifi\">\
    <label \
    class=\"input\">\
    <span>\
    TEXT_Input</span><input type=\"text\"  \
    name=\"text\"></label><input class=\"btn\" \
    type=\"submit\" name=\"Enter\"\
    value=\"Submie\"> </form>");
  }
  else
  {
    server.send(200, "text/html",
                "<!DOCTYPE html><html><head><title>WIFI_Text biubiu</title><meta name=\"viewport\" \
    content=\"width=device-width, initial-scale=1\">\
    </head>   \
    <head>    \
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=GBK\" />\
    </head>\
    Please Input Text  \
    <style type=\"text/css\">.input{display: block; margin-top: 10px;}.\
    input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.\
    input input{height: 130px;width: 2000px;}.\
    btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}\
    </style><body><form method=\"POST\" action=\"configwifi\">\
    <label \
    class=\"input\">\
    <span>\
    TEXT_Input</span><input type=\"text\"  \
    name=\"text\"></label><input class=\"btn\" \
    type=\"submit\" name=\"Enter\"\
    value=\"Submie\"> </form>");
  }
}

void handleled()
{

  //  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //  server.sendHeader("Location", "/");
  //  server.send(303);
  if (server.hasArg("text"))
  {
    if (sign == 0)
    {

      text = server.arg("text");
      // strcpy(sta_pass, server.arg("pass").c_str());
      text += '\0';
      have_text = 1;
      handleRoot();
    }
    else
    {
      text = server.arg("text");

      // strcpy(sta_pass, server.arg("pass").c_str());
      if (text == con_pass)
      {
        sign = 0;
        handle_pass_right();
      }
      else
      {
        handle_pass_wrong();
      }
      handleRoot();
    }
  }
  else
  {
    return;
  }
  return;
  handleRoot();
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found");
  delay(2000);
  handleRoot();
}

void handle_pass_right()
{
  server.send(200, "text/html",
              "<!DOCTYPE html><html><head><title>WIFI_Text biubiu</title><meta name=\"viewport\" \
    content=\"width=device-width, initial-scale=1\">\
    </head>   \
    <head>    \
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=GBK\" />\
    </head>\
    pass_right  \
    <style type=\"text/css\">.input{display: block; margin-top: 10px;}.\
    input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.\
    input input{height: 130px;width: 2000px;}.\
    btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}\
    </style><body><form method=\"POST\" action=\"configwifi\">\
    <label \
    class=\"input\">\
    <span>\
    TEXT_Input</span><input type=\"text\"  \
    name=\"text\"></label><input class=\"btn\" \
    type=\"submit\" name=\"Enter\"\
    value=\"Submie\"> </form>");
}
void handle_pass_wrong()
{
  server.send(200, "text/html",
              "<!DOCTYPE html><html><head><title>WIFI_Text biubiu</title><meta name=\"viewport\" \
    content=\"width=device-width, initial-scale=1\">\
    </head>   \
    <head>    \
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=GBK\" />\
    </head>\
    pass_wrong  \
    <style type=\"text/css\">.input{display: block; margin-top: 10px;}.\
    input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.\
    input input{height: 130px;width: 2000px;}.\
    btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}\
    </style><body><form method=\"POST\" action=\"configwifi\">\
    <label \
    class=\"input\">\
    <span>\
    TEXT_Input</span><input type=\"text\"  \
    name=\"text\"></label><input class=\"btn\" \
    type=\"submit\" name=\"Enter\"\
    value=\"Submie\"> </form>");
}
int mode_select(void)
{
  tft.fillScreen(TFT_BLACK);
  LCD_showstring("通过按键选择工作模式Write:Key1Read:key2");
  while (1)
  {
    if (digitalRead(Key_read_PIN) == 0)
      return 0;
    if (digitalRead(Key_write_PIN) == 0)
      return 1;
  }
}
