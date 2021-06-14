#include "Renders.h"

Renders::Renders(HINSTANCE hInstance) : Objects(hInstance)
{
	//INT
	IloscPlikow[0][0] = 0; IloscPlikow[0][1] = 1; IloscPlikow[1][0] = 0; IloscPlikow[1][1] = 1;
	nowy[0] = 0; nowy[1] = 0; frame[16] = IloLiter; frame[17] = 0; frame[18] = 0;

	for (INT ilo = 0; ilo < 7; ilo++)
		SingleThink[ilo] = 0;

	for (INT fr = 0; fr < 1400; fr++)
		frejmik[fr] = FALSE;

	for (INT ilo = 0; ilo < 12; ilo++)
		frame[ilo] = 0;

	for (INT ilo = 12; ilo < 16; ilo++)
		frame[ilo] = 2;

	for (INT j = 0; j < 2; j++)
		pliki[j] = new string[1000];

	for (INT ilo = 0; ilo < 20; ilo++)
	{
		for (INT ilo2 = 0; ilo2 < 18; ilo2++)
		{
			OdtwDzwMen[ilo][ilo2] = 0;
			Odtw[ilo][ilo2] = 0;
		}
	}
}

Renders::~Renders()
{
}

void Renders::odzn()
{
	t = w, p = 1;
	while (t <= w + IloPunkt - 3 && p <= IloPunkt - 2)
	{
		if (nowy[1] > 0)
		{
			if (frejmik[p + 11] == TRUE)
			{
				if (tab[t] == 1)
				{
					tab[t] = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
				}

				else if (tab[t] == 2)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					SingleThink[0] = 0;
					tab[t] = 0;
				}

				else if (tab[t] == 3)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					SingleThink[1] = 0;
					tab[t] = 0;
				}

				else if (tab[t] == 4)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					SingleThink[2] = 0;
					tab[t] = 0;
				}

				else if (tab[t] == 6)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 0;
				}

				else if (tab[t] == 5)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					SingleThink[3] = 0;
					tab[t] = 0;
				}

				else if (tab[t] == 7)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					SingleThink[4] = 0;
					tab[t] = 0;
				}

				else if (tab[t] == 8)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 0;
				}

				else if (tab[t] == 9)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					SingleThink[5] = 0;
					tab[t] = 0;
				}

				else if (tab[t] == 10)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 0;
				}

				else if (tab[t] == 11)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					SingleThink[6] = 0;
					tab[t] = 0;
				}

				else if (tab[t] == 12)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 0;
				}
			}
		}
		p++;
		t++;
	}
}

void Renders::zaznaczenie(FLOAT xzazn, FLOAT yzazn, FLOAT rozx, FLOAT rozy)
{
	p = 1;
	while (p <= IloPunkt - 2)
	{
		if (tab[t] == 0)
		{
			if (nowy[1] >= 1 && nowy[1] < 13)
			{
				if ((xzazn >= rozx / 1000 * pozycja_sciany[0][p].x1) && (xzazn <= rozx / 1000 * pozycja_sciany[0][p].x2) &&
					(yzazn >= rozy / 500 * pozycja_sciany[0][p].y1) && (yzazn <= rozy / 500 * pozycja_sciany[0][p].y2))
				{
					for (INT fr = 0; fr < 1400; fr++)
						frejmik[fr] = FALSE;

					ZaznGuz = p + 19;
					frejmik[p + 11] = TRUE;
				}
			}
		}
		p++;
	}
}

