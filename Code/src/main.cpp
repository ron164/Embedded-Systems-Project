/*********************************************************************************************************
* File Name     : main.cpp
* Description   : Main function is used to perform obstacle avoidance and localization
* Parameters    : Not applicable
* Return        : Not applicable
* Authors		: Rohan Ijare
**********************************************************************************************************/

#include "rover.h"

/*********************************************************************************************************
 *  Function Name : setup
 *  Description   : This function is used in Arduino framework to initialize and set the initial values,
 *                  we have declared pinMode function for LED output, connectAWS for connecting rover 
 *                  to AWS server, sensor object.SETUP to initialize sensor and motor object.SETUP to 
 *                  initialize motor.
 *  Parameters    : Not applicable
 *  Return        : Not applicable
 * ******************************************************************************************************/

void setup()
{
  Serial.begin(BAUD_RATE);
  awsobject.connectAWS();
  awsobject.stayConnected();
  sensorobject.SETUP();
  motorobject.SETUP();

  xTaskCreate(
      taskOne,   /* Task function. */
      "TaskOne", /* String with name of task. */
      STACK_SIZE,      /* Stack size in bytes. */
      NULL,      /* Parameter passed as input of the task */
      ONE,         /* Priority of the task. */
      NULL);     /* Task handle. */

  xTaskCreate(
      taskTwo,   /* Task function. */
      "TaskTwo", /* String with name of task. */
      STACK_SIZE,      /* Stack size in bytes. */
      NULL,      /* Parameter passed as input of the task */
      TWO,         /* Priority of the task. */
      NULL);     /* Task handle. */
}

/*******************************************************************************************************
 *  Function Name : loop
 *  Description   : This function is used in Arduino framework for consecutive execution allowing your
 *                  program to change and respond.                   
 *  Parameters    : Not applicable
 *  Return        : Not applicable
 * ***************************************************************************************************/
void loop()
{
  /* Do nothing */
}

/*******************************************************************************************************
 *  Function Name : taskOne
 *  Description   : This task is used to calculate the slope between two coordinate points, calculate 
 *                  tan inverse of slope which is being used for calculating the target angle on which
 *                  rover should turn in order to reach the target point.
 *  Parameters    : parameter
 *  Return        : Not applicable
 * ****************************************************************************************************/

