/**
 * @file smartseat.cpp
 * @brief Source code for the SmartSeat class
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
 * The SmartSeat class represents a physically present SmartSeat.
 */

#include "../include/smartseat.hpp"


// operation images
// The operation images are shown on the M5Stack's LCD during the operation.
// The *.c files contain the image as an array of const unsigned char.
// By using this way, it is possible not to use the sd-card-slot as device. 
// The way to create the files is described in "/images/read.me".
#include "../images/image_rfident.c"
#include "../images/image_seat_height.c"
#include "../images/image_seat_angle.c"
#include "../images/image_backrest_distance.c"
#include "../images/image_backrest_angle.c"
#include "../images/image_backrest_height.c"
#include "../images/image_armrest_height.c"


/**
 * @brief Constructor
 * @brief The function creates the SmartSeat object.
 *
 * @param name - a String that defines the SmartSeat's name
 * @return nothing
 */
SmartSeat::SmartSeat(String name) 
{
	seatName = name;
	lcdDisplay = new LCD();
	rfidUnit = new RFID();
	stateMachine = new StateMachine();
}


/**
 * @brief Destructor
 * @brief The function destroys the SmartSeat object.
 *
 * @return 	nothing 
 */
SmartSeat::~SmartSeat()
{
	delete lcdDisplay;
	delete rfidUnit;
	delete stateMachine;
	for (int i = 0; i < speeders.size(); i++){
		delete speeders.at(i);
	}
	for (int i = 0; i < potentiometers.size(); i++){
		delete potentiometers.at(i);
	}
	for (int i = 0; i < motors.size(); i++){
		delete motors.at(i);
	}
	for (int i = 0; i < lights.size(); i++){
		delete lights.at(i);
	}
	for (int i = 0; i < heaters.size(); i++){
		delete heaters.at(i);
	}
	for (int i = 0; i < servos.size(); i++){
		delete servos.at(i);
	}
	for (int i = 0; i < operations.size(); i++){
		delete operations.at(i);
	}
}


/**
 * @brief The function gets a pointer to the SmartSeat's StateMachine.
 *
 * @return StateMachine* - the pointer to the SmartSeat's assigned StateMachine object
 */
StateMachine* SmartSeat::getStateMachine() {
	return stateMachine;
}	


/**
 * @brief The function gets a pointer to the SmartSeat's RFID unit.
 *
 * @return RFID* - the pointer to the SmartSeat's assigned RFID unit
 */
RFID* SmartSeat::getRfidUnit() {
	return rfidUnit;
}	


/**
 * @brief The method initializes the SmartSeat's hardware and operations.
 *
 * @return nothing
 */
