#include "LGS.h"
#include "primitive_matrix.h"
#include "vect3d.h"
#include "Line3d.h"
#include "Plane3d.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

class Matrix : public Primitive_Matrix
{
public:
	//fehler -> err = 1, muss manuell auf 0 gesetzt werden
	bool err = 0;

	//Konstruktoren übernehmen
	using Primitive_Matrix::Primitive_Matrix;

	//Destruktor übernehmen
	using Primitive_Matrix::~Primitive_Matrix;

	//Standardkonstruktor
	/*Matrix();

	//Konstruktor ohne matrix
	Matrix(unsigned int row_num, unsigned int col_num);
	//Destruktor der Basisklasse wird automatisch aufgerufen

	//Konstruktor, bei dem Matrix rüberkopiert wird
	Matrix(unsigned int row_num, unsigned int col_num, float **matrix);

	//Kopierkonstruktor
	Matrix(const Matrix & copyof);*/

	float get(unsigned int row, unsigned int col) const;

	void set(unsigned int row, unsigned int col, float value);

	//operatoren

	//Wiederverwenden
	using Primitive_Matrix::operator=;

	Matrix operator+(const Matrix & m2);

	Matrix operator*(const Matrix & m2);
};

/*Matrix::Matrix() : Primitive_Matrix(){}

Matrix::Matrix(unsigned int row_num, unsigned int col_num) : Primitive_Matrix(row_num, col_num){}

Matrix::Matrix(unsigned int row_num, unsigned int col_num, float ** matrix) : Primitive_Matrix(matrix, row_num, col_num){}

Matrix::Matrix(const Matrix & copyof) : Primitive_Matrix(copyof){}*/

float Matrix::get(unsigned int row, unsigned int col) const
{
	return matrix[row][col];
}

void Matrix::set(unsigned int row, unsigned int col, float value)
{
	matrix[row][col] = value;
}

Matrix Matrix::operator+(const Matrix & m2)
{
	Matrix mres = Matrix(GetHeight(), GetWidth());
	//Matrizen gleich groß -> alle Elemnte zusammen addieren
	if (GetHeight() == m2.GetHeight() && GetWidth() == m2.GetWidth())
	{
		for (int r = 0; r < GetHeight(); r++)
		{
			for (int c = 0; c < GetWidth(); c++)
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

Matrix Matrix::operator*(const Matrix & m2)
{
	Matrix mres = Matrix(this->GetHeight(), m2.GetWidth());
	//breite m1 = höhe m2 ?
	if (this->GetWidth() == m2.GetHeight())
	{
		for (int h = 0; h < this->GetHeight(); h++)
		{
			for (int b = 0; b < m2.GetWidth(); b++)
			{
				//skalarp
				for (int i = 0; i < this->ChangeWidth(); i++)
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

	Plane3d tstplane = Plane3d(v1, v2);
	Line3d tstline = Line3d(v1, v2);

	//

	Primitive_Matrix *prmtest = new Primitive_Matrix(2,2);
	delete prmtest;

	//

	Matrix *mattest = new Matrix(2, 2);
	delete mattest;

}

