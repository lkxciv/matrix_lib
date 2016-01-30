#pragma once
#include "vect3d.h"
#include <string>

class Line3d
{
public:
	Vect3d pV;//Stützvektor
	Vect3d rV;//Richtungsvektor

	Line3d(Vect3d pV, Vect3d rV);

	~Line3d();

	//Prüft Parallelität der Geraden
	static bool IsParallel(const Line3d & g1, const Line3d & g2);

	static double Angle(const Line3d & g1, const Line3d & g2);

	//Prüft, ob zwei Geraden Identisch sind
	static bool IsIdentical(const Line3d & g1, const Line3d & g2);

	//Berechnet den Schnittpunkt zwischen 2 Geraden, bool & intersect wird bei schnitt auf 1 gesetzt
	static Vect3d GetIntersect(const Line3d & g1, const Line3d & g2, bool & intersect);

	//Gibt Punkt auf Gerade mit parameter zurück
	Vect3d GetPoint(float param) const;

	//Gibt Minimale Distanz von Gerade zu Pnkt zurück -> GP
	float MinDistGP(Vect3d point) const;

	//Gibt Minimale Distanz von Gerade Gerade zurück -> GG
	static float MinDistGG(const Line3d & g1, const Line3d & g2);

	//Gerade zu String
	std::string ToString() const;
};
