#ifndef DEVICE_H
#define DEVICE_H

#include <Wire.h>
#include <Arduino.h>
#include "Sample.h"
#include "Sensor.h"
#include "Signal.h"
#include "Transmitter.h"


typedef Sensor* SensorPtr;
typedef Transmitter* TransmitterPtr;
typedef Signal* SignalPtr;


class Device {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:


// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

int id;
String name;
unsigned int num_sensors;
SensorPtr *sensors;

virtual void deleteSensors() {
    if (this->sensors != NULL) {
        int i;

        for (i = 0; i < this->num_sensors; i++ ) {
            delete this->sensors[i];
        }

        delete this->sensors;
    }

    this->num_sensors = 0;
}


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

Device(int id, String name) {
    this->id = id;
    this->name = name;
    this->num_sensors = 0;
    this->sensors = NULL;
}

~Device() {
    this->deleteSensors();
}

int createSensor(SignalPtr signal, TransmitterPtr transmitter) {
    int new_num_sensors = this->num_sensors + 1;
    SensorPtr *new_sensors = new SensorPtr[new_num_sensors];

    int i;

    for (i = 0; i < this->num_sensors; i++) {
        new_sensors[i] = this->sensors[i];
    }

    int id = this->num_sensors;
    new_sensors[id] = new Sensor(id, signal, transmitter);

    delete this->sensors;
    this->sensors = new_sensors;
    this->num_sensors = new_num_sensors;

    return id;
}

bool removeSensor(unsigned int id) {
    if (id < this->num_sensors) {
        int new_num_sensors = this->num_sensors - 1;
        SensorPtr *new_sensors = new SensorPtr[new_num_sensors];

        int i;

        for (i = 0; i < this->num_sensors; i++) {
            if (i == id) {
                delete this->sensors[i];
            } else {
                new_sensors[i] = this->sensors[i];
                new_sensors[i]->setId(i);
            }
        }

        delete this->sensors;
        this->sensors = new_sensors;
        this->num_sensors = new_num_sensors;

        return true;
    }
    
    return false;
}

int getNumSensors() {
    return this->num_sensors;
}

void getSamples(SamplePtr *samples) {
    if (samples != NULL) {
        delete samples;
    }
    
    samples = new SamplePtr[this->num_sensors];
    int i;

    for(i = 0; i < this->num_sensors; i++) {
        samples[i] = this->sensors[i]->getSample();
    }
}

void showSamples() {
    int i;

    for(i = 0; i < this->num_sensors; i++) {
        SamplePtr sample = this->sensors[i]->getSample();
        String output = String(sample->timestamp) + ": " + String(sample->value);
        Serial.println(output);
    }
}

};

#endif