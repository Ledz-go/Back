#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESPDMX.h>

// Wifi vars
String ssid = "ledzGO";
String psk = "password"; // Wifi password
boolean hidden = true;  // Hide wifi ssid
int channel = 10; // Wifi channel
int max_connection = 8;

// Bluetooth vars
#define rxPin D2 // Broche D2 en tant que RX, à raccorder sur TX du HC-05
#define txPin D3 // Broche D3 en tant que TX, à raccorder sur RX du HC-05
SoftwareSerial BTSerial(rxPin, txPin);

// DMX vars
DMXESPSerial dmx;

void setup(){
    Serial.begin(9600);
    Serial.println("LedzGo master node start");
    

    // Wifi setup
    Serial.println("Starting AP");
    Serial.println(WiFi.softAP(ssid, psk, channel, hidden, max_connection) ? "AP ready" : "AP failed");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());

    // Bluetooth setup

    // define pin modes for tx, rx pins
    Serial.println("Bluetooth started");
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);

    BTSerial.begin(9600);

    // DMX setup
    dmx.init(512);

    // To use :
    // dmx.write(channel, value) for on off channel
    // dmx.write() for update dmx bus
    
}

void loop(){

    // Bluetooth transmission on serial
    if (BTSerial.available())
        Serial.write(BTSerial.read());

    if (Serial.available())
        BTSerial.write(Serial.read());

}

