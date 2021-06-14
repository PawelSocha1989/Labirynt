#include "Mechanic.h"

Mechanic::Mechanic(HINSTANCE hInstance) : LoadSave(hInstance)
{
	//BOOL
	IloscKolizji[0] = FALSE; IloscKolizji[1] = FALSE;

	for (INT ilo = 0; ilo < 5; ilo++)
		klik[ilo] = 0;

	for (INT i = 0; i < 19; i++)
		zapxx[i] = 0;

	for (INT i = 0; i < 31; i++)
		ustaw_klaw[i] = 0;
}

Mechanic::~Mechanic()
{
}

void Mechanic::broniki(INT i)
{
	if (tab[i] == 4)
	{
		weapons[0] = true;
		strzaly += 5;
		tab[i] = 0;
		ilo_bron++;
	}

	if (tab[i] == 6)
	{
		strzaly += 5;
		tab[i] = 0;
	}

	if (tab[i] == 5)
	{
		weapons[1] = true;
		tab[i] = 0;
		ilo_bron++;
	}

	if (tab[i] == 7)
	{
		weapons[2] = true;
		tab[i] = 0;
		ilo_bron++;
	}

	if (tab[i] == 12)
	{
		tab[i] = 0;
		punkty += 5;
	}

	if (tab[i] == 3)
	{
		tab[i] = 0;

		INT punkciczek = 500;
		if (sekundy <= 180)
			punkciczek = 500;

		else if (sekundy > 180)
		{
			INT points = sekundy / 8;
			punkciczek = punkciczek - points;
		}

		punkty += punkciczek;
		plansza++;

		if (plansza > 5)
			wygrana = TRUE;

		else if (plansza <= 5)
		{
			WczytLab("GotowePlansze\\", "nowaplansza" + to_string(plansza));
			pozycja_startowa();
			czekanie = state = 0;
			start = clock();
		}
	}
}

void Mechanic::Kolizje(FLOAT dt)
{
	if (XMVectorGetX(TPCam.charPosition) <= -4500.0f)
	{
		TPCam.charPosition = XMVectorSetX(TPCam.charPosition, -4500.0f);
		XMMATRIX Translate = XMMatrixTranslation(-4500.0f, 0.1f, XMVectorGetZ(TPCam.charPosition));
		TPCam.playerCharWorld = Translate;
	}

	if (XMVectorGetX(TPCam.charPosition) >= 4500.0f)
	{
		TPCam.charPosition = XMVectorSetX(TPCam.charPosition, 4500.0f);
		XMMATRIX Translate = XMMatrixTranslation(4500.0f, 0.1f, XMVectorGetZ(TPCam.charPosition));
		TPCam.playerCharWorld = Translate;
	}

	if (XMVectorGetZ(TPCam.charPosition) <= -4500.0f)
	{
		TPCam.charPosition = XMVectorSetZ(TPCam.charPosition, -4500.0f);
		XMMATRIX Translate = XMMatrixTranslation(XMVectorGetX(TPCam.charPosition), 0.1f, -4500.0f);
		TPCam.playerCharWorld = Translate;
	}

	if (XMVectorGetZ(TPCam.charPosition) >= 4500.0f)
	{
		TPCam.charPosition = XMVectorSetZ(TPCam.charPosition, -4500.0f);
		XMMATRIX Translate = XMMatrixTranslation(XMVectorGetX(TPCam.charPosition), 0.1f, 4500.0f);
		TPCam.playerCharWorld = Translate;
	}

	for (INT i = 0; i < IloElLab; i++)
	{
		if ((XMVectorGetX(TPCam.charPosition) >= boxx[i].x1 - 10.0f) &&
			(XMVectorGetX(TPCam.charPosition) <= boxx[i].x1 + 10.0f) &&
			(XMVectorGetZ(TPCam.charPosition) >= boxx[i].z1 - 12.0f) &&
			(XMVectorGetZ(TPCam.charPosition) <= boxx[i].z1 - 10.0f) &&
			(tab[i + 26] != 1) && (tab[i] == 1))
		{
			TPCam.charPosition = XMVectorSetZ(TPCam.charPosition, boxx[i].z1 - 12.0f);
			XMMATRIX Translate = XMMatrixTranslation(XMVectorGetX(TPCam.charPosition), -0.1f, boxx[i].z1 - 12.0f);
			TPCam.playerCharWorld = Translate;
		}

		if ((XMVectorGetX(TPCam.charPosition) >= boxx[i].x1 - 10.0f) &&
			(XMVectorGetX(TPCam.charPosition) <= boxx[i].x1 + 10.0f) &&
			(XMVectorGetZ(TPCam.charPosition) <= boxx[i].z1 + 12.0f) &&
			(XMVectorGetZ(TPCam.charPosition) >= boxx[i].z1 + 10.0f) &&
			(tab[i - 26] != 1) && (tab[i] == 1))
		{
			TPCam.charPosition = XMVectorSetZ(TPCam.charPosition, boxx[i].z1 + 12.0f);
			XMMATRIX Translate = XMMatrixTranslation(XMVectorGetX(TPCam.charPosition), -0.1f, boxx[i].z1 + 12.0f);
			TPCam.playerCharWorld = Translate;
		}

		if ((XMVectorGetZ(TPCam.charPosition) >= boxx[i].z1 - 10.0f) &&
			(XMVectorGetZ(TPCam.charPosition) <= boxx[i].z1 + 10.0f) &&
			(XMVectorGetX(TPCam.charPosition) >= boxx[i].x1 - 12.0f) &&
			(XMVectorGetX(TPCam.charPosition) <= boxx[i].x1 - 10.0) &&
			(tab[i - 1] != 1) && (tab[i] == 1))
		{
			TPCam.charPosition = XMVectorSetX(TPCam.charPosition, boxx[i].x1 - 12.0f);
			XMMATRIX Translate = XMMatrixTranslation(boxx[i].x1 - 12.0f, -0.1f, XMVectorGetZ(TPCam.charPosition));
			TPCam.playerCharWorld = Translate;
		}

		if ((XMVectorGetZ(TPCam.charPosition) >= boxx[i].z1 - 10.0f) &&
			(XMVectorGetZ(TPCam.charPosition) <= boxx[i].z1 + 10.0f) &&
			(XMVectorGetX(TPCam.charPosition) <= boxx[i].x1 + 12.0f) &&
			(XMVectorGetX(TPCam.charPosition) >= boxx[i].x1 + 10.0f) &&
			(tab[i + 1] != 1) && (tab[i] == 1))
		{
			TPCam.charPosition = XMVectorSetX(TPCam.charPosition, boxx[i].x1 + 12.0f);
			XMMATRIX Translate = XMMatrixTranslation(boxx[i].x1 + 12.0f, -0.1f, XMVectorGetZ(TPCam.charPosition));
			TPCam.playerCharWorld = Translate;
		}

		if (tab[i] == 4 || tab[i] == 7 || tab[i] == 5 || tab[i] == 6 || tab[i] == 12 || tab[i] == 3)
		{
			if ((XMVectorGetX(TPCam.charPosition) >= boxx[i].x1 - 2.0f) &&
				(XMVectorGetX(TPCam.charPosition) <= boxx[i].x1 + 2.0f) &&
				(XMVectorGetZ(TPCam.charPosition) >= boxx[i].z1 - 4.0f) &&
				(XMVectorGetZ(TPCam.charPosition) <= boxx[i].z1 - 3.0f))
				broniki(i);

			if ((XMVectorGetX(TPCam.charPosition) >= boxx[i].x1 - 2.0f) &&
				(XMVectorGetX(TPCam.charPosition) <= boxx[i].x1 + 2.0f) &&
				(XMVectorGetZ(TPCam.charPosition) <= boxx[i].z1 + 4.0f) &&
				(XMVectorGetZ(TPCam.charPosition) >= boxx[i].z1 + 3.0f))
				broniki(i);

			if ((XMVectorGetZ(TPCam.charPosition) >= boxx[i].z1 - 2.0f) &&
				(XMVectorGetZ(TPCam.charPosition) <= boxx[i].z1 + 2.0f) &&
				(XMVectorGetX(TPCam.charPosition) >= boxx[i].x1 - 4.0f) &&
				(XMVectorGetX(TPCam.charPosition) <= boxx[i].x1 - 3.0f))
				broniki(i);

			if ((XMVectorGetZ(TPCam.charPosition) >= boxx[i].z1 - 2.0f) &&
				(XMVectorGetZ(TPCam.charPosition) <= boxx[i].z1 + 2.0f) &&
				(XMVectorGetX(TPCam.charPosition) <= boxx[i].x1 + 4.0f) &&
				(XMVectorGetX(TPCam.charPosition) >= boxx[i].x1 + 3.0f))
				broniki(i);
		}
	}
}

