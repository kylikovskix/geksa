#include "motion.h"
#include <arduino.h>
#include <math.h>

void linearFunc(float a, Vector3D offset, Vector3D p0, float k,  Vector3D& res) {
  res = a*offset*k + p0;
}

void rotFunc(float a, Vector3D angles, Vector3D p0, float k, Vector3D& res) {
  Matrix3D m = rotMatrix2(a*angles*k);
  res = m*p0;
}

void copyPose(Vector3D dst[FOOTS_COUNT] ,Vector3D src[FOOTS_COUNT]) {
  for(int i = 0; i < FOOTS_COUNT; i++) {
    dst[i] = src[i];
  }
}

Motion::Motion(long maxTime, bool isLooped, long totalTime, Motion* pNext = NULL) {
  m_MaxTime = maxTime;
  m_TotalTime = totalTime;
  m_IsLooped = isLooped;
  m_pNext = pNext;
}

Motion::~Motion() {
  if(m_pNext != NULL)
    delete m_pNext;
  m_pNext = NULL;
}

TransPoseMotion::TransPoseMotion(Vector3D begPose[FOOTS_COUNT], Vector3D endPose[FOOTS_COUNT], long maxTime, Motion* pNext) : Motion(maxTime, -1, pNext) {
  for(int i = 0; i < FOOTS_COUNT; i++) {
    m_Pose0[i] = begPose[i];
    m_Offset[i] = endPose[i] - begPose[i];
  }
}

int TransPoseMotion::getPose(long time, Vector3D points[FOOTS_COUNT]) {
    for(int i = 0; i < FOOTS_COUNT; i++)
      linearFunc(1.0, m_Offset[i], m_Pose0[i], 1.0*time/m_MaxTime, points[i]);
}

LinearMotion::LinearMotion(int heigth, Vector3D pose0[FOOTS_COUNT], Vector3D speed, long maxTime, long totalTime, Motion* pNext = NULL) : Motion(maxTime, true, totalTime, pNext) {
  copyPose(m_Pose0, pose0);
  m_Offset = 1.0*speed*maxTime/1000;
  m_Heigth = heigth;
}

