/**
 * @file rfid.hpp
 * @brief Header for the RFID class
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
 * The RFID class represents the RFID operator of the SmartSeat.
 */

#ifndef RFID_HPP
#define RFID_HPP
#include <vector>
#include "../lib/MFRC522_I2C.h"
#include "config.hpp"
#include "lcd.hpp"

class RFID {
	public:
		RFID();
		~RFID();

		void init(LCD* display);
		bool detectCard();
		bool writeData(byte blockAddress, String stringData);
		std::vector<byte> readBlockArray(byte blockAddress);
		bool getCardPresence();
		CardState getCardState();
		String byteVector2String(std::vector<byte> bytes);
	private:
		MFRC522* mfrc522;
		LCD* lcdDisplay;
		MFRC522::StatusCode status;
		bool cardIsPresent;
		CardState cardState;
		unsigned long lastCheckTime;
		unsigned long checkInterval;
};

#endif

