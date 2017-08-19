#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


// Wifi
const char* SSID                    = "frugmunster";
const char* SSID_PASSWORD           = "xxx";

// Mqtt general
const char* MQTT_SERVER             = "hassbian.local";
const int MQTT_PORT                 = 1883;
const char* MQTT_ID                 = "fstakem98765";
const char* MQTT_USER               = "fstakem";
const char* MQTT_PASSWORD           = "password";

// Mqtt topics
const String MQTT_NODE_NAME         = "wemos_node/1";
const int NUM_OF_SENSORS            = 5;
const int mqtt_max_conn_attempts    = 3;

String get_data_topics[NUM_OF_SENSORS];
String set_tx_topics[NUM_OF_SENSORS];
String set_tx_rate_ms_topics[NUM_OF_SENSORS];
boolean sensor_tx_state[NUM_OF_SENSORS];

// Loop
unsigned long last_loop_times[NUM_OF_SENSORS];
int loop_times[NUM_OF_SENSORS];

// Buffers
char tx_buff[100];
char rx_buff[100];
char topic_buff[200];


// Globals
WiFiClient wifi_client;
PubSubClient client(wifi_client);


void handle_msg(char* topic, byte* payload, unsigned int length);

void create_topics() {
    int i = 0;

    for(i = 0; i < NUM_OF_SENSORS; i++) {
        get_data_topics[i]          = MQTT_NODE_NAME + "/sensor/" + String(i) + "/data";
        set_tx_topics[i]            = MQTT_NODE_NAME + "/sensor/" + String(i) + "/tx";
        set_tx_rate_ms_topics[i]    = MQTT_NODE_NAME + "/sensor/" + String(i) + "/tx_rate_ms";
        sensor_tx_state[i]          = false;
        last_loop_times[i]          = 0;
        loop_times[i]               = 5000;
    }

    sensor_tx_state[0] = true;
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, SSID_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void mqtt_connect() {
    int mqtt_conn_attempts = 0;

    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        mqtt_conn_attempts += 1;
        
        if (client.connect(MQTT_ID, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }

        if(mqtt_conn_attempts >= mqtt_max_conn_attempts) {
            break;
        }
    }
}

void mqtt_subscribe() {
    int i;

    for(i = 0; i < NUM_OF_SENSORS; i++) {
        set_tx_rate_ms_topics[i].toCharArray(topic_buff, set_tx_rate_ms_topics[i].length()+1);
        client.subscribe(topic_buff);

        set_tx_topics[i].toCharArray(topic_buff, set_tx_topics[i].length()+1);
        client.subscribe(topic_buff);
    }
}

void handle_msg(char* topic, byte* payload, unsigned int length) {
    int i = 0;

    for(i = 0; i < length; i++) {
        rx_buff[i] = payload[i];
    }
    rx_buff[i] = '\0';

    String topic_str = String(topic);
    String msg_str = String(rx_buff);

    Serial.println("Message arrived:  topic: " + topic_str);
    Serial.println("Length: " + String(length, DEC));
    Serial.println("Payload: " + msg_str);

    for(i = 0; i < NUM_OF_SENSORS; i++) {
        if (topic_str.equals(set_tx_rate_ms_topics[i])) {
            handle_set_tx_rate_ms(i, msg_str);
        }

        if (topic_str.equals(set_tx_topics[i])) {
            handle_set_tx_topics(i, msg_str);
        }
    }
}

void handle_set_tx_rate_ms(int sensor_id, String msg) {
    int tx_rate = msg.toInt();

    if (tx_rate >= 100 && tx_rate < 60000) {
        loop_times[sensor_id] = tx_rate;
        Serial.println("Changed sensor(" + String(sensor_id) + ") transmission rate to: " + msg);
    } else {
        Serial.println("Error: Incorrect sensor(" + String(sensor_id) +  ") transmission rate :: " + msg);
        Serial.println("Transmission rate must be between 100 and 60000 ms");
    }
}

void handle_set_tx_topics(int sensor_id, String msg) {
    if (msg.equals("on")) {
        sensor_tx_state[sensor_id] = true;
        Serial.println("Changing state to transmitting");
    } else if (msg.equals("off")) {
        sensor_tx_state[sensor_id] = false;
        Serial.println("Changing state to not transmitting");
    } else {
        Serial.println("Error: Unknown state :: " + msg);
    }
}

void handle_state() {
    int i = 0;

    for(i = 0; i < NUM_OF_SENSORS; i++) {
        if (millis() > (loop_times[i] + last_loop_times[i])) {
            last_loop_times[i] = millis();

            if (sensor_tx_state[i]) {
                get_sensor_data(i);
                transmit_sensor_data(i);
            }
        }
    }
}

void get_sensor_data(int sensor_id) {
    long value = get_sine_wave();
    String data = MQTT_NODE_NAME + "/sensor/" + String(sensor_id) + "/" + String(value);
    data.toCharArray(tx_buff, data.length()+1);
}

void transmit_sensor_data(int sensor_id) {
    String data_topic = get_data_topics[sensor_id];
    data_topic.toCharArray(topic_buff, data_topic.length()+1);
    client.publish(topic_buff, tx_buff);
}

long get_sine_wave() {
    static const unsigned long step_time = 1000;
    static const float step_angle_rads = 0.261799f;
    static unsigned long last_sample_time = millis();
    static float current_angle_rads = 0.0f;
    static long current_noise = random(15);

    unsigned long current_time = millis();

    if(current_time - last_sample_time > step_time) {
        last_sample_time = current_time;
        current_angle_rads = current_angle_rads + step_angle_rads;
        current_noise = random(15);
    }

    float sine_value = sin(current_angle_rads) * 100;
    long value = (long) sine_value + current_noise;

    return value;
}

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
    Serial.begin(9600);
    setup_wifi();
    Serial.println("Setting mqtt server: " + String(MQTT_SERVER));
    Serial.println("Setting mqtt port: " + String(MQTT_PORT));
    delay(10000);

    IPAddress server(192,168,2,207);
    client.setServer(server, MQTT_PORT);
    client.setCallback(handle_msg);
    create_topics();
}

void loop() {
    if (!client.connected()) {
        mqtt_connect();
        mqtt_subscribe();
    }

    handle_state();
    client.loop();
}


