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

SineWave sine_wave;


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

TestSensor()
: Sensor()
{

}

TestSensor(int id, String name) 
: Sensor(id, name)
{
    this->sine_wave = SineWave();
}

~TestSensor() {

}

long getSample() {
    return this->sine_wave.getSample();
}


};

#endif