#include "g_cp_job.h"
#include "errors.h"

void(* resetFunc) (void) = 0;//объявляем функцию reset с адресом 0

CommandProcessorJob::CommandProcessorJob(Geksapod* pGeksapod, HardwareSerial* pInSerial, HardwareSerial* pOutSerial):AJob(pGeksapod) {
  m_pGeksapod = pGeksapod;
  m_pInSerial = pInSerial;
  m_pOutSerial = pOutSerial;
  clear();
}

// CommandProcessorJob::clear() подготавлтвает сосояние для получения новой команды
void CommandProcessorJob::clear() {
  memset(buff,0,SERIAL_BUFF_SIZE+1);
  id = 0;
  count = 0;
  args[count] = buff;
}

// CommandProcessorJob::onRun() выполняет проверку наличия команды, ее прием и  последующую обработку
void CommandProcessorJob::onRun() {

  // интерфейсы ввода и вывода должны быть опрееделены
  if(m_pInSerial == NULL || m_pOutSerial == NULL) {
    return;
  }
  
  char c;
  
  while(m_pInSerial->available() > 0) {                           // пока есть входящие данные 
    c = m_pInSerial->read();                                      // читаем их
    //Serial.println((int)c);
    if(c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == ';') {    
      if(*args[count] == 0) continue;
      buff[id++] = 0;
      if(++count >= ARGS_MAX_COUNT) break;
      args[count] = buff + id;
      if(c == '\n' || c == ';') break;
    } else 
      buff[id++] = toupper(c);
    if(id >= SERIAL_BUFF_SIZE) break;
  }

  // если нет команды или она не окончена, заканичваем работу
  if(args[0][0] == 0 || c != '\n' && c != ';') { 
    return;
  }
 
  //m_pOutSerial->println(buff);
  m_pOutSerial->println("");

  // выполняем полученную команду
  
  char ca,fa,ta;
  int res = E_BAD_PARAM;
    
  if(strcmp(args[0],"RESET") == 0) { 
    res = cmdReset(args, count); 
  } else if(strcmp(args[0],"MOVE") == 0) { 
    res = cmdMove(args, count); 
  } else if(strcmp(args[0],"ROTATE") == 0) { 
    res = cmdRotate(args, count); 
  } else if(strcmp(args[0],"STOP") == 0) {
    res = cmdStop(args, count);
  } else if(strcmp(args[0],"POSE") == 0) {
    res = cmdPose(args,count);
  } else if(strcmp(args[0],"COXA") == 0) {
    res = cmdCoxa(args, count);
  } else if(strcmp(args[0],"FEMORA") == 0) {
    res = cmdFemora(args, count);
  } else if(strcmp(args[0],"TIBIA") == 0) {
    res = cmdTibia(args, count);
  }

  if(res != S_OK) {
    // выводим информацию о результате полученной команды
    for(int i=0;i<=count;i++) {
      if(args[i] != 0) {
        m_pOutSerial->print(args[i]);
        m_pOutSerial->write(32);
      }
    }
    m_pOutSerial->println(res);
  }
  
  // после выполнений команды возвращаем состоение в исходное
  clear();
}

void CommandProcessorJob::printPose(int i, int ca, int fa, int ta) {
  m_pOutSerial->print(i);
  m_pOutSerial->write(58);
  m_pOutSerial->write(123);
  m_pOutSerial->print(ca);
  m_pOutSerial->write(44);
  m_pOutSerial->print(fa);
  m_pOutSerial->write(44);
  m_pOutSerial->print(ta);
  m_pOutSerial->write(125);
  m_pOutSerial->println();
}

int CommandProcessorJob::cmdReset(const char* args[], int count) {
  resetFunc();   
  return 0; 
}

int CommandProcessorJob::cmdMove(const char* args[], int count) {
  // получена команда начать движение
  int result;
  if(count > 1) {
    char s = atoi(args[1]); // скорость
    char t = 0;
    if(count > 2)
      t = atoi(args[2]); // время
    result = m_pGeksapod->move(s,t);
  } else
    result = E_BAD_PARAM;
    
  return result;
}

int CommandProcessorJob::cmdRotate(const char* args[], int count) {
  // получена команда начать движение
  int result;
  if(count > 1) {
    char s = atoi(args[1]); // скорость
    char t = 0;
    if(count > 2)
      t = atoi(args[2]); // время
    result = m_pGeksapod->rotate(s,t);
  } else
    result = E_BAD_PARAM;
    
  return result;
}

int CommandProcessorJob::cmdStop(const char* args[], int count) {
  // получена команда остановить движение
  m_pGeksapod->stop();
  return 0;
}

