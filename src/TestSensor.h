#ifndef TEST_SENSOR_H
#define TEST_SENSOR_H

#include <Wire.h>
#include <Arduino.h>
#include "Sensor.h"
#include "SineWave.h"


class TestSensor: public Sensor {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:

Wave *wave;


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

TestSensor()
: Sensor() {

}

TestSensor(int id, String name, Wave *wave) 
: Sensor(id, name)
{
    this->wave = wave;
}

~TestSensor() {
    delete this->wave;
}

long getSample() {
    return this->wave->getSample();
}


};

#endif