void Renders::zazn()
{
	t = w, p = 1;
	while (t <= w + IloPunkt - 3 && p <= IloPunkt - 2)
	{
		if (tab[t] == 0)
		{
			if (nowy[1] == 1)
			{
				if (frejmik[p + 11])
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 1;
				}
			}

			else if (nowy[1] == 2)
			{
				if (SingleThink[2] == 0)
				{
					if (frejmik[p + 11])
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						tab[t] = 4;
						SingleThink[2] = 1;
					}
				}
			}

			else if (nowy[1] == 3)
			{
				if (SingleThink[3] == 0)
				{
					if (frejmik[p + 11])
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						tab[t] = 5;
						SingleThink[3] = 1;
					}
				}
			}

			else if (nowy[1] == 4)
			{
				if (frejmik[p + 11])
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 6;
				}
			}

			else if (nowy[1] == 5)
			{
				if (SingleThink[4] == 0)
				{
					if (frejmik[p + 11])
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						tab[t] = 7;
						SingleThink[4] = 1;
					}
				}
			}

			else if (nowy[1] == 6)
			{
				if (frejmik[p + 11])
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 8;
				}
			}

			else if (nowy[1] == 7)
			{
				if (SingleThink[5] == 0)
				{
					if (frejmik[p + 11])
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						tab[t] = 9;
						SingleThink[5] = 1;
					}
				}
			}

			else if (nowy[1] == 8)
			{
				if (frejmik[p + 11])
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 10;
				}
			}

			else if (nowy[1] == 9)
			{
				if (SingleThink[6] == 0)
				{
					if (frejmik[p + 11])
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						tab[t] = 11;
						SingleThink[6] = 1;
					}
				}
			}

			else if (nowy[1] == 10)
			{
				if (frejmik[p + 11])
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					tab[t] = 12;
				}
			}

			else if (nowy[1] == 11)
			{
				if (SingleThink[0] == 0)
				{
					if (frejmik[p + 11])
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						tab[t] = 2;
						SingleThink[0] = 1;
					}
				}
			}

			else if (nowy[1] == 12)
			{
				if (SingleThink[1] == 0)
				{
					if (frejmik[p + 11])
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						tab[t] = 3;
						SingleThink[1] = 1;
					}
				}
			}
		}
		p++;
		t++;
	}
}

void Renders::odznaczenie(FLOAT xodzn, FLOAT yodzn, FLOAT rozx, FLOAT rozy)
{
	p = 1;
	while (p <= IloPunkt - 2)
	{
		if (nowy[1] > 0)
		{
			if ((xodzn >= rozx / 1000 * pozycja_sciany[0][p].x1) && (xodzn <= rozx / 1000 * pozycja_sciany[0][p].x2) &&
				(yodzn >= rozy / 500 * pozycja_sciany[0][p].y1) && (yodzn <= rozy / 500 * pozycja_sciany[0][p].y2))
			{
				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = FALSE;

				frejmik[p + 11] = TRUE;
			}
		}
		p++;
	}
}

void Renders::ZaznKeys()
{
	p = 1;
	while (p <= IloPunkt - 2)
	{
		if (tab[t] == 0)
		{
			for (INT i = 1; i < 4; i++)
			{
				if (nowy[1] == i)
				{
					if (ZaznGuz == p + 19)
					{
						for (INT fr = 0; fr < 1400; fr++)
							frejmik[fr] = FALSE;

						frejmik[p + 11] = TRUE;
					}
				}
			}

			if (nowy[1] == 4)
			{
				if (SingleThink[3] == 0)
				{
					if (ZaznGuz == p + 19)
					{
						for (INT fr = 0; fr < 1400; fr++)
							frejmik[fr] = FALSE;

						frejmik[p + 11] = TRUE;
					}
				}
			}

			else if (nowy[1] == 5)
			{
				if (SingleThink[4] == 0)
				{
					if (ZaznGuz == p + 19)
					{
						for (INT fr = 0; fr < 1400; fr++)
							frejmik[fr] = FALSE;

						frejmik[p + 11] = TRUE;
					}
				}
			}

			else if (nowy[1] == 6)
			{
				if (ZaznGuz == p + 19)
				{
					for (INT fr = 0; fr < 1400; fr++)
						frejmik[fr] = FALSE;

					frejmik[p + 11] = TRUE;
				}
			}

			else if (nowy[1] == 7)
			{
				if (SingleThink[5] == 0)
				{
					if (ZaznGuz == p + 19)
					{
						for (INT fr = 0; fr < 1400; fr++)
							frejmik[fr] = FALSE;

						frejmik[p + 11] = TRUE;
					}
				}
			}

			else if (nowy[1] == 8)
			{
				if (ZaznGuz == p + 19)
				{
					for (INT fr = 0; fr < 1400; fr++)
						frejmik[fr] = FALSE;

					frejmik[p + 11] = TRUE;
				}
			}

			else if (nowy[1] == 9)
			{
				if (SingleThink[6] == 0)
				{
					if (ZaznGuz == p + 19)
					{
						for (INT fr = 0; fr < 1400; fr++)
							frejmik[fr] = FALSE;

						frejmik[p + 11] = TRUE;
					}
				}
			}

			else if (nowy[1] == 10)
			{
				if (ZaznGuz == p + 19)
				{
					for (INT fr = 0; fr < 1400; fr++)
						frejmik[fr] = FALSE;

					frejmik[p + 11] = TRUE;
				}
			}

			for (INT i = 11, ii = 0; i < 13, ii < 2; i++, ii++)
			{
				if (nowy[1] == i)
				{
					if (SingleThink[ii] == 0)
					{
						if (ZaznGuz == p + 19)
						{
							for (INT fr = 0; fr < 1400; fr++)
								frejmik[fr] = FALSE;

							frejmik[p + 11] = TRUE;
						}
					}
				}
			}
		}
		p++;
	}
}

