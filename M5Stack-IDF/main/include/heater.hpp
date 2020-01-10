/**
 * @file heater.hpp
 * @brief Header for the Heater class
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
 * The Heater class represents a physically present heating element.
 */

#ifndef HEATER_HPP
#define HEATER_HPP
#include <Arduino.h>
#include "../lib/M5Stack_new.h"
//#include <M5Stack.h>
#include "speeder.hpp"

class Heater
{
public:
    Heater(uint8_t gpio01, Speeder* speeder);
    ~Heater();
    uint8_t getPin01();
    Speeder* getSpeeder();
    void 	init();
    void 	turnOn();
    void 	turnOut();

private:
    uint8_t pin01;
    Speeder* heaterSpeeder;
};

#endif 
