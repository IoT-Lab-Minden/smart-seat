/**
 * @file smartseat.hpp
 * @brief Header for the SmartSeat class
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
 * The SmarSeat class represents a physically present SmartSeat.
 */

#ifndef SMARTSEAT_HPP
#define SMARTSEAT_HPP
#include <vector>
#include "config.hpp"
#include "operation.hpp"
#include "rfid.hpp"
#include "lcd.hpp"
#include "statemachine.hpp"

class SmartSeat {
public:
    SmartSeat(String name);
    ~SmartSeat();

    void init();
	StateMachine* getStateMachine();
	RFID* getRfidUnit();

private:
	LCD* lcdDisplay;
	RFID* rfidUnit;
	StateMachine* stateMachine;
    std::vector<Speeder*> speeders;
    std::vector<Operation*> operations;
    std::vector<Motor*> motors;
    std::vector<Potentiometer*> potentiometers;
    std::vector<Light*> lights;
    std::vector<Heater*> heaters;
    std::vector<Servo*> servos;
};
#endif