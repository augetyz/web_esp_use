#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#define LED_BUILTIN 2 //  按钮引脚D3

const char *ssid = "USTIC";
const char *password = "topstudio";
const byte DNS_PORT = 53;              //设置DNS端口号
const int webPort = 80;                //设置Web端口号
const char *AP_SSID = "NodeMCU-ESP32"; //设置AP热点名称
// const char* AP_PASS  = "";               //设置AP热点密码
const char *HOST_NAME = "MY_ESP32S"; //设置设备名
IPAddress apIP(192, 168, 4, 1);      //设置AP的IP地址
//192.168.4.1
WebServer server(80); // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
DNSServer dnsServer; 
void handleRoot();
void handleNotFound();
void handleled();
String sendHTML(bool buttonState);
uint8_t sign = 1;
String con_pass = "12345678";//通讯密码

void initSoftAP()
{
  WiFi.mode(WIFI_AP);                                         //配置为AP模式
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); //设置AP热点IP和子网掩码
  if (WiFi.softAP(AP_SSID))
  {
    //开启AP热点,如需要密码则添加第二个参数
    //打印相关信息
    Serial.println("ESP-32S SoftAP is right.");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
  }
  else
  { //开启热点失败
    Serial.println("WiFiAP Failed");
    delay(1000);
    Serial.println("restart now...");
    ESP.restart(); //重启复位esp32
  }
}

//初始化DNS服务器
void initDNS()
{
  //判断将所有地址映射到esp32的ip上是否成功
  if (dnsServer.start(DNS_PORT, "*", apIP))
  {
    Serial.println("start dnsserver success.");
  }
  else
  {
    Serial.println("start dnsserver failed.");
  }
}
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  initSoftAP();
  initDNS();
  server.begin();
  server.on("/", HTTP_GET, handleRoot);           //服务器首页
  server.on("/configwifi", HTTP_POST, handleled); // led控制页
  server.onNotFound(handleNotFound);              // 404页面

  Serial.print("HTTP server started");
}

void loop(void)
{
  server.handleClient();
}

void handleRoot()
{
  if(sign)
  {
    server.send(200, "text/html",
                "<!DOCTYPE html><html><head><title>WIFI_Text biubiu</title><meta name=\"viewport\" \
    content=\"width=device-width, initial-scale=1\">\
    </head>   \
    <head>    \
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=GBK\" />\
    </head>\
    please input pass \
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
    please input text \
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
}

void handleled()
{
  
  String text;
  //  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //  server.sendHeader("Location", "/");
  //  server.send(303);
  if (server.hasArg("text"))
  {
    if(sign==0)
    {
    Serial.print("got password:");
    text = server.arg("text"); //获取html表单输入框name名为"pwd"的内容
    // strcpy(sta_pass, server.arg("pass").c_str());
    Serial.println(text);
    handleRoot();
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    }
    else
    {
    Serial.print("got pass:");
    text = server.arg("text"); //获取html表单输入框name名为"pwd"的内容
    // strcpy(sta_pass, server.arg("pass").c_str());
    if(text==con_pass)
      sign = 0;
    else
      Serial.print("error\n");
    handleRoot();
    }
    
  }
  else
  {
    Serial.println("error, not found text");
    //      server.send(200, "text/html", "<meta charset='UTF-8'>error, not found password");
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






