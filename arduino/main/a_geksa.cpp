#include "a_geksa.h"
#include "config.h"
#include <arduino.h>
#include "errors.h"
#include "poses.h"

// class Geksapod

Geksapod::Geksapod():
  m_LeftFrontFoot(LEFT_FRONT_FOOT_POSITION, LEFT_FRONT_FOOT_ROTATION),
  m_LeftMidleFoot(LEFT_MIDLE_FOOT_POSITION, LEFT_MIDLE_FOOT_ROTATION),
  m_LeftBackFoot(LEFT_BACK_FOOT_POSITION, LEFT_BACK_FOOT_ROTATION),
  m_RigthFrontFoot(RIGTH_FRONT_FOOT_POSITION, RIGTH_FRONT_FOOT_ROTATION),
  m_RigthMidleFoot(RIGTH_MIDLE_FOOT_POSITION, RIGTH_MIDLE_FOOT_ROTATION),
  m_RigthBackFoot(RIGTH_BACK_FOOT_POSITION, RIGTH_BACK_FOOT_ROTATION),
  m_MotionJob(this){}

void Geksapod::setup() {
  AJobManager::setup();
  m_LeftFrontFoot.begin(LEFT_FRONT_FOOT_COXA_PIN, LEFT_FRONT_FOOT_FEMORA_PIN, LEFT_FRONT_FOOT_TIBIA_PIN);
  m_LeftMidleFoot.begin(LEFT_MIDLE_FOOT_COXA_PIN, LEFT_MIDLE_FOOT_FEMORA_PIN, LEFT_MIDLE_FOOT_TIBIA_PIN);
  m_LeftBackFoot.begin(LEFT_BACK_FOOT_COXA_PIN, LEFT_BACK_FOOT_FEMORA_PIN, LEFT_BACK_FOOT_TIBIA_PIN);
  m_RigthFrontFoot.begin(RIGTH_FRONT_FOOT_COXA_PIN, RIGTH_FRONT_FOOT_FEMORA_PIN, RIGTH_FRONT_FOOT_TIBIA_PIN);
  m_RigthMidleFoot.begin(RIGTH_MIDLE_FOOT_COXA_PIN, RIGTH_MIDLE_FOOT_FEMORA_PIN, RIGTH_MIDLE_FOOT_TIBIA_PIN);
  m_RigthBackFoot.begin(RIGTH_BACK_FOOT_COXA_PIN, RIGTH_BACK_FOOT_FEMORA_PIN, RIGTH_BACK_FOOT_TIBIA_PIN);
  int pose[FOOTS_COUNT][3] = READY_POSE;
  setPose(pose, -1);
}

int Geksapod::setPose(int idx, int ca, int fa, int ta, int actionTime) {

  Vector3D p;
  Vector3D oldPose[FOOTS_COUNT];
  Vector3D newPose[FOOTS_COUNT];
  
  int result = getPoint(idx, ca, fa, ta, p); // проверяем допустимость заданных углов
  
  if(result != S_OK)
    return result;

  if(actionTime < 0)                       // если время действия равно нулю
    return _setPose(idx, ca, fa, ta);     // надо принять новое положение максимально быстро
    
  getPose(oldPose);                         // иначе узнаем текущее положение
  for(int i = 0; i < FOOTS_COUNT; i++)
    newPose[i] = oldPose[i];
  newPose[idx] = p;                         // описываем новое положение

  result = execute(new TransPoseMotion(oldPose,newPose, actionTime)); // запуск действия смены положения
  return result;    
}

int Geksapod::setPose(int pose[FOOTS_COUNT][3], int actionTime) {

  int result;
  Vector3D p;
  Vector3D oldPose[FOOTS_COUNT];
  Vector3D newPose[FOOTS_COUNT];

  for(int i = 0; i < FOOTS_COUNT; i++) {    // вычисляем точки по заданным углам
    result = getPoint(i, pose[i][0], pose[i][1], pose[i][2], p);

    if(result != S_OK)
      return result;

    newPose[i] = p;
  }

  if(actionTime < 0)            // если время действия равно нулю
    return _setPose(pose);     // надо принять новое положение максимально быстро
    
  getPose(oldPose);                                            // иначе узнаем текущее положение
  result = execute(new TransPoseMotion(oldPose, newPose, actionTime)); // запуск действия смены положения
  return result;
}

