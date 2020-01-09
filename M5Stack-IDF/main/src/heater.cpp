/**
 * @file heater.cpp
 * @brief Source code for the Heater class
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
 * The Heater class represents a physically present heating element.
 */

#include "../include/heater.hpp"



/**
 * @brief Constructor
 * @brief The function creates a Heater object.
 *
 * @param  gpio01 - a uint8_t value that represents the microcontroller's GPIO number that drives the Heater
 * @param  speeder - a pointer to the Heater's assigned Speeder class instance
 * @return no return
 */
Heater::Heater(uint8_t gpio01, Speeder* speeder)
	: pin01(gpio01), heaterSpeeder(speeder)
{
	init();
}


/**
 * @brief Destructor
 * @brief The function destroys a Heater object.
 *
 * @return no return 
 */
Heater::~Heater()
{
	delete heaterSpeeder;
}


/**
 * @brief The function gets the number of the GPIO pin that drives the Heater.
 *
 * @return 	uint8_t - the GPIO-number
 */
uint8_t Heater::getPin01(){
	return pin01;
}


/**
 * @brief The function gets the assigned Speeder of the Heater.
 *
 * @return Speeder* - a pointer to the Heater's assigned Speeder class instance
 */
Speeder* Heater::getSpeeder(){
	return heaterSpeeder;
}


/**
 * @brief The function inits the Heater's pin as output.
 *
 * @return 	no return
 */
void Heater::init(){
    pinMode(pin01, OUTPUT);
}


/**
 * @brief The function sets the Heater's pin to on.
 *
 * @return 	no return
 */
void Heater::turnOn(){
	digitalWrite(pin01, HIGH);
}


/**
 * @brief The function sets the Heater's pin to off.
 *
 * @return 	no return
 */
void Heater::turnOut(){
	digitalWrite(pin01, LOW);
}


