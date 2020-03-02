#include "a_esp_v2.h"

ESP8266Ext::ESP8266Ext(const char* title, Print* pCon, HardwareSerial* pSerial, int wNumber, int ePort):ESP8266_v1(title,pCon,wNumber), m_eServer(ePort) {
  m_pSerial = pSerial;
  m_clientAlreadyConnected = false;
}

void ESP8266Ext::setup() {
  ESP8266_v1::setup();
  
  m_eServer.begin();
  m_eServer.setNoDelay(true);
  if(m_pCon != NULL)
    m_pCon->println("-- TCP server started");
}

void ESP8266Ext::loop() {
  ESP8266_v1::loop();

  if(!m_clientAlreadyConnected) {
    m_eClient = m_eServer.available();
    if(m_eClient == true && m_pCon != NULL) 
      m_pCon->println("client connected");
  }

  if(m_eClient.connected()) {
    // есть данные от клиента
    if (m_clientAlreadyConnected == false) {
      m_eClient.println("Server ready"); // ответ клиенту
      m_clientAlreadyConnected= true; 
    }

    if (m_eClient.available()) {
      size_t len = m_eClient.available();
      uint8_t sbuf[len+1]; 
      m_eClient.readBytes(sbuf, len);
      //push UART data to all connected telnet clients
       if(m_pSerial != NULL)
        m_pSerial->write(sbuf, len);
       delay(1);
    }

    //check UART for data
    if (m_pSerial != NULL && m_pSerial->available()) {
      size_t len = m_pSerial->available();
      uint8_t sbuf[len];
      m_pSerial->readBytes(sbuf, len);
      //push UART data to all connected telnet clients
      if (m_eClient && m_eClient.connected()) {
        m_eClient.write(sbuf, len);
        delay(1);
      }
    }
    
  } else
    m_clientAlreadyConnected = false;
}