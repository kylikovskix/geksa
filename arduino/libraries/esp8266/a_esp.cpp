#include "a_esp.h"
#include "html_content.h"

#define WIFI_CONNECT_TRY_COUNT  10
#define WIFI_CONNECT_TIMEOUT    10000
#define AP_LIFE_TIME            300000

const char SIGN[] = "esp_86_xx_kulibin_v1";
const char DEF_PASSWORD[] = "88888888";

void(* resetFunc) (void) = 0; 
     
ESP8266_v1::ESP8266_v1(const char* title, Print* pCon, int portNumber):m_wServer(portNumber) {
  m_title = title;   
  m_pCon = pCon;
  m_startTime = 0;
  m_gAPRunning = true;
  memset(&m_config,0,sizeof(m_config));
}

void ESP8266_v1::setup() {
  if(m_pCon != NULL) {  
    m_pCon->println("-- ESP Module Start -- ");
  }

  EEPROM.begin(sizeof(m_config));
  EEPROM.get(0,m_config);
  if(strncmp(m_config.sign,SIGN,strlen(SIGN)) != 0) {
    memset(&m_config,0,sizeof(m_config));
    strncpy(m_config.sign,SIGN,strlen(SIGN));
    save_config_and_reset();
  }
  EEPROM.end();

  WiFi.begin(m_config.ssid, m_config.password);
  if(m_pCon != NULL) {
    m_pCon->print("-- Connect to ");
    m_pCon->print(m_config.ssid);
    m_pCon->print("...");
  }   
  
  delay(WIFI_CONNECT_TIMEOUT);

  if(m_pCon == NULL) {
  } else if(WiFi.status() == WL_CONNECTED) {
    m_pCon->print("Ok, IP=");  
    m_pCon->println(WiFi.localIP());
  } else { 
    m_pCon->println("Failed");
  }

  byte mac[6];
  WiFi.macAddress(mac);
  sprintf(m_config.ssid,"%s_%X_%X_%X_%X_%X_%X",m_title,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  sprintf(m_config.password,DEF_PASSWORD);
                   
  if(m_pCon != NULL) {
    m_pCon->print("-- Access Point: "); 
    m_pCon->print(m_config.ssid);
  }  

  WiFi.enableAP(true); 
  delay(1000);
  
  if(m_pCon != NULL) {     
    m_pCon->println(WiFi.softAP(m_config.ssid,m_config.password) ? " is ready" : " is failed");
  }  

  delay(1000);
  m_startTime = millis();
  webServerSetup();
}

void ESP8266_v1::webServerSetup() {
  m_wServer.on("/", [this](){
    m_wServer.send(200, "text/html", "Hello!");
  });   
  
  m_wServer.on("/setup", [this](){
    if(m_wServer.args() > 0) {
      String new_ssid = m_wServer.arg("ssid");
      String new_password = m_wServer.arg("npass");
      String current_password = m_wServer.arg("cpass");
      
      if(current_password.compareTo(m_config.password) == 0) {
        strncpy(m_config.sign,SIGN,strlen(SIGN));
        new_ssid.toCharArray(m_config.ssid, sizeof(m_config.ssid));
        new_password.toCharArray(m_config.password, sizeof(m_config.password));
         
        if(m_pCon != NULL) 
          m_pCon->println("new ssid: " + new_ssid);
                
        save_config_and_reset();
      } else 
        m_wServer.send(200, "text/html", "Access denied!");
    }
    m_wServer.send(200, "text/html", SETUP_HTML_CONTENT);
  });
  
  m_wServer.begin();
  
  if(m_pCon != NULL) 
    m_pCon->println("-- HTTP server started");
}

void ESP8266_v1::loop() {
  m_wServer.handleClient();
  if(m_gAPRunning /*&& millis() - m_startTime > AP_LIFE_TIME*/) {
    WiFi.softAPdisconnect();     
    m_gAPRunning = false;
    if(m_pCon != NULL) {
      m_pCon->println("-- Access point closed");
    }
  }
}

void ESP8266_v1::save_config_and_reset() {
    EEPROM.begin(sizeof(m_config));
    EEPROM.put(0,m_config);
    EEPROM.end();
    if(m_pCon != NULL) {
      m_pCon->println("-- Save config: ");
      m_pCon->print(" SSID=");
      m_pCon->print(m_config.ssid);
      m_pCon->print(" -- PASSWORD= ");
      m_pCon->print(m_config.password);
      m_pCon->println(" -- Reset");
    }
    resetFunc();
}