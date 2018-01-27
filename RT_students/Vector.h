#ifndef TAD_VECTOR3d
#define TAD_VECTOR3d

class Vector3d 
{
public:
  float x,y,z;
  
  Vector3d(float a=0.0,float b=0.0,float c=0.0);
  float modul2();
  float modul(); 
  Vector3d& normalitza();
  Vector3d prodVect(Vector3d &v);
  float prodEsc(Vector3d &v);
  Vector3d operator-(Vector3d &v);
  Vector3d operator+(Vector3d &v) const;
  Vector3d operator*(float r);
  Vector3d operator/(float r);

  Vector3d & operator+=(const Vector3d &v) 
  {
    x += v.x; y += v.y; z += v.z;
    return *this;
  }
  Vector3d & operator-=(const Vector3d &v) {
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
  }
  Vector3d & operator*=(float f) 
  {
    x *= f; y *= f; z *= f;
    return *this;
  }
  Vector3d &operator/=(float f) 
  {
    if (f!=0)
      {
	float inv = 1.f / f;
	x *= inv; y *= inv; z *= inv;
	return *this;
      }
    // else
  }
  
  Vector3d reflexa(Vector3d &N);
};
#endif
