/**
 * @file main.cpp
 * @brief The main function of the SmartSeat 
 *
 * @author	Torsten Niemeier
 * @author	E-mail: <tniemeier1@fh-bielefeld.de>
 * @version 1.0
 * @date 2019/12/21
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
 * The main function ist the global function for the SmartSeat,
 * which is the designated start of the program.
 */

#include "../lib/M5Stack_new.h"
#include "../include/smartseat.hpp"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif


SmartSeat smartseat("The incredible Seat");


// The setup routine runs once when M5Stack starts up
void setup(){

    // M5.begin with LCDEnable, SDEnable, SerialEnable, I2CEnable
	// The begin function is located in the library "M5Stack_new".
	// It is necessary, to start the M5 without enabling Serial, which the 'old' component "M5Stack" doesn't allow.  	
	M5.begin(true, false, false, true); 

	// Serial may not be started if the SmartSeat shall operate normally.
	// Erase the outcomment of the line for debug purposes, but don't give electrical power to the motor drivers then.
	// Serial.begin(115200);

	// begin Wire library for I2C-RFID-functionality
	Wire.begin();
	
	// initialize the SmartSeat
	smartseat.init();
}


void loop() {
	M5.update();
	if(M5.BtnA.wasPressed()){
        smartseat.getStateMachine()->eventButtonA();
    } 
    if(M5.BtnB.read()){
        smartseat.getStateMachine()->eventButtonB();
    }
    if(M5.BtnC.read()){
        smartseat.getStateMachine()->eventButtonC();
    }
    if(smartseat.getRfidUnit()->detectCard()){
        smartseat.getStateMachine()->eventRfid();
    }
}


// The arduino task
void loopTask(void *pvParameters)
{
    setup();
    for(;;) {
        micros(); //update overflow
        loop();
    }
}

extern "C" void app_main()
{
    initArduino();
    xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}


