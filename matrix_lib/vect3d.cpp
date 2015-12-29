#define PI 3.14159265

#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include "vect3d.h"

Vect3d::Vect3d()
{
	vect[0] = NULL;
	vect[1] = NULL;
	vect[2] = NULL;
}

Vect3d::Vect3d(float x, float y, float z)
{
	vect[0] = x;
	vect[1] = y;
	vect[2] = z;
}

Vect3d::~Vect3d()
{
}

//Berechnet den Flächeninhalt des aufgespannten Spats
float Vect3d::Area(Vect3d v1, Vect3d v2)
{
	return(CrossP(v1, v2).AbsV());
}

//Berechnet Volumen des Aufgespannten Polytops
//V = |(v1 x v2) * v3|
float Vect3d::Volume(Vect3d v1, Vect3d v2, Vect3d v3)
{
	return(abs(DotP(CrossP(v1, v2), v3)));
}

//Berechnet betrag von Projektion auf v2
float Vect3d::AbsProjectOn(Vect3d v2)
{
	return(DotP(*this, v2) / (*this).AbsV());
}

//Berechnet Vektor von Projetion auf v2
Vect3d Vect3d::ProjectOn(Vect3d v2)
{
	return(v2 * ((*this).AbsProjectOn(v2) * (*this).AbsV()));
}

//Berechnet Vektorbetrag
float Vect3d::AbsV() const
{
	return(sqrt(vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[1]));

}

//Berechnet Skalarprodukt
float Vect3d::DotP(Vect3d v1, Vect3d v2)
{
	return(v1.vect[0] * v2.vect[0] + v1.vect[1] * v2.vect[1] + v1.vect[2] * v2.vect[2]);
}

//Berechnet Kreuzprodukt
Vect3d Vect3d::CrossP(Vect3d v1, Vect3d v2)
{
	Vect3d vR;

	vR.vect[0] = v1.vect[1] * v2.vect[2] - v1.vect[2] * v2.vect[1];
	vR.vect[1] = v1.vect[2] * v2.vect[0] - v1.vect[0] * v2.vect[2];
	vR.vect[2] = v1.vect[0] * v2.vect[1] - v1.vect[1] * v2.vect[0];

	return vR;
}

//Vektor wird als String ausgegeben
std::string Vect3d::ToString()
{
	std::ostringstream oss;
	std::string sR = "{";

	for (int i = 0; i <= 2; i++)
	{
		oss << vect[i];
		sR += oss.str();
		oss.str("");//Wird zurückgesetzt

		if (i < 2)
		{
			sR += ", ";
		}
	}
	sR += "}";
	return sR;
}

//Prüft Linearabhängigkeit
bool Vect3d::LinDep(Vect3d v1, Vect3d v2)
{
	if (v1.vect[0] / v2.vect[0] == v1.vect[1] / v2.vect[1] && v1.vect[1] / v2.vect[1] == v1.vect[2] / v2.vect[2])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//Berechnet Winkel zw. 2 Vektoren
double Vect3d::Angle(Vect3d v1, Vect3d v2)
{
	return(acos(DotP(v1, v2) / (v1.AbsV() * v2.AbsV()) * 180 / PI));
}

//Berechnet Winkel zw. 2 Vektoren mit Betrag im Zähler für Objekte
double Vect3d::AngleAbs(Vect3d v1, Vect3d v2)
{
	return(acos(abs(DotP(v1, v2)) / (v1.AbsV() * v2.AbsV()) * 180 / PI));
}

//Berechnet Einheitsvektor, ist static
Vect3d Vect3d::UnitV(Vect3d v1)
{
	float b = v1.AbsV();
	for (int i = 0; i <= 2; i++)
	{
		v1.vect[i] /= b;
	}
	return v1;
}

//Überladung addition
Vect3d Vect3d::operator+(Vect3d v2) const
{
	Vect3d res;
	for (int i = 0; i <= 2; i++)
	{
		res.vect[i] = vect[i] + v2.vect[i];
	}
	return res;
}

//Überladung +=
Vect3d Vect3d::operator+=(Vect3d v2)
{
	for (int i = 0; i <= 2; i++)
	{
		vect[i] += v2.vect[i];
	}
	//this ist Zeiger auf Objekt, *this returnt Wert
	return *this;
}

//Überladunng subtraktion
Vect3d Vect3d::operator-(Vect3d v2) const
{
	Vect3d res;
	for (int i = 0; i <= 2; i++)
	{
		res.vect[i] = vect[i] - v2.vect[i];
	}
	return res;
}

//Überladung -=
Vect3d Vect3d::operator-=(Vect3d v2)
{
	for (int i = 0; i <= 2; i++)
	{
		vect[i] -= v2.vect[i];
	}
	//this ist Zeiger auf Objekt, *this returnt Wert
	return *this;
}

//Überladung multiplikation mit skalar
Vect3d Vect3d::operator*(float skalar) const
{
	Vect3d res;
	for (int i = 0; i <= 2; i++)
	{
		res.vect[i] = vect[i] * skalar;
	}
	return res;
}

//Überladung multiplikation mit skalar
Vect3d Vect3d::operator*=(float skalar)
{
	for (int i = 0; i <= 2; i++)
	{
		vect[i] *= skalar;
	}
	return *this;
}

//Überladung division mit skalar
Vect3d Vect3d::operator/(float skalar) const
{
	Vect3d res;
	for (int i = 0; i <= 2; i++)
	{
		res.vect[i] = vect[i] / skalar;
	}
	return res;
}

//Überladung division mit skalar
Vect3d Vect3d::operator/=(float skalar)
{
	for (int i = 0; i <= 2; i++)
	{
		vect[i] /= skalar;
	}
	return *this;
}

//Überladung multiplikation durch skalaprodukt zweier vektoren
float Vect3d::operator*(Vect3d v2) const
{
	return DotP(*this, v2);
}

bool Vect3d::operator==(Vect3d v2) const
{
	if (this->vect[0] == v2.vect[0] && this->vect[1] == v2.vect[1] && this->vect[2] == v2.vect[2])
	{
		return 1;
	}
	else
	{
		return false;
	}
}

bool Vect3d::operator!=(Vect3d v2) const
{
	return !(*this == v2);
}
