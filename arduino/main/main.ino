#include "a_geksa.h"
#include "g_cp_job.h"

Geksapod geksa;
CommandProcessorJob cmdJob(&geksa, &Serial, &Serial);

void setup(void)
{
  Serial.begin(115200);  // инициализация UART
  delay(2000);    // ждем некоторое время, чтобы сервошилд был готов принимать команды)
  geksa.setup();  // инициализируем гексапода
  Serial.println("Geksapod is ready!"); // Сообщаем о готовности работы
}

void loop(void)
{
  geksa.loop();
}
 
