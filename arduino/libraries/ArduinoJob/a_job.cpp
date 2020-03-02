#include <a_job.h>
#include <Arduino.h>

void AJob::onInit() {}
void AJob::onRun() {}
void AJob::onDone() {}

void AJob::run() {
  unsigned long currentMillis = ::millis();
  if(currentMillis - m_currentMillis >= m_delayMillis) {
    m_previousMillis = m_currentMillis;
    m_currentMillis = currentMillis;    
    m_counter++;
    onRun();                                           
  }
}

long AJob::counter() {
  return m_counter;
}

long AJob::setDelay(unsigned long delayMillis) {
  m_delayMillis = delayMillis;
}

unsigned long AJob::previousMillis() { 
  return m_previousMillis; 
}

unsigned long AJob::currentMillis() { 
  return m_currentMillis; 
}

void AJob::finish() {
  onDone();
  if(m_pAJobManager != NULL) 
    m_pAJobManager->deattach(this);
}

AJob::AJob(AJobManager* pAJobManager, unsigned long delay) {
  m_counter = 0L;
  m_previousMillis = -1L;
  m_currentMillis = 0L;
  m_delayMillis = delay;  
  mJobNext = NULL;
  m_pAJobManager = pAJobManager;     
  if(m_pAJobManager != NULL) { 
    m_pAJobManager->attach(this);
  } 
}

AJob::~AJob() {
  if(m_pAJobManager != NULL) 
    m_pAJobManager->deattach(this);
}

void AJobManager::setup() {
  AJob* pItem = mJobFirst;
  while(pItem != NULL) {
    pItem->onInit();
    pItem = pItem->mJobNext;
  }
}

void AJobManager::loop() {
  AJob* pItem = mJobFirst;
  while(pItem != NULL) {
    pItem->run();
    pItem = pItem->mJobNext;
  }
}

AJobManager::AJobManager() {
  mJobFirst = NULL;  
}

AJobManager::~AJobManager() {
  deattachAll();
}

void AJobManager::attach(AJob* pJob) {
  if(pJob != NULL) {
    deattach(pJob);
    pJob->mJobNext = mJobFirst;
    mJobFirst = pJob;
  }
}

void AJobManager::deattach(AJob* pJob) {
  AJob* pLast = NULL;
  AJob* pItem = mJobFirst;
  if (pJob != NULL) { 
    while(pItem != NULL) {
      if(pItem != pJob) {
        pLast = pItem;
      } else if(pLast == NULL) {
        mJobFirst = pItem->mJobNext;   
      } else {
        pLast->mJobNext = pItem->mJobNext;
      } 
      pItem = pItem->mJobNext;    
    }                
  }
}

void AJobManager::deattachAll() {
  AJob* pItem = mJobFirst;
  while(pItem != NULL) {
    mJobFirst = pItem->mJobNext;
    pItem->mJobNext = NULL;
  }
}

AJob_Test::AJob_Test(AJobManager* manager, unsigned int id, Print* pCon, unsigned long delay) :AJob(manager, delay) {
  m_ID = id;
  m_pCon = pCon;
}

void AJob_Test::onInit() {
  if(m_pCon != NULL) {
    m_pCon->print("Job #");
    m_pCon->print(m_ID);
    m_pCon->println(": onInit");    
  }
} 
 
void AJob_Test::onRun() {
  if(m_pCon != NULL) {
    m_pCon->print("Job #");
    m_pCon->print(m_ID);
    m_pCon->print(": onRun");    
    m_pCon->print(", counter = ");
    m_pCon->print(counter());    
    m_pCon->print(", current millis:  ");
    m_pCon->println(millis());    
    
  }
} 
  
void AJob_Test::onDone() {
  if(m_pCon != NULL) {
    m_pCon->print("Job #");
    m_pCon->print(m_ID);
    m_pCon->println(": onDone");    
  }
}  


void ArduinoJob_Test(const int N, Print* pCon) {
  AJobManager manager;  
  AJob_Test* ppJob[N];  // ��������� ������ �����
                          
  // ������� ������ 
  for(int i=0; i < N; i++)
    ppJob[i] = new AJob_Test(&manager, i+1, pCon, (unsigned long)(i*50));
    
  manager.setup();   
      
  // ��������� ������ ��������� ����� 
  for(int i=0; i < 100; i++)
    manager.loop();         
      
  // ���������� ���� ������ 
  // ������������� ������ ���� ������ AJobManager::deattach()
  if(N > 0)
    ppJob[0]->finish();
    
  // ��������� ������ ��������� ����� ��� ���
  for(int i=0; i < 100; i++) {
    manager.loop();           
    delay(10);
  }
    
  // ����� ����������� ������ 
  // ���� ������ ������������� ������� AJobManager::deattach()
  if(N > 1) {
    delete ppJob[1];
    ppJob[1] = NULL;      
  }
      
  // ��� ��� ��������� ������ ��������� �����
  for(int i=0; i < 100; i++) {
    manager.loop();           
    delay(10);
  }
                          
  // ������������� ��������� ��� ������ �� ������
  //manager.deattachAll();       
        
  // ������� ������
  for(int i=0; i < N; i++) {
    if(ppJob[i] != NULL)
      delete ppJob[i];         
  }
      
  if(pCon != NULL) 
    pCon->println("ArduinoJob_Test is succedded");
}
