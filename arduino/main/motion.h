#ifndef MOTION_H
#define MOTION_H

#include "3d_math.h"
#include "a_geksa.h"
#include "errors.h"

// базовый класс Motion реализует алгоритм движения конечностей,
// позволяя получить положение конечностей в заданный момент времни

class Motion {
  friend class MotionJob;
protected:
  long m_MaxTime;     // время выполнения движения в одном цикле
  long m_TotalTime;   // время отведенное на выполнение движения 
  bool m_IsLooped;    // признак цикличности движения
  Motion* m_pNext;    // указатель на следующий  движения
public:  
  Motion(long maxTime, bool isLooped, long totalTime = -1, Motion* pNext = NULL);
  ~Motion();
  
  inline long maxTime() { return m_MaxTime; }
  inline long totalTime() { return m_TotalTime; }
  inline bool isLooped() { return m_IsLooped; }

  // хотя бы один из этих методов должен иметь полноценную реализацию

  // возвращает координаты конечностей на момент времени 0 <= time <= m_MaxTime 
  virtual int getPose(long time, Vector3D points[FOOTS_COUNT]) { return E_NOT_IMPL; };  
  // возвращает углы поворота конечностей на момент времени 0 <= time <= m_MaxTime
  virtual int getPose(long time, int pose[FOOTS_COUNT][3]) { return E_NOT_IMPL; };       
};

// класс TransPoseMotion реализует движения из начальной точки в конечную

class TransPoseMotion: public Motion {
  Vector3D m_Pose0[FOOTS_COUNT];   // начальное положение
  Vector3D m_Offset[FOOTS_COUNT];  // вектор перемещения
public:  
  TransPoseMotion(Vector3D begPose[FOOTS_COUNT], Vector3D endPose[FOOTS_COUNT], long maxTime, Motion* pNext = NULL);
  int getPose(long time, Vector3D points[FOOTS_COUNT]);
};

// класс LinearMotion реализует прямолинейное движение по ровной поверхности

class LinearMotion: public Motion {
  Vector3D m_Pose0[FOOTS_COUNT];   // положение конечности в начале (и конце) шага
  Vector3D m_Offset;               // вектор перемещения в результате шага
  int m_Heigth;                    // высота подъема конечностей
public:  
  LinearMotion(int heigth, Vector3D pose0[FOOTS_COUNT], Vector3D speed, long maxTime, long totalTime = -1, Motion* pNext = NULL);
  int getPose(long time, Vector3D points[FOOTS_COUNT]);
};


// класс RotateMotion реализует поворот на месте

class RotateMotion: public Motion {
  Vector3D m_Pose0[FOOTS_COUNT]; // положение конечности в начале (и конце) шага
  Vector3D m_Angles;             // углы поворота в результате шага
  int m_Heigth;                  // высота подъема конечностей
public:  
  RotateMotion(int heigth, Vector3D pose0[FOOTS_COUNT], Vector3D rotor, long maxTime, long totalTime = -1, Motion* pNext = NULL);
  int getPose(long time, Vector3D points[FOOTS_COUNT]);
};

class TransAnglesMotion: public Motion {
  char m_Pose0[FOOTS_COUNT][3];
  char m_Offset[FOOTS_COUNT][3];
public:  
  TransAnglesMotion(int begPose[FOOTS_COUNT][3], int endPose[FOOTS_COUNT][3], long maxTime, Motion* pNext = NULL);
  int getPose(long actionTime, int pose[FOOTS_COUNT][3]);
};

#endif
