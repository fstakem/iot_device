#ifndef SYNC_TRANSMITTER_H
#define SYNC_TRANSMITTER_H

#include <Wire.h>
#include <Arduino.h>
#include "Transmitter.h"
#include "Sensor.h"



class SyncTransmitter {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:

// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

unsigned long last_tx_time;
unsigned int tx_interval;

// PUBLIC
// ------------------------------------------------------------------------------------------
public:

SyncTransmitter(unsigned int tx_interval)
: Transmitter() {
    this->last_tx_time = 0;
    this->tx_interval = tx_interval;
}

~SyncTransmitter() {

}

bool isReadyToTransmit(SamplePtr sample) {
    if (this->last_tx_time + this->tx_interval >= sample->timestamp) {
        this->last_tx_time = sample->timestamp;
        
        return true;
    }

    return false;
}

};

#endif