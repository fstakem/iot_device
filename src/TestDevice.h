#ifndef TEST_DEVICE_H
#define TEST_DEVICE_H

#include <Wire.h>
#include <Arduino.h>
#include "Device.h"
#include "Sensor.h"
#include "TestSensor.h"
#include "Wave.h"
#include "SineWave.h"

#define STEP_TIME 2000
#define STEP_ANGLE_RADS 0.261799f


class TestDevice: public Device {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:
String wave_type;

Sensor* createSensor(int id, String name) {
    Wave *wave = this->createWave();
    Sensor *sensor = new TestSensor(id, name, wave);

    return sensor;
}

Wave *createWave() {
    if (this->wave_type.equals("sine")) {
        return new SineWave(STEP_TIME, STEP_ANGLE_RADS);
    }

    return NULL;
}


// PROTECTED
// ------------------------------------------------------------------------------------------
protected:
void createSensors() {
    int i = 0;

    for (i = 0; i < this->num_sensors; i++ ) {
        int j = i + 1;
        String name = "sensor_" + String(j);
        this->sensors[i] = this->createSensor(j, name);
    }
}


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

TestDevice(int id, String name, String wave_type) 
: Device(id, name) {
    this->wave_type = wave_type;
    this->createSensors();
}

~TestDevice() {

}

};

#endif