#ifndef THRESHOLD_TRANSMITTER_H
#define THRESHOLD_TRANSMITTER_H

#include <Wire.h>
#include <Arduino.h>
#include "Transmitter.h"
#include "Sensor.h"



class ThresholdTransmitter {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:

// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

long last_tx_value;
bool is_upper_threshold;
int threshold;

// PUBLIC
// ------------------------------------------------------------------------------------------
public:

ThresholdTransmitter(bool is_upper_threshold, int threshold)
: Transmitter() {
    this->last_tx_value = 0;
    this->is_upper_threshold = is_upper_threshold;
    this->threshold = threshold;
}

~ThresholdTransmitter() {

}

virtual bool isReadyToTransmit(SamplePtr sample) {
    if (this->is_upper_threshold) {
        if(sample->value >= this->threshold) {
            this->last_tx_value = sample->value;

            return true;
        }
    } else {
        if(sample->value <= this->threshold) {
            this->last_tx_value = sample->value;

            return true;
        }
    }

    return false;
}

};

#endif