int LinearMotion::getPose(long time, Vector3D points[FOOTS_COUNT]) {
  double k = 1.0*time/m_MaxTime;

  if(k < 0.25) {
    
    linearFunc(2.0, -m_Offset, m_Pose0[LEFT_FRONT_FOOT_IDX], k, points[LEFT_FRONT_FOOT_IDX]);
    linearFunc(2.0, -m_Offset, m_Pose0[LEFT_BACK_FOOT_IDX], k, points[LEFT_BACK_FOOT_IDX]);
    linearFunc(2.0, -m_Offset, m_Pose0[RIGTH_MIDLE_FOOT_IDX], k, points[RIGTH_MIDLE_FOOT_IDX]);

    linearFunc(2.0, m_Offset, m_Pose0[LEFT_MIDLE_FOOT_IDX], k, points[LEFT_MIDLE_FOOT_IDX]);
    linearFunc(2.0, m_Offset, m_Pose0[RIGTH_FRONT_FOOT_IDX], k, points[RIGTH_FRONT_FOOT_IDX]);
    linearFunc(2.0, m_Offset, m_Pose0[RIGTH_BACK_FOOT_IDX], k, points[RIGTH_BACK_FOOT_IDX]);
  
    int h = (int)m_Heigth*cos(M_PI*k*2);
    points[LEFT_MIDLE_FOOT_IDX].z += h;
    points[RIGTH_FRONT_FOOT_IDX].z += h;
    points[RIGTH_BACK_FOOT_IDX].z += h;

  } else if(k < 0.5) {
    
    linearFunc(2.0, m_Offset, m_Pose0[LEFT_FRONT_FOOT_IDX], k - 0.5, points[LEFT_FRONT_FOOT_IDX]);
    linearFunc(2.0, m_Offset, m_Pose0[LEFT_BACK_FOOT_IDX], k - 0.5, points[LEFT_BACK_FOOT_IDX]);
    linearFunc(2.0, m_Offset, m_Pose0[RIGTH_MIDLE_FOOT_IDX], k - 0.5, points[RIGTH_MIDLE_FOOT_IDX]);

    linearFunc(2.0, -m_Offset, m_Pose0[LEFT_MIDLE_FOOT_IDX], k - 0.5, points[LEFT_MIDLE_FOOT_IDX]);
    linearFunc(2.0, -m_Offset, m_Pose0[RIGTH_FRONT_FOOT_IDX], k - 0.5, points[RIGTH_FRONT_FOOT_IDX]);
    linearFunc(2.0, -m_Offset, m_Pose0[RIGTH_BACK_FOOT_IDX], k - 0.5, points[RIGTH_BACK_FOOT_IDX]);

    int h = (int)m_Heigth*sin(M_PI*(k - 0.25)*2);
    points[LEFT_FRONT_FOOT_IDX].z += h;
    points[LEFT_BACK_FOOT_IDX].z += h;
    points[RIGTH_MIDLE_FOOT_IDX].z += h;

  } else if(k < 0.75) {
    
    linearFunc(2.0, m_Offset, m_Pose0[LEFT_FRONT_FOOT_IDX], k - 0.5, points[LEFT_FRONT_FOOT_IDX]);
    linearFunc(2.0, m_Offset, m_Pose0[LEFT_BACK_FOOT_IDX], k - 0.5, points[LEFT_BACK_FOOT_IDX]);
    linearFunc(2.0, m_Offset, m_Pose0[RIGTH_MIDLE_FOOT_IDX], k - 0.5, points[RIGTH_MIDLE_FOOT_IDX]);

    linearFunc(2.0, -m_Offset, m_Pose0[LEFT_MIDLE_FOOT_IDX], k - 0.5, points[LEFT_MIDLE_FOOT_IDX]);
    linearFunc(2.0, -m_Offset, m_Pose0[RIGTH_FRONT_FOOT_IDX], k - 0.5, points[RIGTH_FRONT_FOOT_IDX]);
    linearFunc(2.0, -m_Offset, m_Pose0[RIGTH_BACK_FOOT_IDX], k - 0.5, points[RIGTH_BACK_FOOT_IDX]);
    
    int h = (int)m_Heigth*cos(M_PI*(k - 0.5)*2);
    points[LEFT_FRONT_FOOT_IDX].z += h;
    points[LEFT_BACK_FOOT_IDX].z += h;
    points[RIGTH_MIDLE_FOOT_IDX].z += h;
    
  } else {
    
    linearFunc(2.0, m_Offset, m_Pose0[LEFT_FRONT_FOOT_IDX], 1 - k, points[LEFT_FRONT_FOOT_IDX]);
    linearFunc(2.0, m_Offset, m_Pose0[LEFT_BACK_FOOT_IDX], 1 - k, points[LEFT_BACK_FOOT_IDX]);
    linearFunc(2.0, m_Offset, m_Pose0[RIGTH_MIDLE_FOOT_IDX], 1 - k, points[RIGTH_MIDLE_FOOT_IDX]);

    linearFunc(2.0, -m_Offset, m_Pose0[LEFT_MIDLE_FOOT_IDX], 1 - k, points[LEFT_MIDLE_FOOT_IDX]);
    linearFunc(2.0, -m_Offset, m_Pose0[RIGTH_FRONT_FOOT_IDX], 1 - k, points[RIGTH_FRONT_FOOT_IDX]);
    linearFunc(2.0, -m_Offset, m_Pose0[RIGTH_BACK_FOOT_IDX], 1 - k, points[RIGTH_BACK_FOOT_IDX]);
    
    int h = (int)m_Heigth*sin(M_PI*(k - 0.75)*2);
    points[LEFT_MIDLE_FOOT_IDX].z += h;
    points[RIGTH_FRONT_FOOT_IDX].z += h;
    points[RIGTH_BACK_FOOT_IDX].z += h;
  }

  return S_OK;
}

RotateMotion::RotateMotion(int heigth, Vector3D pose0[FOOTS_COUNT], Vector3D speed, long maxTime, long totalTime = -1, Motion* pNext = NULL) : Motion(maxTime, true, totalTime, pNext) {
  copyPose(m_Pose0, pose0);
  m_Angles = 0.001*maxTime*speed*M_PI/180;
  m_Heigth = heigth;
}

