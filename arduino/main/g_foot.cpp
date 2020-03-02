#include "g_foot.h"
#include <math.h>
#include "config.h"
#include <arduino.h>
#include "errors.h"

GeksaFoot::GeksaFoot(Vector3D p0,Vector3D r0) {
  m_p0 = p0;
  m_r0 = r0;
}

// GeksaFoot::begin() выполняет активацию сервов
void GeksaFoot::begin(int coxaPin, int femoraPin, int tibiaPin) {
  m_coxaServo.attach(coxaPin);
  m_femoraServo.attach(femoraPin);
  m_tibiaServo.attach(tibiaPin);
}

// GeksaFoot::end() выполняет деактивацию сервов
void GeksaFoot::end() {
  m_coxaServo.detach();
  m_femoraServo.detach();
  m_tibiaServo.detach();
}

// GeksaFoot::coxaAngle(int angle) - повернуть плечо на заданный угол
void GeksaFoot::coxaAngle(int angle) {
  // угол не должен выходить из заданного диапазона
  if(COXA_MIN_ANGLE > angle)
    angle = COXA_MIN_ANGLE;
  else if(angle > COXA_MAX_ANGLE)
    angle = COXA_MAX_ANGLE;
  // выполняем пересчет угла с учетом нулевого положения и направления вращения
  angle *= (COXA_ANGLE_INVERSE ? -1 : 1);
  angle += (COXA_ANGLE_0);
  // поворачиваем сервопривод
  m_coxaServo.write(angle);
}

// GeksaFoot::coxaAngle() - возвращает текущий угол попорота плеча
int GeksaFoot::coxaAngle() {
  // считываем текущее значение угла сервопривода
  int angle = m_coxaServo.read();
  // выполняем пересчет угла с учетом нулевого положения и направления вращения
  angle -= (COXA_ANGLE_0);
  angle *= (COXA_ANGLE_INVERSE ? -1 : 1);
  return angle;
}

// GeksaFoot::femoraAngle(int angle) - наклонить бедро на заданный угол
void GeksaFoot::femoraAngle(int angle) {
  // угол не должен выходить из заданного диапазона
  if(FEMORA_MIN_ANGLE > angle)
    angle = FEMORA_MIN_ANGLE;
  else if(angle > FEMORA_MAX_ANGLE)
    angle = FEMORA_MAX_ANGLE;
  // выполняем пересчет угла с учетом нулевого положения и направления вращения
  int _angle = (FEMORA_ANGLE_INVERSE ? -1 : 1)*angle + FEMORA_ANGLE_0;
  // поворачиваем сервопривод
  m_femoraServo.write(_angle);
}

// GeksaFoot::femoraAngle() - возвращает текущий угол наклона бедра
int GeksaFoot::femoraAngle() {
  // считываем текущее значение угла сервопривода
  int angle = m_femoraServo.read();
  // выполняем пересчет угла с учетом нулевого положения и направления вращения
  angle -= (FEMORA_ANGLE_0);
  angle *= (FEMORA_ANGLE_INVERSE ? -1 : 1);
  return angle;
}

// GeksaFoot::tibiaAngle(int angle) - наклонить голень на заданный угол
void GeksaFoot::tibiaAngle(int angle) {
  // угол не должен выходить из заданного диапазона
  if(TIBIA_MIN_ANGLE > angle)
    angle = TIBIA_MIN_ANGLE;
  else if(angle > TIBIA_MAX_ANGLE)
    angle = TIBIA_MAX_ANGLE;
  // выполняем пересчет угла с учетом нулевого положения и направления вращения
  int _angle = (TIBIA_ANGLE_INVERSE ? -1 : 1)*angle + TIBIA_ANGLE_0;
  // поворачиваем сервопривод
  m_tibiaServo.write(_angle);
}

// GeksaFoot::tibiaAngle() - возвращает текущий угол наклона бедра
int GeksaFoot::tibiaAngle() {
  // считываем текущее значение угла сервопривода
  int angle = m_tibiaServo.read();
  // выполняем пересчет угла с учетом нулевого положения и направления вращения
  angle -= TIBIA_ANGLE_0;
  angle *= (TIBIA_ANGLE_INVERSE ? -1 : 1);
  return angle;
}

