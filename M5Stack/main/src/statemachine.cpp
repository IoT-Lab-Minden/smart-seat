/**
 * @file statemachine.cpp
 * @brief Source code for the StateMachine class
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
 * The StateMachine class represents a state machine for the operations
 * of the SmartSeat.
 */

#include "../include/statemachine.hpp"
#include <M5Display.h>


/**
 * @brief Constructor
 * @brief The function creates a StateMachine object.
 *
 * @return nothing
 */
StateMachine::StateMachine()
{
}


/**
 * @brief Destructor
 * @brief The function destroys a StateMachine object.
 *
 * @return 	nothing 
 */
StateMachine::~StateMachine()
{
}


/**
 * @brief The function reacts on the event: 'Button A is pressed'.
 *
 * @return nothing
 */
void StateMachine::eventButtonA()
{
	float currentPosition;
	float currentVoltage = 0.0;

    currentState ++;
	if (currentState >= numberOfStates) {
		currentState = 0;
	} 

	// serial info
	Serial.printf("Operation Modus has changed: current state is %i\n", currentState);
	Serial.print("operationName: ");
	Serial.println(operations.at(currentState)->getOperationName());
	
	lcdDisplay->displayImageArray(operations.at(currentState)->getImageArray());

	if (operations.at(currentState)->getOperationType() == "servo"){
		currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
		lcdDisplay->displayPercentage(currentPosition);
		currentVoltage = operations.at(currentState)->getOperationServo()->getVoltage();
		lcdDisplay->displayVoltage(currentVoltage);
	
		// turn off all speeders
		for(std::size_t i = 0; i < speeders.size(); i++) {
			speeders.at(i)->turnOff();
		} 
	
	}
}


/**
 * @brief The function reacts on the event: 'Button B is pressed'.
 *
 * @return nothing
 */
void StateMachine::eventButtonB()
{
	float currentPosition;
	float currentVoltage;
	float specificSpeed;
	
	// serial info
	Serial.printf("Button B was pressed.\n");
	Serial.printf("currentState = %i\n", currentState);
	Serial.printf("operationName = '%s'\n", operations.at(currentState)->getOperationName().c_str());

	// turn on spezific speeder
	specificSpeed = operations.at(currentState)->getOperationServo()->getPercentageSpeedBackward();
	operations.at(currentState)->getOperationServo()->getMotor()->getSpeeder()->turnOn(specificSpeed);

	if (operations.at(currentState)->getOperationType() == "servo"){
		while(M5.BtnB.read()){
			currentVoltage = operations.at(currentState)->getOperationServo()->getVoltage();
			lcdDisplay->displayVoltage(currentVoltage);
			currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
			lcdDisplay->displayPercentage(currentPosition);
			if(currentPosition > 0){
				operations.at(currentState)->getOperationServo()->moveBackward();
			} else {
				operations.at(currentState)->getOperationServo()->stop();
			}			
		}
		currentVoltage = operations.at(currentState)->getOperationServo()->getVoltage();
		lcdDisplay->displayVoltage(currentVoltage);
		currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
		lcdDisplay->displayPercentage(currentPosition);
		operations.at(currentState)->getOperationServo()->stop();
	}
}




/**
 * @brief The function reacts on the event: 'Button C is pressed'.
 *
 * @return nothing
 */
void StateMachine::eventButtonC()
{
	float currentPosition;
	float currentVoltage;
	float specificSpeed;
	
	// turn on spezific speeder
	specificSpeed = operations.at(currentState)->getOperationServo()->getPercentageSpeedForward();
	operations.at(currentState)->getOperationServo()->getMotor()->getSpeeder()->turnOn(specificSpeed);

	if (operations.at(currentState)->getOperationType() == "servo"){
		while(M5.BtnC.read()){
			currentVoltage = operations.at(currentState)->getOperationServo()->getVoltage();
			lcdDisplay->displayVoltage(currentVoltage);
			currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
			lcdDisplay->displayPercentage(currentPosition);
			if(currentPosition < 100){
				operations.at(currentState)->getOperationServo()->moveForward();
			} else {
				operations.at(currentState)->getOperationServo()->stop();
			}			
		}
		currentVoltage = operations.at(currentState)->getOperationServo()->getVoltage();
		lcdDisplay->displayVoltage(currentVoltage);
		currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
		lcdDisplay->displayPercentage(currentPosition);
		operations.at(currentState)->getOperationServo()->stop();
	}
}


