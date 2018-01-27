#include "Vista.h" 
#include <math.h> 
#include <stdio.h> 
 
Vista::Vista(Vector3d& vO, Vector3d& vC, Vector3d& vup,  
	     float vobertura, float vrati, int vH, int vW):  
  O(vO),C(vC),up(vup),obertura(vobertura),rati(vrati),H(vH),W(vW)  
{ 
  Z = (O - C).normalitza(); 
  X = up.prodVect(Z).normalitza(); 
  Y = Z.prodVect(X); 
   
  w = 2.0f*(float)tan((obertura * 3.14159f/180.0f)/2.0f); 
  h = w * rati; 
  incX = X*(w/(float)W); 
  incY = Y*(h/(float)H); 
  P = O-Z; 
  Vector3d vtemp = incX*(float(W-1)/2.0f); 
  Vector3d vtemp2 = incY*(float(H-1)/2.0f); 
 
  Vector3d aux = vtemp + vtemp2; 
  P0 = P - aux; 
} 
 
Raig  
Vista::definirRaig(int x,int y) 
{ 
  Raig r; 
  r.O=this->O; 
  Vector3d vtemp = incX*float(x); 
  Vector3d vtemp1 = incY*float(y); 
  Vector3d vtemp2 = vtemp + vtemp1; 
  Vector3d vtemp3 = this->P0 + vtemp2; 
  Vector3d vtemp4 = vtemp3 - r.O; 
  r.vd=(vtemp4).normalitza(); 
  return r; 
} 
 

