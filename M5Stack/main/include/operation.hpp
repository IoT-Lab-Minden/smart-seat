/**
 * @file operation.hpp
 * @brief Header for the Operation class
 *
 * @author	Torsten Niemeier
 * @author	E-mail: <tniemeier1@fh-bielefeld.de>
 * @version 1.0
 * @date 2020/01/03
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

#ifndef OPERATION_HPP
#define OPERATION_HPP
#include "servo.hpp"
#include "light.hpp"
#include "heater.hpp"

class Operation {
    public: 
		Operation(String name, const unsigned char imageArray[]);
		Operation(String name, const unsigned char imageArray[], Servo* servo);
		Operation(String name, const unsigned char imageArray[], Light* light);
		Operation(String name, const unsigned char imageArray[], Heater* heater);
		Operation(String name, const unsigned char imageArray[], Motor* motor);
		~Operation();

        String getOperationName();
		String getOperationType();
        const unsigned char* getImageArray();
        Servo* getOperationServo();
        Light* getOperationLight();
        Heater* getOperationHeater();
        Motor* getOperationMotor();

    private:
        String operationName;
        String operationType;
        const unsigned char *operationImageArray = nullptr;
		Servo* operationServo = nullptr;
		Light* operationLight = nullptr;
		Heater* operationHeater = nullptr;
		Motor* operationMotor = nullptr;
};


#endif