void Mechanic::KolPotwZZyw(INT i, SkinningModels * Potworek)
{
	for (INT ilo2 = 0; ilo2 < IloElLab; ilo2++)
	{
		if (tab[ilo2] == 1)
		{
			if ((Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 10.0f) &&
				(Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 12.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 - 10.0f) &&
				(tab[ilo2 + 26] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14))
			{
				IloscKolizji[0] = TRUE;
				IloscKolizji[1] = FALSE;

				for (INT ilo3 = 0; ilo3 < MaxCountArrows; ilo3++)
				{
					if (ShowArrowOnMonster[i][ilo3])
					{
						AngleXTrajShot[i][ilo3] = atan2(CTrajShot[i][ilo3].z1 - XMVectorGetZ(TPCam.charPosition), CTrajShot[i][ilo3].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;

						if (XMVectorGetX(TPCam.charPosition) > CTrajShot[i][ilo3].x1)
							CTrajShot[i][ilo3].x1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo3].x1 > XMVectorGetX(TPCam.charPosition))
							CTrajShot[i][ilo3].x1 -= Potworek[i].Speed[1];

						CTrajShot[i][ilo3].z1 = boxx[ilo2].z1 - 12.0f;
					}
				}
				Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - XMVectorGetZ(TPCam.charPosition), Potworek[i].Pozycja[0].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;

				if (XMVectorGetX(TPCam.charPosition) > Potworek[i].Pozycja[0].x1)
					Potworek[i].Pozycja[0].x1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].x1 > XMVectorGetX(TPCam.charPosition))
					Potworek[i].Pozycja[0].x1 -= Potworek[i].Speed[1];

				Potworek[i].Pozycja[0].z1 = boxx[ilo2].z1 - 12.0f;

				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}

			if ((Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 12.0f) &&
				(Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 + 10.0f) &&
				(tab[ilo2 - 26] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14))
			{
				IloscKolizji[0] = TRUE;
				IloscKolizji[1] = FALSE;

				for (INT ilo3 = 0; ilo3 < MaxCountArrows; ilo3++)
				{
					if (ShowArrowOnMonster[i][ilo3])
					{
						AngleXTrajShot[i][ilo3] = atan2(CTrajShot[i][ilo3].z1 - XMVectorGetZ(TPCam.charPosition), CTrajShot[i][ilo3].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;

						if (XMVectorGetX(TPCam.charPosition) > CTrajShot[i][ilo3].x1)
							CTrajShot[i][ilo3].x1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo3].x1 > XMVectorGetX(TPCam.charPosition))
							CTrajShot[i][ilo3].x1 -= Potworek[i].Speed[1];

						CTrajShot[i][ilo3].z1 = boxx[ilo2].z1 + 12.0f;
					}
				}

				Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - XMVectorGetZ(TPCam.charPosition), Potworek[i].Pozycja[0].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;

				if (XMVectorGetX(TPCam.charPosition) > Potworek[i].Pozycja[0].x1)
					Potworek[i].Pozycja[0].x1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].x1 > XMVectorGetX(TPCam.charPosition))
					Potworek[i].Pozycja[0].x1 -= Potworek[i].Speed[1];

				Potworek[i].Pozycja[0].z1 = boxx[ilo2].z1 + 12.0f;
				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}

			if (((Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 10.0f) &&
				(Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 12.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 - 10.0f) &&
				(tab[ilo2 - 1] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)))
			{
				IloscKolizji[0] = TRUE;
				IloscKolizji[1] = FALSE;

				for (INT ilo3 = 0; ilo3 < MaxCountArrows; ilo3++)
				{
					if (ShowArrowOnMonster[i][ilo3])
					{
						AngleXTrajShot[i][ilo3] = atan2(CTrajShot[i][ilo3].z1 - XMVectorGetZ(TPCam.charPosition), CTrajShot[i][ilo3].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;
						CTrajShot[i][ilo3].x1 = boxx[ilo2].x1 - 12.0f;

						if (XMVectorGetX(TPCam.charPosition) > CTrajShot[i][ilo3].z1)
							CTrajShot[i][ilo3].z1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo3].z1 > XMVectorGetX(TPCam.charPosition))
							CTrajShot[i][ilo3].z1 -= Potworek[i].Speed[1];
					}
				}
				Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - XMVectorGetZ(TPCam.charPosition), Potworek[i].Pozycja[0].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;
				Potworek[i].Pozycja[0].x1 = boxx[ilo2].x1 - 12.0f;

				if (XMVectorGetZ(TPCam.charPosition) > Potworek[i].Pozycja[0].z1)
					Potworek[i].Pozycja[0].z1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].z1 > XMVectorGetZ(TPCam.charPosition))
					Potworek[i].Pozycja[0].z1 -= Potworek[i].Speed[1];

				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}

			if ((Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 12.0f) &&
				(Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 + 10.0f) &&
				(tab[ilo2 + 1] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14))
			{
				IloscKolizji[0] = TRUE;
				IloscKolizji[1] = FALSE;

				for (INT ilo3 = 0; ilo3 < MaxCountArrows; ilo3++)
				{
					if (ShowArrowOnMonster[i][ilo3])
					{
						AngleXTrajShot[i][ilo3] = atan2(CTrajShot[i][ilo3].z1 - XMVectorGetZ(TPCam.charPosition), CTrajShot[i][ilo3].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;
						CTrajShot[i][ilo3].x1 = boxx[ilo2].x1 + 12.0f;

						if (XMVectorGetX(TPCam.charPosition) > CTrajShot[i][ilo3].z1)
							CTrajShot[i][ilo3].z1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo3].z1 > XMVectorGetX(TPCam.charPosition))
							CTrajShot[i][ilo3].z1 -= Potworek[i].Speed[1];
					}
				}
				Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - XMVectorGetZ(TPCam.charPosition), Potworek[i].Pozycja[0].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;
				Potworek[i].Pozycja[0].x1 = boxx[ilo2].x1 + 12.0f;

				if (XMVectorGetZ(TPCam.charPosition) > Potworek[i].Pozycja[0].z1)
					Potworek[i].Pozycja[0].z1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].z1 > XMVectorGetZ(TPCam.charPosition))
					Potworek[i].Pozycja[0].z1 -= Potworek[i].Speed[1];

				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}
		}
	}
}

void Mechanic::KolPotwZZywPow(INT i, SkinningModels * Potworek)
{
	for (INT ilo2 = 0; ilo2 < IloElLab; ilo2++)
	{
		if (tab[ilo2] == 1)
		{
			if (((Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 10.0f) &&
				(Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 12.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 - 10.0f) &&
				(tab[ilo2 + 26] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)) ||
				((Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 10.0f) &&
				(Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 12.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 - 10.0f) &&
				(tab[ilo2 - 26] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)))
			{
				IloscKolizji[1] = TRUE;
				IloscKolizji[0] = FALSE;

				for (INT ilo3 = 0; ilo3 < MaxCountArrows; ilo3++)
				{
					if (ShowArrowOnMonster[i][ilo3])
					{
						AngleXTrajShot[i][ilo3] = atan2(CTrajShot[i][ilo3].z1 - Potworek[i].Pozycja[1].z1, CTrajShot[i][ilo3].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;

						if (Potworek[i].Pozycja[1].x1 >= CTrajShot[i][ilo3].x1)
							CTrajShot[i][ilo3].x1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo3].x1 >= Potworek[i].Pozycja[1].x1)
							CTrajShot[i][ilo3].x1 -= Potworek[i].Speed[1];

						CTrajShot[i][ilo3].z1 = boxx[ilo2].z1 - 12.0f;
					}
				}

				Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - Potworek[i].Pozycja[1].z1, Potworek[i].Pozycja[0].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;

				if (Potworek[i].Pozycja[1].x1 >= Potworek[i].Pozycja[0].x1)
					Potworek[i].Pozycja[0].x1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].x1 >= Potworek[i].Pozycja[1].x1)
					Potworek[i].Pozycja[0].x1 -= Potworek[i].Speed[1];

				Potworek[i].Pozycja[0].z1 = boxx[ilo2].z1 - 12.0f;

				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}

			else if (((Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 12.0f) &&
				(Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 + 10.0f) &&
				(tab[ilo2 + 26] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)) ||
				((Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 12.0f) &&
				(Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 + 10.0f) &&
				(tab[ilo2 - 26] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)))
			{
				IloscKolizji[1] = TRUE;
				IloscKolizji[0] = FALSE;

				for (INT ilo3 = 0; ilo3 < MaxCountArrows; ilo3++)
				{
					if (ShowArrowOnMonster[i][ilo3])
					{
						AngleXTrajShot[i][ilo3] = atan2(CTrajShot[i][ilo3].z1 - Potworek[i].Pozycja[1].z1, CTrajShot[i][ilo3].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;

						if (Potworek[i].Pozycja[1].x1 >= CTrajShot[i][ilo3].x1)
							CTrajShot[i][ilo3].x1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo3].x1 >= Potworek[i].Pozycja[1].x1)
							CTrajShot[i][ilo3].x1 -= Potworek[i].Speed[1];

						CTrajShot[i][ilo3].z1 = boxx[ilo2].z1 + 12.0f;
					}
				}

				Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - Potworek[i].Pozycja[1].z1, Potworek[i].Pozycja[0].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;

				if (Potworek[i].Pozycja[1].x1 >= Potworek[i].Pozycja[0].x1)
					Potworek[i].Pozycja[0].x1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].x1 >= Potworek[i].Pozycja[1].x1)
					Potworek[i].Pozycja[0].x1 -= Potworek[i].Speed[1];

				Potworek[i].Pozycja[0].z1 = boxx[ilo2].z1 + 12.0f;

				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}

			else if (((Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 10.0f) &&
				(Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 12.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 - 10.0f) &&
				(tab[ilo2 - 1] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)) ||
				((Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 10.0f) &&
				(Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 12.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 - 10.0f) &&
				(tab[ilo2 - 1] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)) ||
				((Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 10.0f) &&
				(Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 12.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 - 10.0f) &&
				(tab[ilo2 + 1] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)) ||
				((Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 10.0f) &&
				(Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 - 12.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 - 10.0f) &&
				(tab[ilo2 + 1] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)))

			{
				IloscKolizji[1] = TRUE;
				IloscKolizji[0] = FALSE;

				for (INT ilo3 = 0; ilo3 < MaxCountArrows; ilo3++)
				{
					if (ShowArrowOnMonster[i][ilo3])
					{
						AngleXTrajShot[i][ilo3] = atan2(CTrajShot[i][ilo3].z1 - Potworek[i].Pozycja[1].z1, CTrajShot[i][ilo3].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;
						CTrajShot[i][ilo3].x1 = boxx[ilo2].x1 - 12.0f;

						if (Potworek[i].Pozycja[1].z1 >= CTrajShot[i][ilo3].z1)
							CTrajShot[i][ilo3].z1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo3].z1 >= Potworek[i].Pozycja[1].z1)
							CTrajShot[i][ilo3].z1 -= Potworek[i].Speed[1];
					}
				}

				Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - Potworek[i].Pozycja[1].z1, Potworek[i].Pozycja[0].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;
				Potworek[i].Pozycja[0].x1 = boxx[ilo2].x1 - 12.0f;

				if (Potworek[i].Pozycja[1].z1 >= Potworek[i].Pozycja[0].z1)
					Potworek[i].Pozycja[0].z1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].z1 >= Potworek[i].Pozycja[1].z1)
					Potworek[i].Pozycja[0].z1 -= Potworek[i].Speed[1];

				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}

			else if (((Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 12.0f) &&
				(Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 + 10.0f) &&
				(tab[ilo2 + 1] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)) ||
				((Potworek[i].Pozycja[0].z1 >= boxx[ilo2].z1 - 10.0f) &&
				(Potworek[i].Pozycja[0].z1 <= boxx[ilo2].z1 + 10.0f) &&
				(Potworek[i].Pozycja[0].x1 <= boxx[ilo2].x1 + 12.0f) &&
				(Potworek[i].Pozycja[0].x1 >= boxx[ilo2].x1 + 10.0f) &&
				(tab[ilo2 - 1] != 1 && po % 2 == 1 && !wygrana && Anim[0] != 14)))
			{
				IloscKolizji[1] = TRUE;
				IloscKolizji[0] = FALSE;

				for (INT ilo3 = 0; ilo3 < MaxCountArrows; ilo3++)
				{
					if (ShowArrowOnMonster[i][ilo3])
					{
						AngleXTrajShot[i][ilo3] = atan2(CTrajShot[i][ilo3].z1 - Potworek[i].Pozycja[1].z1, CTrajShot[i][ilo3].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;

						CTrajShot[i][ilo3].x1 = boxx[ilo2].x1 + 12.0f;

						if (Potworek[i].Pozycja[1].z1 >= CTrajShot[i][ilo3].z1)
							CTrajShot[i][ilo3].z1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo3].z1 >= Potworek[i].Pozycja[1].z1)
							CTrajShot[i][ilo3].z1 -= Potworek[i].Speed[1];
					}
				}

				Potworek[i].Pozycja[0].x1 = boxx[ilo2].x1 + 12.0f;

				if (Potworek[i].Pozycja[1].z1 >= Potworek[i].Pozycja[0].z1)
					Potworek[i].Pozycja[0].z1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].z1 >= Potworek[i].Pozycja[1].z1)
					Potworek[i].Pozycja[0].z1 -= Potworek[i].Speed[1];

				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}
		}
	}
}

