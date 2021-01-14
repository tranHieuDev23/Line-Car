#ifndef CAR_H
#define CAR_H

#include <Arduino.h>

#define STATE_STOP 0
#define STATE_MOVING_FORWARD 1
#define STATE_TURNING_LEFT 2
#define STATE_TURNING_RIGHT 3
#define STATE_MANUAL 4

class Car
{
private:
    int pwmA;
    int pwmB;
    int dA;
    int dB;
    int state;
    int pwmValue;

    void startWheel(int pwm, int d, bool forward)
    {
        analogWrite(pwm, pwmValue);
        digitalWrite(d, forward ? HIGH : LOW);
    }

    void stopWheel(int pwm)
    {
        analogWrite(pwm, 0);
    }

    void move(bool forward)
    {
        startWheel(pwmA, dA, forward);
        startWheel(pwmB, dB, forward);
    }

    void turnLeft()
    {
        stopWheel(pwmA);
        startWheel(pwmB, dB, true);
    }

    void turnRight()
    {
        stopWheel(pwmB);
        startWheel(pwmA, dA, true);
    }

    void stop()
    {
        stopWheel(pwmA);
        stopWheel(pwmB);
    }

public:
    Car(int pwmA, int pwmB, int dA, int dB) : pwmA(pwmA), pwmB(pwmB), dA(dA), dB(dB)
    {
        state = STATE_STOP;
    }

    void setup()
    {
        pinMode(pwmA, OUTPUT);
        pinMode(pwmB, OUTPUT);
        pinMode(dA, OUTPUT);
        pinMode(dB, OUTPUT);
        stop();
    }

    void loop()
    {
        switch (state)
        {
        case STATE_STOP:
            stop();
            break;
        case STATE_MOVING_FORWARD:
            move(true);
            break;
        case STATE_TURNING_LEFT:
            turnLeft();
            break;
        case STATE_TURNING_RIGHT:
            turnRight();
            break;
        case STATE_MANUAL:
            break;
        }
    }

    void setState(int state)
    {
        this->state = state;
    }

    void setPwmValue(int pwmValue)
    {
        this->pwmValue = pwmValue;
    }

    void moveLeftWheel(bool forward)
    {
        startWheel(pwmA, dA, forward);
    }

    void moveRightWheel(bool forward)
    {
        startWheel(pwmB, dB, forward);
    }

    void stopLeftWheel()
    {
        stopWheel(pwmA);
    }

    void stopRightWheel()
    {
        stopWheel(pwmB);
    }
};

#endif