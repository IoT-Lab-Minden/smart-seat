/**
 * @file rfid.cpp
 * @brief Source code for the RFID class
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

#include <M5Stack.h>
#include <Wire.h>
#include <vector>
#include "../include/rfid.hpp"

/**
 * @brief Constructor
 * @brief The function creates a RFID object.
 *
 * @return nothing 
 */
RFID::RFID()
{
	mfrc522 = new MFRC522(RFID_I2C_ADDRESS);
}


/**
 * @brief Destructor
 * @brief The function destroys a RFID object.
 *
 * @return nothing 
 */
RFID::~RFID()
{
	delete mfrc522;
	delete lcdDisplay;
}


/**
 * @brief The function initializes the RFID object.
 *
 * @param  	display - a pointer to the SmartSeat's LCD display 
 * @return 	nothing
 */
void RFID::init(LCD* display) {
    Wire.begin();
    mfrc522->PCD_Init();
	lcdDisplay = display;
	lastCheckTime = 0;
	checkInterval = RFID_CHECK_INTERVAL;
	cardIsPresent = false;
	cardState = unknown;
	targets.clear();
}


/**
 * @brief The function gets the presence status of a PICC.
 *
 * @return bool - the return value is true if the presence state is true, false otherwise.
 */
bool RFID::getCardPresence(){
	return cardIsPresent;
}


/**
 * @brief The function gets the current cardState of a nearby PICC.
 *
 * @return CardState - the return value is an int number representing one state of enum CardState:
 *   0 - empty
 *   1 - identified
 *   2 - unknown
 */
CardState RFID::getCardState(){
	return cardState;
}


/**
 * @brief The function clears the vector of Target s for the current near PICC.
 *
 * @return nothing
 */
void RFID::clearTargets(){
	targets.clear();
}


/**
 * @brief The function adds a Target to the PICC's Target vector.
 *
 * @param Target - the Target to be added 
 * @return nothing
 */
void RFID::addTarget(Target theTarget){
	targets.push_back(theTarget);
}


/**
 * @brief The function gets the Target s that are defined on the current near PICC.
 *
 * @return std::vector<Target> - the vector containing the Target s
 */
std::vector<Target> RFID::getTargets(){
	return targets;
}


/**
 * @brief The function detects if a PICC is near and readable.
 * @brief The function also determines if the PICC is identified, unknown or empty.
 *
 * @return bool - the return value is true if the presence state has changed, false otherwise.
 */
bool RFID::detectCard() {
	unsigned long currentTime = millis();
	bool oldPresenceState = cardIsPresent;	
	byte piccType;
	std::vector<byte> byteBlock(RFID_BYTES_PER_BLOCK);
	String ident;
	int i;
	int sum;

	// check for near PICC if interval time has passed
	if (currentTime - lastCheckTime > checkInterval) {
		mfrc522->PCD_StopCrypto1();
		if (!mfrc522->PICC_IsNewCardPresent() || !mfrc522->PICC_ReadCardSerial()) {
			cardIsPresent = false;
			// try to wake up again
			if(mfrc522->PICC_IsNewCardPresent()){
				if(mfrc522->PICC_ReadCardSerial()){
					cardIsPresent = true;
				}
			}
		} else {
			cardIsPresent = true;
		}

		// check if card is already known
		if (cardIsPresent){
			cardState = unknown;
			// check if card is known
			ident = byteVector2String(readBlockArray(RFID_IDENTIFY_BLOCK));
			if (ident.compareTo(RFID_IDENTIFY_NAME) == 0){
				cardState = identified;
			}
			// check card-state (unknown, identified, empty)
			if (cardState == unknown){
				sum = 0;
				i = 0;
				while (sum == 0 && i < (sizeof(RFID_USED_BLOCKS)/sizeof(*RFID_USED_BLOCKS))) {
					byteBlock = readBlockArray(RFID_USED_BLOCKS[i]);
					for(int j = 0 ; j < RFID_BYTES_PER_BLOCK; j++){
						sum += byteBlock.at(j);
					}
					i++;
				}
				if (sum == 0){
					cardState = empty;
				}
			}
		}
				
		// update LCD-display
		lcdDisplay->displayCardPresence(cardIsPresent, cardState);
		// update check time
		lastCheckTime = millis();

		// serial info if card presence has changed
		if (cardIsPresent != oldPresenceState){
			if (cardIsPresent){
				Serial.print("PICC recognized: ");
				Serial.print("PICC-UID: '");
				for (byte i = 0; i < mfrc522->uid.size; i++) {
					Serial.print(mfrc522->uid.uidByte[i] < 0x10 ? " 0" : " ");
					Serial.print(mfrc522->uid.uidByte[i], HEX);
				} 
				Serial.print("' ,type: ");
				piccType = mfrc522->PICC_GetType(mfrc522->uid.sak); 
				Serial.print(mfrc522->PICC_GetTypeName(piccType)); 
				Serial.print(" ,ident: '");
				Serial.print(ident);
				Serial.print("' ,cardState: ");
				Serial.println(cardState);
			} else {
				Serial.println("No PICC near.");
			}
			return true;
		}
	}
	return false;
}


/**
 * @brief The function writes a given String into a given block address of a PICC.
 *
 * @param blockAddress - a byte number of the PICC-block where the data shall be written 
 * @param stringData - a String that contains the data to be written 
 * @return bool - the return value is true if the operation succeeded, false otherwise.
 */