void Mechanic::KolKamZZyw()
{
	for (INT i = 0; i < IloElLab; i++)
	{
		if (tab[i] == 1)
		{
			if ((XMVectorGetX(TPCam.CamPosition) >= boxx[i].x1 - 10.0f) &&
				(XMVectorGetX(TPCam.CamPosition) <= boxx[i].x1 + 10.0f) &&
				(XMVectorGetZ(TPCam.CamPosition) <= boxx[i].z1 + 8.0f) &&
				(XMVectorGetZ(TPCam.CamPosition) >= boxx[i].z1 - 12.0f) &&
				(XMVectorGetZ(TPCam.CamPosition) - (boxx[i].z1 - 12.0f) < 15.0f))
			{
				if (DystansCam > XMVectorGetZ(TPCam.CamPosition) - (boxx[i].z1 - 12.0f))
				{
					DystansCam -= 0.5f;
					TPCam.CamPosition = XMVectorSetZ(TPCam.CamPosition, boxx[i].z1 - 12.0f);
				}

				else if (DystansCam < XMVectorGetZ(TPCam.CamPosition) - (boxx[i].z1 - 12.0f))
				{
					DystansCam += 0.5f;
					TPCam.CamPosition = XMVectorSetZ(TPCam.CamPosition, boxx[i].z1 - 12.0f);
				}
			}

			else if ((XMVectorGetX(TPCam.CamPosition) >= boxx[i].x1 - 20.0f) &&
					(XMVectorGetX(TPCam.CamPosition) <= boxx[i].x1 + 20.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) >= boxx[i].z1 - 20.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) <= boxx[i].z1 - 12.5f))
			{
				DystansCam += 0.5f;
				TPCam.CamPosition = XMVectorSetZ(TPCam.CamPosition, boxx[i].z1 - 12.0f);
			}
					
			else if ((XMVectorGetX(TPCam.CamPosition) >= boxx[i].x1 - 10.0f) &&
					(XMVectorGetX(TPCam.CamPosition) <= boxx[i].x1 + 10.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) >= boxx[i].z1 - 8.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) <= boxx[i].z1 + 12.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) - (boxx[i].z1 + 12.0f) < 15.0f))
			{
				if (DystansCam < XMVectorGetZ(TPCam.CamPosition) - (boxx[i].z1 + 12.0f))
				{
					DystansCam += 0.5f;
					TPCam.CamPosition = XMVectorSetZ(TPCam.CamPosition, boxx[i].z1 + 12.0f);
				}
					
				else if (DystansCam > XMVectorGetZ(TPCam.CamPosition) - (boxx[i].z1 + 12.0f))
				{
					DystansCam -= 0.5f;
					TPCam.CamPosition = XMVectorSetZ(TPCam.CamPosition, boxx[i].z1 + 12.0f);
				}
					
			}

			else if ((XMVectorGetX(TPCam.CamPosition) >= boxx[i].x1 - 20.0f) &&
					(XMVectorGetX(TPCam.CamPosition) <= boxx[i].x1 + 20.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) <= boxx[i].z1 + 20.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) >= boxx[i].z1 + 12.5f))
			{
				DystansCam += 0.5f;
				TPCam.CamPosition = XMVectorSetZ(TPCam.CamPosition, boxx[i].z1 + 12.0f);
			}
					
			else if ((XMVectorGetZ(TPCam.CamPosition) >= boxx[i].z1 - 10.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) <= boxx[i].z1 + 10.0f) &&
					(XMVectorGetX(TPCam.CamPosition) <= boxx[i].x1 + 8.0f) &&
					(XMVectorGetX(TPCam.CamPosition) >= boxx[i].x1 - 12.0f) &&
					(XMVectorGetX(TPCam.CamPosition) - (boxx[i].x1 - 12.0f) < 15.0f))
			{
				if (DystansCam > XMVectorGetX(TPCam.CamPosition) - (boxx[i].x1 - 12.0f))
				{
					DystansCam -= 0.5f;
					TPCam.CamPosition = XMVectorSetX(TPCam.CamPosition, boxx[i].x1 - 12.0f);
				}
					
				else if (DystansCam < XMVectorGetX(TPCam.CamPosition) - (boxx[i].x1 - 12.0f))
				{
					DystansCam += 0.5f;
					TPCam.CamPosition = XMVectorSetX(TPCam.CamPosition, boxx[i].x1 - 12.0f);
				}
						
			}

			else if ((XMVectorGetZ(TPCam.CamPosition) >= boxx[i].z1 - 20.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) <= boxx[i].z1 + 20.0f) &&
					(XMVectorGetX(TPCam.CamPosition) >= boxx[i].x1 - 20.0f) &&
					(XMVectorGetX(TPCam.CamPosition) <= boxx[i].x1 - 12.5f))
			{
				DystansCam += 0.5f;
				TPCam.CamPosition = XMVectorSetX(TPCam.CamPosition, boxx[i].x1 - 12.0f);
			}
					
			else if ((XMVectorGetZ(TPCam.CamPosition) >= boxx[i].z1 - 10.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) <= boxx[i].z1 + 10.0f) &&
					(XMVectorGetX(TPCam.CamPosition) >= boxx[i].x1 - 8.0f) &&
					(XMVectorGetX(TPCam.CamPosition) <= boxx[i].x1 + 12.0f) &&
					(XMVectorGetX(TPCam.CamPosition) - (boxx[i].x1 + 12.0f) < 15.0f))
			{
				if (DystansCam < XMVectorGetX(TPCam.CamPosition) - (boxx[i].x1 + 12.0f))
				{
					DystansCam += 0.5f;
					TPCam.CamPosition = XMVectorSetX(TPCam.CamPosition, boxx[i].x1 + 12.0f);
				}
					
				else if (DystansCam > XMVectorGetX(TPCam.CamPosition) - (boxx[i].x1 + 12.0f))
				{
					DystansCam -= 0.5f;
					TPCam.CamPosition = XMVectorSetX(TPCam.CamPosition, boxx[i].x1 + 12.0f);
				}
					
			}

			else if ((XMVectorGetZ(TPCam.CamPosition) >= boxx[i].z1 - 20.0f) &&
					(XMVectorGetZ(TPCam.CamPosition) <= boxx[i].z1 + 20.0f) &&
					(XMVectorGetX(TPCam.CamPosition) <= boxx[i].x1 + 20.0f) &&
					(XMVectorGetX(TPCam.CamPosition) >= boxx[i].x1 + 12.5f))
			{
				DystansCam += 0.5f;
				TPCam.CamPosition = XMVectorSetX(TPCam.CamPosition, boxx[i].x1 + 12.0f);
			}
		}

		if (DystansCam != TPCam.charCamDist)
			TPCam.charCamDist = DystansCam;
	}

	if (DystansCam >= 0.0f)
	{
		if (DystansCam >= 15.0f)
			DystansCam = 15.0f;
			
		else if (DystansCam <= 5.0f)
			DystansCam = 5.0f;

		if (DystansCam != TPCam.charCamDist)
			TPCam.charCamDist = DystansCam;
	}
}

void Mechanic::KolPostZPotw(INT i, SkinningModels *Potworek)
{
	if ((XMVectorGetX(TPCam.charPosition) >= Potworek[i].Pozycja[0].x1 - 2.0f) &&
		(XMVectorGetX(TPCam.charPosition) <= Potworek[i].Pozycja[0].x1 + 2.0f) &&
		(XMVectorGetZ(TPCam.charPosition) >= Potworek[i].Pozycja[0].z1 - 4.0f) &&
		(XMVectorGetZ(TPCam.charPosition) <= Potworek[i].Pozycja[0].z1 - 2.0f))
	{
		TPCam.charPosition = XMVectorSetZ(TPCam.charPosition, Potworek[i].Pozycja[0].z1 - 4.0f);
		XMMATRIX Translate = XMMatrixTranslation(XMVectorGetX(TPCam.charPosition), -0.1f, Potworek[i].Pozycja[0].z1 - 4.0f);
		TPCam.playerCharWorld = Translate;
	}

	if ((XMVectorGetX(TPCam.charPosition) >= Potworek[i].Pozycja[0].x1 - 2.0f) &&
		(XMVectorGetX(TPCam.charPosition) <= Potworek[i].Pozycja[0].x1 + 2.0f) &&
		(XMVectorGetZ(TPCam.charPosition) <= Potworek[i].Pozycja[0].z1 + 4.0f) &&
		(XMVectorGetZ(TPCam.charPosition) >= Potworek[i].Pozycja[0].z1 + 2.0f))
	{
		TPCam.charPosition = XMVectorSetZ(TPCam.charPosition, Potworek[i].Pozycja[0].z1 + 4.0f);
		XMMATRIX Translate = XMMatrixTranslation(XMVectorGetX(TPCam.charPosition), -0.1f, Potworek[i].Pozycja[0].z1 + 4.0f);
		TPCam.playerCharWorld = Translate;
	}

	if ((XMVectorGetZ(TPCam.charPosition) >= Potworek[i].Pozycja[0].z1 - 2.0f) &&
		(XMVectorGetZ(TPCam.charPosition) <= Potworek[i].Pozycja[0].z1 + 2.0f) &&
		(XMVectorGetX(TPCam.charPosition) >= Potworek[i].Pozycja[0].x1 - 4.0f) &&
		(XMVectorGetX(TPCam.charPosition) <= Potworek[i].Pozycja[0].x1 - 2.0))
	{
		TPCam.charPosition = XMVectorSetX(TPCam.charPosition, Potworek[i].Pozycja[0].x1 - 4.0f);
		XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1 - 4.0f, -0.1f, XMVectorGetZ(TPCam.charPosition));
		TPCam.playerCharWorld = Translate;
	}

	if ((XMVectorGetZ(TPCam.charPosition) >= Potworek[i].Pozycja[0].z1 - 2.0f) &&
		(XMVectorGetZ(TPCam.charPosition) <= Potworek[i].Pozycja[0].z1 + 2.0f) &&
		(XMVectorGetX(TPCam.charPosition) <= Potworek[i].Pozycja[0].x1 + 4.0f) &&
		(XMVectorGetX(TPCam.charPosition) >= Potworek[i].Pozycja[0].x1 + 2.0f))
	{
		TPCam.charPosition = XMVectorSetX(TPCam.charPosition, Potworek[i].Pozycja[0].x1 + 4.0f);
		XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1 + 4.0f, -0.1f, XMVectorGetZ(TPCam.charPosition));
		TPCam.playerCharWorld = Translate;
	}
}

