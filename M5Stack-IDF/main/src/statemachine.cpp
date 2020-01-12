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

bool serviceMode = false;

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
 * @brief The function writes new operation data to the PICC.
 *
 * @return nothing
 */
void StateMachine::writeDataToPICC(float currentPosition)
{
	std::vector<Target> piccTargets;
	Target newTarget;
	bool dataWritten;
	std::vector<byte> byteBlock;
	int sum;

	Serial.printf("StateMachine::writeDataToPICC\n");

	// write new settings if PICC is present
	dataWritten = false;
	if (rfidUnit->getCardPresence()){
		Serial.printf("Card is present.\n");
		if (rfidUnit->getCardState() == identified) {
			Serial.printf("Card is identified.\n");
			// search, if current operation has already a value
			piccTargets = rfidUnit->getTargets();
			for (int i = 0; i < piccTargets.size(); i++){
				if (piccTargets.at(i).operationIndex == currentState) {
					// write data on PICC
					dataWritten = rfidUnit->writeData(piccTargets.at(i).valueBlockOnPICC, String(currentPosition));
					if (dataWritten) {
						piccTargets.at(i).percentageGoal = currentPosition;
					}
				}
			}
			// if current operation has no value on the PICC, take next empty block
			if (!dataWritten) {
				for (int i = 0; i < (sizeof(RFID_NAME_BLOCKS) / sizeof(*RFID_NAME_BLOCKS)); i++){
					if (!dataWritten) {
						byteBlock = rfidUnit->readBlockArray(RFID_NAME_BLOCKS[i]);
						sum = 0;
						for(int j = 0 ; j < RFID_BYTES_PER_BLOCK; j++){
							sum += byteBlock.at(j);
						}
						if (sum == 0) {
							dataWritten = rfidUnit->writeData(RFID_NAME_BLOCKS[i], operations.at(currentState)->getOperationName());
							dataWritten = rfidUnit->writeData(RFID_NAME_BLOCKS[i] + 1, String(currentPosition));
							if (dataWritten) {
								newTarget.operationIndex = currentState;
								newTarget.percentageGoal = currentPosition;
								newTarget.valueBlockOnPICC = RFID_NAME_BLOCKS[i] + 1;
								rfidUnit->addTarget(newTarget);
							}
						}
					}
				}
			}			
		}
		if (rfidUnit->getCardState() == empty) {
			Serial.printf("Card is empty.\n");
			// write identy String on PICC
			rfidUnit->writeData(RFID_IDENTIFY_BLOCK, RFID_IDENTIFY_NAME);
			// write new data on PICC
			dataWritten = rfidUnit->writeData(RFID_NAME_BLOCKS[0], operations.at(currentState)->getOperationName());
			dataWritten = rfidUnit->writeData(RFID_NAME_BLOCKS[0] + 1, String(currentPosition));
			if (dataWritten) {
				newTarget.operationIndex = currentState;
				newTarget.percentageGoal = currentPosition;
				newTarget.valueBlockOnPICC = RFID_NAME_BLOCKS[0] + 1;
				rfidUnit->addTarget(newTarget);
			}
		}
	}
}


/**
 * @brief The function reacts on the event: 'Button A is pressed'.
 *
 * @return nothing
 */
void StateMachine::eventButtonA()
{
	float currentPosition;

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
		if (serviceMode) {lcdDisplay->displayVoltage(operations.at(currentState)->getOperationServo()->getVoltage());}
	
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
	float specificSpeed;
	float currentPosition;
	
	// serial info
	Serial.printf("Button B was pressed.\n");
	Serial.printf("currentState = %i\n", currentState);
	Serial.printf("operationName = '%s'\n", operations.at(currentState)->getOperationName().c_str());

	if (operations.at(currentState)->getOperationType() == "servo"){
		// turn on spezific speeder
		specificSpeed = operations.at(currentState)->getOperationServo()->getPercentageSpeedBackward();
		operations.at(currentState)->getOperationServo()->getMotor()->getSpeeder()->turnOn(specificSpeed);
		// move servo while button is pressed
		while(M5.BtnB.read()){
			if (serviceMode) {lcdDisplay->displayVoltage(operations.at(currentState)->getOperationServo()->getVoltage());}
			currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
			lcdDisplay->displayPercentage(currentPosition);
			if(currentPosition > 0){
				operations.at(currentState)->getOperationServo()->moveBackward();
			} else {
				operations.at(currentState)->getOperationServo()->stop();
			}			
		}
		// stop servo, update current position
		operations.at(currentState)->getOperationServo()->stop();
		if (serviceMode) {lcdDisplay->displayVoltage(operations.at(currentState)->getOperationServo()->getVoltage());}
		currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
		lcdDisplay->displayPercentage(currentPosition);
		writeDataToPICC(currentPosition);
	}
}




/**
 * @brief The function reacts on the event: 'Button C is pressed'.
 *
 * @return nothing
 */
