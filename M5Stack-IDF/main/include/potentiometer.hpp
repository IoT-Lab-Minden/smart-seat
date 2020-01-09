/**
 * @file potentiometer.hpp
 * @brief Header for the Potentiometer class
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
 * The Potentiometer class represents a physically present potentiometer.
 */

#ifndef POTENTIOMETER_HPP
#define POTENTIOMETER_HPP
#include <Arduino.h>
#include <M5Stack.h>

class Potentiometer
{
private:
    uint8_t  potiPin;
    uint16_t voltage;
	uint16_t minVoltageLevel;
	uint16_t maxVoltageLevel;
    void 	 init();

public:
    Potentiometer(uint8_t pin, uint16_t minVolts, uint16_t maxVolts);
    ~Potentiometer();
    uint16_t getVoltage();
    uint16_t getMinVoltageLevel();
    uint16_t getMaxVoltageLevel();
};
#endif