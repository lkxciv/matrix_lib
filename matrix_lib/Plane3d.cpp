#include "vect3d.h"
#include "Line3d.h"
#include "Plane3d.h"
#include <string>
#include <cmath>

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

float Plane3d::MinDistEG(Line3d & g) const
{
	//Überprüfung ob G parrallel zu E
	if (Vect3d::DotP(n, g.rV) != 0)
	{
		return 0;
	}
	else
	{
		return this->MinDistEP(g.pV);
	}
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

double Plane3d::Angle(Plane3d & e1, Plane3d & e2)
{
	return Vect3d::AngleAbs(e1.n, e2.n);
}

double Plane3d::Angle(Plane3d & e, Line3d & g)
{
	return Vect3d::AngleAbsNormal(e.n, g.rV);
}