void StateMachine::eventButtonC()
{
	float specificSpeed;
	float currentPosition;
	bool eraseResult;
	
	// serial info
	Serial.printf("Button C was pressed.\n");
	Serial.printf("currentState = %i\n", currentState);
	Serial.printf("operationName = '%s'\n", operations.at(currentState)->getOperationName().c_str());

	if (operations.at(currentState)->getOperationType() == "servo"){
		// turn on spezific speeder
		specificSpeed = operations.at(currentState)->getOperationServo()->getPercentageSpeedForward();
		operations.at(currentState)->getOperationServo()->getMotor()->getSpeeder()->turnOn(specificSpeed);
		// move servo while button is pressed
		while(M5.BtnC.read()){
			if (serviceMode) {lcdDisplay->displayVoltage(operations.at(currentState)->getOperationServo()->getVoltage());}
			currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
			lcdDisplay->displayPercentage(currentPosition);
			if(currentPosition < 100){
				operations.at(currentState)->getOperationServo()->moveForward();
			} else {
				operations.at(currentState)->getOperationServo()->stop();
			}			
		}
		// stop servo, update current position
		operations.at(currentState)->getOperationServo()->stop();
		if (serviceMode) {lcdDisplay->displayVoltage(operations.at(currentState)->getOperationServo()->getVoltage());}
		currentPosition = operations.at(currentState)->getOperationServo()->getPercentagePosition();
		lcdDisplay->displayPercentage(currentPosition);
		writeDataToPICC(currentPosition);
	}

	if (operations.at(currentState)->getOperationType() == "rfid"){
		eraseResult = rfidUnit->eraseData();
		// serial info
		if (eraseResult) {
			Serial.printf("PICC data erased.\n");
		} else {
			Serial.printf("PICC data erasing failed.\n");
		}
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
	Target operationTarget;
	std::vector<Target> piccTargets;
	float startSpeed;
	bool targetReached;
	
	if (rfidUnit->getCardPresence()){
		// erase the 'old' targets
		rfidUnit->clearTargets();
		if (rfidUnit->getCardState() == identified){
			// read operation data from PICC
			for (int i = 0; i < (sizeof(RFID_NAME_BLOCKS) / sizeof(*RFID_NAME_BLOCKS)); i++){
				byteBlock = rfidUnit->readBlockArray(RFID_NAME_BLOCKS[i]);
				readString = rfidUnit->byteVector2String(byteBlock);
				for (int j = 0; j < operations.size(); j++){
					if (readString.compareTo(operations.at(j)->getOperationName().substring(0, RFID_BYTES_PER_BLOCK)) == 0){
						// valid operation found, read next block for assigned value
						byteBlock = rfidUnit->readBlockArray(RFID_NAME_BLOCKS[i] + 1);
						readString = rfidUnit->byteVector2String(byteBlock);
						operationTarget.operationIndex = j;
						operationTarget.percentageGoal = readString.toFloat();
						operationTarget.valueBlockOnPICC = RFID_NAME_BLOCKS[i] + 1;
						if (operationTarget.percentageGoal >= 0 && operationTarget.percentageGoal <= 100){
							rfidUnit->addTarget(operationTarget);
							// serial info
							Serial.printf("Target added: %s to %.1f%%.\n", operations.at(operationTarget.operationIndex)->getOperationName().c_str(), operationTarget.percentageGoal);
						} 
					}
				}
			}
		}

		piccTargets = rfidUnit->getTargets();

		// serial info
		Serial.printf("Targets found on PICC: %i\n", piccTargets.size());

		if (operations.at(currentState)->getOperationName() == "rfid"){
			for (int i = 0; i < piccTargets.size(); i++){

				// serial info
				Serial.printf("Target %i: ", i);
				Serial.printf("Operation: %s to ", operations.at(piccTargets.at(i).operationIndex)->getOperationName().c_str());
				Serial.printf("%.1f%%\n", piccTargets.at(i).percentageGoal);

				// try to reach target position
				if (piccTargets.at(i).percentageGoal > operations.at(piccTargets.at(i).operationIndex)->getOperationServo()->getPercentagePosition()) {
					startSpeed = operations.at(piccTargets.at(i).operationIndex)->getOperationServo()->getPercentageSpeedForward();
				} else {
					startSpeed = operations.at(piccTargets.at(i).operationIndex)->getOperationServo()->getPercentageSpeedBackward();
				}
				targetReached = operations.at(piccTargets.at(i).operationIndex)->getOperationServo()->moveToTarget(piccTargets.at(i).percentageGoal, startSpeed);
				
				// serial info
				if (targetReached) {
					Serial.printf("Target was reached!\n");
				} else {
					Serial.printf("Target was not reached. Servo-Timeout!\n");
				}
			}	
			
			// turn off all speeders
			for(std::size_t i = 0; i < speeders.size(); i++) {
				speeders.at(i)->turnOff();
			} 
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