int RotateMotion::getPose(long time, Vector3D points[FOOTS_COUNT]) {
  double k = 1.0*time/m_MaxTime;
  if(k < 0.25) {

    rotFunc(2.0, m_Angles, m_Pose0[LEFT_FRONT_FOOT_IDX], k, points[LEFT_FRONT_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_BACK_FOOT_IDX], k, points[LEFT_BACK_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_MIDLE_FOOT_IDX], k, points[RIGTH_MIDLE_FOOT_IDX]);
      
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_MIDLE_FOOT_IDX], -k, points[LEFT_MIDLE_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_FRONT_FOOT_IDX], -k, points[RIGTH_FRONT_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_BACK_FOOT_IDX], -k, points[RIGTH_BACK_FOOT_IDX]);

    int h = (int)m_Heigth*cos(M_PI*k*2);
    points[LEFT_MIDLE_FOOT_IDX].z += h;
    points[RIGTH_FRONT_FOOT_IDX].z += h;
    points[RIGTH_BACK_FOOT_IDX].z += h;

  } else if(k < 0.5) {
    
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_FRONT_FOOT_IDX], 0.5 - k, points[LEFT_FRONT_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_BACK_FOOT_IDX], 0.5 - k, points[LEFT_BACK_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_MIDLE_FOOT_IDX], 0.5 - k, points[RIGTH_MIDLE_FOOT_IDX]);
      
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_MIDLE_FOOT_IDX], k - 0.5, points[LEFT_MIDLE_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_FRONT_FOOT_IDX], k - 0.5, points[RIGTH_FRONT_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_BACK_FOOT_IDX], k - 0.5, points[RIGTH_BACK_FOOT_IDX]);
  
    int h = (int)m_Heigth*sin(M_PI*(k - 0.25)*2);
    points[LEFT_FRONT_FOOT_IDX].z += h;
    points[LEFT_BACK_FOOT_IDX].z += h;
    points[RIGTH_MIDLE_FOOT_IDX].z += h;

  } else if(k < 0.75) {
  
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_FRONT_FOOT_IDX], 0.5 - k, points[LEFT_FRONT_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_BACK_FOOT_IDX], 0.5 - k, points[LEFT_BACK_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_MIDLE_FOOT_IDX], 0.5 - k, points[RIGTH_MIDLE_FOOT_IDX]);
      
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_MIDLE_FOOT_IDX], k - 0.5, points[LEFT_MIDLE_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_FRONT_FOOT_IDX], k - 0.5, points[RIGTH_FRONT_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_BACK_FOOT_IDX], k - 0.5, points[RIGTH_BACK_FOOT_IDX]);
  
    int h = (int)m_Heigth*cos(M_PI*(k - 0.5)*2);
    points[LEFT_FRONT_FOOT_IDX].z += h;
    points[LEFT_BACK_FOOT_IDX].z += h;
    points[RIGTH_MIDLE_FOOT_IDX].z += h;
  } else {
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_FRONT_FOOT_IDX], k - 1, points[LEFT_FRONT_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_BACK_FOOT_IDX], k - 1, points[LEFT_BACK_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_MIDLE_FOOT_IDX], k - 1, points[RIGTH_MIDLE_FOOT_IDX]);
      
    rotFunc(2.0, m_Angles, m_Pose0[LEFT_MIDLE_FOOT_IDX], 1 - k, points[LEFT_MIDLE_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_FRONT_FOOT_IDX], 1 - k, points[RIGTH_FRONT_FOOT_IDX]);
    rotFunc(2.0, m_Angles, m_Pose0[RIGTH_BACK_FOOT_IDX], 1 - k, points[RIGTH_BACK_FOOT_IDX]);

    int h = (int)m_Heigth*sin(M_PI*(k - 0.75)*2);
    points[LEFT_MIDLE_FOOT_IDX].z += h;
    points[RIGTH_FRONT_FOOT_IDX].z += h;
    points[RIGTH_BACK_FOOT_IDX].z += h;
  }
  
  return S_OK;
}

TransAnglesMotion::TransAnglesMotion(int begPose[FOOTS_COUNT][3], int endPose[FOOTS_COUNT][3], long maxTime, Motion* pNext) : Motion(maxTime, -1, pNext) {
  for(int i = 0; i < FOOTS_COUNT; i++) {
    for(int j = 0; j < 3; j++) {
      m_Pose0[i][j] = begPose[i][j];
      m_Offset[i][j] = endPose[i][j] - begPose[i][j];
    }
  }
}

int TransAnglesMotion::getPose(long time, int pose[FOOTS_COUNT][3]) {
  for(int i = 0; i < FOOTS_COUNT; i++) {
    for(int j = 0; j < 3; j++) 
      pose[i][j] = m_Pose0[i][j] + (char)(1.0*m_Offset[i][j]*time/m_MaxTime);
  }
  return S_OK;
}
