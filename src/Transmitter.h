#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <Wire.h>
#include <Arduino.h>
#include "Sample.h"


typedef Sample* SamplePtr;


class Transmitter {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:

// PROTECTED
// ------------------------------------------------------------------------------------------
protected:


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

Transmitter() {
    
}

~Transmitter() {

}

virtual bool isReadyToTransmit(SamplePtr sample) {
    return true;
}

};

#endif