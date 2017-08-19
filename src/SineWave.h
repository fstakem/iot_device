#ifndef SINE_WAVE_H
#define SINE_WAVE_H

#include <Wire.h>
#include <Arduino.h>


class SineWave {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:

unsigned long step_time;
float step_angle_rads;
unsigned long last_sample_time;
float current_angle_rads;
long current_noise;
long value;


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

SineWave() {
    this->step_time = 2000;
    this->step_angle_rads = 0.261799f;
    this->last_sample_time = 0;
    this->current_angle_rads = 0.0f;
    this->current_noise = 0;
    this->value = 0;
}

~SineWave() {

}

long getSample() {
    unsigned long current_time = millis();

    if (current_time - this->last_sample_time > this->step_time) {
        this->last_sample_time = current_time;
        this->current_angle_rads = this->current_angle_rads + this->step_angle_rads;
        this->current_noise = random(15);

        float sine_value = sin(this->current_angle_rads) * 100;
        this->value = (long) sine_value + this->current_noise;
    }

    return this->value;
}


};

#endif