void Mechanic::AkcjaPost(INT i, SkinningModels *Potworek)
{
	if (Potworek[i].traf > 9 && Potworek[i].Anim == 8 && Potworek[i].Ins[8].TimePos >= Potworek[i].Ins[8].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[8].ClipName))
	{
		tab[i] = 0;

		if (Potwor[0][i].traf > 9)
			punkty += 100;

		if (Potwor[1][i].traf > 9)
			punkty += 200;

		if (Potwor[2][i].traf > 9)
			punkty += 100;

		if (Potwor[3][i].traf > 9)
			punkty += 200;
	}


	odleglosc[i] = sqrt(pow((XMVectorGetZ(TPCam.charPosition)) - Potworek[i].Pozycja[0].z1, 2) + pow((XMVectorGetX(TPCam.charPosition)) - Potworek[i].Pozycja[0].x1, 2));

	if (odleglosc[i] <= 40.0f && !Postac.Animation[14])
	{
		IloscKolizji[0] = FALSE;
		IloscKolizji[1] = FALSE;

		if (Potworek[i].Kat[3] > 1.0f)
		{
			Potworek[i].Kat[3] = 0.0f;
			Potworek[i].Kat[2] = 0.0f;
		}

		if (odleglosc[i] > 10.0f)
		{
			Potworek[i].Anim = 1;
			for (INT il = 0; il < 9; il++)
				Potworek[i].Animation[il] = FALSE;

			Potworek[i].trafienie = FALSE;
			Potworek[i].Speed[1] = 0.07f;

			KolPotwZZyw(i, Potworek);

			if (!IloscKolizji[0] && po % 2 == 1 && !wygrana && Anim[0] != 14)
			{
				for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
				{
					if (ShowArrowOnMonster[i][ilo2])
					{
						AngleXTrajShot[i][ilo2] = atan2(CTrajShot[i][ilo2].z1 - XMVectorGetZ(TPCam.charPosition), CTrajShot[i][ilo2].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;

						if (XMVectorGetZ(TPCam.charPosition) > CTrajShot[i][ilo2].z1)
							CTrajShot[i][ilo2].z1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo2].z1 > XMVectorGetZ(TPCam.charPosition))
							CTrajShot[i][ilo2].z1 -= Potworek[i].Speed[1];

						if (XMVectorGetX(TPCam.charPosition) > CTrajShot[i][ilo2].x1)
							CTrajShot[i][ilo2].x1 += Potworek[i].Speed[1];
						else if (CTrajShot[i][ilo2].x1 > XMVectorGetX(TPCam.charPosition))
							CTrajShot[i][ilo2].x1 -= Potworek[i].Speed[1];
					}
				}

				Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - XMVectorGetZ(TPCam.charPosition), Potworek[i].Pozycja[0].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;

				if (XMVectorGetZ(TPCam.charPosition) > Potworek[i].Pozycja[0].z1)
					Potworek[i].Pozycja[0].z1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].z1 > XMVectorGetZ(TPCam.charPosition))
					Potworek[i].Pozycja[0].z1 -= Potworek[i].Speed[1];

				if (XMVectorGetX(TPCam.charPosition) > Potworek[i].Pozycja[0].x1)
					Potworek[i].Pozycja[0].x1 += Potworek[i].Speed[1];
				else if (Potworek[i].Pozycja[0].x1 > XMVectorGetX(TPCam.charPosition))
					Potworek[i].Pozycja[0].x1 -= Potworek[i].Speed[1];

				XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
				XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
				XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
			}
		}

		else if (odleglosc[i] <= 10.0f && po % 2 == 1 && !wygrana && Anim[0] != 14)
		{
			KolPotwZZyw(i, Potworek);
			Walka(i, Potworek, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
		}

		Potworek[i].Kat[2] = Potworek[i].Kat[0];
		Potworek[i].Pozycja[2].x1 = Potworek[i].Pozycja[0].x1;
		Potworek[i].Pozycja[2].z1 = Potworek[i].Pozycja[0].z1;
	}

	else if (Potworek[i].trafienie && !Potworek[i].Animation[2] && !Potworek[i].Animation[3] && !Potworek[i].Animation[4] && !Potworek[i].Animation[5] && !Potworek[i].Animation[6])
	{
		if (Potworek[i].traf <= 9)
		{
			Potworek[i].traf++;
			if (!Potworek[i].Animation[7])
			{
				Potworek[i].Ins[7].TimePos = 0.0f;
				Potworek[i].Animation[7] = TRUE;
			}
			Potworek[i].Anim = 7;

			Potworek[i].Animation[6] = FALSE;
			Potworek[i].Animation[8] = FALSE;
			Potworek[i].Animation[9] = FALSE;
		}

		else if (Potworek[i].traf > 9 && Potworek[i].Ins[8].TimePos < Potworek[i].Ins[8].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[8].ClipName))
		{
			Potworek[i].traf++;
			if (!Potworek[i].Animation[8])
			{
				Potworek[i].Ins[8].TimePos = 0.0f;
				Potworek[i].Animation[8] = TRUE;
			}
			Potworek[i].Anim = 8;

			Potworek[i].Animation[6] = FALSE;
			Potworek[i].Animation[9] = FALSE;
		}
	}

	else
	{
		Potworek[i].Speed[1] = 0.04f;
		Potworek[i].Anim = 0;

		if (po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Pozycja[0].x1 != Potworek[i].Pozycja[1].x1) && (Potworek[i].Pozycja[0].z1 != Potworek[i].Pozycja[1].z1))
		{
			if ((Potworek[i].Kat[2] > 0.0f) && (Potworek[i].Kat[2] <= 1.0f) && (Potworek[i].Kat[3] <= 1.0f))
			{
				Potworek[i].Kat[0] -= Potworek[i].Speed[1];
				Potworek[i].Kat[3] += Potworek[i].Speed[1];
			}


			if (((Potworek[i].Pozycja[0].x1 < Potworek[i].Pozycja[1].x1 - 0.01f) && (Potworek[i].Pozycja[0].z1 < Potworek[i].Pozycja[1].z1 - 0.01f)) ||
				((Potworek[i].Pozycja[0].x1 < Potworek[i].Pozycja[1].x1 - 0.01f) && (Potworek[i].Pozycja[0].z1 > Potworek[i].Pozycja[1].z1 + 0.01f)) ||
				((Potworek[i].Pozycja[0].x1 > Potworek[i].Pozycja[1].x1 + 0.01f) && (Potworek[i].Pozycja[0].z1 < Potworek[i].Pozycja[1].z1 - 0.01f)) ||
				((Potworek[i].Pozycja[0].x1 > Potworek[i].Pozycja[1].x1 + 0.01f) && (Potworek[i].Pozycja[0].z1 > Potworek[i].Pozycja[1].z1 + 0.01f)))
			{
				IloscKolizji[0] = FALSE;
				IloscKolizji[1] = FALSE;

				KolPotwZZywPow(i, Potworek);

				if (!IloscKolizji[1] && po % 2 == 1 && !wygrana && Anim[0] != 14)
				{
					for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
					{
						if (ShowArrowOnMonster[i][ilo2])
						{
							AngleXTrajShot[i][ilo2] = atan2(CTrajShot[i][ilo2].z1 - Potworek[i].Pozycja[1].z1, CTrajShot[i][ilo2].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;

							if (Potworek[i].Pozycja[1].z1 >= CTrajShot[i][ilo2].z1)
								CTrajShot[i][ilo2].z1 += Potworek[i].Speed[1];
							else if (CTrajShot[i][ilo2].z1 >= Potworek[i].Pozycja[1].z1)
								CTrajShot[i][ilo2].z1 -= Potworek[i].Speed[1];

							if (Potworek[i].Pozycja[1].x1 >= CTrajShot[i][ilo2].x1)
								CTrajShot[i][ilo2].x1 += Potworek[i].Speed[1];
							else if (CTrajShot[i][ilo2].x1 >= Potworek[i].Pozycja[1].x1)
								CTrajShot[i][ilo2].x1 -= Potworek[i].Speed[1];
						}
					}

					Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - Potworek[i].Pozycja[1].z1, Potworek[i].Pozycja[0].x1 - Potworek[i].Pozycja[1].x1) / XM_PI;

					if (Potworek[i].Pozycja[1].z1 >= Potworek[i].Pozycja[0].z1)
						Potworek[i].Pozycja[0].z1 += Potworek[i].Speed[1];
					else if (Potworek[i].Pozycja[0].z1 >= Potworek[i].Pozycja[1].z1)
						Potworek[i].Pozycja[0].z1 -= Potworek[i].Speed[1];

					if (Potworek[i].Pozycja[1].x1 >= Potworek[i].Pozycja[0].x1)
						Potworek[i].Pozycja[0].x1 += Potworek[i].Speed[1];
					else if (Potworek[i].Pozycja[0].x1 >= Potworek[i].Pozycja[1].x1)
						Potworek[i].Pozycja[0].x1 -= Potworek[i].Speed[1];

					XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
					XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
					XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
				}
			}

			else if ((po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Pozycja[1].x1 >= Potworek[i].Pozycja[1].x1 - 0.01f) && (Potworek[i].Pozycja[1].z1 >= Potworek[i].Pozycja[1].z1 - 0.01f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Pozycja[1].x1 >= Potworek[i].Pozycja[1].x1 - 0.01f) && (Potworek[i].Pozycja[1].z1 <= Potworek[i].Pozycja[1].z1 + 0.01f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Pozycja[1].x1 <= Potworek[i].Pozycja[1].x1 + 0.01f) && (Potworek[i].Pozycja[1].z1 >= Potworek[i].Pozycja[1].z1 - 0.01f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Pozycja[1].x1 <= Potworek[i].Pozycja[1].x1 + 0.01f) && (Potworek[i].Pozycja[1].z1 <= Potworek[i].Pozycja[1].z1 + 0.01f)))
			{
				IloscKolizji[0] = FALSE;
				Potworek[i].Kat[0] = Potworek[i].Kat[1];
				Potworek[i].Pozycja[0].x1 = Potworek[i].Pozycja[1].x1;
				Potworek[i].Pozycja[0].z1 = Potworek[i].Pozycja[1].z1;
			}
		}

		else if (((tab[i - 25] == 1) && (tab[i - 26] != 1)) ||
			((tab[i - 25] == 1) && (tab[i + 26] != 1)) ||
			((tab[i - 25] == 1) && (tab[i + 26] != 1) && (tab[i - 26] != 1)) ||

			((tab[i + 27] == 1) && (tab[i - 26] != 1)) ||
			((tab[i + 27] == 1) && (tab[i + 26] != 1)) ||
			((tab[i + 27] == 1) && (tab[i + 26] != 1) && (tab[i - 26] != 1)) ||

			((tab[i + 1] == 1) && (tab[i - 26] != 1)) ||
			((tab[i + 1] == 1) && (tab[i + 26] != 1)) ||
			((tab[i + 1] == 1) && (tab[i + 26] != 1) && (tab[i - 26] != 1)) ||

			((tab[i + 1] == 1) && (tab[i - 25] == 1)) ||
			((tab[i + 1] == 1) && (tab[i + 27] == 1)) ||
			((tab[i + 27] == 1) && (tab[i - 25] == 1)) ||
			((tab[i + 1] == 1) && (tab[i + 27] == 1) && (tab[i - 25] == 1)) ||

			((tab[i - 27] == 1) && (tab[i - 26] != 1)) ||
			((tab[i - 27] == 1) && (tab[i + 26] != 1)) ||
			((tab[i - 27] == 1) && (tab[i + 26] != 1) && (tab[i - 26] != 1)) ||

			((tab[i + 25] == 1) && (tab[i - 26] != 1)) ||
			((tab[i + 25] == 1) && (tab[i + 26] != 1)) ||
			((tab[i + 25] == 1) && (tab[i + 26] != 1) && (tab[i - 26] != 1)) ||

			((tab[i - 1] == 1) && (tab[i - 26] != 1)) ||
			((tab[i - 1] == 1) && (tab[i + 26] != 1)) ||
			((tab[i - 1] == 1) && (tab[i + 26] != 1) && (tab[i - 26] != 1)) ||

			((tab[i - 1] == 1) && (tab[i - 27] == 1)) ||
			((tab[i - 1] == 1) && (tab[i + 25] == 1)) ||
			((tab[i + 25] == 1) && (tab[i - 27] == 1)) ||
			((tab[i - 1] == 1) && (tab[i + 25] == 1) && (tab[i - 27] == 1)) ||

			((tab[i - 25] != 1) && (tab[i - 26] != 1)) ||
			((tab[i - 25] != 1) && (tab[i + 26] != 1)) ||
			((tab[i - 25] != 1) && (tab[i + 26] != 1) && (tab[i - 26] != 1)))
		{
			if (po % 2 == 1 && !wygrana && Anim[0] != 14 && ((Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[i + 26] == 1) && (Potworek[i].Pozycja[0].z1 <= boxx[i + 26].z1 + 12.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[i + 52] == 1) && (Potworek[i].Pozycja[0].z1 <= boxx[i + 52].z1 + 12.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[i - 26] == 1) && (Potworek[i].Pozycja[0].z1 >= boxx[i - 26].z1 - 12.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[i - 52] == 1) && (Potworek[i].Pozycja[0].z1 >= boxx[i - 52].z1 - 12.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (Potworek[i].Pozycja[0].z1 <= boxx[i].z1 - 48.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (Potworek[i].Pozycja[0].z1 >= boxx[i].z1 + 48.0f)))
			{
				IloscKolizji[0] = FALSE;
				IloscKolizji[1] = FALSE;

				Potworek[i].Kat[0] += Potworek[i].Speed[1];
				for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
				{
					if (ShowArrowOnMonster[i][ilo2])
						AngleXTrajShot[i][ilo2] += 0.04f;
				}

				if (Potworek[i].Kat[0] > 2.5f)
					Potworek[i].Kat[0] = 0.5f;
			}

			for (INT ilo3 = 0; ilo3 < IloElLab; ilo3++)
			{
				if (po % 2 == 1 && !wygrana && Anim[0] != 14 && ((Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[ilo3] == 8) && (i != ilo3) && (Potworek[i].Pozycja[0].z1 <= Potworek[ilo3].Pozycja[0].z1 + 2.0f) && (Potworek[i].Pozycja[0].z1 >= Potworek[ilo3].Pozycja[0].z1 + 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[ilo3] == 9) && (i != ilo3) && (Potworek[i].Pozycja[0].z1 <= Potworek[ilo3].Pozycja[0].z1 + 2.0f) && (Potworek[i].Pozycja[0].z1 >= Potworek[ilo3].Pozycja[0].z1 + 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[ilo3] == 10) && (i != ilo3) && (Potworek[i].Pozycja[0].z1 <= Potworek[ilo3].Pozycja[0].z1 + 2.0f) && (Potworek[i].Pozycja[0].z1 >= Potworek[ilo3].Pozycja[0].z1 + 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[ilo3] == 11) && (i != ilo3) && (Potworek[i].Pozycja[0].z1 <= Potworek[ilo3].Pozycja[0].z1 + 2.0f) && (Potworek[i].Pozycja[0].z1 >= Potworek[ilo3].Pozycja[0].z1 + 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[ilo3] == 8) && (i != ilo3) && (Potworek[i].Pozycja[0].z1 >= Potworek[ilo3].Pozycja[0].z1 - 2.0f) && (Potworek[i].Pozycja[0].z1 <= Potworek[ilo3].Pozycja[0].z1 - 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[ilo3] == 9) && (i != ilo3) && (Potworek[i].Pozycja[0].z1 >= Potworek[ilo3].Pozycja[0].z1 - 2.0f) && (Potworek[i].Pozycja[0].z1 <= Potworek[ilo3].Pozycja[0].z1 - 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[ilo3] == 10) && (i != ilo3) && (Potworek[i].Pozycja[0].z1 >= Potworek[ilo3].Pozycja[0].z1 - 2.0f) && (Potworek[i].Pozycja[0].z1 <= Potworek[ilo3].Pozycja[0].z1 - 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[ilo3] == 11) && (i != ilo3) && (Potworek[i].Pozycja[0].z1 >= Potworek[ilo3].Pozycja[0].z1 - 2.0f) && (Potworek[i].Pozycja[0].z1 <= Potworek[ilo3].Pozycja[0].z1 - 1.0f)))
				{
					IloscKolizji[0] = FALSE;
					IloscKolizji[1] = FALSE;

					Potworek[i].Kat[0] += Potworek[i].Speed[1];
					for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
					{
						if (ShowArrowOnMonster[i][ilo2])
							AngleXTrajShot[i][ilo2] += 0.04f;
					}

					if (Potworek[i].Kat[0] > 2.5f)
						Potworek[i].Kat[0] = 0.5f;
				}
			}

			if (po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (Potworek[i].Pozycja[0].z1 > boxx[i].z1 - 48.0f))
			{
				IloscKolizji[0] = FALSE;
				IloscKolizji[1] = FALSE;

				for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
				{
					if (ShowArrowOnMonster[i][ilo2])
						CTrajShot[i][ilo2].z1 -= Potworek[i].Speed[1];
				}

				Potworek[i].Pozycja[0].z1 -= Potworek[i].Speed[1];
				Potworek[i].Pozycja[1].x1 = Potworek[i].Pozycja[0].x1;
				Potworek[i].Pozycja[1].z1 = Potworek[i].Pozycja[0].z1;
			}

			if (po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (Potworek[i].Pozycja[0].z1 < boxx[i].z1 + 48.0f))
			{
				IloscKolizji[0] = FALSE;
				IloscKolizji[1] = FALSE;

				for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
				{
					if (ShowArrowOnMonster[i][ilo2])
						CTrajShot[i][ilo2].z1 += Potworek[i].Speed[1];
				}
				Potworek[i].Pozycja[0].z1 += Potworek[i].Speed[1];
				Potworek[i].Pozycja[1].x1 = Potworek[i].Pozycja[0].x1;
				Potworek[i].Pozycja[1].z1 = Potworek[i].Pozycja[0].z1;
			}

			Potworek[i].Kat[1] = Potworek[i].Kat[0];
		}

		else if (((tab[i + 25] == 1) && (tab[i - 1] != 1)) ||
			((tab[i + 25] == 1) && (tab[i + 1] != 1)) ||
			((tab[i + 25] == 1) && (tab[i - 1] != 1) && (tab[i + 1] != 1)) ||

			((tab[i + 27] == 1) && (tab[i - 1] != 1)) ||
			((tab[i + 27] == 1) && (tab[i + 1] != 1)) ||
			((tab[i + 27] == 1) && (tab[i - 1] != 1) && (tab[i + 1] != 1)) ||

			((tab[i + 26] == 1) && (tab[i - 1] != 1)) ||
			((tab[i + 26] == 1) && (tab[i + 1] != 1)) ||
			((tab[i + 26] == 1) && (tab[i + 1] != 1) && (tab[i - 1] != 1)) ||

			((tab[i + 26] == 1) && (tab[i + 27] == 1)) ||
			((tab[i + 25] == 1) && (tab[i + 26] == 1)) ||
			((tab[i + 25] == 1) && (tab[i + 27] == 1)) ||
			((tab[i + 25] == 1) && (tab[i + 26] == 1) && (tab[i + 27] == 1)) ||

			((tab[i - 25] == 1) && (tab[i - 1] != 1)) ||
			((tab[i - 25] == 1) && (tab[i + 1] != 1)) ||
			((tab[i - 25] == 1) && (tab[i + 1] != 1) && (tab[i - 1] != 1)) ||

			((tab[i - 27] == 1) && (tab[i - 1] != 1)) ||
			((tab[i - 27] == 1) && (tab[i + 1] != 1)) ||
			((tab[i - 27] == 1) && (tab[i + 1] != 1) && (tab[i - 1] != 1)) ||

			((tab[i - 26] == 1) && (tab[i - 1] != 1)) ||
			((tab[i - 26] == 1) && (tab[i + 1] != 1)) ||
			((tab[i - 26] == 1) && (tab[i + 1] != 1) && (tab[i - 1] != 1)) ||

			((tab[i - 26] == 1) && (tab[i - 27] == 1)) ||
			((tab[i - 25] == 1) && (tab[i - 26] == 1)) ||
			((tab[i - 25] == 1) && (tab[i - 27] == 1)) ||
			((tab[i - 25] == 1) && (tab[i - 26] == 1) && (tab[i - 27] == 1)))
		{
			if (Potworek[i].Kat[0] < 3.0f)
				Potworek[i].Kat[0] += 2.0f;

			if (po % 2 == 1 && !wygrana && Anim[0] != 14 && ((Potworek[i].Kat[0] >= 3.0f) && (Potworek[i].Kat[0] < 4.0f) && (tab[i + 1] == 1) && (Potworek[i].Pozycja[0].x1 >= boxx[i + 1].x1 - 12.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 3.0f) && (Potworek[i].Kat[0] < 4.0f) && (tab[i + 2] == 1) && (Potworek[i].Pozycja[0].x1 >= boxx[i + 2].x1 - 12.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 4.0f) && (Potworek[i].Kat[0] <= 5.0f) && (tab[i - 1] == 1) && (Potworek[i].Pozycja[0].x1 <= boxx[i - 1].x1 + 12.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 4.0f) && (Potworek[i].Kat[0] <= 5.0f) && (tab[i - 2] == 1) && (Potworek[i].Pozycja[0].x1 <= boxx[i - 2].x1 + 12.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 3.0f) && (Potworek[i].Kat[0] < 4.0f) && (Potworek[i].Pozycja[0].x1 >= boxx[i].x1 + 48.0f)) ||
				(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 4.0f) && (Potworek[i].Kat[0] <= 5.0f) && (Potworek[i].Pozycja[0].x1 <= boxx[i].x1 - 48.0f)))
			{
				IloscKolizji[0] = FALSE;
				IloscKolizji[1] = FALSE;

				Potworek[i].Kat[0] += Potworek[i].Speed[1];

				for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
				{
					if (ShowArrowOnMonster[i][ilo2])
						AngleXTrajShot[i][ilo2] += Potworek[i].Speed[1];
				}

				if (Potworek[i].Kat[0] > 5.0f)
					Potworek[i].Kat[0] = 3.0f;
			}

			for (INT ilo3 = 0; ilo3 < IloElLab; ilo3++)
			{
				if (po % 2 == 1 && !wygrana && Anim[0] != 14 && ((Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[ilo3] == 8) && (i != ilo3) && (Potworek[i].Pozycja[0].x1 <= Potworek[ilo3].Pozycja[0].x1 + 2.0f) && (Potworek[i].Pozycja[0].x1 >= Potworek[ilo3].Pozycja[0].x1 + 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[ilo3] == 9) && (i != ilo3) && (Potworek[i].Pozycja[0].x1 <= Potworek[ilo3].Pozycja[0].x1 + 2.0f) && (Potworek[i].Pozycja[0].x1 >= Potworek[ilo3].Pozycja[0].x1 + 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[ilo3] == 10) && (i != ilo3) && (Potworek[i].Pozycja[0].x1 <= Potworek[ilo3].Pozycja[0].x1 + 2.0f) && (Potworek[i].Pozycja[0].x1 >= Potworek[ilo3].Pozycja[0].x1 + 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 0.5f) && (Potworek[i].Kat[0] < 1.5f) && (tab[ilo3] == 11) && (i != ilo3) && (Potworek[i].Pozycja[0].x1 <= Potworek[ilo3].Pozycja[0].x1 + 2.0f) && (Potworek[i].Pozycja[0].x1 >= Potworek[ilo3].Pozycja[0].x1 + 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[ilo3] == 8) && (i != ilo3) && (Potworek[i].Pozycja[0].x1 >= Potworek[ilo3].Pozycja[0].x1 - 2.0f) && (Potworek[i].Pozycja[0].x1 <= Potworek[ilo3].Pozycja[0].x1 - 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[ilo3] == 9) && (i != ilo3) && (Potworek[i].Pozycja[0].x1 >= Potworek[ilo3].Pozycja[0].x1 - 2.0f) && (Potworek[i].Pozycja[0].x1 <= Potworek[ilo3].Pozycja[0].x1 - 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[ilo3] == 10) && (i != ilo3) && (Potworek[i].Pozycja[0].x1 >= Potworek[ilo3].Pozycja[0].x1 - 2.0f) && (Potworek[i].Pozycja[0].x1 <= Potworek[ilo3].Pozycja[0].x1 - 1.0f)) ||
					(po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 1.5f) && (Potworek[i].Kat[0] <= 2.5f) && (tab[ilo3] == 11) && (i != ilo3) && (Potworek[i].Pozycja[0].x1 >= Potworek[ilo3].Pozycja[0].x1 - 2.0f) && (Potworek[i].Pozycja[0].x1 <= Potworek[ilo3].Pozycja[0].x1 - 1.0f)))
				{
					IloscKolizji[0] = FALSE;
					IloscKolizji[1] = FALSE;

					Potworek[i].Kat[0] += Potworek[i].Speed[1];

					for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
					{
						if (ShowArrowOnMonster[i][ilo2])
							AngleXTrajShot[i][ilo2] += Potworek[i].Speed[1];
					}

					if (Potworek[i].Kat[0] > 5.0f)
						Potworek[i].Kat[0] = 3.0f;
				}
			}

			if (po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 3.0f) && (Potworek[i].Kat[0] < 4.0f) && (Potworek[i].Pozycja[0].x1 < boxx[i].x1 + 48.0f))
			{
				IloscKolizji[0] = FALSE;
				IloscKolizji[1] = FALSE;

				for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
				{
					if (ShowArrowOnMonster[i][ilo2])
						CTrajShot[i][ilo2].x1 += Potworek[i].Speed[1];
				}
				Potworek[i].Pozycja[0].x1 += Potworek[i].Speed[1];
				Potworek[i].Pozycja[1].x1 = Potworek[i].Pozycja[0].x1;
				Potworek[i].Pozycja[1].z1 = Potworek[i].Pozycja[0].z1;
			}

			if (po % 2 == 1 && !wygrana && Anim[0] != 14 && (Potworek[i].Kat[0] >= 4.0f) && (Potworek[i].Kat[0] <= 5.0f) && (Potworek[i].Pozycja[0].x1 > boxx[i].x1 - 48.0f))
			{
				IloscKolizji[0] = FALSE;
				IloscKolizji[1] = FALSE;

				for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
				{
					if (ShowArrowOnMonster[i][ilo2])
						CTrajShot[i][ilo2].x1 -= Potworek[i].Speed[1];
				}
				Potworek[i].Pozycja[0].x1 -= Potworek[i].Speed[1];
				Potworek[i].Pozycja[1].x1 = Potworek[i].Pozycja[0].x1;
				Potworek[i].Pozycja[1].z1 = Potworek[i].Pozycja[0].z1;
			}

			Potworek[i].Kat[1] = Potworek[i].Kat[0];
		}
	}

	KolPostZPotw(i, Potworek);
}

