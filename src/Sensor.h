#ifndef SENSOR_H
#define SENSOR_H

#include <Wire.h>
#include <Arduino.h>
#include "Transmitter.h"


class Sensor {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:


// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

int id;
String name;
bool tx_state;
long data;
Transmitter transmitter;


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

Sensor() {

}

Sensor(int id, String name) {
    this->id = id;
    this->name = name;
    this->tx_state = false;
    this->data = 0;
    this->transmitter = Transmitter();
}

~Sensor() {

}

virtual long getSample() {
    return 0;
}


String getName() {
    return this->name;
}



};

#endif