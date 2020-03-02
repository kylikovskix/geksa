#ifndef G_FOOT_H
#define G_FOOT_H

/*
* Класс: GeksaFoot
* Версия: 1.0
* Дата создания: 10.09.2019
* Дата изменения: 
* Автор: Куликовских Сергей
* Email: kylikovskix@gmail.com
* Описание: инкапсулирует инициализацию и управлние сервами отдельной лапы;
* предоставляет функционал решения задач прямой и обратной кинематики лапы.
*/

#include "3d_math.h"
#include <Wire.h>
#include <Multiservo.h>

class GeksaFoot {
private:
// Положение центра локальной системы координат лапы относительно центра робота
  Vector3D m_p0;
// Углы поворота осей локальной системы координат лапы относительно центра робота
  Vector3D m_r0;
// объекты Multiservo, для непосредственного управления сервоприводами
  Multiservo m_coxaServo;   // сервопривод плеча
  Multiservo m_femoraServo; // сервопривод бедра
  Multiservo m_tibiaServo;  // сервопривод голени
public:
  GeksaFoot(Vector3D p0,Vector3D r0);
// выполняет активацию сервоприводов
  void begin(int coxaPin, int femoraPin, int tibiaPin);
// выполняет дезактивацию сервоприводов
  void end();   

// интерфейс управления лапой

  void coxaAngle(int);      // повернуть плечо на заданный угол
  int coxaAngle();          // возвращает текущий угол поворота плеча

  void femoraAngle(int);    // наклонить бедро на заданный угол
  int femoraAngle();        // возвращает текущий угол наклона бедра

  void tibiaAngle(int);     // наклонить голень на заданный угол
  int tibiaAngle();         // возвращает текущий угол наклона голени

// вспомогательные методы для расчетов прямой и инверсной кинематики 

  // вычисляет углы поворотов ноги для достижения конечностью заданной точки
  int getAngles(Vector3D p, int& coxaAngle, int& femoraAngle, int& tibiaAngle);
  // вычисляет координату кончика лапы по заданным углам
  int getPoint(int coxaAngle, int femoraAngle, int tibiaAngle, Vector3D& p);
};
#endif
