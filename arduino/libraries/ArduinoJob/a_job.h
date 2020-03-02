#ifndef A_JOB
#define A_JOB

#include <stream.h>

class AJobManager;

class AJob {
  friend class AJobManager;
private:
  AJobManager* m_pAJobManager;   
  AJob* mJobNext;            
  unsigned long m_counter;
  unsigned long m_previousMillis; 
  unsigned long m_currentMillis; 
  unsigned long m_delayMillis;
  void run();
public:
  AJob(AJobManager*, unsigned long delay = 0L);
  ~AJob();
                  
  void finish();  
  long counter(); // Счетчик вызовов метода onRun
  // setDelay устанавливает интервал вызовова onRun (в милисекундах)
  long setDelay(unsigned long);
  unsigned long previousMillis();
  unsigned long currentMillis();
                              
  // onInit 
  virtual void onInit();  
  virtual void onRun();   
  // onDone - вызвается при завершении задачи с помощью метода finish
  virtual void onDone();  
};

class AJobManager {
  friend class AJob;
  AJob* mJobFirst;
  void attach(AJob*);   // добавить задачу в список
  void deattach(AJob*); // удалить задачу из списка
  void deattachAll();   // удалить все задачи
public:  
  AJobManager();
  ~AJobManager();
  void setup();
  void loop();
};

class AJob_Test: public AJob {
  unsigned int m_ID;
  Print* m_pCon;
public:  
  void onInit();  
  void onRun();   
  void onDone();
  AJob_Test(AJobManager* manager, unsigned int id, Print* pCon, unsigned long delay = 0);  
};

void ArduinoJob_Test(const int N, Print* pCon);

#endif // A_JOB