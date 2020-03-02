#ifndef A_JOB
#define A_JOB

#include <stream.h>

class AJobManager;

class AJob {
  friend class AJobManager;
private:
  AJobManager* m_pAJobManager;   
  AJob* mJobNext;            
  unsigned long m_counter;         // Счетчик вызовов метода onRun
  unsigned long m_previousMillis; // метка времени предыдущего вызова onRun
  unsigned long m_currentMillis;  // текущая метка времени
  unsigned long m_delayMillis;    // интервал вызова метода onRun
  void run();
protected:
  virtual void onInit();  // вызывается один раз для инициализации задачи
  virtual void onRun();   // вызывается многократно в процессе работы
  virtual void onDone();  // вызывается при завершении задачи с помощью метода finish
public:
  AJob(AJobManager*, unsigned long delay = 0L);
  ~AJob();
                  
  void finish();  // принудительное завершение задачи  
  long counter(); // возвращает количество вызовов onRun с момента старта задачи
  long setDelay(unsigned long); // устанавливает интервал вызова onRun
  unsigned long previousMillis();// возвращает метку времени предыдущего вызова onRun
  unsigned long currentMillis(); // возвращает текущую метку времени
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