void SmartSeat::init() {

	//serial info
	Serial.println("SmartSeat initialization...");

	// speeders (PWM channels)
	// see config.hpp for the settings
	Speeder* speeder01 = new Speeder(PWM_01_PIN, PWM_01_FREQUENCY, PWM_01_CHANNEL, PWM_01_RESOLUTION, PWM_01_MIN_CYCLE, PWM_01_MAX_CYCLE);	
	Speeder* speeder02 = new Speeder(PWM_02_PIN, PWM_02_FREQUENCY, PWM_02_CHANNEL, PWM_02_RESOLUTION, PWM_02_MIN_CYCLE, PWM_02_MAX_CYCLE);	
	speeders.push_back(speeder01); 
	speeders.push_back(speeder02); 
	speeder01->init();
	speeder02->init();

	// motors
	// see config.hpp for the settings
	Motor* motor01 = new Motor(MOTOR_01_PIN_01, MOTOR_01_PIN_02, speeder01);
	Motor* motor02 = new Motor(MOTOR_02_PIN_01, MOTOR_02_PIN_02, speeder01);
	Motor* motor03 = new Motor(MOTOR_03_PIN_01, MOTOR_03_PIN_02, speeder01);
	Motor* motor04 = new Motor(MOTOR_04_PIN_01, MOTOR_04_PIN_02, speeder02);
	Motor* motor05 = new Motor(MOTOR_05_PIN_01, MOTOR_05_PIN_02, speeder02);
	Motor* motor06 = new Motor(MOTOR_06_PIN_01, MOTOR_06_PIN_02, speeder02);
	motors.push_back(motor01); 
	motors.push_back(motor02); 
	motors.push_back(motor03); 
	motors.push_back(motor04); 
	motors.push_back(motor05); 
	motors.push_back(motor06); 

	// potentiometers
	// see config.hpp for the settings
	Potentiometer* potentiometer01 = new Potentiometer(POTI_01_PIN, POTI_01_MIN_VOLTAGE, POTI_01_MAX_VOLTAGE);
	Potentiometer* potentiometer02 = new Potentiometer(POTI_02_PIN, POTI_02_MIN_VOLTAGE, POTI_02_MAX_VOLTAGE);
	Potentiometer* potentiometer03 = new Potentiometer(POTI_03_PIN, POTI_03_MIN_VOLTAGE, POTI_03_MAX_VOLTAGE);
	Potentiometer* potentiometer04 = new Potentiometer(POTI_04_PIN, POTI_04_MIN_VOLTAGE, POTI_04_MAX_VOLTAGE);
	Potentiometer* potentiometer05 = new Potentiometer(POTI_05_PIN, POTI_05_MIN_VOLTAGE, POTI_05_MAX_VOLTAGE);
	Potentiometer* potentiometer06 = new Potentiometer(POTI_06_PIN, POTI_06_MIN_VOLTAGE, POTI_06_MAX_VOLTAGE);
	potentiometers.push_back(potentiometer01); 
	potentiometers.push_back(potentiometer02); 
	potentiometers.push_back(potentiometer03); 
	potentiometers.push_back(potentiometer04); 
	potentiometers.push_back(potentiometer05); 
	potentiometers.push_back(potentiometer06); 

	// servos
	// see config.hpp for the settings
	Servo* servo01 = new Servo(motor01, potentiometer01, false, true, 100.0, 100.0);
	Servo* servo02 = new Servo(motor02, potentiometer02, false, true, 100.0, 100.0);
	Servo* servo03 = new Servo(motor03, potentiometer03, false, true, 100.0, 100.0);
	Servo* servo04 = new Servo(motor04, potentiometer04, true, false, 100.0, 100.0);
	Servo* servo05 = new Servo(motor05, potentiometer05, false, false, 65, 5.0);
	Servo* servo06 = new Servo(motor06, potentiometer06, true, false, 80.0, 10.0);
	servos.push_back(servo01); 
	servos.push_back(servo02); 
	servos.push_back(servo03); 
	servos.push_back(servo04); 
	servos.push_back(servo05); 
	servos.push_back(servo06); 

	// operations
	// There are multiple ways to create operations.
	// For further information check the documentation or 'operation.cpp'/'operation.hpp'.
	Operation* rfid = new Operation("rfid", image_rfident);
	Operation* seat_height = new Operation("seat_height", image_seat_height, servo06);
	Operation* seat_angle = new Operation("seat_angle", image_seat_angle, servo03);
	Operation* backrest_distance = new Operation("backrest_distance", image_backrest_distance, servo02);
	Operation* backrest_angle = new Operation("backrest_angle", image_backrest_angle, servo04);
	Operation* backrest_height = new Operation("backrest_height", image_backrest_height, servo05);
	Operation* armrest_height = new Operation("armrest_height", image_armrest_height, servo01);

	// line up the operations into the vector, rfid must be first
	operations.push_back(rfid); 
	operations.push_back(seat_height); 
	operations.push_back(seat_angle); 
	operations.push_back(backrest_distance); 
	operations.push_back(backrest_angle); 
	operations.push_back(backrest_height); 
	operations.push_back(armrest_height); 

	// initialize the RFID-Unit
	rfidUnit->init(lcdDisplay);

	// initialize the StateMachine
	stateMachine->init(lcdDisplay, rfidUnit, operations, speeders);

	//serial info
	Serial.println("SmartSeat initialized.");
}


