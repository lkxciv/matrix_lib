#include "vect3d.h"
#include "Line3d.h"
#include "LGS.h"
#include <string>
#include <cmath>

//methodendefinitionen von Line3d

Line3d::Line3d(Vect3d pV, Vect3d rV)
{
	this->pV = pV;
	this->rV = rV;
}

Line3d::~Line3d()
{
}

//Prüft Parallelität der Geraden
bool Line3d::IsParallel(const Line3d & g1, const Line3d & g2)
{
	return(Vect3d::LinDep(g1.rV, g2.rV));
}

double Line3d::Angle(const Line3d & g1, const Line3d & g2)
{
	return(Vect3d::AngleAbs(g1.rV, g2.rV));
}

//Prüft, ob zwei Geraden Identisch sind
bool Line3d::IsIdentical(const Line3d & g1, const Line3d & g2)
{
	if (IsParallel(g1, g2) == 1)
	{
		/*Wenn sie parallel sind muss stützp. von g2 auf gerade g1 liegen.
		d.h. der Parameter von g1 muss beim stützp. g2 für die
		x gleichung, y gleichung und z gleichung gleich sein*/
		float par[3];
		for (int i = 0; i <= 2; i++)
		{
			//r = (s2 - s1) / v1; da s2 = s1 + r * v1
			par[i] = (g2.pV.vect[i] - g1.pV.vect[i]) / g1.rV.vect[i];
		}
		if (par[0] == par[1] && par[1] == par[2])
		{
			return 1;
		}
	}

	return 0;
}

Vect3d Line3d::GetPoint(float param) const
{
	return pV + rV * param;
}

float Line3d::MinDistGP(Vect3d point) const
{
	//Parallelogramm durch Länge -> Rechteckhöhe
	//|po x v| / |v|
	return ((Vect3d::CrossP((pV - point), rV)).AbsV() / rV.AbsV());
}

float Line3d::MinDistGG(const Line3d & g1, const Line3d & g2)
{
	//Polytopsvolumen / Fläche Parallelogram v1 und v2
	//|o1o2 * (v1 x v2)| / |v1 x v2|
	Vect3d v1xv2 = Vect3d::CrossP(g2.rV, g1.rV);
	return (abs(Vect3d::DotP((g2.pV - g1.pV), v1xv2)) / (v1xv2.AbsV()));
}

std::string Line3d::ToString() const
{
	return pV.ToString() + " + r * " + rV.ToString();
}

Vect3d Line3d::GetIntersect(const Line3d & g1, const Line3d & g2, bool & intersect)
{
	//Abfrage unendlich oder keine Schnittpunkte
	if (IsIdentical(g1, g2) == 1)
	{
		intersect = 1;
		return Vect3d();
	}
	else if (IsParallel(g1, g2) == 1)
	{
		intersect = 0;
		return Vect3d();
	}

	//**array erzeugen für LGS kontruktor
	float temp0[3] = { g1.rV.vect[0], -1 * g2.rV.vect[0], g2.pV.vect[0] - g1.pV.vect[0] };
	float temp1[3] = { g1.rV.vect[1], -1 * g2.rV.vect[1], g2.pV.vect[1] - g1.pV.vect[1] };
	float temp2[3] = { g1.rV.vect[2], -1 * g2.rV.vect[2], g2.pV.vect[2] - g1.pV.vect[2] };

	//später: free(temp);
	float **temp = (float **)calloc(3, sizeof(float *));
	temp[0] = temp0;
	temp[1] = temp1;
	temp[2] = temp2;

	//Sollte automatisch gelöscht werden
	LGS res = LGS(2, 3, temp);
	//Kann gelöscht werden, da kopie davon gamacht wird
	free(temp);

	//Lgs lösen
	res.RRef();
	//Überprüfen, ob Lösung Punkt ist
	if (res.IsSolutionPoint() == 0)
	{
		intersect = 0;
		return Vect3d();
	}
	else
	{
		intersect = 1;
		//Lösungen sind parameter, arr sollte 2 elemente haben
		float *solPtr = res.GetSolution();
		//Überprüfung ob Beide geraden gleichen Punkt bei Parameter haben
		if (g1.GetPoint(solPtr[0]) == g2.GetPoint(solPtr[0]))
		{
			//sichern
			Vect3d solVct = g1.GetPoint(solPtr[0]);
			//löschen
			delete[] solPtr;
			intersect = 1;

			return solVct;
		}
		else
		{
			//löschen
			delete[] solPtr;
			intersect = 0;

			//fehler
			return Vect3d(NAN, NAN, NAN);
		}
	}
}