void Mechanic::Walka(INT i, SkinningModels *Potworek, INT Chod, INT Run, INT JumpRndKick, INT BackLegKick, INT Box1, INT Box2, INT KickPunch, INT Oberwanie, INT Upadek, INT Oberwanie2)
{
	srand(time(NULL));
	int losewanie_animacji = rand() % 5 + 1;

	if ((!Potworek[i].Animation[Chod] && !Potworek[i].Animation[Run] && !Potworek[i].Animation[JumpRndKick] && Postac.Animation[0] && Potworek[i].Anim != Oberwanie) ||
		(!Potworek[i].Animation[Chod] && !Potworek[i].Animation[Run] && !Potworek[i].Animation[JumpRndKick] && Postac.Animation[1] && Potworek[i].Anim != Oberwanie) ||
		(!Potworek[i].Animation[Chod] && !Potworek[i].Animation[Run] && !Potworek[i].Animation[JumpRndKick] && Postac.Animation[2] && Potworek[i].Anim != Oberwanie) ||
		(!Potworek[i].Animation[Chod] && !Potworek[i].Animation[Run] && !Potworek[i].Animation[JumpRndKick] && Postac.Animation[3] && Potworek[i].Anim != Oberwanie) ||
		(!Potworek[i].Animation[Chod] && !Potworek[i].Animation[Run] && !Potworek[i].Animation[JumpRndKick] && Postac.Animation[4] && Potworek[i].Anim != Oberwanie) ||
		(!Potworek[i].Animation[Chod] && !Potworek[i].Animation[Run] && !Potworek[i].Animation[JumpRndKick] && Postac.Animation[5] && Potworek[i].Anim != Oberwanie))
	{
		if (!Potworek[i].Animation[Chod])
		{
			Potworek[i].Ins[Chod].TimePos = 0.0f;
			Potworek[i].Animation[Chod] = TRUE;
		}
		Potworek[i].Anim = Chod;

		Postac.Animation[0] = FALSE; Postac.Animation[16] = FALSE; Postac.Animation[32] = FALSE; Postac.Animation[47] = FALSE; Postac.Animation[63] = FALSE; Postac.Animation[82] = FALSE; Postac.Animation[98] = FALSE; Potworek[i].trafienie = FALSE;

		Potworek[i].Animation[3] = FALSE;
		Potworek[i].Animation[5] = FALSE;
		Potworek[i].Animation[6] = FALSE;
		Potworek[i].Animation[8] = FALSE;
		Potworek[i].Animation[9] = FALSE;
	}

	else if ((Potworek[i].Anim == JumpRndKick) && (Potworek[i].Ins[JumpRndKick].TimePos >= Potworek[i].Ins[JumpRndKick].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[JumpRndKick].ClipName)) ||
		(Potworek[i].Anim == BackLegKick) && (Potworek[i].Ins[BackLegKick].TimePos >= Potworek[i].Ins[BackLegKick].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[BackLegKick].ClipName)) ||
		(Potworek[i].Anim == Box1) && (Potworek[i].Ins[Box1].TimePos >= Potworek[i].Ins[Box1].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[Box1].ClipName)) ||
		(Potworek[i].Anim == Box2) && (Potworek[i].Ins[Box2].TimePos >= Potworek[i].Ins[Box2].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[Box2].ClipName)) ||
		(Potworek[i].Anim == KickPunch) && (Potworek[i].Ins[KickPunch].TimePos >= Potworek[i].Ins[KickPunch].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[KickPunch].ClipName)) ||
		(Potworek[i].Anim == Oberwanie) && (Potworek[i].Ins[Oberwanie].TimePos >= Potworek[i].Ins[Oberwanie].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[Oberwanie].ClipName)) || (Potworek[i].Anim == Oberwanie2) && (Potworek[i].Ins[Oberwanie2].TimePos >= Potworek[i].Ins[Oberwanie2].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[Oberwanie2].ClipName)))
	{
		if (!Potworek[i].Animation[Chod])
		{
			Potworek[i].Ins[Chod].TimePos = 0.0f;
			Potworek[i].Animation[Chod] = TRUE;
		}
		Potworek[i].Anim = Chod;

		for (INT il = 0; il < 10; il++)
			Potworek[i].Animation[il] = FALSE;
		Potworek[i].trafienie = FALSE;
	}

	else if (Postac.Animation[15] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch])
	{
		if (Potworek[i].traf <= 9)
		{
			if (!Potworek[i].Animation[Oberwanie2])
			{
				Potworek[i].traf++;
				Potworek[i].Ins[Oberwanie2].TimePos = 0.0f;
				Potworek[i].Animation[Oberwanie2] = TRUE;
			}
			Potworek[i].Anim = Oberwanie2;

			Potworek[i].Animation[6] = FALSE;
			Potworek[i].Animation[8] = FALSE;
			Potworek[i].trafienie = FALSE;
		}

		else if (Potworek[i].traf > 9 && Potworek[i].Ins[Upadek].TimePos < Potworek[i].Ins[Upadek].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[Upadek].ClipName))
		{
			if (!Potworek[i].Animation[Upadek])
			{
				Potworek[i].traf++;
				Potworek[i].Ins[Upadek].TimePos = 0.0f;
				Potworek[i].Animation[Upadek] = TRUE;
			}
			Potworek[i].Anim = Upadek;

			Potworek[i].Animation[6] = FALSE;
			Potworek[i].trafienie = FALSE;
		}
	}

	else if ((Postac.Animation[6] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[7] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[8] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[9] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[10] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[11] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[12] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[79] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[80] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[81] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[113] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]) ||
		(Postac.Animation[114] && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch]))
	{
		if (Potworek[i].traf <= 9)
		{
			if (!Potworek[i].Animation[Oberwanie])
			{
				Potworek[i].traf++;
				Potworek[i].Ins[Oberwanie].TimePos = 0.0f;
				Potworek[i].Animation[Oberwanie] = TRUE;
			}
			Potworek[i].Anim = Oberwanie;

			Potworek[i].Animation[6] = FALSE;
			Potworek[i].Animation[8] = FALSE;
			Potworek[i].Animation[9] = FALSE;
			Potworek[i].trafienie = FALSE;
		}
		else if (Potworek[i].traf > 9 && Potworek[i].Ins[Upadek].TimePos < Potworek[i].Ins[Upadek].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[Upadek].ClipName))
		{
			if (!Potworek[i].Animation[Upadek])
			{
				Potworek[i].traf++;
				Potworek[i].Ins[Upadek].TimePos = 0.0f;
				Potworek[i].Animation[Upadek] = TRUE;
			}
			Potworek[i].Anim = Upadek;

			Potworek[i].Animation[6] = FALSE;
			Potworek[i].Animation[9] = FALSE;
			Potworek[i].trafienie = FALSE;
		}
	}

	else if (Potworek[i].trafienie && !Potworek[i].Animation[JumpRndKick] && !Potworek[i].Animation[BackLegKick] && !Potworek[i].Animation[Box1] && !Potworek[i].Animation[Box2] && !Potworek[i].Animation[KickPunch])
	{
		if (Potworek[i].traf <= 9)
		{
			if (!Potworek[i].Animation[Oberwanie])
			{
				Potworek[i].traf++;
				Potworek[i].Ins[Oberwanie].TimePos = 0.0f;
				Potworek[i].Animation[Oberwanie] = TRUE;
			}
			Potworek[i].Anim = Oberwanie;

			Potworek[i].Animation[6] = FALSE;
			Potworek[i].Animation[8] = FALSE;
			Potworek[i].Animation[9] = FALSE;
		}
		else if (Potworek[i].traf > 9 && Potworek[i].Ins[Upadek].TimePos < Potworek[i].Ins[Upadek].Model->SkinnedData.GetClipEndTime(Potworek[i].Ins[Upadek].ClipName))
		{
			if (!Potworek[i].Animation[Upadek])
			{
				Potworek[i].traf++;
				Potworek[i].Ins[Upadek].TimePos = 0.0f;
				Potworek[i].Animation[Upadek] = TRUE;
			}
			Potworek[i].Anim = Upadek;

			Potworek[i].Animation[6] = FALSE;
			Potworek[i].Animation[9] = FALSE;
		}
	}

	else
	{
		if (losewanie_animacji == 1)
		{
			if (Potworek[i].Anim == Chod && Potworek[i].Ins[Chod].TimePos >= 0.8f)
			{
				Postac.Animation[0] = FALSE; Postac.Animation[16] = FALSE; Postac.Animation[32] = FALSE; Postac.Animation[47] = FALSE; Postac.Animation[63] = FALSE; Postac.Animation[82] = FALSE; Postac.Animation[98] = FALSE;
				Postac.Animation[13] = FALSE; Postac.Animation[29] = FALSE; Postac.Animation[44] = FALSE; Postac.Animation[60] = FALSE; Postac.Animation[76] = FALSE; Postac.Animation[95] = FALSE; Postac.Animation[110] = FALSE;

				if (!Potworek[i].Animation[JumpRndKick])
				{
					Potworek[i].Ins[JumpRndKick].TimePos = 0.0f;
					Potworek[i].Ins[Chod].TimePos = 0.0f;
					Potworek[i].Animation[JumpRndKick] = TRUE;
				}
				Potworek[i].Anim = JumpRndKick;

				Potworek[i].Animation[1] = FALSE;
				Potworek[i].Animation[3] = FALSE;
				for (INT il = 5; il < 10; il++)
					Potworek[i].Animation[il] = FALSE;
				Potworek[i].trafienie = FALSE;
			}
		}

		else if (losewanie_animacji == 2)
		{
			if (Potworek[i].Anim == Chod && Potworek[i].Ins[Chod].TimePos >= 0.8f)
			{
				Postac.Animation[0] = FALSE; Postac.Animation[16] = FALSE; Postac.Animation[32] = FALSE; Postac.Animation[47] = FALSE; Postac.Animation[63] = FALSE; Postac.Animation[82] = FALSE; Postac.Animation[98] = FALSE;
				Postac.Animation[13] = FALSE; Postac.Animation[29] = FALSE; Postac.Animation[44] = FALSE; Postac.Animation[60] = FALSE; Postac.Animation[76] = FALSE; Postac.Animation[95] = FALSE; Postac.Animation[110] = FALSE;

				if (!Potworek[i].Animation[BackLegKick])
				{
					Potworek[i].Ins[BackLegKick].TimePos = 0.0f;
					Potworek[i].Ins[Chod].TimePos = 0.0f;
					Potworek[i].Animation[BackLegKick] = TRUE;
				}

				Potworek[i].Anim = BackLegKick;

				Potworek[i].Animation[1] = FALSE;
				for (INT il = 7; il < 10; il++)
					Potworek[i].Animation[il] = FALSE;
				Potworek[i].trafienie = FALSE;
			}
		}

		else if (losewanie_animacji == 3)
		{
			if (Potworek[i].Anim == Chod && Potworek[i].Ins[Chod].TimePos >= 0.8f)
			{
				Postac.Animation[0] = FALSE; Postac.Animation[16] = FALSE; Postac.Animation[32] = FALSE; Postac.Animation[47] = FALSE; Postac.Animation[63] = FALSE; Postac.Animation[82] = FALSE; Postac.Animation[98] = FALSE;
				Postac.Animation[13] = FALSE; Postac.Animation[29] = FALSE; Postac.Animation[44] = FALSE; Postac.Animation[60] = FALSE; Postac.Animation[76] = FALSE; Postac.Animation[95] = FALSE; Postac.Animation[110] = FALSE;

				if (!Potworek[i].Animation[Box1])
				{
					Potworek[i].Ins[Box1].TimePos = 0.0f;
					Potworek[i].Ins[Chod].TimePos = 0.0f;
					Potworek[i].Animation[Box1] = TRUE;
				}

				Potworek[i].Anim = Box1;

				Potworek[i].Animation[1] = FALSE;
				Potworek[i].Animation[3] = FALSE;
				for (INT il = 5; il < 10; il++)
					Potworek[i].Animation[il] = FALSE;
				Potworek[i].trafienie = FALSE;
			}
		}

		else if (losewanie_animacji == 4)
		{
			if (Potworek[i].Anim == Chod && Potworek[i].Ins[Chod].TimePos >= 0.8f)
			{
				Postac.Animation[0] = FALSE; Postac.Animation[16] = FALSE; Postac.Animation[32] = FALSE; Postac.Animation[47] = FALSE; Postac.Animation[63] = FALSE; Postac.Animation[82] = FALSE; Postac.Animation[98] = FALSE;
				Postac.Animation[13] = FALSE; Postac.Animation[29] = FALSE; Postac.Animation[44] = FALSE; Postac.Animation[60] = FALSE; Postac.Animation[76] = FALSE; Postac.Animation[95] = FALSE; Postac.Animation[110] = FALSE;

				if (!Potworek[i].Animation[Box2])
				{
					Potworek[i].Ins[Box2].TimePos = 0.0f;
					Potworek[i].Ins[Chod].TimePos = 0.0f;
					Potworek[i].Animation[Box2] = TRUE;
				}

				Potworek[i].Anim = Box2;

				Potworek[i].Animation[1] = FALSE;
				Potworek[i].Animation[3] = FALSE;
				for (INT il = 6; il < 10; il++)
					Potworek[i].Animation[il] = FALSE;
				Potworek[i].trafienie = FALSE;
			}
		}

		else if (losewanie_animacji == 5)
		{
			if (Potworek[i].Anim == Chod && Potworek[i].Ins[Chod].TimePos >= 0.8f)
			{
				Postac.Animation[0] = FALSE; Postac.Animation[16] = FALSE; Postac.Animation[32] = FALSE; Postac.Animation[47] = FALSE; Postac.Animation[63] = FALSE; Postac.Animation[82] = FALSE; Postac.Animation[98] = FALSE;
				Postac.Animation[13] = FALSE; Postac.Animation[29] = FALSE; Postac.Animation[44] = FALSE; Postac.Animation[60] = FALSE; Postac.Animation[76] = FALSE; Postac.Animation[95] = FALSE; Postac.Animation[110] = FALSE;

				if (!Potworek[i].Animation[KickPunch])
				{
					Potworek[i].Ins[KickPunch].TimePos = 0.0f;
					Potworek[i].Ins[Chod].TimePos = 0.0f;
					Potworek[i].Animation[KickPunch] = TRUE;
				}

				Potworek[i].Anim = KickPunch;

				Potworek[i].Animation[1] = FALSE;
				Potworek[i].Animation[3] = FALSE;
				for (INT il = 7; il < 10; il++)
					Potworek[i].Animation[il] = FALSE;
				Potworek[i].trafienie = FALSE;
			}
		}
	}

	Potworek[i].Speed[1] = 0.04f;
	if (odleglosc[i] >= 4.0f && po % 2 == 1 && !wygrana && Anim[0] != 14)
	{
		Potworek[i].Kat[0] = atan2(Potworek[i].Pozycja[0].z1 - XMVectorGetZ(TPCam.charPosition), Potworek[i].Pozycja[0].x1 - XMVectorGetX(TPCam.charPosition)) / XM_PI;

		if (XMVectorGetZ(TPCam.charPosition) > Potworek[i].Pozycja[0].z1)
			Potworek[i].Pozycja[0].z1 += Potworek[i].Speed[1];
		else if (Potworek[i].Pozycja[0].z1 > XMVectorGetZ(TPCam.charPosition))
			Potworek[i].Pozycja[0].z1 -= Potworek[i].Speed[1];

		if (XMVectorGetX(TPCam.charPosition) > Potworek[i].Pozycja[0].x1)
			Potworek[i].Pozycja[0].x1 += Potworek[i].Speed[1];
		else if (Potworek[i].Pozycja[0].x1 > XMVectorGetX(TPCam.charPosition))
			Potworek[i].Pozycja[0].x1 -= Potworek[i].Speed[1];

		XMMATRIX Translate = XMMatrixTranslation(Potworek[i].Pozycja[0].x1, Potworek[i].Pozycja[0].y1, Potworek[i].Pozycja[0].z1);
		XMMATRIX Rot = XMMatrixRotationX(Potworek[i].Kat[0]);
		XMStoreFloat4x4(&Potworek[i].Ins[Potworek[i].Anim].World, Rot * Translate);
	}
}

