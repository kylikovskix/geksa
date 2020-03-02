#ifndef CMDPROC_H
#define CMDPROC_H

/*
* Библиотека: CommandProcessor
* Версия: 1.0
* Дата создания: 10.09.2019
* Дата изменения: 
* Автор: Куликовских Сергей
* Email: kylikovskix@gmail.com
* Описание: Библиотека CommandProcessor предназначена 
* для организации вызова програмного кода посредством тектовых команд
*/

#include <HardwareSerial.h>

#define BUFF_SIZE  255   // размер буфер для ввода текста
#define CMD_SIZE  255   // размер командного буфера
#define MAX_ARG_COUNT 30 // максимальное количество аргументов для команды

// формат обработчика команды
typedef int CommandHandler(const char* args[MAX_ARG_COUNT], int count);

class CommandProcessor;

// элемент списка для хранения зарегистрированных команд и их обработчиков
class CommandItem {         
  friend class CommandProcessor; 
  const char* command;
  CommandHandler* handler;
  CommandItem* pNext;
  CommandItem(const char* command, CommandHandler* handler);
};

  
class CommandProcessor {
private:
  char mBuff[BUFF_SIZE];    
  int mBuff_pos;             
  char mCommands[CMD_SIZE]; 
  int mCmd_pos;
  HardwareSerial* m_pSerial;    
  CommandItem* m_pFirst;
protected:
  void parseString(char* str);
  void parseCommand(char* str);
  void add(const char* command);  
public:
  CommandProcessor(HardwareSerial* pSerial);
  void loop();  // вызывается в основном цикле программы
  void registerHandler(const char* command, CommandHandler* handler);
};

#endif