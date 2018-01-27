#ifndef TAD_VISTA 
#define TAD_VISTA

#include "Raig.h"

class Vista 
{
public:
  Vector3d O;        // posici� del observador
  Vector3d C;        // centre de la escena
  Vector3d up;       // vector amunt
  float obertura; //obertura horitzontal de la c�mara.
  float rati;     //obertura vertical / obert. hor.
  int H;           // nombre de p�xels verticals (height)
  int W;           // nombre de p�xels horitzontals (width)
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
