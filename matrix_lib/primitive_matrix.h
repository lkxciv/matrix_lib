#include <string>

#ifndef PRIMITIVE_MATRIX
#define PRIMITIVE_MATRIX

class Primitive_Matrix
{
protected:
	//Matrix als dynamische Array
	float **matrix;

private:
	//höche und breite, beginnen bei 1!
	unsigned int height = 0;
	unsigned int width = 0;

public:
	//Standardkonstruktor
	Primitive_Matrix();

	//Konstr. Höhe und Breite
	Primitive_Matrix(unsigned int h, unsigned int w);

	//Kontrs. 2D dyn. Array
	Primitive_Matrix(float **arr, unsigned int h, unsigned int w);

	//Kopierkonstruktor (default, da sonst Move vorgezogen wird)
	Primitive_Matrix(const Primitive_Matrix & copyof);

	//Move Konstruktor für rvalues (Temporäre Werte)
	Primitive_Matrix(Primitive_Matrix && temp);

	//Destruktor
	~Primitive_Matrix();

	//Zuweisungsoperator, Object wird herüberkopiert
	Primitive_Matrix & operator=(const Primitive_Matrix & assign);

	//Zuweisungsoperator rvalues / tempvalues
	Primitive_Matrix & operator=(Primitive_Matrix && temp);

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

	//Returnt Höhe, beginnt bei 1
	unsigned int GetHeight() { return height; }

	//Returnt Breite beginnt bei 1
	unsigned int GetWidth() { return width; }

	//Kopiert Object aus Parameter in Aufrufendes Object
	void CopyFrom(const Primitive_Matrix & copyof);

	//Move Methode für rvalues, da in vererbeten Klassen Move Konstruktor nicht geht
	void MoveFrom(Primitive_Matrix && temp);
};

#endif
