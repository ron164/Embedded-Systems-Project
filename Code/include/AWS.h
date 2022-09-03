/**
 * @file
 * 
 * ESP32 AWS Library
 * 
 * Functions to get the crawler coordinates from the Camera over AWS IoT
 * 
 * Authors: Rohan Ijare
 */

#ifndef aws_h
#define aws_h

#include <MQTTClient.h>

/*********************************************************************************************************************
                                             MACROS
**********************************************************************************************************************/

/* The MQTT topics that this device should publish/subscribe to */
#define AWS_IOT_SUBSCRIBE_TOPIC_ROVER "esp32/rover"
#define AWS_IOT_SUBSCRIBE_TOPIC_TARGET "esp32/target"

//Extraction of Json payload
#define TARGET_START_POINT TargetCoord.indexOf("(") 
#define TARGET_DELIMITER TargetCoord.indexOf(",")
#define TARGET_END_POINT TargetCoord.indexOf(")")
#define ROVER_STAR_POINT RoverCoord.indexOf("(")
#define ROVER_DELIMITER RoverCoord.indexOf(",")
#define ROVER_MID_POINT RoverCoord.indexOf(")")
#define ROVER_END_POINT RoverCoord.indexOf("]")

/*********************************************************************************************************************
                                             Extern Variable declaration
**********************************************************************************************************************/

extern double roverX_value, roverY_value, targetX_value, targetY_value, roverAngle_value;

void messageHandler(String, String);

class myawsclass {
  public:
    myawsclass();

    void connectAWS();                            /* Initialize and connect to AWS */
    void publishMessage(int16_t sensorValue);     /* Publish the values of the sensors */
    void stayConnected();                         /* Maintain the connection */
};

extern myawsclass awsobject;

#endif

/**********************************************************END OF FILE*******************************************************************/