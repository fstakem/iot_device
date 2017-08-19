#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <Wire.h>
#include <Arduino.h>


class Transmitter {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:

unsigned long last_tx_time;
unsigned long tx_interval;


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

Transmitter() {
    this->last_tx_time = 0;
    this->tx_interval = 1000;
}

~Transmitter() {

}


};

#endif