int Geksapod::setPose(int idx, Vector3D p, int actionTime) {
  int ca, fa, ta;
  Vector3D oldPose[FOOTS_COUNT];
  Vector3D newPose[FOOTS_COUNT];
  
  int result = getAngles(idx, p, ca, fa, ta); // проверяем допустимость заданных углов
  
  if(result != S_OK)
    return result;

  if(actionTime < 0)                       // если время действия равно нулю
    return _setPose(idx, ca, fa, ta);     // надо принять новое положение максимально быстро
    
  getPose(oldPose);                         // иначе узнаем текущее положение
  for(int i = 0; i < FOOTS_COUNT; i++)
    newPose[i] = oldPose[i];
  newPose[idx] = p;                         // описываем новое положение
  
  result = execute(new TransPoseMotion(oldPose, newPose, actionTime)); // запуск действия смены положения
  return result;    
}

int Geksapod::setPose(Vector3D points[FOOTS_COUNT], int actionTime) {
  int result;
  int ca, fa, ta;
  Vector3D  oldPose[FOOTS_COUNT];

  for(int i = 0; i < FOOTS_COUNT; i++) {    // проверяем достижимость каждой точки
    result = getAngles(i, points[i], ca, fa, ta);

    if(result != S_OK)
      return result;
  }

  if(actionTime < 0)              // если время действия равно нулю
    return _setPose(points);     // надо принять новое положение максимально быстро
    
  getPose(oldPose);                                           // иначе узнаем текущее положение
  result = execute(new TransPoseMotion(oldPose, points, actionTime)); // запуск действия смены положения
  return result;    
}

int Geksapod::setPose(int ca, int fa, int ta, int actionTime) {

  int result;
  Vector3D p;
  Vector3D oldPose[FOOTS_COUNT];
  Vector3D newPose[FOOTS_COUNT];

  for(int i = 0; i < FOOTS_COUNT; i++) {    // вычисляем точки по заданным углам
    result = getPoint(i, ca, fa, ta, p);

    if(result != S_OK)
      return result;

    newPose[i] = p;
  }

  if(actionTime < 0)            // если время действия равно нулю
    return _setPose(newPose);     // надо принять новое положение максимально быстро
    
  getPose(oldPose);                                            // иначе узнаем текущее положение
  result = execute(new TransPoseMotion(oldPose, newPose, actionTime)); // запуск действия смены положения
  return result;
}


int Geksapod::_setPose(int idx, int ca, int fa, int ta) {
  
  int pose[FOOTS_COUNT][3];
  getPose(pose);
  
  pose[idx][0] = ca;
  pose[idx][1] = fa;
  pose[idx][2] = ta;
  
  return _setPose(pose);
}

int Geksapod::_setPose(Vector3D points[FOOTS_COUNT]) {
  
  int result;
  int ca, fa, ta;
  int pose[FOOTS_COUNT][3];
  
  for(int i=0;i<FOOTS_COUNT;i++) {
    result = getAngles(i, points[i], ca, fa, ta);

    if(result != S_OK)
      return result;
    
    pose[i][0] = ca;
    pose[i][1] = fa;
    pose[i][2] = ta;
  }
  return _setPose(pose);
}


