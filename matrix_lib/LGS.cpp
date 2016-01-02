#include "LGS.h"

LGS::LGS() : Primitive_Matrix() {}

LGS::LGS(unsigned int variable_num, unsigned int equation_num, float ** matrix) : Primitive_Matrix(matrix, equation_num, (variable_num + 1)) {}

LGS::LGS(const LGS & copyof) : Primitive_Matrix(copyof) {}

//Returnt gelöstes LGS in Stufenform
LGS LGS::Ref()
{
	int yzaehler; //Zaelvariable fuer berechnete Gleichungen
	int ypos = 0;//y Richtungsdurchlauf
	unsigned int xpos = 0;//x Richtungsdurchlauf
	unsigned int elimx = 0;//ist beginn der treppendurchlauefe in x richtung, zaehlt die eliminationsvorgaenge der variablen
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

																											//Anfang y-pos
	if (GetHeight() > (GetWidth() - 1))
	{
		//Man hat Breite - 1 Variablen, Arrays beginen ab null -> -2
		ypos = (GetWidth() - 2);
	}
	else if ((GetWidth() - 1) >= GetHeight())
	{
		ypos = GetHeight() - 1;
	}

	//LGS wird diagonal abgefahren von Unten rechts nach oben links
	for (xpos = (GetWidth() - 1) - 1; xpos >= 0; xpos--)
	{
		for (unsigned int varstep = xpos + 1; varstep < (GetWidth() - 1); varstep++)
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
	for (unsigned int i = 0; i < (GetWidth() - 1); i++)
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
	for (unsigned int i = 0; i < (GetWidth() - 1); i++)
	{
		sol[i] = matrix[(GetWidth() - 1)][i];
	}

	return sol;
}
