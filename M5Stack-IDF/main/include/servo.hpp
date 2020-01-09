/**
 * @file servo.hpp
 * @brief Header for the Servo class
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
 * The Servo class represents a physically present servo device,
 * a combination of one dc-motor and one potentiometer.
 */

#ifndef SERVO_HPP
#define SERVO_HPP
#include "motor.hpp"
#include "potentiometer.hpp"
#include "../include/config.hpp"

class Servo
{
public:
    Servo(Motor* motor, Potentiometer* potentiometer, bool changeMotDir, bool changePotiDir, float percSpeedForward, float percSpeedBackward);
    ~Servo();

    Potentiometer* getPotentiometer();
    Motor* getMotor();
    uint16_t getVoltage();
    float getPercentagePosition();
    float getPercentageSpeedForward();
    float getPercentageSpeedBackward();
	bool getChangePotiDirection();
    void moveForward();
    void moveBackward();
    void stop();
    bool moveToTarget(float target, float startSpeed);

private:
    Motor* servoMotor;
    Potentiometer* servoPotentiometer;
	bool changeMotorDirection;
	bool changePotiDirection;
	float percentageSpeedForward;
	float percentageSpeedBackward;
};

#endif