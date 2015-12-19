//std::copy wird nur durch zeiger aufgerufen, deswegen muss die Warnung deaktiviert werden

#include <cstdlib>
#include <cstring>
#include <string>

class Primitive_Matrix
{
private:
	//höche und breite, beginnen bei 1!
	unsigned int height = 0;
	unsigned int width = 0;
	//Matrix als dynamische Array
	float **matrix;

public:
	//Konstr. Höhe und Breite
	Primitive_Matrix(unsigned int h, unsigned int w);

	//Kontrs. 2D dyn. Array
	Primitive_Matrix(float **arr, unsigned int h, unsigned int w);

	//Destruktor
	~Primitive_Matrix();

	//Ändert Höhe
	bool ChangeHeight(unsigned int newHeight);

	//Ändert Breite
	bool ChangeWidth(unsigned int newWidth);

	//Ändert Größe
	bool ChangeSize(unsigned int newHeight, unsigned int newWidth);

	//Zu string
	std::string ToString();

	//Ändert wert in Feld, bei 0 beginnen!
	bool SetValue(unsigned int y, unsigned int x, float value);

	//Returnt wert in Feld, bei 0 beginnen!
	float GetValue(unsigned int y, unsigned int x);

};

Primitive_Matrix::Primitive_Matrix(unsigned int h, unsigned int w)
{
	//höhe und breite zuweisen
	height = h;
	width = w;
	//dyn. array aufspannen
	//matrix = new float*[height];
	matrix = (float **)calloc(height, sizeof(float *));
	for (unsigned int i = 0; i < height; i++)
	{
		//matrix[i] = new float[width];
		matrix[i] = (float *)calloc(width, sizeof(float));
	}
}

Primitive_Matrix::Primitive_Matrix(float **arr ,unsigned int h, unsigned int w)
{
	//höhe und breite zuweisen
	height = h;
	width = w;
	//dyn. array aufspannen
	//matrix = new float*[height];
	matrix = (float **)calloc(height, sizeof(float *));
	for (unsigned int i = 0; i < height; i++)
	{
		//matrix[i] = new float[width];
		matrix[i] = (float *)calloc(width, sizeof(float));
		//Gleichzeitiges Kopieren der Arrayzeilen - TESTEN!
		//std::copy(matrix[i], matrix[i] + width, arr[i]); algorithm und #define _SCL_SECURE_NO_WARNINGS benötigt
		memcpy(matrix[i], arr[i], width * sizeof(float));
	}
}

Primitive_Matrix::~Primitive_Matrix()
{
	for (unsigned int i = 0; i < height; i++)
	{
		//dyn Array auf jeder Zeile wird gelöscht
		//delete[] matrix[i];
		free(matrix[i]);
	}
}

bool Primitive_Matrix::ChangeHeight(unsigned int newHeight)//Testen!
{
	//höhen array realloc mit neuer höhe und überprüfung
	//bei realloc wird mtrix automatisch deallocated, wenn nicht NULL
	float **temp = (float **)realloc(matrix, newHeight * sizeof(float *));
	if (temp != NULL)
	{
		matrix = temp;
	}
	else if (temp == NULL)
	{
		return 0;
	}
	height = newHeight;//neu

	return 1;
}

bool Primitive_Matrix::ChangeWidth(unsigned int newWidth)//Testen!
{
	//spalten realloc zu neuer größe mit überprüfung durch temp == NULL
	for (unsigned int i = 0; i < height; i++)
	{
		float *temp = (float *)realloc(matrix[i], newWidth * sizeof(float));

		if (temp != NULL)
		{
			matrix[i] = temp;
		}
		else if (temp == NULL)
		{
			return 0;
		}
	}
	width = newWidth;

	return 1;
}

bool Primitive_Matrix::ChangeSize(unsigned int newHeight, unsigned int newWidth)
{
	//Beide müssen klappen
	if (ChangeHeight(newHeight) == 1 && ChangeWidth(newWidth) == 1)
	{
		return 1;
	}

	return 0;
}

std::string Primitive_Matrix::ToString()
{
	std::string s = "";

	for (int row = 0; row < height; row++)//Schleife Zeilen
	{
		s += "[ ";
		for (int col = 0; col < width; col++)//Schleife Spalten in einer Zeile
		{
			s += (std::to_string( matrix[row][col] ) + " ");
		}
		s += "]\n";
	}

	return s;
}

bool Primitive_Matrix::SetValue(unsigned int y, unsigned int x, float value)
{
	if (y >= 0 && y < height && x >= 0 && x < width)
	{
		matrix[y][x] = value;

		return 1;
	}

	return 0;
}

float Primitive_Matrix::GetValue(unsigned int y, unsigned int x)
{
	if (y >= 0 && y < height && x >= 0 && x < width)
	{
		return matrix[y][x];
	}

	return NAN;
}

#include <iostream>

int main()
{
	float t0[3] = { 0, 1, 2 };
	float t1[3] = { 3, 4, 5 };
	float t2[3] = { 6, 7, 8 };

	float **test = new float*[3];
	test[0] = t0;
	test[1] = t1;
	test[2] = t2;

	Primitive_Matrix mtrTest = Primitive_Matrix(test, 3, 3);
	std::cout << mtrTest.ToString();

	mtrTest.ChangeSize(2, 2);
	std::cout << mtrTest.ToString();

	mtrTest.SetValue(0, 0, 9);
	std::cout << mtrTest.GetValue(0, 0);
}