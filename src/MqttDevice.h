#ifndef MQTT_DEVICE_H
#define MQTT_DEVICE_H

#include <Wire.h>
#include <Arduino.h>

namespace MqttDevice {

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

    const char* mqtt_server                         = "hassbian.local";
    const int mqtt_port                             = 1883;
    const char* mqtt_node                           = "wemos_node/1";
    const char* mqtt_id                             = "fstakem98765";
    const char* mqtt_user                           = "fstakem";
    const char* mqtt_password                       = "password";
    const int mqtt_max_conn_attempts                = 3;


    struct Transmitter {
        unsigned long last_tx_time;
        unsigned long tx_interval;
    };

    struct MqttSensor {
        // Base
        int id;
        String name;
        bool tx_state;
        long data;
        Transmitter transmitter;

        // Mqtt
        String pub_data_topic;
        String sub_tx_state_topic;
        String sub_tx_rate_ms_topic;
    };

    struct MqttDevice {
        // Base
        int id;
        String name;
        char tx_buff[BUFFER_LEN];
        char rx_buff[BUFFER_LEN];
        char address_buff[BUFFER_LEN];

        // Mqtt
        char* server;
        int port;
        char* mqtt_id;
        char* user;
        char* password;
        int max_conn_attempts;
        MqttSensor sensors[NUM_SENSORS];
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
    MqttDevice init_mqtt_device(int id, String name);
    MqttSensor init_mqtt_sensor(int id, String name);
    Transmitter init_transmitter();
    SineWave init_sine_wave();
    SignalGenerator init_signal_generator();

    long get_next_sample(IotDevice::SineWave *sine_wave);
    void get_fake_data(IotDevice::Device *device, IotDevice::SignalGenerator *generator);
    void show_data(IotDevice::Device *device);
    void mqtt_connect();
    void mqtt_subscribe();
};

#endif