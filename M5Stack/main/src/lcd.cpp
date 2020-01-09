/**
 * @file lcd.cpp
 * @brief Source code for the LCD class
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

#include "../include/lcd.hpp"


/**
 * @brief Constructor
 * @brief The function creates a LCD object.
 *
 * @return nothing 
 */
LCD::LCD()
{
	displayWidth = LCD_DISPLAY_WIDTH;
	displayHeight = LCD_DISPLAY_HEIGHT;
	displayBrightness = LCD_DISPLAY_BRIGHTNESS;
}


/**
 * @brief Destructor
 * @brief The function destroys a LCD object.
 *
 * @return nothing 
 */
LCD::~LCD()
{
}


/**
 * @brief The function displays an image array on the LCD display.
 *
 * @param imageArray[] - a c-array of const unsigned char that contains the image
 * @return nothing
 */
void LCD::displayImageArray(const unsigned char imageArray[])
{
	M5.Lcd.setBrightness(displayBrightness);
	M5.Lcd.invertDisplay(true);
	M5.Lcd.pushImage(0, 0, displayWidth, displayHeight, (uint16_t *)imageArray);
}


/**
 * @brief The function displays a voltage value [0-4095] on the display.
 * @brief This function is mainly intended for service and setup purposes.
 *
 * @param voltage - a uint16_t value representing a voltage value (0-4095)
 * @return nothing
 */
void LCD::displayVoltage(uint16_t voltage)
{
	M5.Lcd.setTextSize(2);
	M5.Lcd.setTextColor(BLACK, WHITE);
	M5.Lcd.setCursor(180, 20);
	M5.Lcd.print("         ");
	M5.Lcd.setCursor(180, 20);
	M5.Lcd.printf("U = %i\n", voltage);
}


/**
 * @brief The function displays a percentage value on the display.
 * @brief The value will be displayed with 4 relevant digits.
 *
 * @param pecentage - a float value representing a percentage value
 * @return nothing
 */
void LCD::displayPercentage(float percentage)
{
	M5.Lcd.setTextSize(2);
	M5.Lcd.setTextColor(BLACK, WHITE);
	if (percentage < 10){
		M5.Lcd.setTextColor(RED, WHITE);
	}
	if (percentage > 90){
		M5.Lcd.setTextColor(RED, WHITE);
	}
	M5.Lcd.setCursor(220, 40);
	M5.Lcd.print("         ");
	M5.Lcd.setCursor(220, 40);
	M5.Lcd.printf("%.1f%%", percentage);
}


/**
 * @brief The function displays the 'RFID card is present'-sign on the LCD-screen.
 * @brief The color of the sign represents the current card state.
 * @brief green - identified
 * @brief red   - unknown
 * @brief blue  - empty
 * @brief The sign is deleted (overpainted) if no card is present.
 *
 * @param isPresent - a bool value, true if RFID card shall be displayed as present
 * @param state - the CardState (empty, identified, unknown) 
 * @return nothing
 */
void LCD::displayCardPresence(bool isPresent, CardState state)
{
	int16_t centerX = 30;
	int16_t centerY = 30;
	uint16_t color;
	if (isPresent){
		if (state == identified){
			color = GREEN;
		}
		if (state == unknown){
			color = RED;
		}
		if (state == empty){
			color = BLUE;
		}
	} else {
		color = WHITE;
	}
	M5.Lcd.invertDisplay(true);
	M5.Lcd.fillCircle(centerX, centerY, 8, color); 
	M5.Lcd.drawCircle(centerX, centerY, 13, color); 
	M5.Lcd.drawCircle(centerX, centerY, 14, color); 
	M5.Lcd.drawCircle(centerX, centerY, 15, color); 
	M5.Lcd.drawCircle(centerX, centerY, 19, color); 
	M5.Lcd.drawCircle(centerX, centerY, 20, color); 
	M5.Lcd.drawCircle(centerX, centerY, 21, color); 
}
