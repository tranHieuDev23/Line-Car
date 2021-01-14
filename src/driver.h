#ifndef DRIVER_H
#define DRIVER_H

#include <Arduino.h>
#define IR_SENSOR_PINS_COUNT 3
#define COMMAND_MOVE_FORWARD 0
#define COMMAND_TURN_LEFT 1
#define COMMAND_TURN_RIGHT 2
#define UNSET_VALUE -1

class Driver
{
private:
    int irSensorPins[IR_SENSOR_PINS_COUNT];
    int lastState;
    int lastBackground;
    int lastCommand;

    int scan()
    {
        int state = 0;
        for (int i = 0; i < IR_SENSOR_PINS_COUNT; i++)
        {
            const int value = digitalRead(irSensorPins[i]);
            state <<= 1;
            if (value == HIGH)
            {
                state |= 1;
            }
        }
        return state;
    }

    int scanValue(int state, int sensorId)
    {
        return (state >> (IR_SENSOR_PINS_COUNT - 1 - sensorId)) & 1;
    }

    int calculateBackground(int state)
    {
        const int leftMostValue = scanValue(state, 0);
        const int rightMostValue = scanValue(state, IR_SENSOR_PINS_COUNT - 1);
        if (leftMostValue == rightMostValue)
        {
            return leftMostValue;
        }
        return lastBackground;
    }

    bool isStateAllBackground(int state, int background)
    {
        if (background == 1)
        {
            return state == (1 << IR_SENSOR_PINS_COUNT) - 1;
        }
        return state == 0;
    }

    std::pair<int, int> findLineRange(int state, int background)
    {
        int lineL = UNSET_VALUE;
        int lineR = UNSET_VALUE;
        for (int i = 0; i < IR_SENSOR_PINS_COUNT; i++)
        {
            int value = scanValue(state, i);
            if (value != background)
            {
                if (lineL == UNSET_VALUE)
                {
                    lineL = i;
                }
                lineR = i + 1;
            }
        }
        return std::make_pair(lineL, lineR);
    }

public:
    Driver(const int *irSensorPins)
    {
        for (int i = 0; i < IR_SENSOR_PINS_COUNT; i++)
        {
            this->irSensorPins[i] = irSensorPins[i];
        }
        lastCommand = COMMAND_MOVE_FORWARD;
    }

    void setup()
    {
        for (int i = 0; i < IR_SENSOR_PINS_COUNT; i++)
        {
            pinMode(this->irSensorPins[i], INPUT);
        }
        lastState = scan();
        lastBackground = calculateBackground(lastState);
    }

    int drive()
    {
        int state = scan();
        int background = calculateBackground(state);
        lastBackground = background;
        if (isStateAllBackground(state, background))
        {
            return lastCommand;
        }
        std::pair<int, int> lineRange = findLineRange(state, background);
        int leftMargin = lineRange.first;
        int rightMargin = IR_SENSOR_PINS_COUNT - lineRange.second;
        if (leftMargin == rightMargin)
        {
            return lastCommand = COMMAND_MOVE_FORWARD;
        }
        if (leftMargin < rightMargin)
        {
            return lastCommand = COMMAND_TURN_LEFT;
        }
        return lastCommand = COMMAND_TURN_RIGHT;
    }
};

#endif