#include "aliyun_date_deal.h"
//WiFi连接
extern unsigned long lastMs;
extern int light_state;
WiFiClient   espClient;
PubSubClient mqttClient(espClient);

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
void wifi_connect(void)
{
    init_wifi(WIFI_SSID, WIFI_PASSWD);
    mqttClient.setCallback(mqtt_callback);
}

void aliyun_light(void)
{
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
}