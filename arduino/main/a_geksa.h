#ifndef A_GEKSA_H
#define A_GEKSA_H

#include "a_job.h"
#include "g_foot.h"

// идентификаторы лап робота
#define LEFT_FRONT_FOOT_IDX   0
#define LEFT_MIDLE_FOOT_IDX   1
#define LEFT_BACK_FOOT_IDX    2
#define RIGTH_FRONT_FOOT_IDX  3
#define RIGTH_MIDLE_FOOT_IDX  4
#define RIGTH_BACK_FOOT_IDX   5
#define FOOTS_COUNT  6            // общее количество лап

#include "motion.h"

class Motion;
class Geksapod;
class CommandProcessorJob;

class MotionJob: public AJob {
  enum STATUS { 
    NONE, RUNING, STOPING 
   } m_Status;
  
  Geksapod* m_pGeksapod;
  Motion* m_pMotion;
  long m_MotionTime;
  long m_TotalTime;
protected:
  void onRun();
public:  
  MotionJob(Geksapod* pGeksapod);
  int execute(Motion* pMotion);
};

class Geksapod: public AJobManager {
//  friend class FootsCtrlJob;
  friend class MotionJob;
  friend CommandProcessorJob;
  
  GeksaFoot m_LeftFrontFoot;
  GeksaFoot m_LeftMidleFoot;
  GeksaFoot m_LeftBackFoot;
  GeksaFoot m_RigthFrontFoot;
  GeksaFoot m_RigthMidleFoot;
  GeksaFoot m_RigthBackFoot;

  MotionJob m_MotionJob;
protected:
  int _setPose(int idx, int ca, int fa, int ta);
  int _setPose(int[FOOTS_COUNT][3]);  
  int _setPose(Vector3D points[FOOTS_COUNT]);  
  
  int setPose(int idx, int ca, int fa, int ta, int actionTime); 
  int setPose(int pose[FOOTS_COUNT][3], int actionTime);
  int setPose(int idx, Vector3D p, int actionTime);
  int setPose(Vector3D points[FOOTS_COUNT], int actionTime = 0);
  int setPose(int ca, int fa, int ta, int actionTime);
  
  void getPose(int idx, int& ca, int& fa, int& ta);
  void getPose(int pose[FOOTS_COUNT][3]);
  void getPose(int idx, Vector3D& p);
  void getPose(Vector3D points[FOOTS_COUNT]);

  int execute(Motion* pMotion);
  
public:
  Geksapod();
  void setup();

  int move(int speed, int time);
  int rotate(int speed, int time);
  void stop();

  int getAngles(int idx, Vector3D p, int& ca, int& fa, int& ta);
  int getPoint(int idx, int coxaAngle, int femoraAngle, int tibiaAngle, Vector3D& p);

  int getAngles(Vector3D points[FOOTS_COUNT], int pose[FOOTS_COUNT][3]);
  int getPoints(int pose[FOOTS_COUNT][3], Vector3D points[FOOTS_COUNT]);
};

#endif
