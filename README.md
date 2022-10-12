# Embedded-Systems-Project

This repository contains a project of Autonoumous Obstacle avoidance Bot. In this project we have used AWS IoT Greengrass service for localization of the object and the target.

## System Design
![Alt text](/screenshot/system_design.png?raw=true "System Design")

## Task 1 : Setting up AWS Greengrass Group
- Configure a Greengrass Core.
- Publish sensor data using an AWS Greengrass device
- Receive sensor data to the second Greengrass device
![Alt text](/screenshot/Task1.png?raw=true "Task1")

## Task 2 : Implement target detection
- Implementing a bot that finds targets randomly distributed in the scene with obstacles.
- Read the 3-Channel wide ToF distance sensor data.
- Detect the target and virutal boundary via camera module of the Localization system.
![Alt text](/screenshot/Task2.png?raw=true "Task2")

## Task 3 : Setup the communication system
- Setup the communication system between the ESP32 board on the bot to the AWS IoT endpoint using Greengrass Device certificates.
- Create a AWS endpoint and setup a publisher and subscriber node.

## Task 4 : Final behavior implementation
- Develop a scheduler using FreeRTOS to receive the data from the distance sensor and camera.
- The goal is to reach the targets placed in different scenes avoiding the obstaceles. 
- This is done by localization system having camera module publishing the movement of the bot in scene and bot subscribering the same end-point and calculating the path to reach the target coordinates.
![Alt text](/screenshot/Task4.png?raw=true "Task4")

