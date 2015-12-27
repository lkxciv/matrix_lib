#include "LGS.h"
#include "primitive_matrix.h"
#include "vect3d.h"
#include <iostream>
#include <stdlib.h>
#include <string>

class Line3d
{
public:
	Vect3d pV;//St�tzvektor
	Vect3d rV;//Richtungsvektor

	Line3d(Vect3d pV, Vect3d rV);

	~Line3d();

	//Pr�ft Parallelit�t der Geraden
	static bool IsParallel(const Line3d & g1, const Line3d & g2);

	static float Angle(const Line3d & g1, const Line3d & g2);

	//Pr�ft, ob zwei Geraden Identisch sind
	static bool IsIdentical(const Line3d & g1, const Line3d & g2);

	//Berechnet den Schnittpunkt zwischen 2 Geraden, bool & intersect wird bei schnitt auf 1 gesetzt
	static Vect3d GetIntersect(const Line3d & g1, const Line3d & g2, bool & intersect);

	//Gibt Punkt auf Gerade mit parameter zur�ck
	Vect3d GetPoint(float param) const;
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

//Pr�ft Parallelit�t der Geraden
bool Line3d::IsParallel(const Line3d & g1, const Line3d & g2)
{
	return(Vect3d::LinDep(g1.rV, g2.rV));
}

float Line3d::Angle(const Line3d & g1, const Line3d & g2)
{
	return(Vect3d::AngleAbs(g1.rV, g2.rV));
}

//Pr�ft, ob zwei Geraden Identisch sind
bool Line3d::IsIdentical(const Line3d & g1, const Line3d & g2)
{
	if (IsParallel(g1, g2) == 1)
	{
		/*Wenn sie parallel sind muss st�tzp. von g2 auf gerade g1 liegen.
		d.h. der Parameter von g1 muss beim st�tzp. g2 f�r die
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
	else
	{
		return 0;
	}
}

Vect3d Line3d::GetPoint(float param) const
{
	return pV + rV * param;
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

	float temp0[3] = { g1.rV.vect[0], -1 * g2.rV.vect[0], g2.pV.vect[0] - g1.pV.vect[0] };
	float temp1[3] = { g1.rV.vect[1], -1 * g2.rV.vect[1], g2.pV.vect[1] - g1.pV.vect[1] };
	float temp2[3] = { g1.rV.vect[2], -1 * g2.rV.vect[2], g2.pV.vect[2] - g1.pV.vect[2] };

	//sp�ter: free(temp);
	float **temp = (float **)calloc(3, sizeof(float *));
	temp[0] = temp0;
	temp[1] = temp1;
	temp[2] = temp2;

	//Sollte automatisch gel�scht werden
	LGS res = LGS(2, 3, temp);
	//Kann gel�scht werden, da kopie davon gamacht wird
	free(temp);

	//Lgs l�sen
	res.RRef();
	//�berpr�fen, ob L�sung Punkt ist
	if (res.IsSolutionPoint() == 0)
	{
		intersect = 0;
		return Vect3d();
	}
	else
	{
		intersect = 1;
		//L�sungen sind parameter, arr sollte 2 elemente haben
		float *solPtr = res.GetSolution();
		//�berpr�fung ob Beide geraden gleichen Punkt bei Parameter haben
		if (g1.GetPoint(solPtr[0]) == g2.GetPoint(solPtr[0]))
		{
			//sichern
			Vect3d solVct = g1.GetPoint(solPtr[0]);
			//l�schen
			delete[] solPtr;
			intersect = 1;

			return solVct;
		}
		else
		{
			//l�schen
			delete[] solPtr;
			intersect = 0;

			//fehler
			return Vect3d(NAN, NAN, NAN);
		}
	}
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
