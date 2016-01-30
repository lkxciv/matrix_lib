#pragma once
#include "vect3d.h"
#include "Line3d.h"
#include <string>

class Plane3d
{
private:
	//Spannvektoren und Normalenvektor
	Vect3d r1, r2, n;

	//Parameterform -> Normalenform aktualisieren
	void ParamToNormal();

	//Normalenform -> Parameterform aktualisieren
	void NormalToParam();

public:
	//Stützvektor
	Vect3d oV;

	//Konstruktor Normalenform(Stützvektor, Normalenvektor)
	Plane3d(Vect3d oV, Vect3d n);

	//Konstruktor Parameterform(Stützvektor, Spannvektoren)
	Plane3d(Vect3d oV, Vect3d r1, Vect3d r2);

	//Mindestabstand Ebene Punkt
	float MinDistEP(Vect3d point) const;

	//Mindestabstand Ebene Gerade
	float MinDistEG(Line3d & g) const;

	//Mindestabstand Zweier Ebenen
	float MinDistEE(Plane3d & e2) const;

	//Vektoren auslesen
	Vect3d GetN() const { return n; }
	Vect3d GetR1() const { return r1; }
	Vect3d GetR2() const { return r2; }

	//Schnittpunkt Ebene-Gerade ohne Überprüfung
	static Vect3d IntersectEG(Plane3d & e, Line3d & g);

	//Schnittgerade zweier Ebenen ohne Überprüfung
	static Line3d IntersectEE(Plane3d & e1, Plane3d & e2);

	//Setter
	void SetN(Vect3d v);
	void SetR1(Vect3d v);
	void SetR2(Vect3d v);

	//Parameterform zu String
	std::string ToStringParam() const;

	//Normalenform zu String
	std::string ToStringNormal() const;

	//Winkel EE
	static double Angle(Plane3d & e1, Plane3d & e2);

	//Winkel EG
	static double Angle(Plane3d & e, Line3d & g);
};