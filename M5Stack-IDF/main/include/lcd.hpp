/**
 * @file lcd.hpp
 * @brief Header for the LCD class
 *
 * @author	Torsten Niemeier
 * @author	E-mail: <tniemeier1@fh-bielefeld.de>
 * @version 1.0
 * @date 2020/01/05
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
 * The LCD class represents the LCD display of the SmartSeat.
 */

#ifndef LCD_HPP
#define LCD_HPP

#include "config.hpp"
//#include <M5Stack.h>
#include "../lib/M5Stack_new.h"
#include <M5Display.h>
#include <math.h>

class LCD {
	public:
		LCD();
		~LCD();

		void displayImageArray(const unsigned char imageArray[]);
		void displayCardPresence(bool isPresent, CardState state);
		void displayVoltage(uint16_t voltage);
		void displayPercentage(float percentage);
	private:
		int displayWidth;
		int displayHeight;
		int displayBrightness;
};

#endif

