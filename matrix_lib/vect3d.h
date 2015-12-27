#include <string>
#ifndef VECT3D
#define VECT3D

class Vect3d
{
public:
	float vect[3];

	Vect3d();

	Vect3d(float x, float y, float z);

	~Vect3d();

	//Berechnet den Fl�cheninhalt des aufgespannten Spats
	static float Area(Vect3d v1, Vect3d v2);

	//Berechnet Volumen des Aufgespannten Polytops
	//V = |(v1 x v2) * v3|
	static float Volume(Vect3d v1, Vect3d v2, Vect3d v3);

	//Berechnet betrag von Projektion auf v2
	float AbsProjectOn(Vect3d v2);

	//Berechnet Vektor von Projetion auf v2
	Vect3d ProjectOn(Vect3d v2);

	//Berechnet Vektorbetrag
	float AbsV();

	//Berechnet Skalarprodukt
	static float DotP(Vect3d v1, Vect3d v2);

	//Berechnet Kreuzprodukt
	static Vect3d CrossP(Vect3d v1, Vect3d v2);

	//Vektor wird als String ausgegeben
	std::string ToString();

	//Pr�ft Linearabh�ngigkeit
	static bool LinDep(Vect3d v1, Vect3d v2);

	//Berechnet Winkel zw. 2 Vektoren
	static double Angle(Vect3d v1, Vect3d v2);

	//Berechnet Winkel zw. 2 Vektoren mit Betrag im Z�hler f�r Objekte
	static double AngleAbs(Vect3d v1, Vect3d v2);

	//Berechnet Einheitsvektor
	static Vect3d UnitV(Vect3d v1);

	//�berladung addition
	Vect3d operator+(Vect3d v2) const;

	//�berladung +=
	Vect3d operator+=(Vect3d v2);

	//�berladunng subtraktion
	Vect3d operator-(Vect3d v2) const;

	//�berladung -=
	Vect3d operator-=(Vect3d v2);

	//�berladung multiplikation mit skalar
	Vect3d operator*(float skalar) const;

	//�berladung multiplikation mit skalar
	Vect3d operator*=(float skalar);

	//�berladung division mit skalar
	Vect3d operator/(float skalar);

	//�berladung division mit skalar
	Vect3d operator/=(float skalar);

	//�berladung multiplikation durch skalaprodukt zweier vektoren
	float operator*(Vect3d v2) const;

	//Vergleichsoperator
	bool operator==(Vect3d v2) const;

	//Vergleichsoperator
	bool operator!=(Vect3d v2) const;
};

#endif // !VECT3D