// проверка матрицы углов на допустимые значения
int Geksapod::_setPose(int pose[FOOTS_COUNT][3]) {

  // проверка вертикальных углов каждой ноги
  int limits_1[][2] = COMPLEX_ANGLE_LIMITS_1;

  for(int i=0;i<FOOTS_COUNT;i++) {
    for(int j=0; limits_1[j][0] < FEMORA_MAX_ANGLE; j++) {
      if(pose[i][1] <= limits_1[j][0] && pose[i][2] < limits_1[j][1]) {
        pose[i][2] = limits_1[j][1];
        break;
      }
    }
  }

  // угол поворота плеча средних кончностей не должен выходить 
  // за рамки диапазона [MIDLE_COXA_MIN_ANGLE, MIDLE_COXA_MAX_ANGLE]
  
  if(pose[LEFT_MIDLE_FOOT_IDX][0] < MIDLE_COXA_MIN_ANGLE)
    pose[LEFT_MIDLE_FOOT_IDX][0] = MIDLE_COXA_MIN_ANGLE;

  if(pose[LEFT_MIDLE_FOOT_IDX][0] > MIDLE_COXA_MAX_ANGLE)
    pose[LEFT_MIDLE_FOOT_IDX][0] = MIDLE_COXA_MAX_ANGLE;

  if(pose[RIGTH_MIDLE_FOOT_IDX][0] < MIDLE_COXA_MIN_ANGLE)
    pose[RIGTH_MIDLE_FOOT_IDX][0] = MIDLE_COXA_MIN_ANGLE;

  if(pose[RIGTH_MIDLE_FOOT_IDX][0] > MIDLE_COXA_MAX_ANGLE)
    pose[RIGTH_MIDLE_FOOT_IDX][0] = MIDLE_COXA_MAX_ANGLE;

  // угловые конечности не должны наезжать на средние
 /*
  int limits_2[][3] = COMPLEX_ANGLE_LIMITS_2;
  
  for(int j=0; limits_2[j][1] <= MIDLE_COXA_MAX_ANGLE; j++) {

    if(j > 0 && limits_2[j-1][1] < m_Pose[RIGTH_MIDLE_FOOT_IDX][0] && m_Pose[RIGTH_MIDLE_FOOT_IDX][0] < limits_2[j][1]) {
      
        if(m_Pose[RIGTH_BACK_FOOT_IDX][0] > limits_2[j-1][0])
          m_Pose[RIGTH_BACK_FOOT_IDX][0] = limits_2[j-1][0];
      
        if(m_Pose[RIGTH_FRONT_FOOT_IDX][0] < limits_2[j][2])
          m_Pose[RIGTH_FRONT_FOOT_IDX][0] = limits_2[j][2];
          
    } else if(m_Pose[RIGTH_MIDLE_FOOT_IDX][0] == limits_2[j][1]) {
      
        if(m_Pose[RIGTH_BACK_FOOT_IDX][0] > limits_2[j][0])
          m_Pose[RIGTH_BACK_FOOT_IDX][0] = limits_2[j][0];
      
        if(m_Pose[RIGTH_FRONT_FOOT_IDX][0] < limits_2[j][2])
          m_Pose[RIGTH_FRONT_FOOT_IDX][0] = limits_2[j][2];
    }
    
    if(j > 0 && limits_2[j-1][1] < m_Pose[LEFT_MIDLE_FOOT_IDX][0] && m_Pose[LEFT_MIDLE_FOOT_IDX][0] < limits_2[j][1]) {
      
        if(m_Pose[LEFT_FRONT_FOOT_IDX][0] > limits_2[j-1][0])
          m_Pose[LEFT_FRONT_FOOT_IDX][0] = limits_2[j-1][0];
      
        if(m_Pose[LEFT_BACK_FOOT_IDX][0] < limits_2[j][2])
          m_Pose[LEFT_BACK_FOOT_IDX][0] = limits_2[j][2];
          
    } else if(m_Pose[RIGTH_MIDLE_FOOT_IDX][0] == limits_2[j][1]) {
      
        if(m_Pose[LEFT_FRONT_FOOT_IDX][0] > limits_2[j][0])
          m_Pose[LEFT_FRONT_FOOT_IDX][0] = limits_2[j][0];
      
        if(m_Pose[LEFT_BACK_FOOT_IDX][0] < limits_2[j][2])
          m_Pose[LEFT_BACK_FOOT_IDX][0] = limits_2[j][2];
    }
  }
*/
  // передние или задние конечности не должны мешать друг другу

/*  if(m_Pose[RIGTH_FRONT_FOOT_IDX][0] - m_Pose[LEFT_FRONT_FOOT_IDX][0] > COMPLEX_COXA_ANGLE_LIMIT) {
    int delta = (m_Pose[RIGTH_FRONT_FOOT_IDX][0] - m_Pose[LEFT_FRONT_FOOT_IDX][0] - COMPLEX_COXA_ANGLE_LIMIT) >> 1;
    m_Pose[RIGTH_FRONT_FOOT_IDX][0] -= delta;
    m_Pose[LEFT_FRONT_FOOT_IDX][0] += delta;
  }
  
  if(m_Pose[LEFT_BACK_FOOT_IDX][0] - m_Pose[RIGTH_BACK_FOOT_IDX][0] > COMPLEX_COXA_ANGLE_LIMIT) {
    int delta = (m_Pose[LEFT_BACK_FOOT_IDX][0] - m_Pose[RIGTH_BACK_FOOT_IDX][0] - COMPLEX_COXA_ANGLE_LIMIT) >> 1;
    m_Pose[LEFT_BACK_FOOT_IDX][0] -= delta;
    m_Pose[RIGTH_BACK_FOOT_IDX][0] += delta;
  }*/

  int _pose[FOOTS_COUNT][3];
  getPose(_pose);

  if(abs(pose[LEFT_FRONT_FOOT_IDX][0] - _pose[LEFT_FRONT_FOOT_IDX][0]) > 1)
    m_LeftFrontFoot.coxaAngle(pose[LEFT_FRONT_FOOT_IDX][0]);
    
  if(abs(pose[LEFT_FRONT_FOOT_IDX][1] - _pose[LEFT_FRONT_FOOT_IDX][1]) > 1)
    m_LeftFrontFoot.femoraAngle(pose[LEFT_FRONT_FOOT_IDX][1]);

  if(abs(pose[LEFT_FRONT_FOOT_IDX][2] - _pose[LEFT_FRONT_FOOT_IDX][2]) > 1)
    m_LeftFrontFoot.tibiaAngle(pose[LEFT_FRONT_FOOT_IDX][2]);
  
  if(abs(pose[LEFT_MIDLE_FOOT_IDX][0] - _pose[LEFT_MIDLE_FOOT_IDX][0]) > 1)
    m_LeftMidleFoot.coxaAngle(pose[LEFT_MIDLE_FOOT_IDX][0]);

  if(abs(pose[LEFT_MIDLE_FOOT_IDX][1] - _pose[LEFT_MIDLE_FOOT_IDX][1]) > 1)
    m_LeftMidleFoot.femoraAngle(pose[LEFT_MIDLE_FOOT_IDX][1]);

  if(abs(pose[LEFT_MIDLE_FOOT_IDX][2] - _pose[LEFT_MIDLE_FOOT_IDX][2]) > 1)
    m_LeftMidleFoot.tibiaAngle(pose[LEFT_MIDLE_FOOT_IDX][2]);
  
  if(abs(pose[LEFT_BACK_FOOT_IDX][0] - _pose[LEFT_BACK_FOOT_IDX][0]) > 1)
    m_LeftBackFoot.coxaAngle(pose[LEFT_BACK_FOOT_IDX][0]);
    
  if(abs(pose[LEFT_BACK_FOOT_IDX][1] - _pose[LEFT_BACK_FOOT_IDX][1]) > 1)
    m_LeftBackFoot.femoraAngle(pose[LEFT_BACK_FOOT_IDX][1]);
  
  if(abs(pose[LEFT_BACK_FOOT_IDX][2] - _pose[LEFT_BACK_FOOT_IDX][2]) > 1)
    m_LeftBackFoot.tibiaAngle(pose[LEFT_BACK_FOOT_IDX][2]);
  
  if(abs(pose[RIGTH_FRONT_FOOT_IDX][0] - _pose[RIGTH_FRONT_FOOT_IDX][0]) > 1)
    m_RigthFrontFoot.coxaAngle(pose[RIGTH_FRONT_FOOT_IDX][0]);

  if(abs(pose[RIGTH_FRONT_FOOT_IDX][1] - _pose[RIGTH_FRONT_FOOT_IDX][1]) > 1)
    m_RigthFrontFoot.femoraAngle(pose[RIGTH_FRONT_FOOT_IDX][1]);
  
  if(abs(pose[RIGTH_FRONT_FOOT_IDX][2] - _pose[RIGTH_FRONT_FOOT_IDX][2]) > 1)
    m_RigthFrontFoot.tibiaAngle(pose[RIGTH_FRONT_FOOT_IDX][2]);
  
  if(abs(pose[RIGTH_MIDLE_FOOT_IDX][0] - _pose[RIGTH_MIDLE_FOOT_IDX][0]) > 1)
    m_RigthMidleFoot.coxaAngle(pose[RIGTH_MIDLE_FOOT_IDX][0]);

  if(abs(pose[RIGTH_MIDLE_FOOT_IDX][1] - _pose[RIGTH_MIDLE_FOOT_IDX][1]) > 1)
    m_RigthMidleFoot.femoraAngle(pose[RIGTH_MIDLE_FOOT_IDX][1]);

  if(abs(pose[RIGTH_MIDLE_FOOT_IDX][2] - _pose[RIGTH_MIDLE_FOOT_IDX][2]) > 1)
    m_RigthMidleFoot.tibiaAngle(pose[RIGTH_MIDLE_FOOT_IDX][2]);
  
  if(abs(pose[RIGTH_BACK_FOOT_IDX][0] - _pose[RIGTH_BACK_FOOT_IDX][0]) > 1)
    m_RigthBackFoot.coxaAngle(pose[RIGTH_BACK_FOOT_IDX][0]);

  if(abs(pose[RIGTH_BACK_FOOT_IDX][1] - _pose[RIGTH_BACK_FOOT_IDX][1]) > 1)
    m_RigthBackFoot.femoraAngle(pose[RIGTH_BACK_FOOT_IDX][1]);

  if(abs(pose[RIGTH_BACK_FOOT_IDX][2] - _pose[RIGTH_BACK_FOOT_IDX][2]) > 1)
    m_RigthBackFoot.tibiaAngle(pose[RIGTH_BACK_FOOT_IDX][2]);  
  return 0;
}

