#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <dinput.h>
#include <xinput.h>
#include "d3dUtil.h"
#include "BasicModel.h"
#include "SkinnedModel.h"

using namespace std;

class Structures
{
public:
	INT ilosc[2], KLA = 111, IloElLab = 1302, MonstersAnimation = 20, PersonAnimation = 200, IloPunkt = 834, IloLiter = 2000, Znaki = 77, *poz_scian, MaxCountArrows = 500, bron = 0, *tab;

	FLOAT *odleglosc, angle = 0.0f;

	BOOL ustawiono[31], ustaw, *ShowArrowOnHedge, **ShowArrowOnMonster;

	struct Vect
	{
		FLOAT x1;
		FLOAT y1;
		FLOAT z1;
		FLOAT x2;
		FLOAT y2;
		FLOAT z2;
	};
	Vect *pozycja_sciany[2], nap[6][5], *punkcior, *boxx, AnglePostac, PozycjaStrzaly;

	struct SkinningModels
	{
		Vect  Pozycja[3];
		FLOAT Kat[4];
		INT Anim;
		SkinnedModelInstance *Ins;
		SkinnedModel **M;
		FLOAT Speed[2];
		BOOL *Animation;
		INT traf;
		BOOL trafienie;
	};
	SkinningModels *Potwor[4], Postac;

	struct Models
	{
		BasicModel *M;
		BasicModel *Model;
		XMFLOAT4X4 World;
		CHAR nazwa_klawisza[2];
	};
	Models *Modele[10], Podloze, **Czarny, **Czerwony, *StrzalaShot, StrzalaShoting;

	struct klawisze
	{
		INT klawisz, ustawiony_klaw;
		string nazwa_klawisza;
		UCHAR keysik;
		INT zapis[19];
	};
	klawisze *klaw;

	struct rozdziel
	{
		INT ind;
		INT width;
		INT height;
	};
	rozdziel rozdz[50];

	ID3D11BlendState *mAlphaEBlendState;
	ID3D11BlendState *mAlphaDBlendState;

public:
	Structures();
	~Structures();

	void InitStructures(ID3D11Device* device);
	void UstWszKl();
};

#endif