#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <TFT_eSPI.h>
#include "Lcd.h"

#define LED_PIN 36 //

const char *ssid = "USTIC";
const char *password = "topstudio";
const byte DNS_PORT = 53;              //设置DNS�???口号
const int webPort = 80;                //设置Web�???口号
const char *AP_SSID = "NodeMCU-ESP32"; //设置AP�???点名�???
// const char* AP_PASS  = "";               //设置AP�???点密�???
const char *HOST_NAME = "MY_ESP32S"; //设置设�?�名
IPAddress apIP(192, 168, 4, 1);      //设置AP的IP地址
// 192.168.4.1
WebServer server(80); // 建立网络服务�?�??�象，�?��?�象用于响应HTTP请求。监�???�???口（80�???
DNSServer dnsServer;
extern TFT_eSPI tft;
String text = "123?abc���?";
uint8_t have_text = 1;

void handleRoot();
void handleNotFound();
void handleled();
void handle_pass_right();
void handle_pass_wrong();
void initSoftAP();
void initDNS();

String sendHTML(bool buttonState);
uint8_t sign = 1;
String con_pass = "12345678"; //

void setup()
{
  LCD_init();
  initSoftAP();
  initDNS();
  server.begin();
  server.on("/", HTTP_GET, handleRoot);           //服务�?�??�页
  server.on("/configwifi", HTTP_POST, handleled); // led控制�???
  server.onNotFound(handleNotFound);              // 404页面

}

void loop(void)
{
  server.handleClient();
  if (have_text)
  {
    tft.fillScreen(TFT_WHITE);
    LCD_showstring(text);
    have_text = 0;
  }
}

void initSoftAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  if (WiFi.softAP(AP_SSID))
  {
    Serial.println("ESP-32S SoftAP is right.");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
  }
  else
  { //开�???�???点失�???
    Serial.println("WiFiAP Failed");
    delay(1000);
    Serial.println("restart now...");
    ESP.restart(); //重启复位esp32
  }
}

//初�?�化DNS服务�???
void initDNS()
{
  //判断将所有地址映射到esp32的ip上是否成�???
  if (dnsServer.start(DNS_PORT, "*", apIP))
  {
    Serial.println("start dnsserver success.");
  }
  else
  {
    Serial.println("start dnsserver failed.");
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
    set_PASS:\
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
    input_TEXT \
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

      text = server.arg("text"); //获取html表单输入框name名为"pwd"的内�???
      // strcpy(sta_pass, server.arg("pass").c_str());
      text += '\0';
      have_text = 1;
      handleRoot();
    }
    else
    {
      text = server.arg("text"); //获取html表单输入框name名为"pwd"的内�???

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
    input_TEXT  \
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
    The Pass is wrong,try again  \
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
