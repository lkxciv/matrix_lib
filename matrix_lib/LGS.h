#include "primitive_matrix.h"

#ifndef LGS_CLASS
#define LGS_CLASS

class LGS : public Primitive_Matrix
{
private:
	//varsize zu GetWidth() - 1
	//eqsize zu GetHeight()
	//lgs zu matrix
	int varcount = 0; //Arbeitsvariable breite
	int eqcount = 0; //Arbeitsvariable höhe

public:
	//Konstruktor
	LGS(unsigned int variable_num, unsigned int equation_num, float **matrix);
	//Destruktor der Basisklasse wird automatisch aufgerufen

	//Kopierkonstruktor
	LGS(const LGS & copyof);

	//Returnt gelöstes LGS in Stufenform (von LGS in Normalform)
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

#endif // !LGS_CLASS

