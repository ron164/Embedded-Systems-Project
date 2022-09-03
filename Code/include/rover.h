/**
 * @file
 * 
 * Rover Library
 * 
 * Functions to get the sensor data from the OPT3101 sensor
 * 
 * Authors: Rohan Ijare
 */

#ifndef rover_h
#define rover_h

#include <Arduino.h>
#include "sensorDriver.h"
#include "motorDriver.h"
#include "AWS.h"

/*********************************************************************************************************************
                                             MACROS
**********************************************************************************************************************/

#define MIDDLE_SENSOR_THRESHOLD 100                        //Threshold value for middle sensor
#define LEFT_SENSOR_THRESHOLD 165                          //Threshold value for left sensor
#define RIGHT_SENSOR_THRESHOLD 165                         //Threshold value for right sensor
#define PI 3.1415926535897932384626433832795               //constant PI value
#define MOTOR_DELAY_ALLIGNMENT delay(1500);                //delay for motor alignment
#define ACCELERATION_DECCELERATION_DELAY delay(30);        //delay for motor acceleration and decceleration
#define ROVER_ANGLE_OFFSET 20                              //offset for rover angle
#define FORWARD_SPEED 150                                  //set threshold speed for forward direction
#define TURN_SPEED 60                                      //set threshold speed for turn direction
#define FORWARD_STEP_SPEED 15                              //step size increase for forward direction
#define TURN_STEP_SPEED  6                                 //step size increase for turn direction
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define OBSTACLE_DETECTED 1                                //obstacle detected
#define OBSTACLE_NOTDETECTED 0                             //obstacle not detected
#define QUADRANT_TWO 180                                   //180 degrees
#define QUADRANT_FOUR 360                                  //360 degrees
#define CONSTANT_180 QUADRANT_TWO                          //180 Value
#define ANGLE_CHECK (roverAngle_value - targetAngleCal_value) < QUADRANT_TWO) //angle check
#define SLOPE_ZERO 0.0                                     //check slope 0.0
#define STACK_SIZE 4096                                    //stack size
#define BAUD_RATE 9600                                     //Baud rate set to 9600

/*********************************************************************************************************************
                                             Variable declaration
**********************************************************************************************************************/

double targetAngleCal_value= 0;
double slope_value= 0, theta_value= 0;
int obstacleDetection_flag= 0;
int leftSensor_value= 0, rightSensor_value= 0, middleSensor_value= 0;

/*********************************************************************************************************************
                                             Functions declaration
**********************************************************************************************************************/

void taskOne(void *parameter);
void taskTwo(void *parameter);
void rover_LocalizationFunction();
void rover_collisionDectectionFunction();
void rover_MotorFunction(Direction motorState_value);

#endif

/**********************************************************END OF FILE*******************************************************************/