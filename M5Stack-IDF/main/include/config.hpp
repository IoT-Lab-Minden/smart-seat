/**
 * @file config.hpp
 * @brief Header for a SmartSeat's general configuration values 
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
 * config.hpp includes definitions and enums used by a SmartSeat
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#ifdef __cplusplus
extern "C" {
#endif

// motor 01
#define MOTOR_01_PIN_01 2
#define MOTOR_01_PIN_02 15

// motor 02
#define MOTOR_02_PIN_01 3
#define MOTOR_02_PIN_02 1

// motor 03
#define MOTOR_03_PIN_01 5
#define MOTOR_03_PIN_02 0

// motor 04
#define MOTOR_04_PIN_01 2
#define MOTOR_04_PIN_02 15

// motor 05
#define MOTOR_05_PIN_01 3
#define MOTOR_05_PIN_02 1

// motor 06
#define MOTOR_06_PIN_01 0
#define MOTOR_06_PIN_02 5

// potentiometer 01
#define POTI_01_PIN 26
#define POTI_01_MIN_VOLTAGE 1000
#define POTI_01_MAX_VOLTAGE 3350

// potentiometer 02
#define POTI_02_PIN 25
#define POTI_02_MIN_VOLTAGE 1270
#define POTI_02_MAX_VOLTAGE 3150

// potentiometer 03
#define POTI_03_PIN 13
#define POTI_03_MIN_VOLTAGE 1060
#define POTI_03_MAX_VOLTAGE 1120

// potentiometer 04
#define POTI_04_PIN 12
#define POTI_04_MIN_VOLTAGE 100
#define POTI_04_MAX_VOLTAGE 2800

// potentiometer 05
#define POTI_05_PIN 35
#define POTI_05_MIN_VOLTAGE 600
#define POTI_05_MAX_VOLTAGE 2500

// potentiometer 06
#define POTI_06_PIN 36
#define POTI_06_MIN_VOLTAGE 200
#define POTI_06_MAX_VOLTAGE 1300

// servo
#define SERVO_TIMEOUT 3000

// pwm 01
#define PWM_01_CHANNEL 0
#define PWM_01_FREQUENCY 30000
#define PWM_01_RESOLUTION 8
#define PWM_01_MIN_CYCLE 160
#define PWM_01_MAX_CYCLE 255
#define PWM_01_PIN 16

// pwm 02
#define PWM_02_CHANNEL 1
#define PWM_02_FREQUENCY 30000
#define PWM_02_RESOLUTION 8
#define PWM_02_MIN_CYCLE 160
#define PWM_02_MAX_CYCLE 255
#define PWM_02_PIN 17

// lcd
#define LCD_DISPLAY_WIDTH 320
#define LCD_DISPLAY_HEIGHT 240
#define LCD_DISPLAY_BRIGHTNESS 200

// rfid
#define RFID_I2C_ADDRESS 0x28
#define RFID_CHECK_INTERVAL 200 //milliseconds
#define RFID_NUMBER_OF_BLOCKS 64
#define RFID_BYTES_PER_BLOCK 16
const int RFID_USED_BLOCKS[21] = {4, 8, 9, 12, 13, 16, 17, 20, 21, 24, 25, 28, 29, 32, 33, 36, 37, 40, 41, 44, 45};
#define RFID_IDENTIFY_BLOCK 4
const int RFID_NAME_BLOCKS[10] = {8, 12, 16, 20, 24, 28, 32, 36, 40, 44};
#define RFID_IDENTIFY_NAME "SmartSeat"
enum CardState {empty, identified, unknown}; 

// operation specific
struct Target
{
    int operationIndex;
    float percentageGoal;
	int valueBlockOnPICC;
};


#ifdef __cplusplus
}
#endif

#endif 
