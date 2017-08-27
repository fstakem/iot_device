#ifndef SENSOR_H
#define SENSOR_H

#include <Wire.h>
#include <Arduino.h>
#include "Signal.h"
#include "Transmitter.h"
#include "Sample.h"


typedef Transmitter* TransmitterPtr;
typedef Signal* SignalPtr;


class Sensor {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:


// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

int id;
String name;
bool is_on;
SamplePtr last_sample;
SignalPtr signal;
TransmitterPtr transmitter;

virtual void sample() {
    if (this->last_sample != NULL) {
        delete this->last_sample;
    }
    
    this->last_sample = new Sample();
    this->last_sample->timestamp = millis();
    this->last_sample->value = this->signal->getSample();
}


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

Sensor() {

}

Sensor(unsigned int id, SignalPtr signal, TransmitterPtr transmitter) {
    this->setId(id);
    this->is_on = true;
    this->last_sample = NULL;
    this->signal = signal;
    this->transmitter = transmitter;
}

~Sensor() {
    if (this->last_sample != NULL) {
        delete this->last_sample;
    }

    if (this->signal != NULL) {
        delete this->signal;
    }

    if (this->transmitter != NULL) {
        delete this->transmitter;
    }
}

SamplePtr getSample() {
    this->sample();

    return this->last_sample;
}

bool isReadyToTransmit(SamplePtr sample) {
    if (this->is_on) {
        return this->transmitter->isReadyToTransmit(sample);
    }
    
    return false;
}

String getName() {
    return this->name;
}

bool isOn() {
    return this->is_on;
}

void setState(bool is_on) {
    this->is_on = is_on;
}

int getId() {
    return this->id;
}

void setId(unsigned int id) {
    this->id = id;
    this->name = "sensor_" + String(this->id);
}



};

#endif