/**
 * @brief The function reacts on the events:
 *   - a new PICC has been recognized
 *   - a PICC is not been recognized any more
 *  
 * @return nothing
 */
void StateMachine::eventRfid()
{
	//byte block;
	String piccData;
	std::vector<byte> byteBlock;
	String readString;
	std::vector<Target> targets;
	Target operationTarget;
	float startSpeed;
	bool targetReached;
	
	if (rfidUnit->getCardPresence()){
		if (operations.at(currentState)->getOperationName() == "rfid"){
			if (rfidUnit->getCardState() == identified){
				// read operation data from PICC
				for (int i = 1; i < (sizeof(RFID_USED_BLOCKS) / sizeof(*RFID_USED_BLOCKS)); i++){
					byteBlock = rfidUnit->readBlockArray(RFID_USED_BLOCKS[i]);
					readString = rfidUnit->byteVector2String(byteBlock);
					for (int j = 0; j < operations.size(); j++){
						if (readString.compareTo(operations.at(j)->getOperationName().substring(0, RFID_BYTES_PER_BLOCK)) == 0){
							// valid operation found, read next block for assigned value
							byteBlock = rfidUnit->readBlockArray(RFID_USED_BLOCKS[i+1]);
							readString = rfidUnit->byteVector2String(byteBlock);
							operationTarget.operationIndex = j;
							operationTarget.percentageGoal = readString.toFloat();
							if (operationTarget.percentageGoal >= 0 && operationTarget.percentageGoal <= 100){
								targets.push_back(operationTarget);
							} 
						}
					}
				}
			}
			// serial info
			Serial.printf("Targets found on PICC: %i\n", targets.size());
			for (int i = 0; i < targets.size(); i++){

				// serial info
				Serial.printf("Target %i: ", i);
				Serial.printf("Operation: %s to ", operations.at(targets.at(i).operationIndex)->getOperationName().c_str());
				Serial.printf("%.1f%%\n", targets.at(i).percentageGoal);

				// try to reach target position
				if (targets.at(i).percentageGoal > operations.at(targets.at(i).operationIndex)->getOperationServo()->getPercentagePosition()) {
					startSpeed = operations.at(targets.at(i).operationIndex)->getOperationServo()->getPercentageSpeedForward();
				} else {
					startSpeed = operations.at(targets.at(i).operationIndex)->getOperationServo()->getPercentageSpeedBackward();
				}
				targetReached = operations.at(targets.at(i).operationIndex)->getOperationServo()->moveToTarget(targets.at(i).percentageGoal, startSpeed);
				
				// serial info
				if (targetReached) {
					Serial.printf("Target was reached!\n");
				} else {
					Serial.printf("Target was not reached. Servo-Timeout!\n");
				}
			}	
			
			// write block 4
			//block = 28;
			//piccData = "backrest_distance";
			//Serial.println(rfidUnit->writeData(block, piccData));
			//block = 29;
			//piccData = "40.0";
			//Serial.println(rfidUnit->writeData(block, piccData));
			
			
			
		}	
	}
}


/**
 * @brief The function initializes the StateMachine Object
 *
 * @param  	display - a pointer to the SmartSeat's LCD display 
 * @param  	rfid - a pointer to the SmartSeat's RFID device 
 * @param  	std::vector<Operation*> - a vector of pointers to the SmartSeat's executable Operation s 
 * @param  	std::vector<Speeder*> - a vector of pointers to the SmartSeat's Speeder instances 
 * @return 	nothing
 */void StateMachine::init(LCD* display, RFID* rfid, std::vector<Operation*> ops, std::vector<Speeder*> speeds)
{
	rfidUnit = rfid;
	lcdDisplay = display;
	operations = ops;
	speeders = speeds;
    numberOfStates = operations.size();
	currentState = -1;
	eventButtonA();
}

