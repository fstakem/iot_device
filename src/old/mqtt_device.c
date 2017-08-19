#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "IotDevice.h"

using namespace IotDevice;


// Wifi
const char* SSID                    = "frugmunster";
const char* SSID_PASSWORD           = "----";

// Loop
const unsigned long loop_delay      = 100;
const int display_interval          = 5000;
unsigned long last_record_time      = 0;


// Globals
WiFiClient wifi_client;
IotDevice::Device device;
IotDevice::SignalGenerator generator;


// Device code
// ----------------------------------------------------------------------------------------
void show_data() {
    unsigned long current_time = millis();
    
    if (current_time - last_record_time  > display_interval) {
        IotDevice::show_data(&device);
        last_record_time = current_time;
    }
}


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
    device = IotDevice::init_device(1, "simple");
    generator = IotDevice::init_signal_generator();
    delay(10000);

    IPAddress server(192,168,2,207);
}

void loop() {
    IotDevice::get_fake_data(&device, &generator);
    show_data();
    delay(loop_delay);
}