void Mechanic::klawa(INT klawiszek)
{
	if (po % 2 == 1 && ZapiszGiere % 2 == 1 && !wygrana && Anim[0] != 14)
	{
		reset_czasu = 0;

		if (klaw[klawiszek].ustawiony_klaw == 1 || klaw[klawiszek].ustawiony_klaw == 16)
			klawisze[0] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 2 || klaw[klawiszek].ustawiony_klaw == 17)
			klawisze[1] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 3 || klaw[klawiszek].ustawiony_klaw == 18)
			klawisze[2] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 4 || klaw[klawiszek].ustawiony_klaw == 19)
			klawisze[3] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 5 || klaw[klawiszek].ustawiony_klaw == 20)
			klawisze[8] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 6 || klaw[klawiszek].ustawiony_klaw == 21)
			klawisze[9] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 7 || klaw[klawiszek].ustawiony_klaw == 22)
			klawisze[10] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 8 || klaw[klawiszek].ustawiony_klaw == 23)
			klawisze[11] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 9 || klaw[klawiszek].ustawiony_klaw == 24)
			klawisze[4] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 10 || klaw[klawiszek].ustawiony_klaw == 25)
		{
			box++;
			if (bron == 3 || bron == 2 || bron == 0)
				klawisze[6] = TRUE;

			else if (bron == 1 && strzaly > 0)
			{
				klawisze[6] = TRUE;
				strzaly--;
			}

			else if (bron == 1 && strzaly <= 0)
				klawisze[6] = FALSE;

		}

		if (klaw[klawiszek].ustawiony_klaw == 11 || klaw[klawiszek].ustawiony_klaw == 26)
			klawisze[7] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 12 || klaw[klawiszek].ustawiony_klaw == 27)
			klawisze[5] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 13 || klaw[klawiszek].ustawiony_klaw == 28)
			klawisze[12] = TRUE;

		if (klaw[klawiszek].ustawiony_klaw == 14 || klaw[klawiszek].ustawiony_klaw == 29)
		{
			if (bron == 0 && !weapons[0] && !weapons[1] && !weapons[2]) bron = 0;
			else if (bron == 0 && weapons[0] && !weapons[1] && !weapons[2]) bron = 1;
			else if (bron == 0 && !weapons[0] && weapons[1] && !weapons[2]) bron = 2;
			else if (bron == 0 && !weapons[0] && !weapons[1] && weapons[2]) bron = 3;
			else if (bron == 0 && weapons[0] && weapons[1] && !weapons[2]) bron = 2;
			else if (bron == 0 && !weapons[0] && weapons[1] && weapons[2]) bron = 3;
			else if (bron == 0 && weapons[0] && !weapons[1] && weapons[2]) bron = 3;
			else if (bron == 0 && weapons[0] && weapons[1] && weapons[2]) bron = 3;
			else if ((bron == 1 && weapons[0] && !weapons[1] && !weapons[2]) ||
				(bron == 1 && weapons[0] && weapons[1] && !weapons[2]) ||
				(bron == 1 && weapons[0] && !weapons[1] && weapons[2]) ||
				(bron == 1 && weapons[0] && weapons[1] && weapons[2])) bron = 0;
			else if (bron == 2 && !weapons[0] && weapons[1] && !weapons[2]) bron = 0;
			else if (bron == 2 && weapons[0] && weapons[1] && !weapons[2]) bron = 1;
			else if (bron == 2 && !weapons[0] && weapons[1] && weapons[2]) bron = 0;
			else if (bron == 2 && weapons[0] && weapons[1] && weapons[2]) bron = 1;
			else if (bron == 3 && !weapons[0] && !weapons[1] && weapons[2]) bron = 0;
			else if (bron == 3 && weapons[0] && !weapons[1] && weapons[2]) bron = 1;
			else if (bron == 3 && !weapons[0] && weapons[1] && weapons[2]) bron = 2;
			else if (bron == 3 && weapons[0] && weapons[1] && weapons[2]) bron = 2;
		}

		if (klaw[klawiszek].ustawiony_klaw == 15 || klaw[klawiszek].ustawiony_klaw == 30)
		{
			if (bron == 0 && !weapons[0] && !weapons[1] && !weapons[2]) bron = 0;
			else if (bron == 0 && weapons[0] && !weapons[1] && !weapons[2]) bron = 1;
			else if (bron == 0 && !weapons[0] && weapons[1] && !weapons[2]) bron = 2;
			else if (bron == 0 && !weapons[0] && !weapons[1] && weapons[2]) bron = 3;
			else if (bron == 0 && weapons[0] && weapons[1] && !weapons[2]) bron = 1;
			else if (bron == 0 && !weapons[0] && weapons[1] && weapons[2]) bron = 2;
			else if (bron == 0 && weapons[0] && !weapons[1] && weapons[2]) bron = 1;
			else if (bron == 0 && weapons[0] && weapons[1] && weapons[2]) bron = 1;
			else if (bron == 1 && weapons[0] && !weapons[1] && !weapons[2]) bron = 0;
			else if (bron == 1 && weapons[0] && weapons[1] && !weapons[2]) bron = 2;
			else if (bron == 1 && weapons[0] && !weapons[1] && weapons[2]) bron = 3;
			else if (bron == 1 && weapons[0] && weapons[1] && weapons[2])  bron = 2;
			else if (bron == 2 && !weapons[0] && weapons[1] && !weapons[2]) bron = 0;
			else if (bron == 2 && weapons[0] && weapons[1] && !weapons[2]) bron = 0;
			else if (bron == 2 && !weapons[0] && weapons[1] && weapons[2]) bron = 3;
			else if (bron == 2 && weapons[0] && weapons[1] && weapons[2]) bron = 3;
			else if ((bron == 3 && !weapons[0] && !weapons[1] && weapons[2]) ||
				(bron == 3 && weapons[0] && !weapons[1] && weapons[2]) ||
				(bron == 3 && !weapons[0] && weapons[1] && weapons[2]) ||
				(bron == 3 && weapons[0] && weapons[1] && weapons[2])) bron = 0;
		}
	}
}

