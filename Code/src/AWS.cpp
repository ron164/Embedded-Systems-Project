/**
 * ESP32 AWS Library
 * 
 * Functions to get the crawler coordinates from the Camera over AWS IoT
 * 
 * Authors: Rohan Ijare
 */

/*
  Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
  Permission is hereby granted, free of charge, to any person obtaining a copy of this
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify,
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "AWS.h"
#include <string.h>
#include <unordered_map>
#include <stdlib.h>

/*********************************************************************************************************************
                                            Extern Variable declaration
**********************************************************************************************************************/

double targetX_value;
double targetY_value;
double roverX_value;
double roverY_value;
double roverAngle_value;

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

myawsclass::myawsclass()
{
}

void messageHandler(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);
  StaticJsonDocument<200> doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err)
  {
    Serial.print("Error: ");
    Serial.println(err.c_str());
    return;
  }
  if (topic == "esp32/target")
  {
    String TargetCoord = doc["target"];
    targetX_value = (double)atoi(TargetCoord.substring(TARGET_START_POINT + 1, TARGET_DELIMITER).c_str());
    targetY_value = (double)atoi(TargetCoord.substring(TARGET_DELIMITER + 1, TARGET_END_POINT).c_str());
  }
  if (topic == "esp32/rover")
  {
    String RoverCoord = doc["rover"];
    roverX_value = (double)atoi(RoverCoord.substring(ROVER_STAR_POINT + 1, ROVER_DELIMITER).c_str());
    roverY_value = (double)atoi(RoverCoord.substring(ROVER_DELIMITER + 1, ROVER_MID_POINT).c_str());
    roverAngle_value = (double)atoi(RoverCoord.substring(ROVER_MID_POINT + 2, ROVER_END_POINT).c_str());
    Serial.println("x coordinate of rover");
    Serial.println(roverX_value);
    Serial.println("y coordinate of rover");
    Serial.println(roverY_value);
    Serial.println("direction of rover");
    Serial.println(roverAngle_value);

  }
}

void myawsclass::stayConnected()
{
  client.loop();
}

void myawsclass::connectAWS()
{

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("Connecting...!");
  }

  Serial.print("CONNECTED...!\n");

  /* Configure WiFiClientSecure to use the AWS IoT device credentials */
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  /* Connect to the MQTT broker on the AWS endpoint we defined earlier */
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  /* Create a message handler */
  client.onMessage(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  /* Subscribe to a topic */
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC_TARGET);
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC_ROVER);

  Serial.println("AWS IoT Connected!");
  //Serial.println("TARGET VALUES: ");
  //Serial.println(client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC_TARGET));
  //Serial.println("ROVER VALUES: ");
  //Serial.println(client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC_ROVER));
}

void myawsclass::publishMessage(int16_t sensorValue)
{

  StaticJsonDocument<200> doc;
  //doc["time"] = millis();
  doc["sensor"] = sensorValue;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); /* print to client */
  Serial.print("Check:");

  Serial.print(client.publish(AWS_IOT_SUBSCRIBE_TOPIC_ROVER, jsonBuffer));
}

myawsclass awsobject = myawsclass(); /* creating an object of class aws */

/**********************************************************END OF FILE*******************************************************************/