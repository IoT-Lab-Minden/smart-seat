/**
 * @file light.cpp
 * @brief Source code for the Light class
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
 * The Light class represents a physically present light.
 */

#include "../include/light.hpp"



/**
 * @brief Constructor
 * @brief The function creates a Light object.
 *
 * @param  	gpio01 - the GPIO-Pin that drives the Light 
 * @param  	speeder - the Speeder class instance for the PWM control of the light 
 * @return 	nothing
 */
Light::Light(uint8_t gpio01, Speeder* speeder)
	: pin01(gpio01), lightSpeeder(speeder)
{
	init();
}


/**
 * @brief Destructor
 * @brief The function destroys a Light object.
 *
 * @return 	nothing 
 */
Light::~Light()
{
	delete lightSpeeder;
}


/**
 * @brief The function gets the number of the GPIO-pin that drives the Light.
 *
 * @return uint8_t - the Light's GPIO pin number
 */
uint8_t Light::getPin01(){
	return pin01;
}


/**
 * @brief The function gets the Speeder of the light.
 *
 * @return Speeder* - a pointer to the Light's assigned Speeder class instance
 */
Speeder* Light::getSpeeder(){
	return lightSpeeder;
}


/**
 * @brief The function inits the Light's GPIO-pin as output.
 *
 * @return nothing
 */
void Light::init(){
    pinMode(pin01, OUTPUT);
}


/**
 * @brief The function turns the Light's GPIO-pin to on.
 *
 * @return nothing
 */
void Light::turnOn(){
	digitalWrite(pin01, HIGH);
}


/**
 * @brief The function turns the Light's GPIO-pin to out.
 *
 * @return 	nothing
 */
void Light::turnOut(){
	digitalWrite(pin01, LOW);
}