void Mechanic::klawa_up(INT klawiszek)
{
	if (scen[0] == 11)
	{
		reset_czasu = 0;

		if (klaw[klawiszek].ustawiony_klaw == 1 || klaw[klawiszek].ustawiony_klaw == 16)
			klawisze[0] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 2 || klaw[klawiszek].ustawiony_klaw == 17)
			klawisze[1] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 3 || klaw[klawiszek].ustawiony_klaw == 18)
			klawisze[2] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 4 || klaw[klawiszek].ustawiony_klaw == 19)
			klawisze[3] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 5 || klaw[klawiszek].ustawiony_klaw == 20)
			klawisze[8] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 6 || klaw[klawiszek].ustawiony_klaw == 21)
			klawisze[9] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 7 || klaw[klawiszek].ustawiony_klaw == 22)
			klawisze[10] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 8 || klaw[klawiszek].ustawiony_klaw == 23)
			klawisze[11] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 9 || klaw[klawiszek].ustawiony_klaw == 24)
			klawisze[4] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 10 || klaw[klawiszek].ustawiony_klaw == 25)
			klawisze[6] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 11 || klaw[klawiszek].ustawiony_klaw == 26)
			klawisze[7] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 12 || klaw[klawiszek].ustawiony_klaw == 27)
			klawisze[5] = FALSE;

		if (klaw[klawiszek].ustawiony_klaw == 13 || klaw[klawiszek].ustawiony_klaw == 28)
			klawisze[12] = FALSE;
	}
}

