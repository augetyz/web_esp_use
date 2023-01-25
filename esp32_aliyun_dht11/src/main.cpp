
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "aliyun_mqtt.h"
#include <ArduinoJson.h>


//相关的宏定义
#define WIFI_SSID        "昌俣的小电瓶"//替换自己的WIFI
#define WIFI_PASSWD      "123456789"//替换自己的WIFI

#define PRODUCT_KEY      "hmq8kmgN8Fc" //替换自己的PRODUCT_KEY
#define DEVICE_NAME      "xDitVCIxdOE6Ozb7Vv9Y" //替换自己的DEVICE_NAME
#define DEVICE_SECRET    "3ff0101352249c0532e48a4b74d58bce"//替换自己的DEVICE_SECRET
//相关topics定义
#define ALINK_BODY_FORMAT         "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_POSTRSP  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET      "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_METHOD_PROP_POST    "thing.event.property.post"
#define ALINK_TOPIC_DEV_INFO      "/ota/device/inform/" PRODUCT_KEY "/" DEVICE_NAME ""    
#define ALINK_VERSION_FROMA      "{\"id\": 123,\"params\": {\"version\": \"%s\"}}"

#define light_pin 2

unsigned long lastMs = 1;

int light_state=0;//light的状态标志

typedef char* string_char;

WiFiClient   espClient;
PubSubClient mqttClient(espClient);

//WiFi连接
void init_wifi(const char *ssid, const char *password)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi does not connect, try again ...");
        delay(500);
    }

    Serial.println("Wifi is connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

string_char get_string(char x,char y,string_char target)
{
	string_char biu;
	char *biu1;
	int a=0,b=0,c=0,d;
	while(*(target+a++)!=x);
	//a--;		
	c=strlen(target);
	while(*(target+c-b++)!=y);
	b--;
	for(d=0;d<(c-b-a);d++)
	{
		biu1[d]=*(target+a+d);
	}
	biu1[d]='\0';
	biu=biu1;
	return biu;
}

void light_switch(char *payload)
{
    char *biu;
    biu=get_string('{','}',payload);
    Serial.println(biu);
    biu=get_string('{','}',payload);
    Serial.println(biu);
    if(biu[9]=='1')
    {
        light_state=1;
    }
    else
    {
        if(biu[9]=='0')
        {
            light_state=0;
        }
        else
            {
                light_state=2;               
            }
    }
}



void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';
    Serial.println((char *)payload);
    Serial.println(payload[77]);
    //light_switch((char *)payload);
    if(payload[76]=='1')
    {
        light_state=1;
    }
    else
    {
        if(payload[76]=='0')
        {
            light_state=0;
        }
        else
            {
                light_state=2;               
            }
    }
    if (strstr(topic, ALINK_TOPIC_PROP_SET))
    {
        StaticJsonBuffer<100> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);
        if (!root.success())
        {
            Serial.println("parseObject() failed");
            return;
        }
    }
}

void mqtt_check_connect()
{
    while (!mqttClient.connected())//mqttδ����
    {
        while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
        {
            Serial.println("MQTT connect succeed!");
            //client.subscribe(ALINK_TOPIC_PROP_POSTRSP);
            mqttClient.subscribe(ALINK_TOPIC_PROP_SET);
            
            Serial.println("subscribe done");
            //mqtt_version_post();
        }
    }
    
}
//回调函数
void mqtt_interval_post()
{
   // char param[512];
    char jsonBuf[1024];

    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(jsonBuf, "{\"id\":\"bc:dd:c2:30:cb:dc\",\"params\":{\"ID\":\"admin\",\"Password\": \"123456\",\"m_idtf_code\":\"code001\",\"m_telphone\":\"13015667456\",\"m_mode\":1,\"m_status\":2,\"m_weight\":50,\"m_infrare_01\":0,\"m_infrare_02\":1,\"picture\":\"252648165414548651665\",\"m_swtich\":0,\"Error\":0}}");
    //sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}

void setup() 
{
  // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(light_pin,OUTPUT);
    Serial.println("Demo Start");
    init_wifi(WIFI_SSID, WIFI_PASSWD);
    mqttClient.setCallback(mqtt_callback);
}

void loop() {

    if (millis() - lastMs >= 20000)
    {
        lastMs = millis();
        mqtt_check_connect();
        /* Post */        
       // mqtt_interval_post();
    }
    mqttClient.loop();
    delay(1000);
    switch(light_state)
    {
        case 1:digitalWrite(light_pin,HIGH);break;
        case 0:digitalWrite(light_pin,LOW);break;
        case 2:
            digitalWrite(light_pin,LOW);delay(500);
            digitalWrite(light_pin,HIGH);delay(500);
            digitalWrite(light_pin,LOW);delay(500);
            digitalWrite(light_pin,HIGH);delay(500);break;
    }
    light_state=3;
}