void taskOne(void *parameter)
{
  for (;;)
  {
    if (((targetX_value - roverX_value) != ZERO) && (obstacleDetection_flag == OBSTACLE_NOTDETECTED))
    {
      slope_value = ((targetY_value - roverY_value) / (targetX_value - roverX_value));
      theta_value = atan(slope_value);
      theta_value = abs(theta_value * (CONSTANT_180 / PI));
      
      if ((targetY_value - roverY_value) > ZERO)
      {
        //rover moving towards target in quadrant four
        if (slope_value > SLOPE_ZERO)
        {
         // Serial.println("Q4");
          targetAngleCal_value = QUADRANT_FOUR - theta_value;
          rover_LocalizationFunction();
        }
        else
        {
          //rover moving towards target in quadrant three
          targetAngleCal_value = QUADRANT_TWO + theta_value;
          rover_LocalizationFunction();
        }
      }
      else
      {
        if (slope_value> SLOPE_ZERO)
        {
          //rover moving towards target in quadrant two
          targetAngleCal_value = QUADRANT_TWO - theta_value;
          rover_LocalizationFunction();
        }
        else
        {
          //rover moving towards target in quadrant one
          targetAngleCal_value = theta_value;
          rover_LocalizationFunction();
        }
      }
    }
    else 
    {
      /* Do nothing */
    }
    ////blocks the current task for the given delay and execute other tasks
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

/**********************************************************************************************************
 *  Function Name : taskTwo
 *  Description   : This task is used for fetching the sensor values from three channel OPT3101 IR sensor
 *                  which detects obstacle at the front, right and left side of the rover which is send to
 *                  rover_collisionDectection function for avoiding collision.
 *  Parameters    : parameter
 *  Return        : Not applicable
 * *******************************************************************************************************/

void taskTwo(void *parameter)
{
  int16_t *sensorData_value;
  for (;;)
  {
    //AWS stays connected with the system to get the coordinates
    awsobject.stayConnected();

    //sensor values are read from the sensor
    sensorData_value = sensorobject.reading();

    //stored in sensorData_value pointer
    leftSensor_value = *sensorData_value;
    middleSensor_value = *(sensorData_value + ONE);
    rightSensor_value = *(sensorData_value + TWO);

    //function call to detect obstacle to avoid collision
    rover_collisionDectectionFunction();

    //blocks the current task for the given delay and execute other tasks
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

/*******************************************************************************************************
 *  Function Name : rover_LocalizationFunction
 *  Description   : This function is used to turn the rover to move towards the target location
 *                  received by the localization system.                   
 *  Parameters    : Not applicable
 *  Return        : Not applicable
 * ****************************************************************************************************/

void rover_LocalizationFunction()
{
  //initializing motoe state local variable
  Direction motorState_value = Forward;
  
  if ((targetAngleCal_value >= roverAngle_value + ROVER_ANGLE_OFFSET) || (targetAngleCal_value <= roverAngle_value - ROVER_ANGLE_OFFSET))
  {
    if ((roverAngle_value > targetAngleCal_value) && (ANGLE_CHECK)
    {
      //turn rover to right side when obstacle detected on the left side
      motorState_value = Right;
      rover_MotorFunction(motorState_value);
    }
    else
    {
      //turn rover to left side when obstacle detected on the right side
      motorState_value = Left;
      rover_MotorFunction(motorState_value);
    }
  }
  else
  {
    //move rover on a straight path when no obstacle is detected
    motorState_value = Forward;
    rover_MotorFunction(motorState_value);
  }
}

/*******************************************************************************************************
 *  Function Name : rover_collisionDectection
 *  Description   : This function is used to detect the obstacle in the field of vision of rover 
 *                  using OPT3101 with a field view of 180 degree, receiving distance values (in mm) to 
 *                  the right, straight and left, we have set a threshold for each sensor value so that 
 *                  rover stops at that particular distance and avoids collision.              
 *  Parameters    : Not applicableNot applicable
 *  Return        : 
 * ****************************************************************************************************/

void rover_collisionDectectionFunction()
{
  //initializing motoe state local variable
  Direction motorState_value = Forward;

  if ((middleSensor_value > MIDDLE_SENSOR_THRESHOLD) && (leftSensor_value > LEFT_SENSOR_THRESHOLD) && (rightSensor_value >RIGHT_SENSOR_THRESHOLD ) && (obstacleDetection_flag == OBSTACLE_DETECTED))
  {   
    //run the rover on a straight path
    motorState_value = Forward;
    rover_MotorFunction(motorState_value);
    MOTOR_DELAY_ALLIGNMENT
    obstacleDetection_flag = OBSTACLE_NOTDETECTED;
  }
  else
  {
    if ((leftSensor_value < LEFT_SENSOR_THRESHOLD) && (obstacleDetection_flag == OBSTACLE_NOTDETECTED))
    {
      //rotate the rover to the right direction
      motorState_value = Right;
      rover_MotorFunction(motorState_value);

      //flag is set to 1 when obstacle is detected on left side
      obstacleDetection_flag = OBSTACLE_DETECTED; 
    }
    else if ((rightSensor_value < RIGHT_SENSOR_THRESHOLD) && (obstacleDetection_flag == OBSTACLE_NOTDETECTED))
    {    
      //rotate rover to the left direction
      motorState_value = Left;
      rover_MotorFunction(motorState_value);

       //flag is set to 1 when obstacle is detected on right side
      obstacleDetection_flag = OBSTACLE_DETECTED;
    }
    else 
    {
      /* Do nothing */
    }
  }
}

/*******************************************************************************************************************************
 *  Function Name : rover_MotorFunction
 *  Description   : This function is used to synchronize the 
 *                  motors to turn left and right based on the
 *                  state received by the fution.After which 
 *                  the program identifies the relevant state 
 *                  and turns the motor.*               
 *  Parameters    : motorState_value
 *  Return        : Not Applicable
 * *****************************************************************************************************************************/

void rover_MotorFunction(Direction motorState_value)
{

  int tempVar = ZERO;
  
  //checks the motor state based on motor class enums
  switch (motorState_value)
    {
    case Forward:
      for (tempVar = ZERO; tempVar < FORWARD_SPEED;)
      {
        motorobject.set_speed(MotorA, Forward, tempVar);
        motorobject.set_speed(MotorB, Backward, tempVar);
        tempVar = tempVar + FORWARD_STEP_SPEED;
        ACCELERATION_DECCELERATION_DELAY
      }
      break;
    case Left:
      for (tempVar = ZERO; tempVar < TURN_SPEED;)
      {
        motorobject.set_speed(MotorA, Backward, tempVar);
        motorobject.set_speed(MotorB, Backward, tempVar);
        tempVar = tempVar + TURN_STEP_SPEED;
        ACCELERATION_DECCELERATION_DELAY
      }
      break;
    case Right:
      for (tempVar = ZERO; tempVar < TURN_SPEED;)
      {
        motorobject.set_speed(MotorA, Forward, tempVar);
        motorobject.set_speed(MotorB, Forward, tempVar);
        tempVar = tempVar + TURN_STEP_SPEED;
        ACCELERATION_DECCELERATION_DELAY
      }
    default:
      /*  Do Nothing */
      break;
    }

}

/**********************************************************END OF FILE*******************************************************************/