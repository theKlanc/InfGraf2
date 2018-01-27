#ifndef TAD_ESCENA
#define TAD_ESCENA

#include <math.h>
#include <vector>
#include <GL/glut.h>
#include "Vector.h"
#include "Raig.h"
#include "Vista.h"

class Hit;

class Color {
public:
	float r, g, b;
	Color(float pr = 0, float pg = 0, float pb = 0) :r(pr), g(pg), b(pb) {};
	Color(Vector3d t) :r(t.x), g(t.y), b(t.z) {};
	void clip() {
		Color c = (*this)*255.0;
		if (c.r > 255.0) c.r = 255.0;
		if (c.g > 255.0) c.g = 255.0;
		if (c.b > 255.0) c.b = 255.0;
	};
	Color operator+(Color &c) { return Color(r + c.r, g + c.g, b + c.b); };
	Color operator*(Color &c) { return Color(r*c.r, g*c.g, b*c.b); };
	Color operator*(float c) { return Color(r*c, g*c, b*c); };
	Color operator^(float c) { return Color(pow(r, c), pow(g, c), pow(b, c));}
};

class Focus {
public:
	Color color;
	Vector3d P;
	GLfloat light_ambient[4];
	GLfloat light_diffuse[4];
	GLfloat light_specular[4];
	GLfloat light_position[4];

	Focus(Color &c, Vector3d& v) : color(c), P(v) {
		light_ambient[0] = 0.0; light_ambient[1] = 0.0; light_ambient[2] = 0.0;
		light_ambient[3] = 1.0;
		light_diffuse[0] = color.r; light_diffuse[1] = color.g;
		light_diffuse[2] = color.b; light_diffuse[3] = 1.0;
		light_specular[0] = color.r; light_specular[1] = color.g;
		light_specular[2] = color.b; light_specular[3] = 1.0;
		light_position[0] = P.x; light_position[1] = P.y; light_position[2] = P.z;
		light_position[3] = 1.0;
	};
	void lightGL();
};

class Objecte {
public:
	Color color; // color de l'objecte (kd)
	float ks;   // constant d'especularitat (Punts brillats) de 0..infinit
	float kr;   // constant de reflexió (miralls) de 0(gens mirall)..1(mirall perfecte)

	Objecte(Color c, float kEspec = 0, float kReflex = 0) :color(c), ks(kEspec), kr(kReflex) {};
	Color getColor() { return color; };
	float getKs() { return ks; };
	float getKr() { return kr; };
	virtual bool intersecta(Raig r, Hit &h) = 0;
	virtual void dibuixa() = 0;
};

class Esfera : public Objecte {
public:
	Vector3d O;
	float R;

	GLfloat ambient[4];
	GLfloat difus[4];
	GLfloat especular[4];
	GLfloat shininess[1];
	Esfera(Vector3d o, float r, Color c, float ks = 0, float kr = 0) :
		Objecte(c, ks, kr), O(o), R(r) {
		shininess[0] = ks;
		ambient[0] = 0.f; ambient[1] = 0.f; ambient[2] = 0.f; ambient[3] = 1.0;
		difus[0] = c.r; difus[1] = c.g; difus[2] = c.b; difus[3] = 1.0;
		especular[0] = c.r; especular[1] = c.g; especular[2] = c.b; especular[3] = 1.0;
	};
	virtual bool intersecta(Raig r, Hit &h);
	void dibuixa();
};

class Cilindro : public Objecte {
public:
	Vector3d O, L;
	float R;
	Cilindro(Vector3d o, Vector3d l, float r, Color c,
		float ks = 0, float kr = 0) :
		Objecte(c, ks, kr), O(o), L(l), R(r) {};
	virtual bool intersecta(Raig r, Hit &h) {}
};

class Cara4 : public Objecte {
public:
	Vector3d pol_[4];
	float a_, b_, c_, d_;
	Cara4(const Vector3d &p1, const Vector3d & p2, const Vector3d & p3,
		const Vector3d & p4, Color c, float ks = 0, float kr = 0) :
		Objecte(c, ks, kr) {
		pol_[0] = p1; pol_[1] = p2; pol_[2] = p3; pol_[3] = p4;
		initpol();
	};
	void initpol() {
		Vector3d v1 = pol_[0] - pol_[1], v2 = pol_[1] - pol_[2];
		Vector3d n = v1.prodVect(v2);
		n.normalitza();
		a_ = n.x; b_ = n.y; c_ = n.z; d_ = -n.prodEsc(pol_[0]);
	}

	virtual bool intersecta(Raig r, Hit &h);
	bool intersectPlane(Raig r, Vector3d &p);
};

class Hit {
public:
	Vector3d p, N;
	Objecte* o;
};


class Escena {
private:
	int configuracio;
	Vista vista;		// Definició de la càmara
	std::vector<Objecte*> objectes;		// Objectes de l'escena
	Color Ia;				// Llum ambient de l'escena
	Color colorFons;			// Color de fons
	std::vector<Focus*> focus;		// Focus de llum

public:
	static const int AMBIENT;
	static const int DIFOSA;
	static const int ESPECULAR;
	static const int OMBRES;
	static const int REFLEXIO;
	static const int REFRACCIO;
	static const int MAX_NIVELLS_RECURSIO;

	typedef std::vector<Objecte*>::iterator iObjectes; // iterador del objectes 
	typedef std::vector<Focus*>::iterator iFocus;	// iterador dels focus

	void configura(int c) { configuracio = c; };
	void defVista(Vista v) { vista = v; };
	Vista getVista() { return vista; }
	void defLlumAmbient(Color c) { Ia = c; };
	void defColorFons(Color c) { colorFons = c; }
	void afegirObjecte(Objecte* o) { objectes.push_back(o); }
	void afegirFocus(Focus* f) { focus.push_back(f); }
	bool intersecta(Raig r, Hit& h, Objecte *excepte = NULL);
	Color ilumina(Raig r, int nivell = 0, Objecte *excepte = NULL);
	Raig definirRaig(int x, int y) { return vista.definirRaig(x, y); }
	void dibuixa();
	void lightGL();
};

#endif
