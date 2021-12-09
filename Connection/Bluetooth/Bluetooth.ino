#include <SoftwareSerial.h>
#define rxPin D2 // Broche D2 en tant que RX, à raccorder sur TX du HC-05
#define txPin D3 // Broche D3 en tant que TX, à raccorder sur RX du HC-05
#define keyPin D4 // Broche D4 en tant que key, à raccorder sur Key du HC-05
SoftwareSerial BTSerial(rxPin, txPin);
void setup()
{
    // define pin modes for tx, rx pins:
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);

    BTSerial.begin(9600);
    Serial.begin(9600); 

    // setup key pin
    pinMode(keyPin, OUTPUT);
    digitalWrite(keyPin, LOW); // key pin 
    
    delay(10000); //10s

    // digitalWrite(keyPin, HIGH);


    //if(BTSerial.write("AT") == "OK"){
        // BTSerial.write("AT+NAME=Ledz Go");
        // BTSerial.write("AT+ROLE=1");
        // BTSerial.write("AT+RESET");
        // BTSerial.write("AT+INIT");
    //}

}

void loop()
{
  // Keep reading from HC-05 and send to Arduino 
  // Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor 
  //  and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());
}
