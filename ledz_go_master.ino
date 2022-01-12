#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>

//bluetooth
#include <SoftwareSerial.h>
#define rxPin D2 // Broche D2 en tant que RX, à raccorder sur TX du HC-05
#define txPin D3 // Broche D3 en tant que TX, à raccorder sur RX du HC-05
#define keyPin D4 // Broche D4 en tant que key, à raccorder sur Key du HC-05
SoftwareSerial BTSerial(rxPin, txPin);



unsigned long horloge;
DynamicJsonDocument functionActuel(1024);
DynamicJsonDocument functionRun(1024);
DynamicJsonDocument receivedData(1024);
DynamicJsonDocument nodes(1024);

//initialization as mode terminate and waiting for the message Discovery to start, 0 for run,1 for discovery, 2 for terminate
int masterMode=1;
//definition wifi
String ssid = "ledzGO";
String psk = "password";

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
IPAddress Broadcast(192,168,4,255);
IPAddress dest;
unsigned int localPort = 8888;
boolean hidden = false;
int channel = 10;
int max_connection = 8;
WiFiUDP Udp;



// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acked\r\n";       // a string to send back
char testData[]="{\"Data\": {\"Type\": \"Command\",\"NodeID\": 2354,\"Channels\": [ 125, 0, 62, 214 ]}}";
//maintenant tout est en Serial et on va le remplacer après
void sendCommand(DynamicJsonDocument json){
  if(json["Data"]["NodeID"]==WiFi.macAddress().c_str()){
    //DMX show
    Serial.println("DMX Command");
    int RGB[]={json["Data"]["Channels"][0],json["Data"]["Channels"][1],json["Data"]["Channels"][2],json["Data"]["Channels"][3]};
    Serial.println(RGB[0]);
    Serial.println(RGB[1]);
    Serial.println(RGB[2]);
    Serial.println(RGB[3]);
  }else{
    //Udp.beginPacket(nodes[receivedData["Data"]["NodeID"]], localPort);
    uint8_t ipaddress_1,ipaddress_2,ipaddress_3,ipaddress_4;
    String ipDest=nodes["1"];
    dest.fromString(ipDest);
    Udp.beginPacket(dest, localPort);
    String commandSlave;
    serializeJson(json,commandSlave);
    Serial.println(commandSlave);
    Udp.println(commandSlave);
    Udp.endPacket();
  }
}
void runFunctionActuel(){
  Serial.println("run function");
  if(functionActuel["Function"][0]==NULL){
    horloge=millis();
    functionRun["Function"]=functionActuel["Function"];
  }
  if(millis()-horloge>=functionRun["Function"][0]["time"]){
    DynamicJsonDocument command(1024);
    DynamicJsonDocument commandJson(1024);
    command["NodeID"]=functionRun["Function"][0]["NodeID"];
    command["Channels"]=functionRun["Function"][0]["Channels"];
    commandJson["Data"]=command;
    sendCommand(commandJson);
    functionActuel.remove(0);
  }
}

void treatJson(DynamicJsonDocument json){
  if(json["Data"]["Type"]=="Function"){
    Serial.println("Run Function");
    //restart horloge
    horloge=millis();
    functionActuel["Function"]=json["Data"]["Sequence"];
    functionRun["Function"]=json["Data"]["Sequence"];
    runFunctionActuel();
  }else if(json["Data"]["Type"]=="Command"){
    sendCommand(json);
  }else if(json["Data"]["Type"]=="Terminate"){
    masterMode=2;
    Serial.println("Terminate Mode");
  }else{
    Serial.println("Wrong Type of JSON");
  }
}

void setup()
{
  Serial.begin(9600);  
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.println(WiFi.softAP(ssid, psk, channel, hidden, max_connection) ? "AP ready" : "AP failed");
  Udp.begin(localPort);
  Serial.println("Begin");
  nodes[WiFi.macAddress().c_str()]=WiFi.localIP().toString().c_str();

  //bluetooth
  
  // define pin modes for tx, rx pins:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  BTSerial.begin(9600);
  Serial.begin(9600); 

  // setup key pin
  pinMode(keyPin, OUTPUT);
  digitalWrite(keyPin, LOW); // key pin
  
  horloge=millis();
}

void loop() {
  if(masterMode==2){
    delay(1000);
  }else if(masterMode==0){

    //bluetooth
    
    if (BTSerial.available()){
      deserializeJson(receivedData,BTSerial.readString());
      treatJson(receivedData);
      delay(1000);
    }
  }else{
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      Serial.println("DiscoveryMode");
      int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
      packetBuffer[n] = 0;
      Serial.println(packetBuffer);
      char IP[16];
      char Mac[16];
      sscanf(packetBuffer,"%s %s",IP,Mac);
      
      
      Serial.println(IP);
      Serial.println(Mac);
      nodes[Mac]=IP;
    }
    if(millis()-horloge>20000){
      masterMode=0;
      Serial.println("Run mode");
    }
  }
}
