/**
 * @file motor.cpp
 * @brief Source code for the Motor class
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
 * The Motor class represents a physically present dc-motor.
 */

#include "../include/motor.hpp"



/**
 * @brief Constructor
 * @brief The function creates a Motor object.
 *
 * @param gpio01 - a uint8_t value that represents the first microcontroller's GPIO number that drives the Motor
 * @param gpio02 - a uint8_t value that represents the second microcontroller's GPIO number that drives the Motor
 * @param speeder - a pointer to the Motor's assigned Speeder class instance
 * @return nothing
 */
Motor::Motor(uint8_t gpio01, uint8_t gpio02, Speeder* speeder)
	: pin01(gpio01), pin02(gpio02), motorSpeeder(speeder)
{
	init();
}


/**
 * @brief Destructor
 * @brief The function destroys a Motor object.
 *
 * @return nothing 
 */
Motor::~Motor()
{
	delete motorSpeeder;
}



/**
 * @brief The function gets the number of the first GPIO pin that drives the Motor.
 *
 * @return 	uint8_t - the GPIO-number
 */
uint8_t Motor::getPin01(){
	return pin01;
}


/**
 * @brief The function gets the number of the second GPIO pin that drives the Motor.
 *
 * @return 	uint8_t - the GPIO-number
 */
uint8_t Motor::getPin02(){
	return pin02;
}


/**
 * @brief The function gets the assigned Speeder of the Motor.
 *
 * @return Speeder* - a pointer to the Motor's assigned Speeder class instance
 */
Speeder* Motor::getSpeeder(){
	return motorSpeeder;
}


/**
 * @brief The function inits the Motor's pins as output and sets the defaults.
 *
 * @return 	nothing
 */
void Motor::init(){
    pinMode(pin01, OUTPUT);
    pinMode(pin02, OUTPUT);
	digitalWrite(pin01, LOW);
	digitalWrite(pin02, LOW);
	pinState01 = 0;
	pinState02 = 0;
}


/**
 * @brief The function sets the Motor's pins to let move the Motor forward.
 *
 * @return 	nothing
 */
void Motor::moveForward(){
	digitalWrite(pin01, HIGH);
	digitalWrite(pin02, LOW);

	// serial info
	if (pinState01 == 0 || pinState02 == 1) {
		Serial.printf("Motor at Pin %i/%i turned to move forward.\n", pin01, pin02);
	}

	pinState01 = 1;
	pinState02 = 0;
}


/**
 * @brief The function sets the Motor's pins to let move the Motor backward.
 *
 * @return 	nothing
 */
void Motor::moveBackward(){
	digitalWrite(pin01, LOW);
	digitalWrite(pin02, HIGH);

	// serial info
	if (pinState01 == 1 || pinState02 == 0) {
		Serial.printf("Motor at Pin %i/%i turned to move backward.\n", pin01, pin02);
	}

	pinState01 = 0;
	pinState02 = 1;
}


/**
 * @brief The function sets the Motor's pins to stop the Motor.
 *
 * @return 	nothing
 */
void Motor::stop(){
	digitalWrite(pin01, LOW);
	digitalWrite(pin02, LOW);

	// serial info
	if (pinState01 == 1 || pinState02 == 1) {
		Serial.printf("Motor at Pin %i/%i stopped.\n", pin01, pin02);
	}

	pinState01 = 0;
	pinState02 = 0;
}
