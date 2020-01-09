/**
 * @file potentiometer.cpp
 * @brief Source code for the Potentiometer class
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
 * The Potentiometer class represents a physically present voltage divider.
 */

#include "../include/potentiometer.hpp"



/**
 * @brief Constructor
 * @brief The function creates a Potentiometer object.
 *
 * @param pin - a uint8_t value that represents the microcontroller's GPIO number to that the Potentiometer is attached
 * @param minVolts - a uint16_t value that desribes the minimal safe voltage [0 - 4095] the Potentiometer should deliver during operation
 * @param maxVolts - a uint16_t value that desribes the maximal safe voltage [0 - 4095] the Potentiometer should deliver during operation
 * @return nothing
 */
 Potentiometer::Potentiometer(uint8_t pin, uint16_t minVolts, uint16_t maxVolts) {
	potiPin = pin;
	minVoltageLevel = minVolts;
	maxVoltageLevel = maxVolts;
	init();
	getVoltage();
}


/**
 * @brief Destructor
 * @brief The function destroys a Potentiometer object.
 *
 * @return nothing 
 */
Potentiometer::~Potentiometer()
{
}


/**
 * @brief The function gets the minimal safe voltage of the Potentiometer.
 *
 * @return float - the minimal voltage in millivolt
 */
uint16_t Potentiometer::getMinVoltageLevel(){
	return minVoltageLevel;;
}


/**
 * @brief The function gets the maximal safe voltage of the Potentiometer.
 *
 * @return float - the maximal voltage in millivolt
 */
uint16_t Potentiometer::getMaxVoltageLevel(){
	return maxVoltageLevel;;
}


/**
 * @brief The function gets the voltage at the Potentiometer.
 *
 * @return uint16_t - the current voltage as uint16_t. Range 0 - 4095
 */
uint16_t Potentiometer::getVoltage(){
	voltage = analogRead(potiPin);
	return voltage;
}


/**
 * @brief The function inits the Potentiometer pins as input.
 *
 * @return 	nothing
 */
void Potentiometer::init(){

	analogSetSamples(1);
    pinMode (potiPin, INPUT);

}






