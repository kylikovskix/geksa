#include <CmdProc.h>

CommandProcessor::CommandProcessor(HardwareSerial* pSerial) {
  mBuff_pos = 0;
  mCmd_pos = 0;     
  mCommands[0] = 0;               
  m_pSerial = pSerial;      
  m_pFirst = NULL;
}

// ��������� ��������� ������� � ��������� �����
void CommandProcessor::add(const char* command) {
  int len = strlen(command);
  if(mCmd_pos + len + 1 < CMD_SIZE) {
    strncpy(mCommands + mCmd_pos, command,len);
    mCmd_pos += len;
    mCommands[mCmd_pos++] = ';';
    mCommands[mCmd_pos] = 0;
  }
}

// ��������� ������� ��������� ������, 
void CommandProcessor::loop() {
  
  if(m_pSerial != NULL) { 
    while(m_pSerial->available() > 0) {
      char c = Serial.read();
      if( c==';' || c == '\n' || c == '\r') {
        if(mBuff_pos > 0) {
          mBuff[mBuff_pos] = 0;     
          add(mBuff);
          mBuff_pos = 0;
        }
      } else
        mBuff[mBuff_pos++] = c;
    }
                         
    parseString(mCommands);
    memset(mCommands,0,CMD_SIZE);
    mCmd_pos = 0;
  }
}

// ��������� ������ ������
void CommandProcessor::parseString(char* str) {
  
  int len = strlen(str);
  char* s1 = str;
  
  while (s1 - str < len) {
    char* s2 = strchr(s1, ';');
    if (s2 != NULL) {
      *s2 = 0;
      parseCommand(s1);
      s1 = s2 + 1;
    } else {
      parseCommand(s1);
      break;
    }
  }
}

// ������ ��������� �������.
// ������ �������: �����_������� ��������1 ��������2 ...
// ����������� �� ����� N ����������
void CommandProcessor::parseCommand(char* str) {
  const char* args[MAX_ARG_COUNT]; // ��������� ���������
  int count = 0;  // ���������� ����������
  int len = strlen(str);
  char* s1 = str;
  
  while(*s1 != 0) {
    *s1 = toupper(*s1); 
    s1++;
  };
  s1 = str;
  
  while(strchr("\t ",*s1) != 0) {s1++;};
  
  while (s1 - str < len) {
    char* s2  = strchr(s1,' ');  // ������� ������� ���������� �����������
    if (s2 != NULL) { // �������� ��������� ��������
      *s2 = 0;
      args[count++] = s1;
      s1 = s2 + 1;
    } else {
      args[count++] = s1;
      break;
    }
    // ����������� �� ���������� ����������
    if (count >= MAX_ARG_COUNT) break;
  }

  CommandItem* pItem = m_pFirst;
  while(pItem != NULL) {
    if(strcmp(args[0],pItem->command) == 0)
      break;
    pItem = pItem->pNext;  
  }
  
  if(pItem != NULL)
    if(pItem->handler != NULL) {
      if(m_pSerial != NULL) {
        m_pSerial->print("Excecute ");
        m_pSerial->print(pItem->command);        
        m_pSerial->println("...");
      }
        
      int result = (*pItem->handler)(args, count);
        
      if(m_pSerial != NULL) {
        m_pSerial->print(pItem->command);        
        m_pSerial->print(" return code: ");
        m_pSerial->println(result);      
      }
    } 
}  

// ��������� ����������� ������� � �� �����������
void CommandProcessor::registerHandler(const char* command, CommandHandler* handler) {
  CommandItem* pItem = new CommandItem(command,handler);
  pItem->pNext = m_pFirst;
  m_pFirst = pItem;
}

CommandItem::CommandItem(const char* command, CommandHandler* handler) {
  this->command = command;
  this->handler = handler;
  pNext = NULL;  
}
