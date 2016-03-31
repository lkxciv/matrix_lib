#include "LGS.h"
#include "primitive_matrix.h"
#include "vect3d.h"
#include "Line3d.h"
#include "Plane3d.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

class Matrix : public virtual Primitive_Matrix
{
	//virtual, falls Matrix vererbt, um Konflikte zu vermeiden
private:
	//Determinante 2x2 Matrix (nach Satz)
	float det22() const;

	//Determinante 3x3 Matrix (Sarrus)
	float det33() const;

	//Anzahl nullen in Spalte (ab 0!)
	int zerosC(unsigned int c) const;

	//Anzal nullen in Zeile (ab 0!)
	int zerosR(unsigned int r) const;

	//Rekursive determinantenberechnung nach Laplasschen Entwicklungssatz
	float det_rek() const;

public:
	//fehler -> err = 1, muss manuell auf 0 gesetzt werden
	bool err = 0;

	//Konstruktoren übernehmen
	using Primitive_Matrix::Primitive_Matrix;

	//Destuktor der Basisklasse: ~Matrix() -> ~Primitive_Matrix

	float get(unsigned int rowy, unsigned int colx) const;

	void set(unsigned int rowy, unsigned int colx, float value);

	//operatoren

	//Wiederverwenden
	using Primitive_Matrix::operator=;

	Matrix operator+(const Matrix & m2) const;

	Matrix operator*(const Matrix & m2) const;

	Matrix operator*(const float f) const;

	Matrix operator-(const Matrix & m2) const;

	bool operator==(const Matrix & m2) const;

	//funcs

	//transponieren
	Matrix transp() const;

	//true, wenn quadratische Matrix
	bool isQuad() const;

	//Liefert Untermatrix durch streichen der Zeile und Spalte
	Matrix getSubMat(unsigned int rowy, unsigned int colx) const;

	//Liefert determinante
	float det() const;

	//Liefert Unteradjunkte / Minor
	float minor(unsigned int rowy, unsigned int colx) const;

	//Zeilenmatrix aus Vectoren
	static Matrix getRowMat(const Vect3d & v);

	//Zeilenmatrix aus Vectoren
	static Matrix getRowMat(const Vect3d & v0, const Vect3d & v1, const Vect3d & v2);

	//Spaltenmatrix aus Vektoren
	static Matrix getColMat(const Vect3d & v);

	//Spaltenmatrix aus Vektoren
	static Matrix getColMat(const Vect3d & v0, const Vect3d & v1, const Vect3d & v2);

	//Einheitsmatrix, size ab 1 !
	static Matrix getIdentMat(unsigned int size);
};

