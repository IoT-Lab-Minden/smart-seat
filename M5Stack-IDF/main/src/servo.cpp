/**
 * @file servo.cpp
 * @brief Source code for the Servo class
 *
 * @author	Torsten Niemeier
 * @author	E-mail: <tniemeier1@fh-bielefeld.de>
 * @version 1.0
 * @date 2019/12/21
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The Servo class represents a combination of one motor and one potentiometer.
 */

#include "../include/servo.hpp"



/**
 * @brief Constructor
 * @brief The function creates a Servo object.
 *
 * @param motor - a pointer to the Servo's assigned Motor class instance
 * @param potentiometer - a pointer to the Servo's assigned Potentiometer class instance
 * @param changeMotDir - a bool value that defines if the Motor direction shall be inverted
 * @param changePotiDir - a bool value that defines if the Potentiometer direction shall be inverted
 * @return nothing
 */
Servo::Servo(Motor* motor, Potentiometer* potentiometer, bool changeMotDir, bool changePotiDir, float percSpeedForward, float percSpeedBackward)
	: servoMotor(motor), servoPotentiometer(potentiometer), changeMotorDirection(changeMotDir), changePotiDirection(changePotiDir), percentageSpeedForward(percSpeedForward), percentageSpeedBackward(percSpeedBackward)
{
}


/**
 * @brief Destructor
 * @brief The function destroys a Servo object.
 *
 * @return nothing 
 */
Servo::~Servo()
{
	delete servoMotor;
	delete servoPotentiometer;
}


/**
 * @brief The function gets the Servo's potentiometer.
 *
 * @return Potentiometer* - a pointer to the Servo's assigned Potentiometer class instance
 */
Potentiometer* Servo::getPotentiometer(){
	return servoPotentiometer;
}


/**
 * @brief The function gets the Servo's Motor.
 *
 * @return Motor* - a pointer to the Servo's assigned Motor class instance
 */
Motor* Servo::getMotor(){
	return servoMotor;
}


/**
 * @brief The function gets the Servo's Potentiometer voltage.
 *
 * @return uint16_t - the value represents the Potentiometer's current voltage level (0-4096)
 */
uint16_t Servo::getVoltage(){
	return servoPotentiometer->getVoltage();
}


/**
 * @brief The function gets if the Servo's Potentiometer direction is flipped.
 *
 * @return bool - the return value is true if the Potentiometer direction is flipped, false otherwise.
 */
bool Servo::getChangePotiDirection(){
	return changePotiDirection;
}


/**
 * @brief The function gets the Servo's assigned percentaged speed in forward direction.
 *
 * @return float - the return value is the Servo's assigned percentaged (0.0 - 100.0) forward speed.
 */
float Servo::getPercentageSpeedForward(){
	return percentageSpeedForward;
}


/**
 * @brief The function gets the Servo's assigned percentaged speed in backward direction.
 *
 * @return float - the return value is the Servo's assigned percentaged (0.0 - 100.0) backward speed.
 */
float Servo::getPercentageSpeedBackward(){
	return percentageSpeedBackward;
}


/**
 * @brief The function sets the Servo's Motor to move forward.
 *
 * @return nothing
 */
void Servo::moveForward(){
	if(changeMotorDirection){
		servoMotor->moveBackward();
	} else {
		servoMotor->moveForward();
	}
}


/**
 * @brief The function sets the Servo's Motor to move backward.
 *
 * @return nothing
 */
void Servo::moveBackward(){
	if(changeMotorDirection){
		servoMotor->moveForward();
	} else {
		servoMotor->moveBackward();
	}
}


/**
 * @brief The function sets the Servo's Motor to stop.
 *
 * @return 	nothing
 */
void Servo::stop(){
	servoMotor->stop();
}


/**
 * @brief The function get the current percentage position of the Servo.
 *
 * @return float - the return value represents the current percentage position of the Servo.
 * If the Servo's Potentiometer is initialized irregular the function returns -100.0.
 */
float Servo::getPercentagePosition(){
	float currentVoltage = getVoltage();
	float minVoltage = servoPotentiometer->getMinVoltageLevel();
	float maxVoltage = servoPotentiometer->getMaxVoltageLevel();
	float percentage;

	if (maxVoltage - minVoltage > 0){
		if(getChangePotiDirection()){
			percentage = 100 - (currentVoltage - minVoltage) / (maxVoltage - minVoltage) * 100;
		} else {
			percentage = (currentVoltage - minVoltage) / (maxVoltage - minVoltage) * 100;
		}
		if (percentage > 100.0) {
			percentage = 100.0;
		}
		if (percentage < 0.0) {
			percentage = 0.0;
		}
		return percentage;
	} else {
		return -100.0;
	}
}


/**
 * @brief The function moves the Servo to a given position target.
 *
 * @param target - float value that represents the percentage amount of voltage level at the Servo's Potentiometer that shall be reached
 * @return bool - the return value is true if the voltage target could be reached, false otherwise
 */
bool Servo::moveToTarget(float target, float startSpeed){
	unsigned long startTime = millis();
	float currentPosition = getPercentagePosition();
	
	Serial.printf("Servo's current position is %.1f%%\n", currentPosition);
	Serial.printf("Servo tries to reach target %.1f%%\n", target);

	// set speeder
	getMotor()->getSpeeder()->turnOn(startSpeed);

	// drive to target
	if (currentPosition > target){
		while (getPercentagePosition() > target && getPercentagePosition() > 0 && millis() < startTime + SERVO_TIMEOUT){
			currentPosition = getPercentagePosition();
			if(currentPosition > target){
				moveBackward();
			} else {
				stop();
			}
			if (abs(currentPosition - target) < 10.0){
				getMotor()->getSpeeder()->turnOn(startSpeed * 0.8);
			}
			if (abs(currentPosition - target) < 5.0){
				getMotor()->getSpeeder()->turnOn(startSpeed * 0.6);
			}
			if (abs(currentPosition - target) < 1.0){
				getMotor()->getSpeeder()->turnOn(startSpeed * 0.2);
			}
		}
		stop();
	} else {
		while (getPercentagePosition() < target && getPercentagePosition() < 100 && millis() < startTime + SERVO_TIMEOUT){
			currentPosition = getPercentagePosition();
			if(currentPosition < target){
				moveForward();
			} else {
				stop();
			}
			if (abs(currentPosition - target) < 10.0){
				getMotor()->getSpeeder()->turnOn(startSpeed * 0.8);
			}
			if (abs(currentPosition - target) < 5.0){
				getMotor()->getSpeeder()->turnOn(startSpeed * 0.6);
			}
			if (abs(currentPosition - target) < 1.0){
				getMotor()->getSpeeder()->turnOn(startSpeed * 0.2);
			}
		}
		stop();
	}
	stop();
	getMotor()->getSpeeder()->turnOff();

	// check result
	if (millis() < startTime + SERVO_TIMEOUT) {
		return true;
	} else {
		return false;
	}
}
