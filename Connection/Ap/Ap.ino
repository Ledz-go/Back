#include <ESP8266WiFi.h>

String ssid = "ledzGO";
String psk = "password";
boolean hidden = true;
int channel = 10;
int max_connection = 8;

void setup(){
    Serial.begin(115200);
    Serial.println("LedzGo master node start");

    Serial.println("Starting AP");
    Serial.println(WiFi.softAP(ssid, psk, channel, hidden, max_connection) ? "AP ready" : "AP failed");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
}

void loop(){

}
