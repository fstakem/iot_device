#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <Wire.h>


// Wifi
const char* SSID                    = "iot_lab";
const char* SSID_PASSWORD           = "***";

// Loop
const unsigned long loop_delay      = 1000;

// Globals
WiFiClient wifi_client;


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
    delay(10000);

    IPAddress server(192,168,2,207);
}

void loop() {
    long current_time = millis();
    long sensor_data = get_sine_wave();

    Serial.print(current_time);
    Serial.print(": ");
    Serial.println(sensor_data);

    delay(loop_delay);
}


