#ifndef A_ESP_EXT
#define A_ESP_EXT

/*
Назначение: реализует расширеную работу модуля ESP82XX
Версия: 2.0
Создан: 03.10.2019
Изменен:
Автор: Куликовских Сергей
E-mail: kylikovskix@gmail.com 
*/

#include "a_esp.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <HardwareSerial.h>

class ESP8266Ext: public ESP8266_v1 {
protected:
  WiFiServer m_eServer; 
  WiFiClient m_eClient; 
  boolean m_clientAlreadyConnected;
  HardwareSerial* m_pSerial;
public:
  ESP8266Ext(const char* title, Print* pCon, HardwareSerial* pSerial, int wsPort, int ePort);
  void setup();
  void loop();
};

#endif