void Geksapod::getPose(int pose[FOOTS_COUNT][3]) {
  int ca, fa, ta;
  for(int i = 0; i < FOOTS_COUNT; i++) {
    getPose(i,ca,fa,ta);
    pose[i][0] = ca;
    pose[i][1] = fa;
    pose[i][2] = ta;
  }
}

void Geksapod::getPose(Vector3D points[FOOTS_COUNT]) {
  Vector3D p;
  for(int i = 0; i < FOOTS_COUNT; i++) {
    getPose(i, p);
    points[i] = p;
  }
}

void Geksapod::getPose(int idx, Vector3D& p) {
  int ca, fa, ta;
  getPose(idx, ca, fa, ta);
  getPoint(idx, ca, fa, ta, p);
}

void Geksapod::getPose(int idx, int& ca, int& fa, int& ta) {
  switch(idx) {
    case LEFT_FRONT_FOOT_IDX:
      ca = m_LeftFrontFoot.coxaAngle();
      fa = m_LeftFrontFoot.femoraAngle();
      ta = m_LeftFrontFoot.tibiaAngle();
      break;
    case LEFT_MIDLE_FOOT_IDX:
      ca = m_LeftMidleFoot.coxaAngle();
      fa = m_LeftMidleFoot.femoraAngle();
      ta = m_LeftMidleFoot.tibiaAngle();
      break;
    case LEFT_BACK_FOOT_IDX:
      ca = m_LeftBackFoot.coxaAngle();
      fa = m_LeftBackFoot.femoraAngle();
      ta = m_LeftBackFoot.tibiaAngle();
      break;
    case RIGTH_FRONT_FOOT_IDX:
      ca = m_RigthFrontFoot.coxaAngle();
      fa = m_RigthFrontFoot.femoraAngle();
      ta = m_RigthFrontFoot.tibiaAngle();
      break;
    case RIGTH_MIDLE_FOOT_IDX:
      ca = m_RigthMidleFoot.coxaAngle();
      fa = m_RigthMidleFoot.femoraAngle();
      ta = m_RigthMidleFoot.tibiaAngle();
      break;
    case RIGTH_BACK_FOOT_IDX:
      ca = m_RigthBackFoot.coxaAngle();
      fa = m_RigthBackFoot.femoraAngle();
      ta = m_RigthBackFoot.tibiaAngle();
      break;
  }
}

