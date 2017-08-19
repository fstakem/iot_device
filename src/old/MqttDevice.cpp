#include "MqttDevice.h"



namespace MqttDevice {

    IotDevice::MqttDevice init_mqtt_device(int id, String name) {
        IotDevice::MqttpDevice device;
        device.id = id;
        device.name = name;

        int i = 0;

        for (i = 0; i < NUM_SENSORS; i++ ) {
            int j = i + 1;
            String name = "sensor_" + String(j);
            device.sensors[i] = init_mqtt_sensor(j, "sensor");
        }

        device.server = mqtt_server;
        device.port = mqtt_port;
        device.mqtt_id = mqtt_id;
        device.user = mqtt_user
        device.password = mqtt_password;
        device.max_conn_attempts = mqtt_max_conn_attempts;

        return device;
    }

    IotDevice::MqttSensor init_mqtt_sensor(int id, String name) {
        IotDevice::MqttSensor sensor;
        sensor.id = id;
        sensor.name = name;
        sensor.tx_state = sensor_tx_state;
        sensor.data = data;
        sensor.transmitter = init_transmitter();

        sensor.pub_data_topic = mqtt_node + "/sensor/" + String(sensor.id) + "/data";
        sensor.sub_tx_state_topic = mqtt_node + "/sensor/" + String(sensor.id) + "/tx_state";
        sensor.sub_tx_rate_ms_topic = mqtt_node + "/sensor/" + String(sensor.id) + "/tx_rate_ms";

        return sensor;
    }

    IotDevice::Transmitter init_transmitter() {
        IotDevice::Transmitter transmitter;
        transmitter.last_tx_time = transmitter_last_tx_time;
        transmitter.tx_interval = transmitter_tx_interval;

        return transmitter;
    }

    SineWave init_sine_wave() {
        IotDevice::SineWave sine_wave;
        sine_wave.step_time = step_time;
        sine_wave.step_angle_rads = step_angle_rads;
        sine_wave.last_sample_time = 0;
        sine_wave.current_angle_rads = 0;
        sine_wave.current_noise = 0;
        sine_wave.value = 0;

        return sine_wave;
    }

    SignalGenerator init_signal_generator() {
        IotDevice::SignalGenerator generator;

        int i = 0;

        for (i = 0; i < NUM_SENSORS; i++ ) {
            generator.signals[i] = init_sine_wave();
        }

        return generator;
    }

    long get_next_sample(IotDevice::SineWave *wave) {
        unsigned long current_time = millis();

        if (current_time - wave->last_sample_time > wave->step_time) {
            wave->last_sample_time = current_time;
            wave->current_angle_rads = wave->current_angle_rads + wave->step_angle_rads;
            wave->current_noise = random(15);

            float sine_value = sin(wave->current_angle_rads) * 100;
            wave->value = (long) sine_value + wave->current_noise;
        }

        return wave->value;
    }

    void get_fake_data(IotDevice::MqttDevice *device, IotDevice::SignalGenerator *generator) {
        int i = 0;

        for (i = 0; i < NUM_SENSORS; i++ ) {
            device->sensors[i].data = get_next_sample(&generator->signals[i]);
        }
    }

    void show_data(IotDevice::MqttDevice *device) {
        unsigned long current_time = millis();
        Serial.print(String(current_time));
        Serial.print(": ");
        Serial.println(device->name);

        int i = 0;

        for (i = 0; i < NUM_SENSORS; i++ ) {
            Serial.print("\t" + device->sensors[i].name);
            Serial.print(": ");
            Serial.println(String(device->sensors[i].data));
        }
    }

    void mqtt_connect() {

    }

    void mqtt_subscribe() {
        
    }
};




