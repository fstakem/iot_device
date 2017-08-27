#ifndef PREDICTIVE_TRANSMITTER_H
#define PREDICTIVE_TRANSMITTER_H

#include <Wire.h>
#include <Arduino.h>
#include "Transmitter.h"
#include "Sensor.h"



class PredictiveTransmitter {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:

// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

// PUBLIC
// ------------------------------------------------------------------------------------------
public:

PredictiveTransmitter()
: Transmitter() {
    
}

~PredictiveTransmitter() {

}

virtual bool isReadyToTransmit(SamplePtr sample) {
    return true;
}

};

#endif