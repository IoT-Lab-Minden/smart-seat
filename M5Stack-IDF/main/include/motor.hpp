/**
 * @file motor.hpp
 * @brief Header for the Motor class
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
 * The Motor class represents a physically present dc-motor.
 */

#ifndef MOTOR_HPP
#define MOTOR_HPP
//#include <Arduino.h>
#include "../lib/M5Stack_new.h"
//#include <M5Stack.h>
#include "speeder.hpp"

class Motor
{

public:
    Motor(uint8_t gpio01, uint8_t gpio02, Speeder* speeder);
    ~Motor();

    uint8_t getPin01();
    uint8_t getPin02();
    Speeder* getSpeeder();
    void init();
    void moveForward();
    void moveBackward();
    void stop();

private:
    uint8_t pin01;
    uint8_t pin02;
    uint8_t pinState01;
    uint8_t pinState02;
    Speeder* motorSpeeder;
};

#endif 
