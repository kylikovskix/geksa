#include <a_esp_v2.h>
#include <a_job.h>
#include <CmdProc.h>

ESP8266Ext esp("GeksaESP",&Serial1,&Serial,80,2000);

void setup(void){
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(500);
  esp.setup();
}

void loop(void){
  esp.loop();
}