int Geksapod::getAngles(int idx, Vector3D p, int& ca, int& fa, int& ta) {
  switch(idx) {
    case LEFT_FRONT_FOOT_IDX:
      return m_LeftFrontFoot.getAngles(p,ca,fa,ta);
      break;
    case LEFT_MIDLE_FOOT_IDX:
      return m_LeftMidleFoot.getAngles(p,ca,fa,ta);
      break;
    case LEFT_BACK_FOOT_IDX:
      return m_LeftBackFoot.getAngles(p,ca,fa,ta);
      break;
    case RIGTH_FRONT_FOOT_IDX:
      return m_RigthFrontFoot.getAngles(p,ca,fa,ta);
      break;
    case RIGTH_MIDLE_FOOT_IDX:
      return m_RigthMidleFoot.getAngles(p,ca,fa,ta);
      break;
    case RIGTH_BACK_FOOT_IDX:
      return m_RigthBackFoot.getAngles(p,ca,fa,ta);
      break;
  }
  return E_BAD_PARAM;
}
  
int Geksapod::getPoint(int idx, int ca, int fa, int ta, Vector3D& p) {
  switch(idx) {
    case LEFT_FRONT_FOOT_IDX:
      return m_LeftFrontFoot.getPoint(ca,fa,ta, p);
      break;
    case LEFT_MIDLE_FOOT_IDX:
      return m_LeftMidleFoot.getPoint(ca,fa,ta, p);
      break;
    case LEFT_BACK_FOOT_IDX:
      return m_LeftBackFoot.getPoint(ca,fa,ta, p);
      break;
    case RIGTH_FRONT_FOOT_IDX:
      return m_RigthFrontFoot.getPoint(ca,fa,ta, p);
      break;
    case RIGTH_MIDLE_FOOT_IDX:
      return m_RigthMidleFoot.getPoint(ca,fa,ta, p);
      break;
    case RIGTH_BACK_FOOT_IDX:
      return m_RigthBackFoot.getPoint(ca,fa,ta, p);
      break;
  }
  return E_BAD_PARAM;
}