float Matrix::det22() const
{
	return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

float Matrix::det33() const
{
	return 
			matrix[0][0] * matrix[1][1] * matrix[2][2]
		+ matrix[1][0] * matrix[2][1] * matrix[0][2]
		+ matrix[2][0] * matrix[0][1] * matrix[1][2]
		- matrix[0][2] * matrix[1][1] * matrix[2][0]
		- matrix[1][2] * matrix[2][1] * matrix[0][0]
		- matrix[2][2] * matrix[0][1] * matrix[1][0];
}

int Matrix::zerosC(unsigned int c) const
{
	unsigned int z = 0;
	for (unsigned int i = 0; i < GetHeight(); i++)
	{
		if (this->get(i, c) == 0)
		{
			z++;
		}
	}
	return z;
}

int Matrix::zerosR(unsigned int r) const
{
	unsigned int z = 0;
	for (unsigned int i = 0; i < GetWidth(); i++)
	{
		if (this->get(r, i) == 0)
		{
			z++;
		}
	}
	return z;
}

float Matrix::get(unsigned int rowy, unsigned int colx) const
{
	return matrix[rowy][colx];
}

void Matrix::set(unsigned int rowy, unsigned int colx, float value)
{
	matrix[rowy][colx] = value;
}

Matrix Matrix::operator+(const Matrix & m2) const
{
	Matrix mres = Matrix(GetHeight(), GetWidth());
	//Matrizen gleich groß -> alle Elemnte zusammen addieren
	if (GetHeight() == m2.GetHeight() && GetWidth() == m2.GetWidth())
	{
		for (unsigned int r = 0; r < GetHeight(); r++)
		{
			for (unsigned int c = 0; c < GetWidth(); c++)
			{
				mres.set(r, c, this->get(r, c) + m2.get(r, c));
			}
		}
	}
	//Wenn nicht, err flag 1
	else
	{
		mres.err = 1;
	}
	//sollte wie rvalue behandelt werden
	return mres;
}

Matrix Matrix::operator*(const Matrix & m2) const
{
	Matrix mres = Matrix(this->GetHeight(), m2.GetWidth());
	//breite m1 = höhe m2 ?
	if (this->GetWidth() == m2.GetHeight())
	{
		for (unsigned int h = 0; h < this->GetHeight(); h++)
		{
			for (unsigned int b = 0; b < m2.GetWidth(); b++)
			{
				//skalarp
				for (unsigned int i = 0; i < this->GetWidth(); i++)
				{
					mres.set(h, b, mres.get(h, b) + this->get(h, i) * m2.get(i, b));
				}
			}
		}
	}
	//sonst err flag auf 1
	else
	{
		mres.err = 1;
	}
	return mres;
}

Matrix Matrix::operator*(const float scal) const
{
	//Kopie von Matrix verändern
	Matrix mres = Matrix(GetHeight(), GetWidth());
	for (unsigned int r = 0; r < GetHeight(); r++)
	{
		for (unsigned int c = 0; c < GetWidth(); c++)
		{
			mres.set(r, c, this->get(r, c) * scal);
		}
	}
	return mres;
}

Matrix Matrix::operator-(const Matrix & m2) const 
{
	return *this + (m2 * -1);
}

bool Matrix::operator==(const Matrix & m2) const
{
	//Stimmen höhen und breiten nicht überein?
	if (this->GetHeight() != m2.GetHeight() || this->GetWidth() != m2.GetWidth())
	{
		return false;
	}
	//Stimmen Werte nicht überein?
	for (unsigned int y = 0; y < GetHeight(); y++)
	{
		for (unsigned int x = 0; x < GetWidth(); x++)
		{
			if (this->get(y, x) != m2.get(y, x))
			{
				return false;
			}
		}
	}
	//Sonst true
	return true;
}

Matrix Matrix::transp() const
{
	//neue Matrix anders herum
	Matrix mres = Matrix(GetWidth(), GetHeight());
	for (unsigned int y = 0; y < GetHeight(); y++)
	{
		for (unsigned int x = 0; x < GetWidth(); x++)
		{
			//alle Werte vertauschen
			mres.set(x, y, this->get(y, x));
		}
	}
	return mres;
}

bool Matrix::isQuad() const
{
	if (GetHeight() == GetWidth())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Matrix Matrix::getSubMat(unsigned int rowy, unsigned int colx) const
{
	Matrix smat = Matrix(this->GetHeight() - 1, this->GetWidth() - 1);
	//smat befüllen mit this, rowy und colx überspringen
	unsigned int thisx = 0, thisy = 0, sx = 0, sy = 0;
	for (;thisy < this->GetHeight(); thisy++)
	{
		if (thisy == rowy)
		{
			//betroffene Zeile getroffen -> abbrechen
			continue;
		}
		for (;thisx < this->GetWidth(); thisx++)
		{
			if (thisx == colx)
			{
				//betroffene Spalte getroffen -> abbrechen
				continue;
			}
			smat.set(sy, sx, this->get(thisy, thisx));
		}
		sy++;
	}

	return smat;
}

float Matrix::det_rek() const
{
	float res = 0;
	//mehr nullen in zeile oder spalte?
	if (zerosC(0) > zerosR(0))
	{
		//0te spalte durchlaufen -> x = 0
		for (unsigned int i = 0; i < GetHeight(); i++)
		{
			res += get(i, 0) * minor(i, 0);
		}
	}
	else
	{
		for (unsigned int i = 0; i < GetHeight(); i++)
		{
			res += get(0, i) * minor(0, i);
		}
	}
	//todo: vorherige methode nur bei kleinen Matrizen, 
	//			bei größeren Zeile / Spalte mit wenigsten nullen suchen
	return res;
}

float Matrix::det() const
{
	//Zeilenanz. = Spaltenanz.?
	if (!isQuad())
	{
		return NAN;
	}
	// ab 3 festes Verfahren
	if (GetHeight() == 3)
	{
		return det33();
	}
	if (GetHeight() == 2)
	{
		return det22();
	}
	if (GetHeight() == 1)
	{
		return matrix[0][0];
	}
	if (GetHeight() == 0)
	{
		return NAN;
	}

	//todo
	return det_rek();
}

float Matrix::minor(unsigned int rowy, unsigned int colx) const
{
	//-1 hoch zeile + spalte * det. von Untermatrix
	return (float)pow(-1, rowy + 1 + colx + 1) * this->getSubMat(rowy, colx).det();
}

Matrix Matrix::getRowMat(const Vect3d & v)
{
	Matrix res = Matrix(1, 3);
	for (unsigned int i = 0; i < 3; i++)
	{
		res.set(0, i, v.vect[i]);
	}
	return res;
}

Matrix Matrix::getRowMat(const Vect3d & v0, const Vect3d & v1, const Vect3d & v2)
{
	Matrix res = Matrix(3, 3);
	for (unsigned int i = 0; i < 3; i++)
	{
		res.set(0, i, v0.vect[i]);
		res.set(1, i, v1.vect[i]);
		res.set(2, i, v2.vect[i]);
	}
	return res;
}

Matrix Matrix::getColMat(const Vect3d & v)
{
	Matrix res = Matrix(3, 1);
	for (unsigned int i = 0; i < 3; i++)
	{
		res.set(i, 0, v.vect[i]);
	}
	return res;
}

Matrix Matrix::getColMat(const Vect3d & v0, const Vect3d & v1, const Vect3d & v2)
{
	Matrix res = Matrix(3, 3);
	for (unsigned int i = 0; i < 3; i++)
	{
		res.set(i, 0, v0.vect[i]);
		res.set(i, 1, v1.vect[i]);
		res.set(i, 2, v2.vect[i]);
	}
	return res;
}

Matrix Matrix::getIdentMat(unsigned int size)
{
	Matrix res = Matrix(size, size);
	for (unsigned int i = 0; i < size; i++)
	{
		res.set(i, i, 1);
	}
	return res;
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

	/*Primitive_Matrix *mtrTest = new Primitive_Matrix(test, 3, 3);
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

	Plane3d tstplane = Plane3d(v1, v2);
	Line3d tstline = Line3d(v1, v2);

	//

	Primitive_Matrix *prmtest = new Primitive_Matrix(2,2);
	delete prmtest;*/

	//

	Matrix *mattest1 = new Matrix(test, 3, 3);
	Matrix *mattest2 = new Matrix(test, 3, 3);
	Matrix mattest3 = *mattest1 + *mattest2;
	std::cout << mattest1->ToString() << std::endl;
	std::cout << mattest1->transp().ToString();
	delete mattest1;
	delete mattest2;

}

