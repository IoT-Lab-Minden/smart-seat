/**
 * @file light.hpp
 * @brief Header for the Light class
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
 * The Light class represents a physically present light.
 */

#ifndef LIGHT_HPP
#define LIGHT_HPP
#include <Arduino.h>
#include <M5Stack.h>
#include "speeder.hpp"

class Light
{
public:
    Light(uint8_t gpio01, Speeder* speeder);
    ~Light();
    uint8_t  getPin01();
    Speeder* getSpeeder();
    void 	 init();
    void 	 turnOn();
    void 	 turnOut();

private:
    uint8_t pin01;
    Speeder* lightSpeeder;
};

#endif 
