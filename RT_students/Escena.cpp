#include "Escena.h" 
#include <math.h> 
#include <GL/glut.h> 
#include <algorithm>

bool
Esfera::intersecta(Raig r, Hit& hit) {
	float xA = r.O.x;
	float yA = r.O.y;
	float zA = r.O.z;
	Vector3d punt2 = r.O + r.vd;
	float xB = punt2.x;
	float yB = punt2.y;
	float zB = punt2.z;
	float xC = O.x;
	float yC = O.y;
	float zC = O.z;
	float a = pow((xB - xA), 2) + pow((yB - yA), 2) + pow((zB - zA), 2);
	float b = 2 * ((xB - xA)*(xA - xC) + (yB - yA)*(yA - yC) + (zB - zA)*(zA - zC));
	float c = pow((xA - xC), 2) + pow((yA - yC), 2) + pow((zA - zC), 2) - pow(R, 2);
	float Delta = pow(b, 2) - (4*a*c);
	if (Delta < 0)return false;
	else {
		if (Delta == 0) {
			float d = -b / 2*a;
			hit.o = this;
			hit.p = Vector3d(xA + d * (xB - xA), yA + d * (yB - yA), zA + d * (zB - zA));
			hit.N = (hit.p - O).normalitza();
		}
		else {
			float d1 = (-b - sqrt(Delta)) / (2*a);
			float d2 = (-b + sqrt(Delta)) / (2 * a);
			Vector3d hit1 = Vector3d(xA + d1 * (xB - xA), yA + d1 * (yB - yA), zA + d1 * (zB - zA));
			Vector3d hit2 = Vector3d(xA + d2 * (xB - xA), yA + d2 * (yB - yA), zA + d2 * (zB - zA));
			hit.o = this;
			hit.p = ((r.O-hit1).modul()<(r.O-hit2).modul()?hit1:hit2);
			hit.N = (hit.p - O).normalitza();
		}
		return true;
	}
}

void
Esfera::dibuixa() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difus);
	glMaterialfv(GL_FRONT, GL_SPECULAR, especular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	//printf("dibuixa esfera\n"); 
	glPushMatrix();
	Color c = this->getColor();
	glColor3f(c.r, c.g, c.b);
	Vector3d O = this->O;
	glTranslatef(O.x, O.y, O.z);
	glutSolidSphere(this->R, 40, 40);
	glPopMatrix();
}

void
Focus::lightGL() {

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

#define INFINIT 1e10 

// Calcula la intersecció del raig r amb l'escena 
// cal que la direcció del raig estigui normalitzada 
bool
Escena::intersecta(Raig r, Hit& minHit, Objecte *excepte) {
	Hit hit;
	bool intersecta = false;

	std::vector<Hit> hits;
	for (Objecte *o : objectes) {
		Hit temp;
		if (o->intersecta(r, temp)) {
			hits.push_back(temp);
			intersecta = true;
		}
	}
	if (intersecta) {
		minHit = hits[0];
		for (Hit h : hits) {
			float candidat = (r.O - h.p).modul();//sqrt(pow(r.O.x - h.p.x, 2) + pow(r.O.y - h.p.y, 2) + pow(r.O.z - h.p.z, 2));
			float minim = (r.O - minHit.p).modul();//sqrt(pow(r.O.x - minHit.p.x, 2) + pow(r.O.y - minHit.p.y, 2) + pow(r.O.z - minHit.p.z, 2));
			if (candidat < minim) {
				minHit = h;
			}
		}
	}
	return intersecta;
}

void
Escena::dibuixa() {
	for (iObjectes i = objectes.begin(); i != objectes.end(); i++)
		(*i)->dibuixa();
}

void
Escena::lightGL() {
	int l = 0;
	for (iFocus i = focus.begin(); i != focus.end(); i++, l++) {
		switch (l) {
		case 0:
			glLightfv(GL_LIGHT0, GL_POSITION, (*i)->light_position);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, (*i)->light_diffuse);
			glLightfv(GL_LIGHT0, GL_AMBIENT, (*i)->light_ambient);
			glLightfv(GL_LIGHT0, GL_SPECULAR, (*i)->light_specular);
			glEnable(GL_LIGHT0);
			break;
		case 1:
			glLightfv(GL_LIGHT1, GL_POSITION, (*i)->light_position);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, (*i)->light_diffuse);
			glLightfv(GL_LIGHT1, GL_AMBIENT, (*i)->light_ambient);
			glLightfv(GL_LIGHT1, GL_SPECULAR, (*i)->light_specular);
			glEnable(GL_LIGHT1);
			break;
		}
	}
}

Color
Escena::ilumina(Raig r, int nivell, Objecte *excepte) {
	Color c = Color(0, 0, 0);

	Hit temp;
	if (intersecta(r, temp)) {
		c = temp.o->getColor()*Ia;
		for(Focus *f : focus) {
			Vector3d difusa(f->light_diffuse[0], f->light_diffuse[1], f->light_diffuse[2]);
			Vector3d specular(f->light_specular[0], f->light_specular[1], f->light_specular[2]);
			Vector3d ambient(f->light_ambient[0], f->light_ambient[1], f->light_ambient[2]);
			Vector3d reflexio = f->P.reflexa(temp.N);
			Vector3d vectorIlum(f->P - temp.p);
			Vector3d vObs(r.O - temp.p);
			double cosPhi = vectorIlum.normalitza().prodEsc(vObs.reflexa(temp.N).normalitza());//RM*V
			double cosTheta = temp.N.normalitza().prodEsc(vectorIlum.normalitza());
			c = c + Color(difusa*cosTheta)*temp.o->getColor() + ((Color(specular)*temp.o->getColor()) * pow(cosPhi, temp.o->getKs()));
			//Difusa
		}
	}
	else {
		c = colorFons;
	}

	return c;
}


const int Escena::AMBIENT = 1;
const int Escena::DIFOSA = 2;
const int Escena::ESPECULAR = 4;
const int Escena::OMBRES = 8;
const int Escena::REFLEXIO = 16;
const int Escena::REFRACCIO = 32;

const int Escena::MAX_NIVELLS_RECURSIO = 1;
