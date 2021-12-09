#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
unsigned long horloge;
DynamicJsonDocument functionActuel(1024);
DynamicJsonDocument functionRun(1024);
//initialization as mode terminate and waiting for the message Discovery to start, 0 for run,1 for discovery, 2 for terminate
int masterMode=2;
//maintenant tout est en Serial et on va le remplacer après

void runFunctionActuel(){
  Serial.println("run function");
  if(functionActuel[0]==null){
    horloge=millis();
    copyArray(functionActuel,functionRun);
  }
  if(millis()-horloge>=functionRun[0]["time"]){
    //sendRGB(funcionRun[0]);
    functionActuel.remove(0);
  }
}
void treatJson(DynamicJsonDocument json){
  if(json["Data"]["Type"]=="Function"){
      Serial.println("Run Function");
      //restart horloge
      horloge=millis();
    }else if(json["Data"]["Type"]=="Command"){
      Serial.println("DMX Command");
      int RGB[]={json["Data"]["Channels"][0],json["Data"]["Channels"][1],json["Data"]["Channels"][2],json["Data"]["Channels"][3]};
      Serial.println(RGB[0]);
      Serial.println(RGB[1]);
      Serial.println(RGB[2]);
      Serial.println(RGB[3]);
    }else if(json["Data"]["Type"]=="Terminate"){
      masterMode=2;
      Serial.println("Terminate Mode");
    }else if(json["Data"]["Type"]=="Discovery"){
      masterMode=1;
      Serial.println("Discovery Mode");
    }else{
      Serial.println("Wrong Type of JSON");
    }
}

void setup()
{
  Serial.begin(115200);  
}

void loop() {
  //maintenant j'ai mis Serial mais ici ça devais remplacé par le reception BlueTooth
  if(Serial.available()){
    String read;
    DynamicJsonDocument json(1024);
    read=Serial.readString();
    deserializeJson(json,read);
    treatJson(json);
    
  }else{
    runFunctionActuel();
    delay(1000);
  }
  
}
