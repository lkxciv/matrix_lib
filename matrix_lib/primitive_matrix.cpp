//std::copy wird nur durch zeiger aufgerufen, deswegen muss die Warnung deaktiviert werden

#include <cstdlib>
#include <cstring>
#include <string>

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
	bool ChangeHeight(unsigned int newHeight);

	//�ndert Breite
	bool ChangeWidth(unsigned int newWidth);

	//�ndert Gr��e
	bool ChangeSize(unsigned int newHeight, unsigned int newWidth);

	//Zu string
	std::string ToString();

};

Primitive_Matrix::Primitive_Matrix(unsigned int h, unsigned int w)
{
	//h�he und breite zuweisen
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
	//h�he und breite zuweisen
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
		//std::copy(matrix[i], matrix[i] + width, arr[i]); algorithm und #define _SCL_SECURE_NO_WARNINGS ben�tigt
		memcpy(matrix[i], arr[i], width * sizeof(float));
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

bool Primitive_Matrix::ChangeHeight(unsigned int newHeight)//Testen!
{
	//h�hen array realloc mit neuer h�he und �berpr�fung
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
			matrix[i] = (float *)calloc(width, sizeof(float));
		}
		height = newHeight;
	}

	return 1;
}

bool Primitive_Matrix::ChangeWidth(unsigned int newWidth)//Testen!
{
	//spalten realloc zu neuer gr��e mit �berpr�fung durch temp == NULL
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
	//Beide m�ssen klappen
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