int Geksapod::execute(Motion* pMotion) {
  m_MotionJob.execute(pMotion); 
}

int Geksapod::move(int speed, int time) {

  if(speed == 0)
    return E_BAD_PARAM;

  Vector3D points[FOOTS_COUNT];
  int pose[FOOTS_COUNT][3] = READY_POSE;
  getPoints(pose, points);
  Motion* pMotion = new LinearMotion(50, points, {speed, 0, 0}, (int)abs(1000.0*MOVE_STEP/speed), time > 0 ? time*1000 : -1);

  if(pMotion == NULL)
    return E_NOT_MEMORY;

  return execute((Motion*)pMotion);
}

int Geksapod::rotate(int speed, int time) {
  
  if(speed == 0)
    return E_BAD_PARAM;

  Vector3D points[FOOTS_COUNT];
  int pose[FOOTS_COUNT][3] = READY_POSE;
  getPoints(pose, points);
  Motion* pMotion = new RotateMotion(50, points, {0, 0, speed}, (int)abs(1000.0*ROTATE_STEP/speed), time > 0 ? time*1000 : -1);

  if(pMotion == NULL)
    return E_NOT_MEMORY;

  return execute((Motion*)pMotion);
}

void Geksapod::stop() {
  int pose[FOOTS_COUNT][3] = READY_POSE;
  setPose(pose, 1000);
}

