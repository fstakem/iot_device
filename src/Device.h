#ifndef DEVICE_H
#define DEVICE_H

#include <Wire.h>
#include <Arduino.h>
#include "Sensor.h"

#define BUFFER_LEN 200
#define NUM_SENSORS 5


class Device {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:


// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

int id;
String name;
char tx_buff[BUFFER_LEN];
char rx_buff[BUFFER_LEN];
char address_buff[BUFFER_LEN];
int num_sensors;
Sensor *sensors[NUM_SENSORS];
long last_samples[NUM_SENSORS];

virtual void createSensors() {
    int i = 0;

    for (i = 0; i < this->num_sensors; i++ ) {
        int j = i + 1;
        String name = "sensor_" + String(j);
        this->sensors[i] = new Sensor(j, name);
    }
}


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

Device(int id, String name) {
    this->id = id;
    this->name = name;
    this->num_sensors = NUM_SENSORS;
    this->createSensors();
}

~Device() {
    int i;
    
    for (i = 0; i < this->num_sensors; i++ ) {
        delete this->sensors[i];
    }
}

int getNumSensors() {
    return this->num_sensors;
}

long* getSamples() {
    int i = 0;

    for(i = 0; i < this->num_sensors; i++) {
        this->last_samples[i] = this->sensors[i]->getSample();
    }

    return this->last_samples;
}

void showSamples() {
    int i;

    for(i = 0; i < this->num_sensors; i++) {
        String output = "Sensor: " + this->sensors[i]->getName() + " " + String(this->last_samples[i]);
        Serial.println(output);
    }
}

};

#endif