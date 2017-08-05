#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <Wire.h>


// Wifi
const char* SSID                    = "frugmunster";
const char* SSID_PASSWORD           = "xxx";

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

void handle_state() {
    int i = 0;

    for(i = 0; i < NUM_OF_SENSORS; i++) {
        if (millis() > (loop_times[i] + last_loop_times[i])) {
            last_loop_times[i] = millis();

            if (sensor_tx_state[i]) {
                // TODO
            }
        }
    }
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

    handle_state();
}


