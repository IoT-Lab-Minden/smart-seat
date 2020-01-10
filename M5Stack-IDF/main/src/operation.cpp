/**
 * @file operation.cpp
 * @brief Source code for the Operation class
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
 * The Operation class represents a function the SmartSeat can perform.
 */

#include "../include/operation.hpp"



/**
 * @brief Constructor
 * @brief The function creates a Operation object of the intern 'wait'-type.
 *
 * @param name - a String representing the the Operation's name
 *	 The first 16 characters of the name have to be unique to distinct the Operation from other ones.
 * @param imageArray[] - an array of const unsigned chars describing the Operation's associated image
 * @return nothing
 */
Operation::Operation(String name, const unsigned char imageArray[])
	: operationName(name), operationImageArray(imageArray)
{
	operationType = "rfid";
}


/**
 * @brief Constructor
 * @brief The function creates a Operation object of the intern 'servo'-type.
 *
 * @param name - a String representing the the Operation's name
 *	 The first 16 characters of the name have to be unique to distinct the Operation from other ones.
 * @param imageArray[] - an array of const unsigned chars describing the Operation's associated image
 * @param servo - a pointer to the Operation's assigned Servo class instance
 * @return nothing
 */
Operation::Operation(String name, const unsigned char imageArray[], Servo* servo)
	: operationName(name), operationImageArray(imageArray), operationServo(servo)
{
	operationType = "servo";
}


/**
 * @brief Constructor
 * @brief The function creates a Operation object of the intern 'light'-type.
 *
 * @param name - a String representing the the Operation's name
 *	 The first 16 characters of the name have to be unique to distinct the Operation from other ones.
 * @param imageArray[] - an array of const unsigned chars describing the Operation's associated image
 * @param light - a pointer to the Operation's assigned Light class instance
 * @return nothing
 */
Operation::Operation(String name, const unsigned char imageArray[], Light* light)
	: operationName(name), operationImageArray(imageArray), operationLight(light)
{
	operationType = "light";
}


/**
 * @brief Constructor
 * @brief The function creates an Operation object of the intern 'heater'-type.
 *
 * @param name - a String representing the the Operation's name
 *	 The first 16 characters of the name have to be unique to distinct the Operation from other ones.
 * @param imageArray[] - an array of const unsigned chars describing the Operation's associated image
 * @param heater - a pointer to the Operation's assigned Heater class instance
 * @return nothing
 */
Operation::Operation(String name, const unsigned char imageArray[], Heater* heater)
	: operationName(name), operationImageArray(imageArray), operationHeater(heater)
{
	operationType = "heater";
}


/**
 * @brief Constructor
 * @brief The function creates an Operation object of the intern 'motor'-type.
 *
 * @param name - a String representing the the Operation's name
 *	 The first 16 characters of the name have to be unique to distinct the Operation from other ones.
 * @param imageArray[] - an array of const unsigned chars describing the Operation's associated image
 * @param motor - a pointer to the Operation's assigned Motor class instance
 * @return nothing
 */
Operation::Operation(String name, const unsigned char imageArray[], Motor* motor)
	: operationName(name), operationImageArray(imageArray), operationMotor(motor)
{
	operationType = "motor";
}


/**
 * @brief Destructor
 * @brief The function destroys an Operation object
 *
 * @return no return 
 */
Operation::~Operation()
{
	delete operationServo;
	delete operationMotor;
	delete operationLight;
	delete operationHeater;
	delete operationImageArray;
}


/**
 * @brief The function gets the Operation's name.
 *
 * @return String - the name of the Operation as String
 */
String Operation::getOperationName(){
	return operationName;
}


/**
 * @brief The funtion gets the Operation's type.
 *
 * @return String - the type of the Operation as String
 */
String Operation::getOperationType(){
	return operationType;
}


/**
 * @brief The function gets the Operation's image array.
 *
 * @return const unsigned char* - a pointer to the Operation's associated image array
 */
const unsigned char* Operation::getImageArray(){
	return operationImageArray;
}


/**
 * @brief The function gets the Operation's Servo unit.
 *
 * @return Servo* - the pointer to the Operation's assigned Servo class instance
 */
Servo* Operation::getOperationServo(){
	return operationServo;
}


/**
 * @brief The function gets the Operation's Heater unit.
 *
 * @return Heater* - the pointer to the Operation's assigned Heater class instance
 */
Heater* Operation::getOperationHeater(){
	return operationHeater;
}


/**
 * @brief The function gets the Operation's Light unit.
 *
 * @return Light* - the pointer to the Operation's assigned Light class instance
 */
Light* Operation::getOperationLight(){
	return operationLight;
}


/**
 * @brief The function gets the Operation's Motor unit.
 *
 * @return Motor* - the pointer to the Operation's assigned Motor class instance
 */
Motor* Operation::getOperationMotor(){
	return operationMotor;
}