void Mechanic::ustawianie_klawiszy(INT klawiszek)
{
	if ((scen[2] == 14 && klik[0] == 1) || (scen[0] == 22))
	{
		for (INT k = 0; k < 31; k++)
		{
			if (ustaw_klaw[k] == 1 && ustawione == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				klaw[klawiszek].klawisz = 1;
				klaw[klawiszek].ustawiony_klaw = k + 1;
				ustawione = 1;

				UstWszKl();
			}
		}
	}

	if ((scen[0] == 20) || (po % 2 == 0 && ZapiszGiere % 2 == 0))
	{
		if ((klawiszek >= 16 && klawiszek <= 28) ||
			(klawiszek >= 31 && klawiszek <= 43) ||
			(klawiszek >= 45 && klawiszek <= 55) ||
			(klawiszek >= 58 && klawiszek <= 67) ||
			klawiszek == 71)
		{
			klaw[klawiszek].zapis[dl] = dl + 1;

			if ((klawiszek >= 18 && klawiszek <= 28) ||
				(klawiszek >= 33 && klawiszek <= 35) ||
				(klawiszek >= 39 && klawiszek <= 43) ||
				(klawiszek >= 45 && klawiszek <= 53) ||
				(klawiszek >= 58 && klawiszek <= 63) ||
				(klawiszek == 31) ||
				(klawiszek == 37) ||
				(klawiszek == 67))
			{
				zapxx[dl] = 1;
				string ws(klaw[klawiszek].nazwa_klawisza);
				string str(ws.begin(), ws.end());
				NazwaZapLab += str;
			}

			else if (klawiszek == 38 ||
				klawiszek == 54 ||
				klawiszek == 55 ||
				klawiszek == 65 ||
				klawiszek == 16 ||
				klawiszek == 17 ||
				klawiszek == 66)
			{
				zapxx[dl] = 2;
				string ws(klaw[klawiszek].nazwa_klawisza);
				string str(ws.begin(), ws.end());
				NazwaZapLab += str;
			}

			else if (klawiszek == 32 ||
				klawiszek == 36 ||
				klawiszek == 64)
			{
				zapxx[dl] = 3;
				string ws(klaw[klawiszek].nazwa_klawisza);
				string str(ws.begin(), ws.end());
				NazwaZapLab += str;
			}

			else if (klawiszek == 71)
			{
				zapxx[dl] = 4;
				NazwaZapLab += " ";
			}

			if (dl<19)
				dl++;
		}

		else if (klawiszek == 29)
		{
			if (dl > 0)
				dl--;

			for (INT kla = 0; kla < KLA - 25; kla++)
				klaw[kla].zapis[dl] = 0;

			NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - 1);
		}
	}
}

void Mechanic::KlawiszologiaKamery(FLOAT dt)
{
	//kamera gora
	if (klawisze[8] && !klawisze[9] && !klawisze[10] && !klawisze[11])
	{
		TPCam.SpeedCamera = 8.0f;
		TPCam.LookUp();
	}

	//kamera gora lewo
	else if (klawisze[8] && !klawisze[9] && klawisze[10] && !klawisze[11])
	{
		TPCam.SpeedCamera = 8.0f;
		TPCam.LookUp();
		TPCam.LookLeft();
	}

	//kamera gora lewo
	else if (klawisze[8] && !klawisze[9] && !klawisze[10] && klawisze[11])
	{
		TPCam.SpeedCamera = 8.0f;
		TPCam.LookUp();
		TPCam.LookRight();
	}

	//kamera dol
	else if (!klawisze[8] && klawisze[9] && !klawisze[10] && !klawisze[11])
	{
		TPCam.SpeedCamera = 8.0f;
		TPCam.LookDown();
	}

	//kamera dol lewo
	else if (!klawisze[8] && klawisze[9] && klawisze[10] && !klawisze[11])
	{
		TPCam.SpeedCamera = 8.0f;
		TPCam.LookDown();
		TPCam.LookLeft();
	}

	//kamera dol prawo
	else if (!klawisze[8] && klawisze[9] && !klawisze[10] && klawisze[11])
	{
		TPCam.SpeedCamera = 8.0f;
		TPCam.LookDown();
		TPCam.LookRight();
	}

	//kamera lewo
	else if (!klawisze[8] && !klawisze[9] && klawisze[10] && !klawisze[11])
	{
		TPCam.SpeedCamera = 8.0f;
		TPCam.LookLeft();
	}

	//kamera prawo
	else if (!klawisze[8] && !klawisze[9] && !klawisze[10] && klawisze[11])
	{
		TPCam.SpeedCamera = 8.0f;
		TPCam.LookRight();
	}
}

void Mechanic::KlawiszologiaPostaci(FLOAT dt, ThirdPersonCamera &TPCam)
{
	// przód || skok w przód || bieg w przód
	if ((klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		TPCam.WalkForward(dt);
		walks[0] = TRUE;
		walks[1] = FALSE;
		walks[2] = FALSE;
		walks[3] = FALSE;
	}

	// tyl || bieg w tyl || skok w tyl
	else if ((!klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		TPCam.WalkBack(dt);
		walks[0] = FALSE;
		walks[1] = TRUE;
		walks[2] = FALSE;
		walks[3] = FALSE;
	}

	// lewo || skok w lewo || bieg w lewo
	else if ((!klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		TPCam.WalkLeft(dt);
		walks[0] = FALSE;
		walks[1] = FALSE;
		walks[2] = TRUE;
		walks[3] = FALSE;
	}

	// prawo || skok w prawo || bieg w prawo
	else if ((!klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		TPCam.WalkRight(dt);
		walks[0] = FALSE;
		walks[1] = FALSE;
		walks[2] = FALSE;
		walks[3] = TRUE;
	}

	// przód i lewo || bieg w przód i w lewo || skok w przód i lewo
	else if ((klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		TPCam.WalkForward(dt);
		TPCam.WalkLeft(dt);
		walks[0] = TRUE;
		walks[1] = FALSE;
		walks[2] = TRUE;
		walks[3] = FALSE;
	}

	// przód i prawo || skokw w przód i w prawo || bieg w przód i w prawo
	else if ((klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		TPCam.WalkForward(dt);
		TPCam.WalkRight(dt);
		walks[0] = TRUE;
		walks[1] = FALSE;
		walks[2] = FALSE;
		walks[3] = TRUE;
	}

	// tyl i lewo || skok w ty³ i w lewo || bieg w tyl i w lewo
	else if ((!klawisze[0] && klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		TPCam.WalkBack(dt);
		TPCam.WalkLeft(dt);
		walks[0] = FALSE;
		walks[1] = TRUE;
		walks[2] = TRUE;
		walks[3] = FALSE;
	}

	//ty³ i prawo || skok w ty³ i w prawo || bieg w ty³ i w prawo
	else if ((!klawisze[0] && klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		TPCam.WalkBack(dt);
		TPCam.WalkRight(dt);
		walks[0] = FALSE;
		walks[1] = TRUE;
		walks[2] = FALSE;
		walks[3] = TRUE;
	}
}

void Mechanic::czas()
{
	if (state == 1)
	{
		state = 0;
		if (reset_czasu == 1)
		{
			czekanie = state = 0;
			start = clock();
		}

		else
			czekanie += (INT)(clock() - stop);
	}

	if (po % 2 == 1 && !wygrana && Anim[0] != 14)
	{
		sekundy = (INT)(clock() - start - czekanie);
		sekundy /= 1000;
	}

	else if (reset_czasu == 0)
	{
		stop = clock();
		state = 1;
	}

	else
	{
		czekanie = state = 0;
		start = clock();
	}
}