#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BUFFER_LEN 200

typedef PubSubClient* PubSubClientPtr;
typedef Device* DevicePtr;


class MqttClient {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:


// PROTECTED
// ------------------------------------------------------------------------------------------
protected:

DevicePtr device;
PubSubClientPtr client;
unsigned int mqtt_max_conn_attempts;
String *pub_data_topics;
String *sub_tx_state_topics;
String *sub_tx_rate_topics;

// Buffers
char tx_buff[BUFFER_LEN];
char rx_buff[BUFFER_LEN];
char topic_buff[BUFFER_LEN];

void createTopics(String root_name) {
    int num_sensors = this->device->getNumSensors();

    this->pub_data_topics = new String[num_sensors];
    this->sub_tx_state_topics = new String[num_sensors];
    this->sub_tx_rate_topics = new String[num_sensors];
    int i;

    for(i = 0; i < num_sensors; i++) {
        this->pub_data_topics[i]        = root_name + "/sensor/" + String(i) + "/data";
        this->sub_tx_state_topics[i]    = root_name + "/sensor/" + String(i) + "/tx";
        this->sub_tx_rate_topics[i]     = root_name + "/sensor/" + String(i) + "/tx_rate_ms";
    }
}

void deleteTopics() {
    delete this->pub_data_topics;
    delete this->sub_tx_state_topics;
    delete this->sub_tx_rate_topics;
}


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

MqttClient(DevicePtr device, WiFiClient wifi_client, IPAddress server, int port) {
    this->device = device;
    this->mqtt_max_conn_attempts = 5;
    this->client = new PubSubClient(wifi_client);
    this->client->setServer(server, port);
    this->client->setCallback([this] (char* topic, byte* payload, unsigned int length) { this->handleMsg(topic, payload, length); });
}

~MqttClient() {
    this->deleteTopics();
    delete this->client;
}

bool connect(char* id, char* user, char* password, String root_name) {
    int mqtt_conn_attempts = 0;

    while (!this->client->connected()) {
        Serial.print("Attempting MQTT connection...");
        mqtt_conn_attempts += 1;
        
        if (this->client->connect(id, user, password)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(this->client->state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }

        if(mqtt_conn_attempts >= this->mqtt_max_conn_attempts) {
            return false;
        }
    }

    this->subscribe(root_name);

    return true;
}

void subscribe(String root_name) {
    this->createTopics(root_name);

    int num_sensors = this->device->getNumSensors();
    int i;

    for(i = 0; i < num_sensors; i++) {
        this->sub_tx_state_topics[i].toCharArray(this->topic_buff, this->sub_tx_state_topics[i].length()+1);
        this->client->subscribe(this->topic_buff);

        this->sub_tx_rate_topics[i].toCharArray(this->topic_buff,this->sub_tx_rate_topics[i].length()+1);
        this->client->subscribe(this->topic_buff);
    }
}

void handleMsg(char* topic, byte* payload, unsigned int length) {

}

void transmitData() {
    long* samples = this->device->getSamples();
    String data = MQTT_NODE_NAME + "/sensor/" + String(sensor_id) + "/" + String(value);
    data.toCharArray(tx_buff, data.length()+1);


    String data_topic = this->pub_data_topics[sensor_id];
    data_topic.toCharArray(this->tx_buff, data_topic.length()+1);
    this->client.publish(this->topic_buff, this->tx_buff);





    int num_sensors = this->device->getNumSensors();
    int i;

    for(i = 0; i < num_sensors; i++) {
        this->transmitData(i);
    }
}



PubSubClientPtr getClient() {
    this->client;
}

};

#endif