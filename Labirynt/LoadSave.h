#ifndef LOADSAVE_H
#define LOADSAVE_H

#include "Meshes.h"

class LoadSave : public Meshes
{
protected:
	INT ZaznaczGuziczek = 462, pierwszy[31], strzaly = 0, punkty = 0, plansza = 1, ilo_bron = 0, scen[3], state = 0, czekanie = 0, sekundy = 0;
	BOOL full = FALSE, zmiana_full = FALSE, zapis[4], wczyt[4], weapons[3];
	FLOAT roz_w, roz_h, xBroniki = 1.65f, ScaleMenu4 = 0.0033f, ScaleGuzik = 0.095f, PosSuwakX = 0.0f, ScaleSuwak = 0.0f, jas_sw = 1.0f, ObjektyScen12X[2], MenuStworzLabX[6], Tlx[3], OpcjeNapX[5], Grafika1Pos[4], OpcjeDzwiekuX[2];
	LONG glosnosc_tla = -5000, glosnosc_efektow = -5000;
	Vect PozGuz[16], vGraNapisy;

	time_t start = 0, TempTime = 0;

	ThirdPersonCamera TPCam;
public:
	LoadSave(HINSTANCE hInstance);
	~LoadSave();

	void pozycja_startowa();
	void PozARozdz();

	void WczytPost(INT i, SkinningModels *Potworek);
	void WczytLab(string sciezka, string nazwa);
	void WczytUstSter(string ZapisUstSterowania, INT klaw1, INT klaw2, INT pierw1, INT pierw2, INT ster);
	void WczytUstDzw();
	void WczytUstGraf();

	void ZapUstSter(string ZapisUstSterowania, INT klaw1, INT klaw2, INT ster);
	void ZapUstGraf();
	void ZapUstDzw();
	void ZapLab(string nazwa);
};

#endif