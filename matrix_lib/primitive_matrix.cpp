//std::copy wird nur durch zeiger aufgerufen, deswegen muss die Warnung deaktiviert werden
#define _SCL_SECURE_NO_WARNINGS

#include <algorithm>
#include <cstdlib>

class Primitive_Matrix
{
private:
	//h�che und breite, beginnen bei 1!
	unsigned int height = 0;
	unsigned int width = 0;
	//Matrix als dynamische Array
	float **matrix;

public:
	//Konstr. H�he und Breite
	Primitive_Matrix(unsigned int h, unsigned int w);

	//Kontrs. 2D dyn. Array
	Primitive_Matrix(float **arr, unsigned int h, unsigned int w);

	//Destruktor
	~Primitive_Matrix();

	//�ndert H�he
	void ChangeHeight(unsigned int newHeight);

	//�ndert Breite
	bool ChangeWidth(unsigned int newWidth);

};

Primitive_Matrix::Primitive_Matrix(unsigned int h, unsigned int w)
{
	//h�he und breite zuweisen
	height = h;
	width = w;
	//dyn. array aufspannen
	//matrix = new float*[height];
	matrix = (float **)calloc(height, sizeof(float **));
	for (unsigned int i = 0; i < height; i++)
	{
		//matrix[i] = new float[width];
		matrix[i] = (float *)calloc(width, sizeof(float *));
	}
}

Primitive_Matrix::Primitive_Matrix(float **arr ,unsigned int h, unsigned int w)
{
	//h�he und breite zuweisen
	height = h;
	width = w;
	//dyn. array aufspannen
	//matrix = new float*[height];
	matrix = (float **)calloc(height, sizeof(float **));
	for (unsigned int i = 0; i < height; i++)
	{
		//matrix[i] = new float[width];
		matrix[i] = (float *)calloc(width, sizeof(float *));
		//Gleichzeitiges Kopieren der Arrayzeilen - TESTEN!
		std::copy(matrix[i], matrix[i] + width, arr[i]);
	}
}

Primitive_Matrix::~Primitive_Matrix()
{
	for (unsigned int i = 0; i < height; i++)
	{
		//dyn Array auf jeder Zeile wird gel�scht
		//delete[] matrix[i];
		free(matrix[i]);
	}
}

void Primitive_Matrix::ChangeHeight(unsigned int newHeight)
{
	//neue h�he kleiner als alte: zeilen l�schen, bis newHeight -1
	if (newHeight < height)
	{
		for (unsigned int i = height - 1; i >= newHeight; i--)
		{
			//dyn Array auf betroffenen Zeile wird gel�scht
			//delete[] matrix[i];
			free(matrix[i]);
		}
		height = newHeight;
	}
	//neue h�he gr��er als alte: zeilen hinzuf�gen
	else if (newHeight > height)
	{
		for (unsigned int i = height - 1; i < newHeight; i++)
		{
			//dyn Array auf betroffenen Zeile wird hinzugef�gt
			//matrix[i] = new float[width];
			matrix[i] = (float *)calloc(width, sizeof(float *));
		}
		height = newHeight;
	}
}

bool Primitive_Matrix::ChangeWidth(unsigned int newWidth)//Testen!
{
	//neue breite kleiner als alte: spalten realloc
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

	return 1;
}
