#include <ESP8266WiFi.h>
#include "coap_client.h"

// Wifi
const char* SSID                    = "frugmunster";
const char* SSID_PASSWORD           = "xxx";

// Sensors
const int NUM_OF_SENSORS            = 5;

// Coap
coapClient coap;
const int COAP_PORT                 = 5683;

// Urls
const String ROOT_URL               = "wemos-node/1";
String data_urls[NUM_OF_SENSORS];

// Loop
unsigned long last_loop_times[NUM_OF_SENSORS];
int loop_times[NUM_OF_SENSORS];
boolean tx_states[NUM_OF_SENSORS];

// Buffers
char tx_buff[100];
char rx_buff[100];
char url_buff[200];

// Network
IPAddress server(129,132,15,80);
WiFiClient wifi_client;
int port = 5683;


void coap_response_handler(coapPacket &packet, IPAddress ip, int port);

void coap_response_handler(coapPacket &packet, IPAddress ip, int port) {
    char p[packet.payloadlen + 1];
    memcpy(p, packet.payload, packet.payloadlen);
    p[packet.payloadlen] = NULL;

    if(packet.type==3 && packet.code==0){
      Serial.println("ping ok");
    }

    Serial.println(p);
}

void handle_state() {
    int i = 0;

    for(i = 0; i < NUM_OF_SENSORS; i++) {
        if (millis() > (loop_times[i] + last_loop_times[i])) {
            last_loop_times[i] = millis();

            if (sensor_tx_state[i]) {
                transmit_sensor_data(i);
            }
        }
    }
}

void transmit_sensor_data(int sensor_id) {
    long value = get_sine_wave();
    
    String url = data_urls[sensor_id]
    url.toCharArray(url_buff, url.length()+1);
    String data = String(value);
    data.toCharArray(tx_buff, data.length()+1);
    int msgid = coap.put(server, port, url, data, data.length()+1);
    Serial.println(msgid);
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

void create_urls() {
    int i = 0;

    for(i = 0; i < NUM_OF_SENSORS; i++) {
        data_urls[i]                = ROOT_URL+ "/sensor/" + String(i+1) + "/data";
        tx_states[i]                = false;
        last_loop_times[i]          = 0;
        loop_times[i]               = 5000;
    }

    tx_states[0] = true;
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

void setup() {
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
    Serial.begin(9600);
    setup_wifi();
    delay(10000);

    create_urls();
    coap.response(coap_response_handler);
    coap.start();
}

void loop() {
    handle_state();
    bool state = coap.loop();
}