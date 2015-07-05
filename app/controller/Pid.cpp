/*
 * Copyright 2015 BrewPi / Elco Jacobs
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "Pid.h"

Pid::Pid(BasicTempSensor * input,
         LinearActuator *        output)
{
    setConstants(temp(10.0), temp(0.2), temp(-1.5));
    setMinMax(temp::min(), temp::max());

    error           = 0;
    derivative      = 0;
    integral        = 0;
    failedReadCount = 0;

    setInputSensor(input);
    setOutputActuator(output);

    autotune = true;
    tuning = false;
    outputLag = 0;
    maxDerivative = 0.0;
}

Pid::~Pid(){}

void Pid::setConstants(temp kp,
                       temp ki,
                       temp kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
    Ka = temp(5.0) * Kp;
}

void Pid::update()
{
    temp inputVal;

    if (!inputSensor || (inputVal = inputSensor -> read()).isDisabledOrInvalid()){
        // Could not read from input sensor
        if (failedReadCount < 255){    // limit
            failedReadCount++;
        }

        // Try to reconnect
        if (inputSensor -> init()){
            if (failedReadCount > 60){
                // re-initialize filters if sensor has been lost longer than 60 seconds
                inputVal = inputSensor -> read();

                inputFilter.init(inputVal);
                derivativeFilter.init(temp_precise(0.0));

                failedReadCount = 0;
            }
        } else{
            if (failedReadCount > 60){
                outputActuator -> setActive(false);
            }

            return;
        }
    }

    inputFilter.add(inputVal);

    error = setPoint - inputFilter.readOutput();

    temp_precise delta = inputFilter.readOutput() - inputFilter.readPrevOutput();
    derivativeFilter.add(delta * temp_precise(60.0)); // use slope per minute

    derivative = derivativeFilter.readOutput();

    // calculate PID parts.
    p = Kp * error;
    i = Ki * integral / temp_long(60.0); // from the user's perspective, the integral is per minute
    d = Kd * derivative;

    temp_long pidResult = temp_long(p) + temp_long(i) + temp_long(d);
    temp      output    = pidResult;    // will be constrained to -128/128 here

    outputActuator -> setValue(output);

    // get actual value from actuator
    output = outputActuator->readValue();

    // update integral with anti-windup back calculation
    // pidResult - output is zero when actuator is not saturated
    integral = integral + error + Ka * (output - pidResult);

    static uint16_t lagTimer = 0;
    if(autotune){
        temp min = outputActuator->min();
        temp max = outputActuator->max();
        temp tuningThreshold = (max >> uint8_t(1)) + (min >> uint8_t(1));

        // tune only when driving the output hard enough
        tuning = (output > tuningThreshold);
    }
    else{
        tuning = false;
    }
    if(tuning){
        if(derivativeFilter.detectPosPeak(&maxDerivative)){
            uint16_t filterDelay = derivativeFilter.getDelay();
            outputLag = (lagTimer <filterDelay) ? 0  : lagTimer - filterDelay;

            // set filters to 1/2 of lag time for max noise suppression that can still follow the input signal
            inputFilter.setFilteringForDelay(outputLag/2);
            derivativeFilter.setFilteringForDelay(outputLag/2);
            tuning = false;
        }
        lagTimer++;
    }
    else{
        lagTimer = 0;
    }
}

void Pid::setSetPoint(temp val)
{
    if ((val - setPoint) > temp(0.25)){
        integral = 0;    // reset integrator for big jumps in setpoint
    }

    setPoint = val;
}

void Pid::setMinMax(temp min,
                    temp max)
{
    this -> min = min;
    this -> max = max;
}

void Pid::setFiltering(uint8_t b){
    inputFilter.setFiltering(b);
    derivativeFilter.setFiltering(b);
}

uint8_t Pid::getFiltering(){
    return inputFilter.getFiltering();
}

void Pid::setInputFilter(uint8_t b)
{
    inputFilter.setFiltering(b);
}

void Pid::setDerivativeFilter(uint8_t b)
{
    derivativeFilter.setFiltering(b);
}

bool Pid::setInputSensor(BasicTempSensor * s)
{
    temp t = s -> read();

    if (t.isDisabledOrInvalid()){
        return false;    // could not read from sensor
    }

    inputSensor = s;

    inputFilter.init(t);
    derivativeFilter.init(0.0);

    return true;
}

bool Pid::setOutputActuator(LinearActuator * a)
{
    outputActuator = a;

    return true;
}

