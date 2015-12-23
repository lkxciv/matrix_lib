#include <cstdlib>
#include <cstring>
#include <string>

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

	//Returnt Höhe, beginnt bei 1
	unsigned int GetHeight() { return height; }

	//Returnt Breite beginnt bei 1
	unsigned int GetWidth() { return width; }
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
	free(matrix);
}

bool Primitive_Matrix::ChangeHeight(unsigned int newHeight)
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

class LGS : Primitive_Matrix
{
private:
	//varsize zu GetWidth() - 1
	//eqsize zu GetHeight()
	//lgs zu matrix
	int varcount = 0; //Arbeitsvariable breite
	int eqcount = 0; //Arbeitsvariable höhe
	int solState = 0; //Todo: Lösungsstatus des LGS

public:
	//Kontruktor
	LGS(unsigned int variable_num, unsigned int equation_num, float **matrix);
	//Destruktor der Basisklasse wird automatisch aufgerufen

	//Returnt gelöstes LGS in Stufenform
	LGS Ref();

	//Returnt LGS in vollständig gelöster Version, MUSS mit LGS in REF Form aufgerufen werden
	LGS OnlyRRef();

	//Returnt direkt von Normalform zu Ergebnisform
	LGS RRef();

	//Prüft, ob lösung des LGS Punkt entspricht, LGS muss in RREf Form vorliegen
	bool IsSolutionPoint();

	//returnt Lösung in dyn. Float array, Länge ist GetWidth - 1
	//später Löschen nicht vergessen! (mit delete)
	float *GetSolution();
};

LGS::LGS(unsigned int variable_num, unsigned int equation_num, float ** matrix) : Primitive_Matrix(matrix, equation_num, (variable_num + 1))
{
}

//Returnt gelöstes LGS in Stufenform
LGS LGS::Ref()
{
	int yzaehler; //Zaelvariable fuer berechnete Gleichungen
								//int dgaenge; //Zaehlvariable für Rechendurchgaenge
	int ypos = 0;//y Richtungsdurchlauf
	int xpos = 0;//x Richtungsdurchlauf
	int elimx = 0;//ist beginn der treppendurchlauefe in x richtung, zaehlt die eliminationsvorgaenge der variablen
	int start_step = 0;//ist beginn der treppendurchlauefe in y richtung, bestimmt, ab welcher Gleichung weiter eliminiert wird
	float puffer = 0;
	LGS Lgs_ref((GetWidth() - 1), GetHeight(), matrix);//LGS, dass bearbeitet wird

																		//Zähler höchstens quadratisch machen
	if (GetHeight() > (GetWidth() - 1))
	{
		yzaehler = (GetWidth() - 1);
	}
	else if ((GetWidth() - 1) > GetHeight() || (GetWidth() - 1) == GetHeight())
	{
		yzaehler = GetHeight() - 1;
	}

	start_step = 0;
	for (elimx = 0; elimx <= (GetWidth() - 1); elimx++)//elimx ist beginn der treppendurchlauefe in x richtung, zaehlt die eliminationsvorgaenge der variablen
	{
		for (ypos = start_step; ypos < yzaehler; ypos++)//y Richtungsdurchlauf
		{
			puffer = Lgs_ref.matrix[ypos + 1][elimx];
			for (xpos = elimx; xpos <= (GetWidth() - 1); xpos++)//x Richtungsdurchlauf
			{
				//Variable wir eliminiert oder mit El9minationsgleichung subtrahiert
				Lgs_ref.matrix[ypos + 1][xpos] = Lgs_ref.matrix[ypos + 1][xpos] - ((Lgs_ref.matrix[start_step][xpos] / Lgs_ref.matrix[start_step][elimx]) * puffer);
			}
		}
		start_step++;//start_step ist beginn der treppendurchlauefe in y richtung, bestimmt, ab welcher Gleichung weiter eliminiert wird
	}

	return Lgs_ref;
}

//Return LGS in vollständig gelöster Version, MUSS mit LGS in REF Form aufgerufen werden
LGS LGS::OnlyRRef()
{
	//int yzaehler; //Zaelvariable fuer berechnete Gleichungen
	//int dgaenge; //Zaehlvariable für Rechendurchgaenge
	int ypos = 0;//y Richtungsdurchlauf
	int xpos = 0;//x Richtungsdurchlauf
	LGS Lgs_rref((GetWidth() - 1), GetHeight(), matrix);//matrix sollte in ref form sein

																		 //Anfangs y-pos
	if (GetHeight() > (GetWidth() - 1))
	{
		ypos = (GetWidth() - 1);
	}
	else if ((GetWidth() - 1) > GetHeight() || (GetWidth() - 1) == GetHeight())
	{
		ypos = GetHeight() - 1;
	}

	//LGS wird diagonal abgefahren von Unten rechts nach oben links
	for (xpos = (GetWidth() - 1) - 1; xpos >= 0; xpos--)
	{
		for (int varstep = xpos + 1; varstep < (GetWidth() - 1); varstep++)
		{
			//Variable mit letztem Ergebnis multiplizieren, von Gleichungen subtrahieren
			Lgs_rref.matrix[ypos][(GetWidth() - 1)] -= Lgs_rref.matrix[ypos][varstep] * Lgs_rref.matrix[varstep][(GetWidth() - 1)];
			Lgs_rref.matrix[ypos][varstep] = 0;
		}
		//Gleichung durch Faktor von Var.
		Lgs_rref.matrix[ypos][(GetWidth() - 1)] /= Lgs_rref.matrix[ypos][xpos];
		Lgs_rref.matrix[ypos][xpos] = 1;
		ypos--;
	}

	return Lgs_rref;
}

//Returnt direkt von Normalform zu Ergebnisform
LGS LGS::RRef()
{
	LGS Solved((GetWidth() - 1), GetHeight(), matrix);
	Solved = Ref();
	Solved = OnlyRRef();
	return Solved;
}

bool LGS::IsSolutionPoint()
{
	//Prüfen, ob es überhaupt lösenbar ist
	if ((GetWidth() - 1) > GetHeight())
	{
		return 0;
	}
	//Diagonal abfahren
	for (int i = 0; i < (GetWidth() - 1); i++)
	{
		//Es sollte immer 1 dort sein
		if (matrix[i][i] == 0)
		{
			return 0;
		}
	}

	return 1;
}

float *LGS::GetSolution()
{
	//Lösungsspeicher
	float *sol = new float[(GetWidth() - 1)];
	//Rechte Seite des LGS abfahren
	for (int i = 0; i < (GetWidth() - 1); i++)
	{
		sol[i] = matrix[(GetWidth() - 1)][i];
	}

	return sol;
}

#include <iostream>

int main()
{
	float t0[3] = { 0, 1, 2 };
	float t1[3] = { 3, 4, 5 };
	float t2[3] = { 6, 7, 8 };

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
}


