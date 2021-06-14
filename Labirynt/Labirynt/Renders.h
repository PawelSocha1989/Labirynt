#ifndef RENDERS_H
#define RENDERS_H

#include "dirent.h"
#include "Objects.h"

class Renders : public Objects
{
protected:
	INT t = 0, p = 0, w = 391, ZaznGuz = 15, index2 = 0, SingleThink[7], frame[19], IloscPlikow[2][2], OdtwDzwMen[20][18], Odtw[20][18], nowy[2];
	BOOL frejmik[1400];
	string *pliki[2];

public:
	Renders(HINSTANCE hInstance);
	~Renders();

	void wysw();
	void odzn();
	void zazn();
	void odznaczenie(FLOAT xodzn, FLOAT yodzn, FLOAT rozx, FLOAT rozy);
	void zaznaczenie(FLOAT xzazn, FLOAT yzazn, FLOAT rozx, FLOAT rozy);
	void ZaznKeys();

	void ListaRozdz();
	void ListaLabiryntow();
	void fram();
};

#endif