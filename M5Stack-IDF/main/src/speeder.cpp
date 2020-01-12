/**
 * @file speeder.cpp
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
 * The speeder class represents a microcontroller's pwm output to drive a dc-motor.
 */

#include "../include/speeder.hpp"



/**
 * @brief Constructor
 * @brief The function creates a Speeder object.
 *
 * @param gpio - a uint8_t value that represents the microcontroller's GPIO number for the PWM output
 * @param freq - a uint16_t value that defines the operating frequency of the PWM channel
 * @param channel - a uint8_t value that defines the channel number for the PWM output
 * @param res - a uint8_t value that defines the sample resolution for the PWM output
 * @param minCycle - a uint8_t value that defines the minimal numbers of duty cycles [0-100] for this Speeder
 * @param maxCycle - a uint8_t value that defines the maximal numbers of duty cycles [0-100] for this Speeder
 * @return nothing
 */
Speeder::Speeder(uint8_t gpio, uint16_t freq, uint8_t channel, uint8_t res, uint8_t minCycle, uint8_t maxCycle)
	: enablePin(gpio), frequency(freq), pwmChannel(channel), resolution(res), minDutyCycle(minCycle), maxDutyCycle(maxCycle)
{
	// set to minimal duty cycle
    dutyCycle = minDutyCycle;
}


/**
 * @brief Destructor
 * @brief The function destroys a Speeder object.
 *
 * @return nothing 
 */
Speeder::~Speeder()
{
}


/**
 * @brief The function gets the GPIO pin of the Speeder.
 *
 * @return 	uint8_t - the Speeder's assigned GPIO number
 */
uint8_t Speeder::getPin(){
    return enablePin;
}


/**
 * @brief The function initializes the Speeder object.
 *
 * @return nothing
 */
void Speeder::init(){
	pinMode(enablePin, OUTPUT);
	ledcSetup(pwmChannel, frequency, resolution);
	ledcAttachPin(enablePin, pwmChannel);
    ledcWrite(pwmChannel, dutyCycle);   
}


/**
 * @brief The function turns off the Speeder.
 *
 * @return nothing
 */
void Speeder::turnOff(){
    dutyCycle = 0;
    ledcWrite(pwmChannel, dutyCycle);   
	// serial info
	Serial.printf("Speeder at Pin %i turned off to dutyCycle %i.\n", enablePin, dutyCycle);
}


/**
 * @brief The function turns on the Speeder at given percentage speed.
 *
 * @param float - percentage speed (0.0-100.0)
 * @return 	nothing
 */
void Speeder::turnOn(float speed){
	uint8_t oldDutyCycle = dutyCycle;
    if (speed >= 0 && speed <= 100) {
		dutyCycle = (uint8_t)(minDutyCycle + speed/100.0 * (maxDutyCycle - minDutyCycle));
	} 
	ledcWrite(pwmChannel, dutyCycle);   

	// serial info
	if (dutyCycle != oldDutyCycle) {
		Serial.printf("Speeder at Pin %i turned on to dutyCycle %i.\n", enablePin, dutyCycle);
	}
}

