/**
 * @file speeder.hpp
 * @brief Header for the Speeder class
 *
 * @author	Torsten Niemeier
 * @author	E-mail: <tniemeier1@fh-bielefeld.de>
 * @version 1.0
 * @date 2019/12/30
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
 * The Speeder class represents a Microcontroller's PWM-driven GPIO.
 * It is used to control the speed or the power of the assigned device.
 */

#ifndef SPEEDER_HPP
#define SPEEDER_HPP
#include <Arduino.h>
#include "../lib/M5Stack_new.h"
//#include <M5Stack_new.h>

class Speeder
{
public:
    Speeder(uint8_t gpio, uint16_t freq, uint8_t channel, uint8_t res, uint8_t minCycle, uint8_t maxCycle);
    ~Speeder();

    uint8_t getPin();
    void turnOff();
    void turnOn(float speed);
	void init();

private:
    uint8_t enablePin;
    uint16_t frequency;
    uint8_t pwmChannel;
    uint8_t resolution;
    uint8_t dutyCycle;
    uint8_t minDutyCycle;
    uint8_t maxDutyCycle;
};
#endif