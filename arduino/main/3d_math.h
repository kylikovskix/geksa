#ifndef MATH_3D_H
#define MATH_3D_H

struct Vector3D {
  float x,y,z;

  Vector3D operator -(void) {
    return {-x, -y ,-z};
  }                        

  Vector3D& operator=(const Vector3D a);
  
  float len();
};

Vector3D operator +=(Vector3D left, const Vector3D right);
Vector3D operator -=(Vector3D left, const Vector3D right);
Vector3D operator- (Vector3D a, Vector3D b);
Vector3D operator+ (Vector3D a, Vector3D b);
Vector3D operator* (Vector3D a, Vector3D b);
Vector3D operator* (float a, Vector3D b);
Vector3D operator* (Vector3D a, float b);
Vector3D operator/ (Vector3D a, int b);

struct Matrix3D {
  float a[3][3];

  Vector3D operator *(Vector3D& v) {
    Vector3D p;
    p.x = v.x*a[0][0] + v.y*a[0][1] + v.z*a[0][2];
    p.y = v.x*a[1][0] + v.y*a[1][1] + v.z*a[1][2];
    p.z = v.x*a[2][0] + v.y*a[2][1] + v.z*a[2][2];
    return p;
  };                                               

  Matrix3D operator *(Matrix3D m) {
    Matrix3D r;
    for(int i=0; i<3; i++) {
      for(int j=0; j<3; j++) {
        r.a[i][j] = 0;
        for(int k=0; k<3; k++)
          r.a[i][j] += a[i][k]*m.a[k][j];
      }
    }
    return r;
  };
};

Matrix3D rotMatrix(Vector3D r);
Matrix3D rotMatrix2(Vector3D r);
float arcctn(float);

#endif