int CommandProcessorJob::cmdPose(const char* args[], int count) {
  // получена команда принять заданную позу или сообщить текщие данные,
  // команда вариативна по количеству параметров:
  int res = E_BAD_PARAM;
  int ca,fa,ta;
  switch(--count) {
    case 0: {// если нет параметров, возвращаем текущее состояние
      int pose[FOOTS_COUNT][3];
      m_pGeksapod->getPose(pose);
//      for(char i=0; i<FOOTS_COUNT; i++) {
//        printPose(i,pose[i][0],pose[i][1],pose[i][2]);
//      }
  m_pOutSerial->print("JSON:{POSE:[[");
  m_pOutSerial->print((int)pose[0][0]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[0][1]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[0][2]);
  m_pOutSerial->print("],[");
  m_pOutSerial->print((int)pose[1][0]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[1][1]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[1][2]);
  m_pOutSerial->print("],[");
  m_pOutSerial->print((int)pose[2][0]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[2][1]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[2][2]);
  m_pOutSerial->print("],[");
  m_pOutSerial->print((int)pose[3][0]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[3][1]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[3][2]);
  m_pOutSerial->print("],[");
  m_pOutSerial->print((int)pose[4][0]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[4][1]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[4][2]);
  m_pOutSerial->print("],[");
  m_pOutSerial->print((int)pose[5][0]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[5][1]);
  m_pOutSerial->print(",");
  m_pOutSerial->print((int)pose[5][2]);
  m_pOutSerial->println("]]}");

      res = S_OK;
      break;
    }
    case 1: {
      char i = atoi(args[1]);
      m_pGeksapod->getPose(i,ca,fa,ta);
      printPose(i,ca,fa,ta);
      res = 0;
      break;
    }
//    case 3: {// получено общее положение углов для всех лап 
//      ca = atoi(args[1]);
//      fa = atoi(args[2]);
//      ta = atoi(args[3]);
//      res = m_pGeksapod->setPose(ca,fa,ta);
//      break;
//    }
    case FOOTS_COUNT*3:{ // получено конкретное положение кадого угла
      for(char i=0; i<FOOTS_COUNT; i++) {
        ca = atoi(args[1]);
        fa = atoi(args[2]);
        ta = atoi(args[3]);
        res = m_pGeksapod->_setPose(i,ca,fa,ta);
        if(res != 0) break;
      }
      break;
    }
    case 4: 
    case 8:
    case 12:
    case 16:
    case 20:
    case 24:
      // получен массив идентификаторв лап и значений углов
      for(char i=0; i<((count) % (FOOTS_COUNT+1)); i++) {
        char id = atoi(args[i*4+1]);  
        ca = atoi(args[i*4+2]);
        fa = atoi(args[i*4+3]);
        ta = atoi(args[i*4+4]);
        res = m_pGeksapod->_setPose(id,ca,fa,ta);
        if(res != 0) break;
      }
      break;
  }
  return res;
}

int CommandProcessorJob::cmdCoxa(const char* args[], int count) {
  // получена команда изменить угол поворота плеча
  // команда вариативна по количеству параметров:
  int _ca, ca, fa, ta;
  int res = E_BAD_PARAM;
  switch(count-1) {
    case 1: // повернуть все плечи на заданный угол
      _ca = atoi(args[1]);
      for(char i=0; i<FOOTS_COUNT; i++) {
        m_pGeksapod->getPose(i,ca,fa,ta);
        res = m_pGeksapod->_setPose(i,_ca,fa,ta);
        if(res != 0) break;
      }
      break;
    case 2: // повернуть конерктное плечо на заданный угол
      _ca = atoi(args[2]);
      char* s = args[1];
      while(*s != 0) {
        char i = *s-'0';
        m_pGeksapod->getPose(i,ca,fa,ta);
        res = m_pGeksapod->_setPose(i,_ca,fa,ta);
        if(res != 0) break;
        s++;
      }
      break;
  }
  return res;
}

int CommandProcessorJob::cmdFemora(const char* args[], int count) {
  // получена команда изменить угол наклона бедра
  // команда вариативна по количеству параметров:
  int _fa, ca, fa, ta;
  int res = E_BAD_PARAM;
  switch(count-1) {
    case 1: // повернуть бедра всех лап на заданный угол
      _fa = atoi(args[1]);
      for(char i=0; i<FOOTS_COUNT; i++) {
        m_pGeksapod->getPose(i,ca,fa,ta);
        res = m_pGeksapod->_setPose(i,ca,_fa,ta);
        if(res != 0) break;
      }
      break;
    case 2: // повернуть бедро конерктной лапы на заданный угол
      _fa = atoi(args[2]);
      char* s = args[1];
      while(*s != 0) {
        char i = *s-'0';
        m_pGeksapod->getPose(i,ca,fa,ta);
        res = m_pGeksapod->_setPose(i,ca,_fa,ta);
        if(res != 0) break;
        s++;
      }
      break;
  }
  return res;
}

int CommandProcessorJob::cmdTibia(const char* args[], int count) {
  // получена команда изменить угол наклона голени
  // команда вариативна по количеству параметров:
  int _ta, ca, fa, ta;
  int res = E_BAD_PARAM;
  switch(count-1) {
    case 1: // наклонить голени всех лап на заданный угол
      _ta = atoi(args[1]);
      for(char i=0; i<FOOTS_COUNT; i++) {
        m_pGeksapod->getPose(i,ca,fa,ta);
        res = m_pGeksapod->_setPose(i,ca,fa,_ta);
        if(res != 0) break;
      }
      break;
    case 2: // наклонить голень конерктной лапы на заданный угол
      _ta = atoi(args[2]);
      char* s = args[1];
      while(*s != 0) {
        char i = *s-'0';
        m_pGeksapod->getPose(i,ca,fa,ta);
        res = m_pGeksapod->_setPose(i,ca,fa,_ta);
        if(res != 0) break;
        s++;
      }
      break;
  }
  return res;
}

int CommandProcessorJob::cmdCalc(const char* args[], int count) {
  int res = E_BAD_PARAM;
  int c,f,t,i; 

  if(count == 5) {
    i = atoi(args[1]); 
    Vector3D v = {atof(args[2]), atof(args[3]), atof(args[4])};
    res = m_pGeksapod->getAngles(i,v,c,f,t);
  }

  if(!res) {
    m_pOutSerial->print(c);
    m_pOutSerial->write(32);
    m_pOutSerial->print(f);
    m_pOutSerial->write(32);
    m_pOutSerial->println(t);
  }

  return res;
}