void Renders::wysw()
{
	t = w; p = 1;
	while ((p <= IloPunkt - 2) && (t <= w + IloPunkt - 3))
	{
		for (INT no = 1; no <= 12; no++)
			if (nowy[1] == no)
				poz_scian[p] = tab[t];

		p++;
		t++;
	}
}

void Renders::fram()
{
	frame[16] = IloLiter;

	for (INT ilo = 0; ilo < 16; ilo++)
		frame[ilo] = 0;

	for (INT ilo = 0; ilo < 20; ilo++)
	{
		for (INT ilo2 = 0; ilo2 < 18; ilo2++)
		{
			OdtwDzwMen[ilo][ilo2] = 0;
			Odtw[ilo][ilo2] = 0;
		}
	}

	for (INT ilo = 17; ilo < 19; ilo++)
		frame[ilo] = 0;
}

void Renders::ListaRozdz()
{
	DEVMODE dm;
	INT index = 0;
	index2 = 0;

	while (0 != EnumDisplaySettings(NULL, index++, &dm))
	{
		if ((dm.dmBitsPerPel == 32 && dm.dmDisplayFixedOutput == 1 && dm.dmDisplayFrequency == 60 && dm.dmPelsWidth >= 800 && dm.dmPelsHeight >= 600 && dm.dmPelsWidth <= 1680 && dm.dmPelsHeight <= 1050) ||
			(dm.dmBitsPerPel == 32 && dm.dmDisplayFixedOutput == 0 && dm.dmDisplayFrequency == 60 && dm.dmPelsWidth == 1920 && dm.dmPelsHeight == 1080))
		{
			rozdz[index2].width = (INT)dm.dmPelsWidth;
			rozdz[index2].height = (INT)dm.dmPelsHeight;
			index2++;
		}
	}
}

void Renders::ListaLabiryntow()
{
	DIR* katalog[2];

	struct dirent* pozycja[2] = { 0, 0 };
	string plik[2];

	if (WczytLabiryntu % 2 == 1)
	{
		katalog[0] = opendir("./ZapisaneLabirynty");

		while (pozycja[0] = readdir(katalog[0]))
		{
			plik[0] = (*pozycja[0]).d_name;
			if (plik[0].find(".") == (plik[0].length() - 4))
			{
				pliki[0][IloscPlikow[0][0]] = plik[0].erase(plik[0].length() - 4);
				IloscPlikow[0][0]++;
			}
		}
		(void)closedir(katalog[0]);
	}

	else if (WczytLabiryntu % 2 == 0)
	{
		katalog[1] = opendir("./ZapisanaGra");

		while (pozycja[1] = readdir(katalog[1]))
		{
			plik[1] = (*pozycja[1]).d_name;
			if (plik[1].find(".") == (plik[1].length() - 4))
			{
				pliki[1][IloscPlikow[1][0]] = plik[1].erase(plik[1].length() - 4);
				IloscPlikow[1][0]++;
			}
		}
		(void)closedir(katalog[1]);
	}
}