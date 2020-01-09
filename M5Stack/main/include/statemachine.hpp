/**
 * @file statemachine.hpp
 * @brief Header for the StateMachine class
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
 * The StateMachine class represents a state machine for the operations
 * of the SmartSeat.
 */

#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <M5Stack.h>
#include "config.hpp"
#include "operation.hpp"
#include "speeder.hpp"
#include "rfid.hpp"



class StateMachine{
    public: 
		StateMachine();
		~StateMachine();
		void init(LCD* display, RFID* rfid, std::vector<Operation*> ops, std::vector<Speeder*> speeds);
        void eventButtonA();
        void eventButtonB();
        void eventButtonC();
        void eventRfid();
    private:
		RFID* rfidUnit;
		LCD* lcdDisplay;
		std::vector<Operation*> operations;
		std::vector<Speeder*> speeders;
        uint16_t numberOfStates;
		int currentState = 0;
};
#endif