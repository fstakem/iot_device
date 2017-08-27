#ifndef DIFF_TRANSMITTER_H
#define DIFF_TRANSMITTER_H

#include <Wire.h>
#include <Arduino.h>
#include <cmath>
#include "Transmitter.h"
#include "Sensor.h"



class DiffTransmitter {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:

// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

long last_tx_value;
unsigned int threshold;

// PUBLIC
// ------------------------------------------------------------------------------------------
public:

DiffTransmitter(unsigned int threshold)
: Transmitter() {
    this->last_tx_value = 0.0f;
    this->threshold = threshold;
}

~DiffTransmitter() {

}

bool isReadyToTransmit(SamplePtr sample) {
    long diff = abs(sample->value - last_tx_value);

    if (diff >= this->threshold) {
        this->last_tx_value = sample->value;

        return true;
    }

    return false;
}

};

#endif