#include "LGS.h"
#include "primitive_matrix.h"
#include "vect3d.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

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
	return ( abs(Vect3d::DotP((g2.pV - g1.pV), v1xv2)) / (v1xv2.AbsV()) );
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

void Plane3d::ParamToNormal()
{
	n = Vect3d::CrossP(r1, r2);
}

void Plane3d::NormalToParam()
{
	r1 = Vect3d(1, 0, -(n.vect[0] / n.vect[2]));
	r2 = Vect3d(0, 1, -(n.vect[1] / n.vect[2]));
}

Plane3d::Plane3d(Vect3d oV, Vect3d n)
{
	this->oV = oV;
	this->n = n;
	NormalToParam();
}

Plane3d::Plane3d(Vect3d oV, Vect3d r1, Vect3d r2)
{
	this->oV = oV;
	this->r1 = r1;
	this->r2 = r2;
	ParamToNormal();
}

//Abstand Ist Betrag von Abbildung 
//von Vektor PO auf n
float Plane3d::MinDistEP(Vect3d point) const
{
	return Vect3d(point, oV).AbsProjectOn(n);
}

float Plane3d::MinDistEE(Plane3d & e2) const
{
	//Auf Linearabhängigkeit prüfen
	if (!Vect3d::LinDep(this->n, e2.n))
	{
		return 0;
	}
	else
	{
		return this->MinDistEP(e2.oV);
	}
}

Vect3d Plane3d::IntersectEG(Plane3d & e, Line3d & g)
{
	//Parameter berechen und in Gerade Einsetzen
	return (g.GetPoint((Vect3d::DotP(e.oV, e.GetN()) - Vect3d::DotP(e.GetN(), g.pV)) / Vect3d::DotP(e.GetN(), g.rV)));
}

Line3d Plane3d::IntersectEE(Plane3d & e1, Plane3d & e2)
{
	//Ebene1 wird in zwei spann geraden aufgeteilt,
	//deren Schnittpunkte mit E2 eine Gerade ergeben
	Vect3d p1 = IntersectEG(e2, Line3d(e1.oV, e1.r1));
	Vect3d p2 = IntersectEG(e2, Line3d(e1.oV, e1.r2));
	
	//Schnittpkts zu Gerade
	return Line3d(p1, Vect3d(p1, p2));
}

void Plane3d::SetN(Vect3d v)
{
	n = v;
	NormalToParam();
}

void Plane3d::SetR1(Vect3d v)
{
	r1 = v;
	ParamToNormal();
}

void Plane3d::SetR2(Vect3d v)
{
	r2 = v;
	ParamToNormal();
}

std::string Plane3d::ToStringParam() const
{
	return oV.ToString() + " + r * " + r1.ToString() + " + s * " + r2.ToString();
}

std::string Plane3d::ToStringNormal() const
{
	return "(x - " + oV.ToString() + ") * " + n.ToString() + " = 0";
}

int main()
{
	float t0[3] = { 1, 2, 3 };
	float t1[3] = { 4, 5, 6 };
	float t2[3] = { 7, 8, 9 };

	//float **test = new float*[3]; //geht
	float **test = (float **)calloc(3, sizeof(float *));
	test[0] = t0;
	test[1] = t1;
	test[2] = t2;

	Primitive_Matrix *mtrTest = new Primitive_Matrix(test, 3, 3);
	std::cout << mtrTest->ToString();

	mtrTest->ChangeSize(2, 2);
	std::cout << mtrTest->ToString();

	mtrTest->SetValue(0, 0, 9);
	std::cout << mtrTest->GetValue(0, 0);

	delete mtrTest;

	//

	LGS *lgsTest = new LGS(2, 3, test);
	std::cout << lgsTest->ToString();

	*lgsTest = lgsTest->Ref();
	std::cout << "\n" << lgsTest->ToString();

	*lgsTest = lgsTest->OnlyRRef();
	std::cout << "\n" << lgsTest->ToString();

	std::cout << "\n" << lgsTest->IsSolutionPoint();

	delete lgsTest;

	//

	Vect3d v1(1, 2, 3);
	Vect3d v2(3, 2, 1);
	Vect3d res = Vect3d::UnitV(v1);

	std::cout <<"\n"<< res.ToString();
}


