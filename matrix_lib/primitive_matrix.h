#include <string>

#ifndef PRIMITIVE_MATRIX
#define PRIMITIVE_MATRIX

class Primitive_Matrix
{
protected:
	//Matrix als dynamische Array
	float **matrix;

private:
	//h�che und breite, beginnen bei 1!
	unsigned int height = 0;
	unsigned int width = 0;

public:
	//Konstr. H�he und Breite
	Primitive_Matrix(unsigned int h, unsigned int w);

	//Kontrs. 2D dyn. Array
	Primitive_Matrix(float **arr, unsigned int h, unsigned int w);

	//Kopierkonstruktor
	Primitive_Matrix(const Primitive_Matrix &copyof);

	//Destruktor
	~Primitive_Matrix();

	//Zuweisungsoperator, Object wird her�berkopiert
	Primitive_Matrix & operator=(const Primitive_Matrix & assign);

	//�ndert H�he
	bool ChangeHeight(unsigned int newHeight);

	//�ndert Breite
	bool ChangeWidth(unsigned int newWidth);

	//�ndert Gr��e
	bool ChangeSize(unsigned int newHeight, unsigned int newWidth);

	//Zu string
	std::string ToString();

	//�ndert wert in Feld, bei 0 beginnen!
	bool SetValue(unsigned int y, unsigned int x, float value);

	//Returnt wert in Feld, bei 0 beginnen!
	float GetValue(unsigned int y, unsigned int x);

	//Returnt H�he, beginnt bei 1
	unsigned int GetHeight() { return height; }

	//Returnt Breite beginnt bei 1
	unsigned int GetWidth() { return width; }

	//Kopiert Object aus Parameter in Aufrufendes Object
	void CopyFrom(const Primitive_Matrix &copyof);
};

#endif
