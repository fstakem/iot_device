#ifndef IOT_DEVICE_H
#define IOT_DEVICE_H

#include <Wire.h>
#include <Arduino.h>

namespace IotDevice {
    // Initializers
    const int BUFFER_LEN                            = 200;
    const unsigned int NUM_SENSORS                  = 5;

    const String device_name                        = "generic_device";
    const String sensor_name                        = "generic_sensor";
    const bool sensor_tx_state                      = false;
    const unsigned long transmitter_last_tx_time    = 0;
    const unsigned long transmitter_tx_interval     = 1000;
    const long data                                 = 0;
    static const unsigned long step_time            = 2000;
    static const float step_angle_rads              = 0.261799f;


    struct Transmitter {
        unsigned long last_tx_time;
        unsigned long tx_interval;
    };

    struct Sensor {
        int id;
        String name;
        bool tx_state;
        long data;
        Transmitter transmitter;
    };

    struct Device {
        int id;
        String name;
        char tx_buff[BUFFER_LEN];
        char rx_buff[BUFFER_LEN];
        char address_buff[BUFFER_LEN];
        Sensor sensors[NUM_SENSORS];
    };

    struct SineWave {
        unsigned long step_time;
        float step_angle_rads;
        unsigned long last_sample_time;
        float current_angle_rads;
        long current_noise;
        long value;
    };

    struct SignalGenerator {
        SineWave signals[NUM_SENSORS];
    };


    // Constructors
    Device init_device(int id, String name);
    Sensor init_sensor(int id, String name);
    Transmitter init_transmitter();
    SineWave init_sine_wave();
    SignalGenerator init_signal_generator();

    long get_next_sample(IotDevice::SineWave *sine_wave);
    void get_fake_data(IotDevice::Device *device, IotDevice::SignalGenerator *generator);
    void show_data(IotDevice::Device *device);
};

#endif