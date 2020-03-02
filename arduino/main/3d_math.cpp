#include "3d_math.h"
#include <math.h>

Vector3D& Vector3D::operator= (const Vector3D a) {
  x = a.x;
  y = a.y;
  z = a.z;
  return *this;
}

float Vector3D::len() {
  return sqrt(x*x + y*y + z*z);
}

Vector3D operator +=(Vector3D left, const Vector3D right) {
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;   
    return left;  
}

Vector3D operator -=(Vector3D left, const Vector3D right) {
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;   
    return left;  
}

Vector3D operator- (Vector3D a, Vector3D b) {
  Vector3D res;
  res.x = a.x - b.x;
  res.y = a.y - b.y;
  res.z = a.z - b.z;
  return res;
}

Vector3D operator+ (Vector3D a, Vector3D b) {
  Vector3D res;
  res.x = a.x + b.x;
  res.y = a.y + b.y;
  res.z = a.z + b.z;
  return res;
}

Vector3D operator* (Vector3D a, Vector3D b) {
  Vector3D res;
  res.x = a.x * b.x;
  res.y = a.y * b.y;
  res.z = a.z * b.z;
  return res;
}

Vector3D operator* (float a, Vector3D b) {
  Vector3D res;
  res.x = a * b.x;
  res.y = a * b.y;
  res.z = a * b.z;
  return res;
}

Vector3D operator* (Vector3D a, float b) {
  Vector3D res;
  res.x = a.x * b;
  res.y = a.y * b;
  res.z = a.z * b;
  return res;
}

Vector3D operator/ (Vector3D a, int b) {
  Vector3D res;
  res.x = 1.0*a.x / b;
  res.y = 1.0*a.y / b;
  res.z = 1.0*a.z / b;
  return res;
}

Matrix3D rotMatrix(Vector3D r) {
  Matrix3D mx = { 1, 0, 0, 
                  0, cos(r.x), -sin(r.x), 
                  0, sin(r.x), cos(r.x) 
  }; 

  Matrix3D my = { cos(r.y), 0, sin(r.y), 
                  0, 1, 0, 
                  -sin(r.y), 0, cos(r.y) 
  }; 

  Matrix3D mz = { cos(r.z), -sin(r.z), 0, 
                  sin(r.z), cos(r.z), 0,
                  0, 0, 1 
  };
  return mx*my*mz;   
}

Matrix3D rotMatrix2(Vector3D r) {
  Vector3D v;
  v.x = M_PI*r.x/180;
  v.y = M_PI*r.y/180;
  v.z = M_PI*r.z/180;
  return rotMatrix(v);   
}

float arcctn(float a) {
  float result = asin(1/sqrt(1+a*a)); 
  return a >= 0 ? result : M_PI - result;
}
