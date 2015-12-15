#include <algorithm>

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

};

Primitive_Matrix::Primitive_Matrix(unsigned int h, unsigned int w)
{
	//höhe und breite zuweisen
	height = h;
	width = w;
	//dyn. array aufspannen
	matrix = new float*[height];
	for (int i = 0; i < height; i++)
	{
		matrix[i] = new float[width];
	}
}

Primitive_Matrix::Primitive_Matrix(float **arr ,unsigned int h, unsigned int w)
{
	//höhe und breite zuweisen
	height = h;
	width = w;
	//dyn. array aufspannen
	matrix = new float*[height];
	for (int i = 0; i < height; i++)
	{
		matrix[i] = new float[width];
		//Gleichzeitiges Kopieren der Arrayzeilen - TESTEN!
		std::copy(matrix[i], matrix[i] + width, arr[i]);
	}
}

Primitive_Matrix::~Primitive_Matrix()
{
	for (int i = 0; i < height; i++)
	{
		//dyn Array auf jeder Zeile wird gelöscht
		delete[] matrix[i];
	}
}
