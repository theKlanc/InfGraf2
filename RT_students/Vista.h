#ifndef TAD_VISTA 
#define TAD_VISTA

#include "Raig.h"

class Vista 
{
public:
  Vector3d O;        // posició del observador
  Vector3d C;        // centre de la escena
  Vector3d up;       // vector amunt
  float obertura; //obertura horitzontal de la càmara.
  float rati;     //obertura vertical / obert. hor.
  int H;           // nombre de píxels verticals (height)
  int W;           // nombre de píxels horitzontals (width)
  Vector3d X,Y,Z;	   // eixos de l'observador
  float w,h;
  Vector3d incX, incY;
  Vector3d P,P0;

  Vista() {};
  Vista(Vector3d& vO, Vector3d& vC, Vector3d& vup, float vobertura, 
	float vrati, int vH, int vW);
  Raig definirRaig(int x,int y);
};
#endif
