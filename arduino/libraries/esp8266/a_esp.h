#ifndef A_ESP
#define A_ESP

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

class ESP8266_v1 {
  long m_startTime;
  bool m_gAPRunning;

  struct Config {
    char sign[25];
    char ssid[128];
    char password[128];     
  } m_config;

  void save_config_and_reset(); 
protected:
  Print* m_pCon;
  const char* m_title;
  ESP8266WebServer m_wServer;    
  void webServerSetup();
public:
  ESP8266_v1(const char* title, Print* pCon, int wsPort);
  void setup();
  void loop();
};

#endif