MotionJob::MotionJob(Geksapod* pGeksapod): AJob(pGeksapod, MOTION_JOB_PERIOD) {
  m_pGeksapod = pGeksapod;
  m_pMotion = NULL;
  m_Status = NONE;
  m_MotionTime = 0L;
  m_TotalTime = 0L;
}

int MotionJob::execute(Motion* pMotion) {
  if(m_Status == NONE && pMotion != NULL) {

     m_pMotion = pMotion;
     m_MotionTime = 0;
     m_TotalTime = m_pMotion->totalTime();
     m_Status = RUNING;
  } else if(pMotion != NULL){
    
    if(m_pMotion->m_pNext != NULL) 
      delete m_pMotion->m_pNext;
      
    m_pMotion->m_pNext = pMotion;
     m_Status = STOPING;
  }
  return S_OK;
}

void MotionJob::onRun() {

  Vector3D points[FOOTS_COUNT];
  int pose[FOOTS_COUNT][3];
  // если движение не задано, завершаем работу
  if(m_pMotion == NULL)
    return;
  // Положение конечностей робота может быть задано как набор углов каждого сустава,
  // или как массив координат точек в которых должны располагаться концы лап робота.
  // Один из вариантов должен быть реализован.
  int res;
  if(m_pMotion->getPose(min(m_MotionTime, m_pMotion->maxTime()), pose) == S_OK) {
     res = m_pGeksapod->_setPose(pose);
  } else if(m_pMotion->getPose(min(m_MotionTime, m_pMotion->maxTime()), points) == S_OK) {
     res = m_pGeksapod->_setPose(points);
  }
  // обновляем время исполнения текущего движения
  m_MotionTime += MOTION_JOB_PERIOD;
  // если движение ограничено по времени, уменьшаем его на значение,
  // не допуская проваливание в отрицательный диапазон:
  // отрицательное значение m_TotalTime - признак неограниченного по времени движения
  if(m_TotalTime > 0) { 
    m_TotalTime -= MOTION_JOB_PERIOD;
    m_TotalTime = max(0,m_TotalTime); 
  }
  // если время выполнения зацикленного движения истекло, 
  // переводим его в состояние принудительной остановки
  if(m_TotalTime == 0 && m_pMotion->isLooped()) {
     m_Status = STOPING;
  }
  // Не циклическое движение или движении в состоянии остановки 
  // при достижении окончания должно быть завершено и удалено из памяти.
  if( (m_MotionTime - m_pMotion->maxTime() >= MOTION_JOB_PERIOD && (!m_pMotion->isLooped() || m_Status == STOPING)) ) { // время истекло
    // запоминаем указатель на возможное следующее движение
    Motion* pNextMotion = m_pMotion->m_pNext;
    m_pMotion->m_pNext = NULL;
    delete m_pMotion;
    m_pMotion = NULL;
    // и запускаем его
    m_Status = NONE;
    execute(pNextMotion);
  } else if(m_MotionTime - m_pMotion->maxTime() >= MOTION_JOB_PERIOD) {
    // для зацикленного движения не требующего остановки
    // мы сбрасываем счетчик времени и начинаем новый цикл движения
    m_MotionTime = 0; 
  }
}

void copyPose(int dst[FOOTS_COUNT][3] , const int src[FOOTS_COUNT][3]) {
  for(int i = 0; i < FOOTS_COUNT; i++) {
    dst[i][0] = src[i][0];
    dst[i][1] = src[i][1];
    dst[i][2] = src[i][2];
  }
}

int Geksapod::getAngles(Vector3D points[FOOTS_COUNT], int pose[FOOTS_COUNT][3]) {
  int ca, fa,ta;
  for(int i = 0; i < FOOTS_COUNT; i++) {
    getAngles(i, points[i], ca, fa, ta);
    pose[i][0] = ca;
    pose[i][1] = fa;
    pose[i][2] = ta;
  }
}

int Geksapod::getPoints(int pose[FOOTS_COUNT][3], Vector3D points[FOOTS_COUNT]) {
  for(int i = 0; i < FOOTS_COUNT; i++) {
    getPoint(i, pose[i][0], pose[i][1], pose[i][2], points[i]);
  }
}
