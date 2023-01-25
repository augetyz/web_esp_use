#ifndef _ALIYUN_DATE_DEAL_H
#define _ALIYUN_DATE_DEAL_H

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


void init_wifi(const char *ssid, const char *password);
void mqtt_callback(char *topic, byte *payload, unsigned int length);
void mqtt_check_connect();
void mqtt_interval_post();
void wifi_connect(void);
void aliyun_light(void);
#endif
