/****************************************/
//esp32与设备连接同一WiFi，访问服务器，当esp32接收到open后，亮灯，否则灭灯
//已实现
/***************************************/

#include <Arduino.h>
#include<WiFi.h>

const char *ssid = "昌俣的小电瓶"; //wifi名
const char *password = "123456789";//wifi密码

const IPAddress serverIP(192,168,75,130); //欲访问的服务端IP地址
uint16_t serverPort = 8000;         //服务端口号

WiFiClient client; //声明一个ESP32客户端对象，用于与服务器进行连接

void setup()
{
    Serial.begin(115200);
    Serial.println();

    pinMode(2,OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高响应速度
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
}


void loop()
{
    Serial.println("尝试访问服务器");
    if (client.connect(serverIP, serverPort)) //尝试访问目标地址
    {
        Serial.println("访问成功");
        client.print("Hello world!");                    //向服务器发送数据
        while (client.connected() || client.available()) //如果已连接或有收到的未读取的数据
        {
            if (client.available()) //如果有数据可读取
            {
                String line = client.readStringUntil('\n'); //读取数据到换行符
                Serial.print("读取到数据：");
                Serial.println(line);
                if(line=="open")
                {
                    digitalWrite(2,HIGH);
                }
                else
                {
                    digitalWrite(2,LOW);
                }
              //client.write(line.c_str()); //将收到的数据回发
            }
        }
        Serial.println("关闭当前连接");
        client.stop(); //关闭客户端
    }
    else
    {
        Serial.println("访问失败");
        client.stop(); //关闭客户端
    }
    delay(5000);
}
