#ifndef G_CP_JOB_H
#define G_CP_JOB_H

#include "a_job.h"
#include "a_geksa.h"
#include <HardwareSerial.h>



#define SERIAL_BUFF_SIZE  64  // максимальный размер буфера для хранения команды
#define ARGS_MAX_COUNT  25    // максимальное число параметров в команде

/*
* Класс: CommandProcessorJob
* Версия: 1.0
* Дата создания: 10.09.2019
* Дата изменения: 
* Автор: Куликовских Сергей
* Email: kylikovskix@gmail.com
* Описание: транслирует текстовую команду заданного формата 
* в вызов соответствующего публичного метода указанного экземпляра класса Geksa
*/

// class CommandProcessorJob реализует обработку команд,
// поступающих через UART (Serial)
class CommandProcessorJob : AJob {
  Geksapod* m_pGeksapod;
  HardwareSerial* m_pInSerial;
  HardwareSerial* m_pOutSerial;
  char buff[SERIAL_BUFF_SIZE+1];
  char* args[ARGS_MAX_COUNT];
  char id;
  char count;
  void clear();

int cmdReset(const char* args[], int count);
int cmdMove(const char* args[], int count);
int cmdRotate(const char* args[], int count);
int cmdStop(const char* args[], int count);
int cmdPose(const char* args[], int count);
int cmdCoxa(const char* args[], int count);
int cmdFemora(const char* args[], int count);
int cmdTibia(const char* args[], int count);
int cmdCalc(const char* args[], int count);

void printPose(int i, int ca, int fa, int ta);

public:
  CommandProcessorJob(Geksapod* pGeksapod, HardwareSerial* pInSerial, HardwareSerial* pOutSerial);
  void onRun();
};

#endif
