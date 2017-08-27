#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "Sample.h";
#include "Device.h"
#include "Transmitter.h"
#include "SineWave.h"
//#include "MqttClient.h"

#define STEP_TIME 2000
#define STEP_ANGLE_RADS 0.261799f


// Wifi
const char* SSID                    = "frugmunster";
const char* SSID_PASSWORD           = "----";

// Loop
const unsigned long loop_delay      = 3000;
const int display_interval          = 5000;
unsigned long last_record_time      = 0;

// Other
const int MQTT_PORT                 = 1883;


// Globals
WiFiClient wifi_client;
Device device = Device(1, "device_1");
SamplePtr *samples;


// Generic code
// ----------------------------------------------------------------------------------------
void setupWifi() {
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
    setupWifi();
    delay(10000);

    IPAddress server(192,168,2,207);
    MqttClient mqtt_client = MqttClient(&device, wifi_client, server, MQTT_PORT);
    device.createSensor(new SineWave(STEP_TIME, STEP_ANGLE_RADS), new Transmitter());
}

void showSamples() {
    int i;

    for (i = 0; i < device.getNumSensors(); i++) {
        Serial.println(samples[i]->timestamp);
    }
}

void loop() {
    delay(loop_delay);
    device.getSamples(samples);
    //device.showSamples();
    showSamples();
}

