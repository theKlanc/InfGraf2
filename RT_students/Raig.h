#ifndef TAD_RAIG
#define TAD_RAIG

#include "Vector.h"

class Raig {
public:
	Vector3d O,vd;
	Raig() {};
	Raig(Vector3d p,Vector3d d): O(p),vd(d){};
};

#endif
