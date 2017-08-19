#ifndef TEST_DEVICE_H
#define TEST_DEVICE_H

#include <Wire.h>
#include <Arduino.h>
#include "Device.h"
#include "TestSensor.h"


class TestDevice: public Device {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

TestDevice(int id, String name) 
: Device(id, name)
{
    this->createSensors(this->num_sensors);
}

~TestDevice() {

}

void createSensors(int num_sensors) {
    this->num_sensors = num_sensors;

    int i = 0;

    for (i = 0; i < this->num_sensors; i++ ) {
        int j = i + 1;
        String name = "sensor_" + String(j);
        this->sensors[i] = new TestSensor(j, name);
    }
}


};

#endif