#include "DHTesp.h"
#include "aliyun_mqtt.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>

//相关的宏定义
#define WIFI_SSID "昌俣的小电瓶" //替换自己的WIFI
#define WIFI_PASSWD "123456789"  //替换自己的WIFI

#define PRODUCT_KEY "hmq8OsDjgrS"          //替换自己的PRODUCT_KEY
#define DEVICE_NAME "uhnP9ZcCgtBbioTidgLD" //替换自己的DEVICE_NAME
#define DEVICE_SECRET \
  "8b665d51cf21167be446d66ef4985904" //替换自己的DEVICE_SECRET
//相关topics定义
#define ALINK_BODY_FORMAT \
  "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST \
  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"

#define ALINK_TOPIC_PROP_POSTRSP \
  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"

#define ALINK_TOPIC_PROP_SET \
  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/post_reply"

#define ALINK_METHOD_PROP_POST "thing.event.property.post"

#define ALINK_TOPIC_DEV_INFO \
  "/ota/device/inform/" PRODUCT_KEY "/" DEVICE_NAME ""
#define ALINK_VERSION_FROMA                                                 \
  "{\"id\":1663346839370,\"params\":{\"CurrentTemperature\":%.2f,"          \
  "\"CurrentHumidity\":%.2f},\"version\":\"1.0\",\"method\":\"thing.event." \
  "property.post\"}"

#define light_pin 2

unsigned long lastMs = 1;

float temperature = 0;
float humidity = 0;

WiFiClient espClient;
DHTesp dht;

PubSubClient mqttClient(espClient);

// WiFi连接
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

void mqtt_check_connect() //连接服务器
{
  while (!mqttClient.connected()) //是否连接，未连接进入循环
  {
    while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME,
                               DEVICE_SECRET)) //一旦连接，发送信号已连接
    {
      Serial.println("MQTT connect succeed!");
      // client.subscribe(ALINK_TOPIC_PROP_POSTRSP);
      // mqttClient.subscribe(ALINK_TOPIC_PROP_SET);

      Serial.println("subscribe done");
      // mqtt_version_post();
    }
  }
}
//回调函数
void mqtt_interval_post()
{
  // char param[512];
  char jsonBuf[1024];

  // sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
  sprintf(jsonBuf, ALINK_VERSION_FROMA, temperature, humidity);
  // sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
  Serial.println(jsonBuf);
  mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.setup(15, DHTesp::DHT11);
  pinMode(light_pin, OUTPUT);
  Serial.println("Demo Start");
  init_wifi(WIFI_SSID, WIFI_PASSWD);
  delay(1000);
  mqttClient.setCallback(mqtt_callback);
}

void loop()
{
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  delay(50);
  if (millis() - lastMs >= 20000)
  {
    lastMs = millis();
    mqtt_check_connect();
    /* Post */
    mqtt_interval_post();
  }
  mqttClient.loop();
}