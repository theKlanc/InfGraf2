#include "Vector.h"
#include <math.h>

Vector3d::Vector3d(float a,float b,float c) {
	x=a; y=b; z=c;
}

float 
Vector3d::modul2() { return (x*x+y*y+z*z); }

float 
Vector3d::modul() { return (float)sqrt(modul2()); }

Vector3d& 
Vector3d::normalitza() {
	float r=modul();
	x/=r; y/=r; z/=r;
	return *this;
}

Vector3d 
Vector3d::prodVect(Vector3d &v) {
	return *new Vector3d(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
}

float 
Vector3d::prodEsc(Vector3d &v) {
	return (x*v.x+y*v.y+z*v.z);
}

Vector3d 
Vector3d::operator-(Vector3d &v) {
	return Vector3d(x-v.x,y-v.y,z-v.z);
}

Vector3d 
Vector3d::operator+(Vector3d &v) const{
	return Vector3d(x+v.x,y+v.y,z+v.z);
}

Vector3d 
Vector3d::operator*(float r){
	return Vector3d(x*r,y*r,z*r);
}

Vector3d 
Vector3d::operator/(float r){
	return Vector3d(x/r,y/r,z/r);
}

// Calcula la reflexió del vector I amb la normal N. 
// Tots dos vectors han de ser normalitzats.
Vector3d 
Vector3d::reflexa(Vector3d &N)
{
  float cosAlfa=prodEsc(N);
  Vector3d vtemp = N*cosAlfa*2.0;
  return *this + vtemp;
}


