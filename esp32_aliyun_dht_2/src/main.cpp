#include <WiFi.h>
#include "DHTesp.h"
#include "PubSubClient.h"
#include <Arduino.h>


/* 连接WIFI SSID和密码 */
#define WIFI_SSID         "昌俣的小电瓶"
#define WIFI_PASSWD       "123456789"

/* 设备的三元组信息*/
#define PRODUCT_KEY       "hmq8OsDjgrS"
#define DEVICE_NAME       "uhnP9ZcCgtBbioTidgLD"
#define DEVICE_SECRET     "8b665d51cf21167be446d66ef4985904"
#define REGION_ID         "cn-shanghai"

/* 线上环境域名和端口号，不需要改 */
#define MQTT_SERVER       PRODUCT_KEY".iot-as-mqtt."REGION_ID".aliyuncs.com"
#define MQTT_PORT         1883
#define MQTT_USRNAME      DEVICE_NAME"&"PRODUCT_KEY

#define CLIENT_ID         "hmq8OsDjgrS.uhnP9ZcCgtBbioTidgLD|securemode=2,signmethod=hmacsha256,timestamp=1663346563798|"
#define MQTT_PASSWD       "aa0ab6757e46dbdd67de980bd4f297e25403b7d37f7b87a705098e025dcfa363"


#define ALINK_BODY_FORMAT         "{\"id\":1663346839370,\"params\":{\"CurrentTemperature\":%.2f,\"CurrentHumidity\":%.2f},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"

unsigned long lastMs = 0;
WiFiClient espClient;
PubSubClient  client(espClient);

float temperature=0;
float humidity=0;  
DHTesp dht;
//连接wifi
void wifiInit()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("WiFi not Connect");
    }
    
}

//mqtt连接
void mqttCheckConnect()
{
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT Server ...");
        if(client.connect(CLIENT_ID, MQTT_USRNAME, MQTT_PASSWD))
        {
          Serial.println("MQTT Connected!");
        }
        else
          {
            Serial.print("MQTT Connect err:");
            Serial.println(client.state());
            //delay(5000);
          }
        
    }
}

void mqttIntervalPost()
{
    char param[32];
    char jsonBuf[128];
    
    sprintf(jsonBuf, ALINK_BODY_FORMAT, temperature,humidity);
    
    Serial.println(jsonBuf);
    boolean b = client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
    if(b){
      Serial.println("publish Humidity success"); 
    }else{
      Serial.println("publish Humidity fail"); 
    } 
}

void setup() 
{
    Serial.begin(115200);
    dht.setup(15, DHTesp::DHT11);
    wifiInit();
    client.setServer(MQTT_SERVER, MQTT_PORT);   /* 连接MQTT服务器 */
}

void loop()
{
    humidity    = dht.getHumidity();
    temperature = dht.getTemperature(); 
    if (millis() - lastMs >= 5000)
    {
        lastMs = millis();
        mqttCheckConnect(); 
        /* 上报 */
        mqttIntervalPost();
    }
    client.loop();
    delay(2000);
}