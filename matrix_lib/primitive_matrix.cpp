#include <cstdlib>
#include <cstring>
#include <string>
#include "primitive_matrix.h"



Primitive_Matrix::Primitive_Matrix()
{
	matrix = nullptr;
}

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

//Daten kopieren
Primitive_Matrix::Primitive_Matrix(const Primitive_Matrix & copyof)
{
	//höhe und breite zuweisen
	height = copyof.height;
	width = copyof.width;

	//dyn. array aufspannen
	matrix = (float **)calloc(height, sizeof(float *));
	for (unsigned int i = 0; i < height; i++)
	{
		//matrix[i] = new float[width];
		matrix[i] = (float *)calloc(width, sizeof(float));
		//Gleichzeitiges Kopieren der Arrayzeilen
		memcpy(matrix[i], copyof.matrix[i], width * sizeof(float));
	}
}

Primitive_Matrix::Primitive_Matrix(Primitive_Matrix && temp)
{
	this->height = temp.height;
	this->width = temp.width;
	this->matrix = temp.matrix;

	temp.matrix = nullptr;
}

Primitive_Matrix::~Primitive_Matrix()
{
	if (matrix != NULL)
	{
		for (unsigned int i = 0; i < height; i++)
		{
			//dyn Array auf jeder Zeile wird gelöscht
			//delete[] matrix[i];
			free(matrix[i]);
		}
		free(matrix);
	}
}

Primitive_Matrix & Primitive_Matrix::operator=(const Primitive_Matrix & assign)
{
	this->CopyFrom(assign);

	return *this;
}

Primitive_Matrix & Primitive_Matrix::operator=(Primitive_Matrix && temp)
{
	if (this != &temp)
	{
		delete this; //changed

		this->height = temp.height;
		this->width = temp.width;
		this->matrix = temp.matrix;

		temp.matrix = nullptr;
	}

	return *this;
}

bool Primitive_Matrix::ChangeHeight(unsigned int newHeight)
{
	//neue Höhe kleiner als die alte -> floats, auf die matrix[i] zeigt müssen gelöscht werden
	if (newHeight < height)
	{
		for (unsigned int i = height - 1; i >= newHeight; i--)
		{
			free(matrix[i]);
		}
	}
	//höhen array realloc mit neuer höhe und überprüfung
	//bei realloc werden die zeiger, auf der "zeiger zeiger" matrix zeigt, automatisch gelöscht,
	//jedoch NICHT die floats auf die die zeiger zeigen
	float **temp = (float **)realloc(matrix, newHeight * sizeof(float *));
	if (temp != NULL)
	{
		matrix = temp;
	}
	else if (temp == NULL)
	{
		return 0;
	}
	//neu höhe größer als alte -> neue höhenzeigern matrix[i] zeigen auf
	//floats, für die platz geschaffen werden muss
	if (newHeight > height)
	{
		for (unsigned int i = height; i < newHeight; i++)
		{
			matrix[i] = (float *)calloc(width, sizeof(float));
		}
	}
	height = newHeight;//neu

	return 1;
}

bool Primitive_Matrix::ChangeWidth(unsigned int newWidth)
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

	for (unsigned int row = 0; row < height; row++)//Schleife Zeilen
	{
		s += "[ ";
		for (unsigned int col = 0; col < width; col++)//Schleife Spalten in einer Zeile
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

void Primitive_Matrix::CopyFrom(const Primitive_Matrix & copyof)
{
	//erstmal alles Löschen
	this->~Primitive_Matrix();

	//höhe und breite zuweisen
	height = copyof.height;
	width = copyof.width;

	//dyn. array aufspannen
	matrix = (float **)calloc(height, sizeof(float *));
	for (unsigned int i = 0; i < height; i++)
	{
		//matrix[i] = new float[width];
		matrix[i] = (float *)calloc(width, sizeof(float));
		//Gleichzeitiges Kopieren der Arrayzeilen
		memcpy(matrix[i], copyof.matrix[i], width * sizeof(float));
	}
}

void Primitive_Matrix::MoveFrom(Primitive_Matrix && temp)
{
	if (this != &temp)
	{
		this->height = temp.height;
		this->width = temp.width;
		this->matrix = temp.matrix;

		temp.matrix = nullptr;
	}
}