bool RFID::writeData(byte blockAddress, String stringData) {
    MFRC522::StatusCode status;
	MFRC522::MIFARE_Key key;	
	byte trailerBlock;
	byte dataBlock[RFID_BYTES_PER_BLOCK] = {0}; 
	String readString;
	
	if (!cardIsPresent){
        return false;
    }
	// build the key
	for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) { 
		key.keyByte[i] = 0xFF; 
	} 
	trailerBlock = (blockAddress / 4 + 1) * 4 - 1;
	// authenticate
	status = (MFRC522::StatusCode) mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522->uid)); 
	// write
	Serial.printf("Write Data: Block: %i, value %s\n", blockAddress, stringData.c_str());
	if (status != MFRC522::STATUS_OK) {
		Serial.print("Authentification failed: ");
		Serial.println(mfrc522->GetStatusCodeName(status));
        return false;
	} else {
		stringData.getBytes(dataBlock, RFID_BYTES_PER_BLOCK + 1);
		status = (MFRC522::StatusCode) mfrc522->MIFARE_Write(blockAddress, dataBlock, RFID_BYTES_PER_BLOCK); 		
        if (status != MFRC522::STATUS_OK) {
            Serial.print("MIFARE_write() failed: ");
            Serial.println(mfrc522->GetStatusCodeName(status));
            return false;
        }
		// check the result
		readString = byteVector2String(readBlockArray(blockAddress));
		if (readString.compareTo(stringData.substring(0, RFID_BYTES_PER_BLOCK)) != 0){
			return false;
		}
	} 
    return true;
}


/**
 * @brief The function erases SmartSeat Operation value data from a PICC.
 * @brief Erasing will only be performed if PICC is identified as SmartSeat PICC. 
 * @brief The identify status of the PICC keeps untouched.
 *
 * @return bool - the return value is true if the operation succeeded, false otherwise.
 */
bool RFID::eraseData() {
    MFRC522::StatusCode status;
	MFRC522::MIFARE_Key key;	
	byte blockAddress;
	byte trailerBlock;
	byte dataBlock[RFID_BYTES_PER_BLOCK] = {0}; 
	std::vector<byte> readBlock; 
	String readString;
	
	if (!cardIsPresent){
        return false;
    }
	// build the key
	for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) { 
		key.keyByte[i] = 0xFF; 
	} 

	if (cardState == identified) {
		for (int i = 1; i < (sizeof(RFID_USED_BLOCKS) / sizeof(*RFID_USED_BLOCKS)); i++){
			blockAddress = RFID_USED_BLOCKS[i];
			trailerBlock = (blockAddress / 4 + 1) * 4 - 1;
			// authenticate
			status = (MFRC522::StatusCode) mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522->uid)); 
			// write
			if (status != MFRC522::STATUS_OK) {
				Serial.print("Authentification failed: ");
				Serial.println(mfrc522->GetStatusCodeName(status));
				return false;
			} else {
				status = (MFRC522::StatusCode) mfrc522->MIFARE_Write(blockAddress, dataBlock, RFID_BYTES_PER_BLOCK); 		
				if (status != MFRC522::STATUS_OK) {
					Serial.print("MIFARE_write() failed: ");
					Serial.println(mfrc522->GetStatusCodeName(status));
					return false;
				}
				// check the result
				readBlock = readBlockArray(blockAddress);
				for (int j = 0; j < RFID_BYTES_PER_BLOCK; j++){
					if (readBlock[j] != 0) {
						return false;
					}
				}
			}
		}
	} else {
		return false;
	} 
    return true;
}


/**
 * @brief The function reads out a given block of a PICC.
 * @brief The result is a vector of 16 bytes.
 *
 * @param blockAddress - the byte number of the block that shall be read 
 * @return std::vector<byte> - the returned vector represents the read data from the PICC.
 *	 If the read operation failed, the vector contains 16 times byte 255.
 */
std::vector<byte> RFID::readBlockArray(byte blockAddress) {
    MFRC522::StatusCode status;
	MFRC522::MIFARE_Key key;	
	byte trailerBlock;
	uint8_t bufferSize = RFID_BYTES_PER_BLOCK + 2; 
	byte buffer[bufferSize]; 
	std::vector<byte> result(RFID_BYTES_PER_BLOCK, 255);
	
	if (cardIsPresent){
		// build the key
		for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) { 
			key.keyByte[i] = 0xFF; 
		} 
		trailerBlock = (blockAddress / 4 + 1) * 4 - 1;
		// authenticate
		status = (MFRC522::StatusCode) mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522->uid)); 
		// read
		if (status != MFRC522::STATUS_OK) {
			Serial.print("Authentification failed: ");
			Serial.println(mfrc522->GetStatusCodeName(status));
		} else {
			status = (MFRC522::StatusCode) mfrc522->MIFARE_Read(blockAddress, buffer, &bufferSize); 
			if (status == MFRC522::STATUS_OK) { 
				for (int i = 0; i < RFID_BYTES_PER_BLOCK; i++) { 
					result.at(i) = buffer[i];
				} 
			} 
		}
	}
	return result;
}


/**
 * @brief The function converts a given vector of bytes into a String.
 * @brief The result is a String of the same length as the size of byte vector.
 *
 * @param bytes - the vector of bytes. 
 * @return String - the String that represents the line of chars defined by the byte numbers.
 */
String RFID::byteVector2String(std::vector<byte> bytes) {
	char letter;
	String result = "";
	for(int i = 0; i < bytes.size(); i++) {
		letter = bytes.at(i);
		result += letter;
	}
	return result;
}	

