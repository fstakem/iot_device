#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "TestDevice.h"


// Wifi
const char* SSID                    = "frugmunster";
const char* SSID_PASSWORD           = "-";

// Loop
const unsigned long loop_delay      = 3000;
const int display_interval          = 5000;
unsigned long last_record_time      = 0;


// Globals
WiFiClient wifi_client;
TestDevice device = TestDevice(1, "device_1");
int num_sensors;


// Generic code
// ----------------------------------------------------------------------------------------
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

    num_sensors = device.getNumSensors();
    IPAddress server(192,168,2,207);
}

void loop() {
    delay(loop_delay);
    device.getSamples();
    device.showSamples();
}