// GeksaFoot::getAngles(Vector3D<int> p, int& coxaAngle, int& femoraAngle, int& tibiaAngle)
// вычисляет углы поворотов ноги для достижения конечностью заданой точки (инверсная кинематика)
int GeksaFoot::getAngles(Vector3D p, int& coxaAngle, int& femoraAngle, int& tibiaAngle) {
  
  // вычисляем матрицу поворота ЛКС лапы по заданным углам
  Matrix3D m = rotMatrix2(-m_r0);

  // переводим координаты точки в ЛСК лапы
  Vector3D v = p - m_p0;
  v = m*v; 
  
  // значение искомых углов в вещественном виде
  double _coxaAngle;   
  double _femoraAngle; 
  double _tibiaAngle;  

  // в ЛСК конец лапы не может иметь отрицательнуюю координату X
  if(v.x < 0) return E_BAD_COORD;

  // находим coxaAngle (угол поворота плеча)
  if (fabs(v.y) < fabs(v.x)) { 
    _coxaAngle = atan(v.y/v.x);
  } else if(v.y != 0){
    _coxaAngle = arcctn(v.x/v.y);        
  } else {
    return E_BAD_COORD;
  }

  // угол должен находится в диапазоне (-90 .. 90) (градусов)
  if(_coxaAngle > 3*M_PI/4)
    _coxaAngle =- 2*M_PI;

  // размеры частей лапы будем использовать в вещественном виде
  double coxaLength = COXA_LENGTH;      // длина плеча
  double femoraLength = FEMORA_LENGTH;  // длина бедра
  double tibiaLength = TIBIA_LENGTH;    // длина голени
  double tibiaOffset = TIBIA_OFFSET;    // смещение голени относительноузла наклона 
  
  // вычисление промежуточныхь параметров
  double r = sqrt(v.x*v.x+v.y*v.y)-coxaLength;
  double t = tibiaLength*v.z-r*tibiaOffset;
  double q = r*tibiaLength+tibiaOffset*v.z;
  double s = sqrt(t*t+q*q);

  if(s == 0) return E_FAIL; // на 0 делить нельзя

  double cos_angle = (femoraLength*femoraLength - tibiaLength*tibiaLength - tibiaOffset*tibiaOffset - v.z*v.z - r*r)/s/2;
  if(cos_angle < -1 || cos_angle > 1) {
    return false; // значения косинуса угла не могут выходить из диапазона [-1 .. 1]
  }             

  double angle = acos(cos_angle) - acos(t/s); 
  
  double sin_femoraAngle = (v.z + tibiaLength*cos(angle) - tibiaOffset*sin(angle))/femoraLength;
  if(sin_femoraAngle < -1 || sin_femoraAngle > 1) {
    return E_FAIL; // значения синуса угла не могут выходить из диапазона [-1 .. 1]
  }
  
  _femoraAngle = asin(sin_femoraAngle); // находим угол наклона бедра
  _tibiaAngle = angle - _femoraAngle;   // находим угол наклона голени
  // переводим результат из вещественного вида и радиан в целые градусы
  coxaAngle = round(_coxaAngle/M_PI*180);   
  femoraAngle = round(_femoraAngle/M_PI*180);
  tibiaAngle = round(_tibiaAngle/M_PI*180);
  return S_OK;
}

// GeksaFoot::getPoint(int coxaAngle, int femoraAngle, int tibiaAngle, Vector3D<int>& p)
// вычисляет координату кончика лапы по заданным углам
int GeksaFoot::getPoint(int coxaAngle, int femoraAngle, int tibiaAngle, Vector3D& p) {

  double coxaLength = COXA_LENGTH;
  double femoraLength = FEMORA_LENGTH;
  double tibiaLength = sqrt(TIBIA_LENGTH*TIBIA_LENGTH + TIBIA_OFFSET*TIBIA_OFFSET);
  
  // переводим градусы в радианы
  double _coxaAngle = M_PI*coxaAngle/180;
  double _femoraAngle = M_PI*femoraAngle/180;
  double _tibiaAngle = M_PI*(tibiaAngle - 90)/180 + atan((double)TIBIA_OFFSET/TIBIA_LENGTH);
  
  // находим коррдинаты точки
  p.z = femoraLength*sin(_femoraAngle) + tibiaLength*sin(_tibiaAngle+_femoraAngle);
  p.y = coxaLength + femoraLength*cos(_femoraAngle) + tibiaLength*cos(_tibiaAngle+_femoraAngle);
  p.x = p.y*cos(_coxaAngle);
  p.y = p.y*sin(_coxaAngle);
  
  Matrix3D m = rotMatrix2(m_r0);
  p = m*p + m_p0;

  return S_OK;
}
