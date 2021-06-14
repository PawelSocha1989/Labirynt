#ifndef MECHANIC_H
#define MECHANIC_H

#include "Dzwieki.h"
#include "LoadSave.h"

class Mechanic : public LoadSave
{
protected:
	Dzwieki muza;

	string NazwaZapLab;

	BOOL IloscKolizji[2];

	INT reset_czasu = 0, klik[5], ustaw_klaw[31], zapxx[19], ustawione = 1, dl = 0, po = 1;

	time_t stop = 0;

	FLOAT DystansCam = TPCam.charCamDist;

public:
	Mechanic(HINSTANCE hInstance);
	~Mechanic();

	void broniki(INT i);
	void Kolizje(FLOAT dt);
	void KolPotwZZyw(INT i, SkinningModels *Potworek);
	void KolPotwZZywPow(INT i, SkinningModels *Potworek);
	void KolPostZPotw(INT j, SkinningModels *Potworek);
	void KolKamZZyw();

	void AkcjaPost(INT i, SkinningModels *Potworek);
	void Walka(INT i, SkinningModels *Potworek, INT Chod, INT Run, INT JumpRndKick, INT BackLegKick, INT Box1, INT Box2, INT KickPunch, INT Oberwanie, INT Upadek, INT Oberwanie2);

	void ustawianie_klawiszy(INT klawiszek);
	void klawa(INT klawiszek);
	void klawa_up(INT klawiszek);

	void KlawiszologiaKamery(FLOAT dt);
	void KlawiszologiaPostaci(FLOAT dt, ThirdPersonCamera &TPCam);

	void czas();
};

#endif