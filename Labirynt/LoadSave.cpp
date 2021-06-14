#include "LoadSave.h"

LoadSave::LoadSave(HINSTANCE hInstance) : Meshes(hInstance)
{
	//INT
	scen[0] = 0; scen[1] = 0; scen[2] = 0;

	//FLOAT
	MenuStworzLabX[0] = -1.52f; MenuStworzLabX[1] = -1.58f; MenuStworzLabX[2] = -1.49f; MenuStworzLabX[3] = -1.55f; MenuStworzLabX[4] = -0.86f; MenuStworzLabX[5] = -0.7f;
	Tlx[0] = 1.1f; Tlx[1] = 1.1f; Tlx[2] = 1.1f;

	OpcjeNapX[0] = -1.6f; OpcjeNapX[1] = 1.3f; OpcjeNapX[2] = -1.55f; OpcjeNapX[3] = -1.5f; OpcjeNapX[4] = -1.4f; 
	Grafika1Pos[0] = 0.85f; Grafika1Pos[1] = 1.1f; Grafika1Pos[2] = 1.2f; Grafika1Pos[3] = 1.05f;
	ObjektyScen12X[0] = 0.93f; ObjektyScen12X[1] = 0.65f;
	OpcjeDzwiekuX[0] = -1.53f; OpcjeDzwiekuX[1] = -1.58f;

	rozdzielczosc_w = 1280.0f;
	rozdzielczosc_h = 600.0f;

	roz_w = rozdzielczosc_w;
	roz_h = rozdzielczosc_h;

	PozGuz[0].x1 = -0.857f; PozGuz[0].y1 = 0.762f; PozGuz[1].x1 = -0.722f; PozGuz[1].y1 = 0.762f; PozGuz[2].x1 = -0.857f; PozGuz[2].y1 = 0.627f; PozGuz[3].x1 = -0.722f; PozGuz[3].y1 = 0.627f; PozGuz[4].x1 = -0.857f; PozGuz[4].y1 = 0.492f; PozGuz[5].x1 = -0.722f; PozGuz[5].y1 = 0.492f; PozGuz[6].x1 = -0.857f; PozGuz[6].y1 = 0.357f; PozGuz[7].x1 = -0.722f; PozGuz[7].y1 = 0.357f; PozGuz[8].x1 = -0.857f; PozGuz[8].y1 = 0.222f; PozGuz[9].x1 = -0.722f; PozGuz[9].y1 = 0.222f; PozGuz[10].x1 = -0.857f; PozGuz[10].y1 = 1.0f; PozGuz[11].x1 = -0.722f; PozGuz[11].y1 = 1.0f;
	PozGuz[15].x1 = -0.722f; PozGuz[15].y1 = 1.0f;

	vGraNapisy.x1 = -1.72f; vGraNapisy.x2 = 1.6f;

	//BOOL
	weapons[0] = FALSE; weapons[1] = FALSE; weapons[2] = FALSE;
	zapis[0] = TRUE; zapis[1] = TRUE; zapis[2] = TRUE; zapis[3] = FALSE;

	for (INT i = 0; i < 4; i++)
		wczyt[i] = FALSE; 

	for (INT i = 0; i < 31; i++)
		pierwszy[i] = 0;
}

LoadSave::~LoadSave()
{
}

void LoadSave::pozycja_startowa()
{
	for (INT j = 0; j < 4; j++)
		for (INT il = 0; il < IloElLab; il++)
			Potwor[j][il].traf = 0;

	for (INT i = 0; i < IloElLab; i++)
	{
		if (tab[i] == 2)
		{
			TPCam.charPosition = XMVectorSetX(TPCam.charPosition, boxx[i].x1);
			TPCam.charPosition = XMVectorSetY(TPCam.charPosition, 0.1f);
			TPCam.charPosition = XMVectorSetZ(TPCam.charPosition, boxx[i].z1);
			XMMATRIX Translate = XMMatrixTranslation(boxx[i].x1, 0.1f, boxx[i].z1);
			TPCam.playerCharWorld = Translate;
		}
	}
}

void LoadSave::PozARozdz()
{
	if (rozdzielczosc_w >= 800.0f && rozdzielczosc_w < 1024.0f && rozdzielczosc_h >= 600.0f && rozdzielczosc_h < 768.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -0.94f;
		nap[3][0].x2 = -0.94f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.003f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.04f;
		vGraNapisy.x2 = 0.89f;
		x_dl_zycia = -0.91f;
		xBroniki = 0.97f;

		MenuStworzLabX[0] = -1.0f;
		MenuStworzLabX[1] = -1.05f;
		MenuStworzLabX[2] = -0.94f;
		MenuStworzLabX[3] = -1.0f;
		MenuStworzLabX[4] = -0.54f;
		MenuStworzLabX[5] = -0.41f;

		ObjektyScen12X[0] = -0.336f;
		ObjektyScen12X[1] = -0.619f;

		PozGuz[0].x1 = -0.546f;
		PozGuz[1].x1 = -0.41f;
		PozGuz[2].x1 = -0.546f;
		PozGuz[3].x1 = -0.41f;
		PozGuz[4].x1 = -0.546f;
		PozGuz[5].x1 = -0.41f;
		PozGuz[6].x1 = -0.546f;
		PozGuz[7].x1 = -0.41f;
		PozGuz[8].x1 = -0.546f;
		PozGuz[9].x1 = -0.41f;

		IloPunkt = 782;
		ZaznaczGuziczek = 774;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.798f; ty > -1.117f; ty -= 0.056f)
				for (FLOAT tx = -0.303f; tx < 1.098f; tx += 0.056f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.056f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.056f)
				for (FLOAT px = -0.303f; px < 1.098f; px += 0.056f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 16.67f;
			while (yy1 < 500.1 && yy2 < 516.77)
			{
				FLOAT xx1 = 350, xx2 = 375;
				while (xx1 < 1000 && xx2 < 1025)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 25;
					xx2 += 25;
				}
				yy1 += 16.67f;
				yy2 += 16.67f;
			}
		}

		OpcjeNapX[0] = -1.0f;
		OpcjeNapX[1] = 0.73f;
		OpcjeNapX[2] = -1.03f;
		OpcjeNapX[3] = -0.98f;
		OpcjeNapX[4] = -0.87f;

		Grafika1Pos[0] = 0.48f;
		Grafika1Pos[1] = 0.72f;
		Grafika1Pos[2] = 0.84f;
		Grafika1Pos[3] = 0.68f;

		PosSuwakX = -0.47f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -0.99f;
		OpcjeDzwiekuX[1] = -1.03f;
	}

	else if (rozdzielczosc_w >= 1024.0f && rozdzielczosc_w < 1152.0f && rozdzielczosc_h >= 768.0f && rozdzielczosc_h < 864.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -0.95f;
		nap[3][0].x2 = -0.95f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.003f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.04f;
		vGraNapisy.x2 = 0.89f;
		x_dl_zycia = -0.91f;
		xBroniki = 0.97f;

		MenuStworzLabX[0] = -1.0f;
		MenuStworzLabX[1] = -1.05f;
		MenuStworzLabX[2] = -0.94f;
		MenuStworzLabX[3] = -1.0f;
		MenuStworzLabX[4] = -0.538f;
		MenuStworzLabX[5] = -0.408f;

		ObjektyScen12X[0] = -0.337f;
		ObjektyScen12X[1] = -0.619f;

		PozGuz[0].x1 = -0.547f;
		PozGuz[1].x1 = -0.411f;
		PozGuz[2].x1 = -0.547f;
		PozGuz[3].x1 = -0.411f;
		PozGuz[4].x1 = -0.547f;
		PozGuz[5].x1 = -0.411f;
		PozGuz[6].x1 = -0.547f;
		PozGuz[7].x1 = -0.411f;
		PozGuz[8].x1 = -0.547f;
		PozGuz[9].x1 = -0.411f;

		IloPunkt = 782;
		ZaznaczGuziczek = 774;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.8f; ty > -1.117f; ty -= 0.055f)
				for (FLOAT tx = -0.303f; tx < 1.073f; tx += 0.055f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.055f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.055f)
				for (FLOAT px = -0.303f; px < 1.073f; px += 0.055f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 16.67f;
			while (yy1 < 500.1 && yy2 < 516.77)
			{
				FLOAT xx1 = 351.56f, xx2 = 376.5f;
				while (xx1 < 1000 && xx2 < 1024.94)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 24.94f;
					xx2 += 24.94f;
				}
				yy1 += 16.67f;
				yy2 += 16.67f;
			}
		}

		OpcjeNapX[0] = -1.0f;
		OpcjeNapX[1] = 0.73f;
		OpcjeNapX[2] = -1.02f;
		OpcjeNapX[3] = -0.96f;
		OpcjeNapX[4] = -0.86f;

		Grafika1Pos[0] = 0.46f;
		Grafika1Pos[1] = 0.72f;
		Grafika1Pos[2] = 0.82f;
		Grafika1Pos[3] = 0.67f;

		PosSuwakX = -0.47f;
		ScaleSuwak = -0.01f;

		OpcjeDzwiekuX[0] = -0.99f;
		OpcjeDzwiekuX[1] = -1.03f;
	}

	else if (rozdzielczosc_w >= 1152.0f && rozdzielczosc_w < 1280.0f && rozdzielczosc_h >= 864.0f && rozdzielczosc_h < 960.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -0.95f;
		nap[3][0].x2 = -0.95f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.003f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.04f;
		vGraNapisy.x2 = 0.89f;
		x_dl_zycia = -0.91f;
		xBroniki = 0.97f;

		MenuStworzLabX[0] = -1.0f;
		MenuStworzLabX[1] = -1.05f;
		MenuStworzLabX[2] = -0.94f;
		MenuStworzLabX[3] = -1.0f;
		MenuStworzLabX[4] = -0.538f;
		MenuStworzLabX[5] = -0.408f;

		ObjektyScen12X[0] = -0.338f;
		ObjektyScen12X[1] = -0.62f;

		PozGuz[0].x1 = -0.547f;
		PozGuz[1].x1 = -0.411f;
		PozGuz[2].x1 = -0.547f;
		PozGuz[3].x1 = -0.411f;
		PozGuz[4].x1 = -0.547f;
		PozGuz[5].x1 = -0.411f;
		PozGuz[6].x1 = -0.547f;
		PozGuz[7].x1 = -0.411f;
		PozGuz[8].x1 = -0.547f;
		PozGuz[9].x1 = -0.411f;

		IloPunkt = 782;
		ZaznaczGuziczek = 774;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.801f; ty > -1.117f; ty -= 0.056f)
				for (FLOAT tx = -0.303f; tx < 1.098f; tx += 0.056f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.056f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.056f)
				for (FLOAT px = -0.303f; px < 1.098f; px += 0.056f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 16.69f;
			while (yy1 < 500.7f && yy2 < 517.39f)
			{
				FLOAT xx1 = 349.8f, xx2 = 374.82f;
				while (xx1 < 999.8f && xx2 < 1024.8f)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 25.0f;
					xx2 += 25.0f;
				}
				yy1 += 16.69f;
				yy2 += 16.69f;
			}
		}

		OpcjeNapX[0] = -1.0f;
		OpcjeNapX[1] = 0.73f;
		OpcjeNapX[2] = -1.03f;
		OpcjeNapX[3] = -0.96f;
		OpcjeNapX[4] = -0.87f;

		Grafika1Pos[0] = 0.46f;
		Grafika1Pos[1] = 0.72f;
		Grafika1Pos[2] = 0.82f;
		Grafika1Pos[3] = 0.67f;

		PosSuwakX = -0.47f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -0.99f;
		OpcjeDzwiekuX[1] = -1.03f;
	}

	else if (rozdzielczosc_w >= 1280.0f && rozdzielczosc_w < 1360.0f && rozdzielczosc_h >= 600.0f && rozdzielczosc_h < 720)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.01f;
		nap[3][0].x2 = -1.01f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.72f;
		vGraNapisy.x2 = 1.57f;
		x_dl_zycia = -1.59f;
		xBroniki = 1.65f;

		MenuStworzLabX[0] = -1.52f;
		MenuStworzLabX[1] = -1.58f;
		MenuStworzLabX[2] = -1.49f;
		MenuStworzLabX[3] = -1.55f;
		MenuStworzLabX[4] = -0.86f;
		MenuStworzLabX[5] = -0.7f;

		ObjektyScen12X[0] = -0.648f;
		ObjektyScen12X[1] = -0.93f;

		PozGuz[0].x1 = -0.857f;
		PozGuz[1].x1 = -0.722f;
		PozGuz[2].x1 = -0.857f;
		PozGuz[3].x1 = -0.722f;
		PozGuz[4].x1 = -0.857f;
		PozGuz[5].x1 = -0.722f;
		PozGuz[6].x1 = -0.857f;
		PozGuz[7].x1 = -0.722f;
		PozGuz[8].x1 = -0.857f;
		PozGuz[9].x1 = -0.722f;

		IloPunkt = 522;
		ZaznaczGuziczek = 462;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.784f; ty > -1.117f; ty -= 0.095f)
				for (FLOAT tx = -0.6f; tx < 1.776f; tx += 0.095f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.095f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.76f; py > -1.027f; py -= 0.094f)
				for (FLOAT px = -0.6f; px < 1.785f; px += 0.095f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 28.5;
			while (yy1 < 542 && yy2 < 571)
			{
				FLOAT xx1 = 317, xx2 = 343.75;
				while (xx1 < 986 && xx2 < 1013)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 26.75;
					xx2 += 26.75;
				}
				yy1 += 28.5;
				yy2 += 28.5;
			}
		}

		OpcjeNapX[0] = -1.6f;
		OpcjeNapX[1] = 1.3f;
		OpcjeNapX[2] = -1.55f;
		OpcjeNapX[3] = -1.5f;
		OpcjeNapX[4] = -1.4f;

		Grafika1Pos[0] = 0.85f;
		Grafika1Pos[1] = 1.1f;
		Grafika1Pos[2] = 1.2f;
		Grafika1Pos[3] = 1.05f;

		PosSuwakX = 0.0f;
		ScaleSuwak = 0.0f;

		OpcjeDzwiekuX[0] = -1.53f;
		OpcjeDzwiekuX[1] = -1.58f;
	}

	else if (rozdzielczosc_w >= 1280.0f && rozdzielczosc_w < 1360.0f && rozdzielczosc_h >= 720.0f && rozdzielczosc_h < 768.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.41f;
		vGraNapisy.x2 = 1.25f;
		x_dl_zycia = -1.28f;
		xBroniki = 1.34f;

		MenuStworzLabX[0] = -1.26f;
		MenuStworzLabX[1] = -1.32f;
		MenuStworzLabX[2] = -1.21f;
		MenuStworzLabX[3] = -1.26f;
		MenuStworzLabX[4] = -0.712f;
		MenuStworzLabX[5] = -0.578f;

		ObjektyScen12X[0] = -0.514f;
		ObjektyScen12X[1] = -0.793f;

		PozGuz[0].x1 = -0.72f;
		PozGuz[1].x1 = -0.587f;
		PozGuz[2].x1 = -0.72f;
		PozGuz[3].x1 = -0.587f;
		PozGuz[4].x1 = -0.72f;
		PozGuz[5].x1 = -0.587f;
		PozGuz[6].x1 = -0.72f;
		PozGuz[7].x1 = -0.587f;
		PozGuz[8].x1 = -0.72f;
		PozGuz[9].x1 = -0.587f;

		IloPunkt = 574;
		ZaznaczGuziczek = 566;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.788f; ty > -1.117f; ty -= 0.077f)
				for (FLOAT tx = -0.469f; tx < 1.457f; tx += 0.077f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.077f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.77f; py > -1.037f; py -= 0.077f)
				for (FLOAT px = -0.469f; px < 1.457f; px += 0.077)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		FLOAT MouseXp = 419.0f;
		FLOAT HelpH = rozdzielczosc_w / 1005.0f;
		FLOAT HelpG = rozdzielczosc_h / 510.0f;
		FLOAT HelpW = ((rozdzielczosc_w - MouseXp) / HelpH) / 26;
		FLOAT HelpIlosc = (IloPunkt - 2) / 26;
		FLOAT HelpT = rozdzielczosc_h / HelpIlosc / HelpG;
		FLOAT HelpI = HelpT * HelpIlosc;
		FLOAT HelpZ = HelpI + HelpT;

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0.0f, yy2 = HelpT;
			while (yy1 < HelpI && yy2 < HelpZ)
			{
				FLOAT xx1 = MouseXp / HelpH;
				FLOAT xx2 = xx1 + HelpW;
				FLOAT HelpU = xx1 + (25 * HelpW);
				FLOAT HelpJ = HelpU + HelpW;
				while (xx1 < HelpU && xx2 < HelpJ)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += HelpW;
					xx2 += HelpW;
				}
				yy1 += HelpT;
				yy2 += HelpT;
			}
		}

		OpcjeNapX[0] = -1.37f;
		OpcjeNapX[1] = 1.1f;
		OpcjeNapX[2] = -1.2f;
		OpcjeNapX[3] = -1.16f;
		OpcjeNapX[4] = -1.07f;

		Grafika1Pos[0] = 0.66f;
		Grafika1Pos[1] = 0.92f;
		Grafika1Pos[2] = 1.02f;
		Grafika1Pos[3] = 0.86f;

		PosSuwakX = -0.19f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -1.31f;
		OpcjeDzwiekuX[1] = -1.35f;
	}

	else if (rozdzielczosc_w >= 1280.0f && rozdzielczosc_w < 1360.0f && rozdzielczosc_h >= 768.0f && rozdzielczosc_h < 800.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.32f;
		vGraNapisy.x2 = 1.16f;
		x_dl_zycia = -1.19f;
		xBroniki = 1.25f;

		MenuStworzLabX[0] = -1.18f;
		MenuStworzLabX[1] = -1.24f;
		MenuStworzLabX[2] = -1.14f;
		MenuStworzLabX[3] = -1.2f;
		MenuStworzLabX[4] = -0.658f;
		MenuStworzLabX[5] = -0.524f;

		ObjektyScen12X[0] = -0.46f;
		ObjektyScen12X[1] = -0.74f;

		PozGuz[0].x1 = -0.668f;
		PozGuz[1].x1 = -0.533f;
		PozGuz[2].x1 = -0.668f;
		PozGuz[3].x1 = -0.533f;
		PozGuz[4].x1 = -0.668f;
		PozGuz[5].x1 = -0.533f;
		PozGuz[6].x1 = -0.668f;
		PozGuz[7].x1 = -0.533f;
		PozGuz[8].x1 = -0.668f;
		PozGuz[9].x1 = -0.533f;

		IloPunkt = 652;
		ZaznaczGuziczek = 618;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.8f; ty > -1.117f; ty -= 0.071f)
				for (FLOAT tx = -0.421f; tx < 1.355f; tx += 0.071f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.071f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.071f)
				for (FLOAT px = -0.421f; px < 1.355f; px += 0.071f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		FLOAT MouseXp = 434.0f;
		FLOAT HelpH = rozdzielczosc_w / 1000.0f;
		FLOAT HelpG = rozdzielczosc_h / 530.0f;
		FLOAT HelpW = ((rozdzielczosc_w - MouseXp) / HelpH) / 26;
		FLOAT HelpIlosc = (IloPunkt - 2) / 26;
		FLOAT HelpT = rozdzielczosc_h / HelpIlosc / HelpG;
		FLOAT HelpI = HelpT * HelpIlosc;
		FLOAT HelpZ = HelpI + HelpT;

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0.0f, yy2 = HelpT;
			while (yy1 < HelpI && yy2 < HelpZ)
			{
				FLOAT xx1 = MouseXp / HelpH;
				FLOAT xx2 = xx1 + HelpW;
				FLOAT HelpU = xx1 + (25 * HelpW);
				FLOAT HelpJ = HelpU + HelpW;
				while (xx1 < HelpU && xx2 < HelpJ)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += HelpW;
					xx2 += HelpW;
				}
				yy1 += HelpT;
				yy2 += HelpT;
			}
		}

		OpcjeNapX[0] = -1.29f;
		OpcjeNapX[1] = 1.03f;
		OpcjeNapX[2] = -1.16f;
		OpcjeNapX[3] = -1.11f;
		OpcjeNapX[4] = -1.01f;

		Grafika1Pos[0] = 0.63f;
		Grafika1Pos[1] = 0.89f;
		Grafika1Pos[2] = 0.99f;
		Grafika1Pos[3] = 0.83f;

		PosSuwakX = -0.21f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -1.21f;
		OpcjeDzwiekuX[1] = -1.25f;
	}

	else if (rozdzielczosc_w >= 1280.0f && rozdzielczosc_w < 1360.0f && rozdzielczosc_h >= 800.0f && rozdzielczosc_h < 960.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.26f;
		vGraNapisy.x2 = 1.11f;
		x_dl_zycia = -1.13f;
		xBroniki = 1.2f;

		MenuStworzLabX[0] = -1.14f;
		MenuStworzLabX[1] = -1.2f;
		MenuStworzLabX[2] = -1.09f;
		MenuStworzLabX[3] = -1.15f;
		MenuStworzLabX[4] = -0.638f;
		MenuStworzLabX[5] = -0.504f;

		ObjektyScen12X[0] = -0.44f;
		ObjektyScen12X[1] = -0.72f;

		PozGuz[0].x1 = -0.648f;
		PozGuz[1].x1 = -0.512f;
		PozGuz[2].x1 = -0.648f;
		PozGuz[3].x1 = -0.512f;
		PozGuz[4].x1 = -0.648f;
		PozGuz[5].x1 = -0.512f;
		PozGuz[6].x1 = -0.648f;
		PozGuz[7].x1 = -0.512f;
		PozGuz[8].x1 = -0.648f;
		PozGuz[9].x1 = -0.512f;

		IloPunkt = 652;
		ZaznaczGuziczek = 644;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.793f; ty > -1.117f; ty -= 0.068f)
				for (FLOAT tx = -0.402f; tx < 1.366f; tx += 0.068f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.068f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.068f)
				for (FLOAT px = -0.402f; px < 1.366f; px += 0.068f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 20.0f;
			while (yy1 < 500.0f && yy2 < 520.0f)
			{
				FLOAT xx1 = 338.28f, xx2 = 363.73f;
				while (xx1 < 999.98f && xx2 < 1025.43f)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 25.45f;
					xx2 += 25.45f;
				}
				yy1 += 20.0f;
				yy2 += 20.0f;
			}
		}

		OpcjeNapX[0] = -1.23f;
		OpcjeNapX[1] = 0.98f;
		OpcjeNapX[2] = -1.16f;
		OpcjeNapX[3] = -1.11f;
		OpcjeNapX[4] = -1.0f;

		Grafika1Pos[0] = 0.59f;
		Grafika1Pos[1] = 0.85f;
		Grafika1Pos[2] = 0.95f;
		Grafika1Pos[3] = 0.8f;

		PosSuwakX = -0.25f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -1.18f;
		OpcjeDzwiekuX[1] = -1.22f;
	}

	else if (rozdzielczosc_w >= 1280.0f && rozdzielczosc_w < 1360.0f && rozdzielczosc_h >= 960.0f && rozdzielczosc_h < 1024.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -0.92f;
		nap[3][0].x2 = -0.92f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0029f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.03f;
		vGraNapisy.x2 = 0.89f;
		x_dl_zycia = -0.9f;
		xBroniki = 0.97f;

		MenuStworzLabX[0] = -1.0f;
		MenuStworzLabX[1] = -1.06f;
		MenuStworzLabX[2] = -0.96f;
		MenuStworzLabX[3] = -1.01f;
		MenuStworzLabX[4] = -0.55f;
		MenuStworzLabX[5] = -0.414f;

		ObjektyScen12X[0] = -0.347f;
		ObjektyScen12X[1] = -0.628f;

		PozGuz[0].x1 = -0.556f;
		PozGuz[1].x1 = -0.42f;
		PozGuz[2].x1 = -0.556f;
		PozGuz[3].x1 = -0.42f;
		PozGuz[4].x1 = -0.556f;
		PozGuz[5].x1 = -0.42f;
		PozGuz[6].x1 = -0.556f;
		PozGuz[7].x1 = -0.42f;
		PozGuz[8].x1 = -0.556f;
		PozGuz[9].x1 = -0.42f;

		IloPunkt = 782;
		ZaznaczGuziczek = 774;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.801f; ty > -1.117f; ty -= 0.056f)
				for (FLOAT tx = -0.314f; tx < 1.087f; tx += 0.056f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.056f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.056f)
				for (FLOAT px = -0.314f; px < 1.087f; px += 0.056f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 16.67f;
			while (yy1 < 500.1f && yy2 < 516.77f)
			{
				FLOAT xx1 = 348.44f, xx2 = 373.5f;
				while (xx1 < 1000.0f && xx2 < 1025.06f)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 25.06f;
					xx2 += 25.06f;
				}
				yy1 += 16.67f;
				yy2 += 16.67f;
			}
		}

		OpcjeNapX[0] = -1.01f;
		OpcjeNapX[1] = 0.74f;
		OpcjeNapX[2] = -1.03f;
		OpcjeNapX[3] = -0.98f;
		OpcjeNapX[4] = -0.88f;

		Grafika1Pos[0] = 0.46f;
		Grafika1Pos[1] = 0.72f;
		Grafika1Pos[2] = 0.82f;
		Grafika1Pos[3] = 0.66f;

		PosSuwakX = -0.46f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -1.0f;
		OpcjeDzwiekuX[1] = -1.03f;
	}

	else if (rozdzielczosc_w >= 1280.0f && rozdzielczosc_w < 1360.0f && rozdzielczosc_h >= 1024.0f && rozdzielczosc_h < 1050.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -0.89f;
		nap[3][0].x2 = -0.89f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0028f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -0.97f;
		vGraNapisy.x2 = 0.82f;
		x_dl_zycia = -0.84f;
		xBroniki = 0.91f;

		MenuStworzLabX[0] = -0.95f;
		MenuStworzLabX[1] = -1.01f;
		MenuStworzLabX[2] = -0.89f;
		MenuStworzLabX[3] = -0.95f;
		MenuStworzLabX[4] = -0.51f;
		MenuStworzLabX[5] = -0.374f;

		ObjektyScen12X[0] = -0.31f;
		ObjektyScen12X[1] = -0.59f;

		PozGuz[0].x1 = -0.518f;
		PozGuz[1].x1 = -0.383f;
		PozGuz[2].x1 = -0.518f;
		PozGuz[3].x1 = -0.383f;
		PozGuz[4].x1 = -0.518f;
		PozGuz[5].x1 = -0.383f;
		PozGuz[6].x1 = -0.518f;
		PozGuz[7].x1 = -0.383f;
		PozGuz[8].x1 = -0.518f;
		PozGuz[9].x1 = -0.383f;

		IloPunkt = 834;
		ZaznaczGuziczek = 826;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.803f; ty > -1.117f; ty -= 0.052f)
				for (FLOAT tx = -0.28f; tx < 1.021f; tx += 0.052f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.052f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.052f)
				for (FLOAT px = -0.28f; px < 1.021f; px += 0.052f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 15.63f;
			while (yy1 < 500.16f && yy2 < 515.79f)
			{
				FLOAT xx1 = 355.37f, xx2 = 380.26f;
				while (xx1 < 999.91f && xx2 < 1024.7f)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 24.79f;
					xx2 += 24.79f;
				}
				yy1 += 15.63f;
				yy2 += 15.63f;
			}
		}

		OpcjeNapX[0] = -0.96f;
		OpcjeNapX[1] = 0.69f;
		OpcjeNapX[2] = -0.99f;
		OpcjeNapX[3] = -0.94f;
		OpcjeNapX[4] = -0.84f;

		Grafika1Pos[0] = 0.4f;
		Grafika1Pos[1] = 0.66f;
		Grafika1Pos[2] = 0.76f;
		Grafika1Pos[3] = 0.6f;

		PosSuwakX = -0.53f;
		ScaleSuwak = -0.03f;

		OpcjeDzwiekuX[0] = -0.94f;
		OpcjeDzwiekuX[1] = -0.97f;
	}

	else if (rozdzielczosc_w >= 1360.0f && rozdzielczosc_w < 1366.0f && rozdzielczosc_h >= 768.0f && rozdzielczosc_h < 1050.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.38f;
		vGraNapisy.x2 = 1.24f;
		x_dl_zycia = -1.25f;
		xBroniki = 1.33f;

		MenuStworzLabX[0] = -1.24f;
		MenuStworzLabX[1] = -1.3f;
		MenuStworzLabX[2] = -1.19f;
		MenuStworzLabX[3] = -1.25f;
		MenuStworzLabX[4] = -0.698f;
		MenuStworzLabX[5] = -0.563f;

		ObjektyScen12X[0] = -0.5f;
		ObjektyScen12X[1] = -0.782f;

		PozGuz[0].x1 = -0.709f;
		PozGuz[1].x1 = -0.573f;
		PozGuz[2].x1 = -0.709f;
		PozGuz[3].x1 = -0.573f;
		PozGuz[4].x1 = -0.709f;
		PozGuz[5].x1 = -0.573f;
		PozGuz[6].x1 = -0.709f;
		PozGuz[7].x1 = -0.573f;
		PozGuz[8].x1 = -0.709f;
		PozGuz[9].x1 = -0.573f;

		IloPunkt = 574;
		ZaznaczGuziczek = 566;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.79f; ty > -1.117f; ty -= 0.076f)
				for (FLOAT tx = -0.461f; tx < 1.44f; tx += 0.076f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.076f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.77f; py > -1.037f; py -= 0.076f)
				for (FLOAT px = -0.461f; px < 1.44f; px += 0.076f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		FLOAT MouseXp = 454.0f;
		FLOAT HelpH = rozdzielczosc_w / 1001;
		FLOAT HelpG = rozdzielczosc_h / 500;
		FLOAT HelpW = ((rozdzielczosc_w - MouseXp) / HelpH) / 26;
		FLOAT HelpIlosc = (IloPunkt - 2) / 26;
		FLOAT HelpT = rozdzielczosc_h / HelpIlosc / HelpG;
		FLOAT HelpI = HelpT * HelpIlosc;
		FLOAT HelpZ = HelpI + HelpT;

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0.0f, yy2 = HelpT;
			while (yy1 < HelpI && yy2 < HelpZ)
			{
				FLOAT xx1 = MouseXp / HelpH;
				FLOAT xx2 = xx1 + HelpW;
				FLOAT HelpU = xx1 + (25 * HelpW);
				FLOAT HelpJ = HelpU + HelpW;
				while (xx1 < HelpU && xx2 < HelpJ)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += HelpW;
					xx2 += HelpW;
				}
				yy1 += HelpT;
				yy2 += HelpT;
			}
		}

		OpcjeNapX[0] = -1.38f;
		OpcjeNapX[1] = 1.11f;
		OpcjeNapX[2] = -1.2f;
		OpcjeNapX[3] = -1.15f;
		OpcjeNapX[4] = -1.06f;

		Grafika1Pos[0] = 0.63f;
		Grafika1Pos[1] = 0.89f;
		Grafika1Pos[2] = 0.99f;
		Grafika1Pos[3] = 0.83f;

		PosSuwakX = -0.22f;
		ScaleSuwak = -0.01f;

		OpcjeDzwiekuX[0] = -1.27f;
		OpcjeDzwiekuX[1] = -1.3f;
	}

	else if (rozdzielczosc_w >= 1366.0f && rozdzielczosc_w < 1400.0f && rozdzielczosc_h >= 768.0f && rozdzielczosc_h < 1050.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.4f;
		vGraNapisy.x2 = 1.25f;
		x_dl_zycia = -1.27f;
		xBroniki = 1.34f;

		MenuStworzLabX[0] = -1.24f;
		MenuStworzLabX[1] = -1.3f;
		MenuStworzLabX[2] = -1.19f;
		MenuStworzLabX[3] = -1.25f;
		MenuStworzLabX[4] = -0.707f;
		MenuStworzLabX[5] = -0.571f;

		ObjektyScen12X[0] = -0.507f;
		ObjektyScen12X[1] = -0.787f;

		PozGuz[0].x1 = -0.715f;
		PozGuz[1].x1 = -0.58f;
		PozGuz[2].x1 = -0.715f;
		PozGuz[3].x1 = -0.58f;
		PozGuz[4].x1 = -0.715f;
		PozGuz[5].x1 = -0.58f;
		PozGuz[6].x1 = -0.715f;
		PozGuz[7].x1 = -0.58f;
		PozGuz[8].x1 = -0.715f;
		PozGuz[9].x1 = -0.58f;

		IloPunkt = 574;
		ZaznaczGuziczek = 566;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.789f; ty > -1.117f; ty -= 0.076f)
				for (FLOAT tx = -0.461f; tx < 1.44f; tx += 0.076f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.076f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.775f; py > -1.042f; py -= 0.076f)
				for (FLOAT px = -0.461f; px < 1.44f; px += 0.076f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		FLOAT MouseXp = 454.0f;
		FLOAT HelpH = rozdzielczosc_w / 1001;
		FLOAT HelpG = rozdzielczosc_h / 500;
		FLOAT HelpW = ((rozdzielczosc_w - MouseXp) / HelpH) / 26;
		FLOAT HelpIlosc = (IloPunkt - 2) / 26;
		FLOAT HelpT = rozdzielczosc_h / HelpIlosc / HelpG;
		FLOAT HelpI = HelpT * HelpIlosc;
		FLOAT HelpZ = HelpI + HelpT;

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0.0f, yy2 = HelpT;
			while (yy1 < HelpI && yy2 < HelpZ)
			{
				FLOAT xx1 = MouseXp / HelpH;
				FLOAT xx2 = xx1 + HelpW;
				FLOAT HelpU = xx1 + (25 * HelpW);
				FLOAT HelpJ = HelpU + HelpW;
				while (xx1 < HelpU && xx2 < HelpJ)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += HelpW;
					xx2 += HelpW;
				}
				yy1 += HelpT;
				yy2 += HelpT;
			}
		}

		OpcjeNapX[0] = -1.38f;
		OpcjeNapX[1] = 1.11f;
		OpcjeNapX[2] = -1.21f;
		OpcjeNapX[3] = -1.17f;
		OpcjeNapX[4] = -1.07f;

		Grafika1Pos[0] = 0.63f;
		Grafika1Pos[1] = 0.89f;
		Grafika1Pos[2] = 0.99f;
		Grafika1Pos[3] = 0.83f;

		PosSuwakX = -0.22f;
		ScaleSuwak = -0.01f;

		OpcjeDzwiekuX[0] = -1.27f;
		OpcjeDzwiekuX[1] = -1.3f;
	}

	else if (rozdzielczosc_w >= 1400.0f && rozdzielczosc_w < 1440.0f && rozdzielczosc_h >= 1050.0f && rozdzielczosc_h < 1080.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -0.94f;
		nap[3][0].x2 = -0.94f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.003f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.03f;
		vGraNapisy.x2 = 0.9f;
		x_dl_zycia = -0.9f;
		xBroniki = 0.98f;

		MenuStworzLabX[0] = -1.01f;
		MenuStworzLabX[1] = -1.06f;
		MenuStworzLabX[2] = -0.95f;
		MenuStworzLabX[3] = -1.01f;
		MenuStworzLabX[4] = -0.545f;
		MenuStworzLabX[5] = -0.41f;

		ObjektyScen12X[0] = -0.341f;
		ObjektyScen12X[1] = -0.622f;

		PozGuz[0].x1 = -0.549f;
		PozGuz[1].x1 = -0.413f;
		PozGuz[2].x1 = -0.549f;
		PozGuz[3].x1 = -0.413f;
		PozGuz[4].x1 = -0.549f;
		PozGuz[5].x1 = -0.413f;
		PozGuz[6].x1 = -0.549f;
		PozGuz[7].x1 = -0.413f;
		PozGuz[8].x1 = -0.549f;
		PozGuz[9].x1 = -0.413f;

		IloPunkt = 782;
		ZaznaczGuziczek = 774;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.8f; ty > -1.117f; ty -= 0.056f)
				for (FLOAT tx = -0.309f; tx < 1.092f; tx += 0.056f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.056f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.056f)
				for (FLOAT px = -0.309f; px < 1.092f; px += 0.056f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 16.67f;
			while (yy1 < 500.1 && yy2 < 516.67f)
			{
				FLOAT xx1 = 350.71f, xx2 = 375.68f;
				while (xx1 < 999.93f && xx2 < 1024.9)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 24.97f;
					xx2 += 24.97f;
				}
				yy1 += 16.67f;
				yy2 += 16.67f;
			}
		}

		OpcjeNapX[0] = -1.05f;
		OpcjeNapX[1] = 0.78f;
		OpcjeNapX[2] = -1.04f;
		OpcjeNapX[3] = -0.99f;
		OpcjeNapX[4] = -0.9f;

		Grafika1Pos[0] = 0.46f;
		Grafika1Pos[1] = 0.72f;
		Grafika1Pos[2] = 0.82f;
		Grafika1Pos[3] = 0.67f;

		PosSuwakX = -0.45f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -0.99f;
		OpcjeDzwiekuX[1] = -1.02f;
	}

	else if (rozdzielczosc_w >= 1440.0f && rozdzielczosc_w < 1600.0f && rozdzielczosc_h >= 900.0f && rozdzielczosc_h < 1050.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.24f;
		vGraNapisy.x2 = 1.11f;
		x_dl_zycia = -1.11f;
		xBroniki = 1.19f;

		MenuStworzLabX[0] = -1.15f;
		MenuStworzLabX[1] = -1.21f;
		MenuStworzLabX[2] = -1.1f;
		MenuStworzLabX[3] = -1.16f;
		MenuStworzLabX[4] = -0.639f;
		MenuStworzLabX[5] = -0.504f;

		ObjektyScen12X[0] = -0.44f;
		ObjektyScen12X[1] = -0.721f;

		PozGuz[0].x1 = -0.648f;
		PozGuz[1].x1 = -0.513f;
		PozGuz[2].x1 = -0.648f;
		PozGuz[3].x1 = -0.513f;
		PozGuz[4].x1 = -0.648f;
		PozGuz[5].x1 = -0.513f;
		PozGuz[6].x1 = -0.648f;
		PozGuz[7].x1 = -0.513f;
		PozGuz[8].x1 = -0.648f;
		PozGuz[9].x1 = -0.513f;

		IloPunkt = 652;
		ZaznaczGuziczek = 644;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.794f; ty > -1.117f; ty -= 0.068f)
				for (FLOAT tx = -0.402f; tx < 1.299f; tx += 0.068f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.068f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.068f)
				for (FLOAT px = -0.402f; px < 1.299f; px += 0.068f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 20.0f;
			while (yy1 < 500.0f && yy2 < 520.0f)
			{
				FLOAT xx1 = 338.2f, xx2 = 363.64f;
				while (xx1 < 999.9f && xx2 < 1025.35f)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 25.45f;
					xx2 += 25.45f;
				}
				yy1 += 20.0f;
				yy2 += 20.0f;
			}
		}

		OpcjeNapX[0] = -1.23;
		OpcjeNapX[1] = 0.96;
		OpcjeNapX[2] = -1.14f;
		OpcjeNapX[3] = -1.09f;
		OpcjeNapX[4] = -0.99f;

		Grafika1Pos[0] = 0.58f;
		Grafika1Pos[1] = 0.84f;
		Grafika1Pos[2] = 0.94f;
		Grafika1Pos[3] = 0.78f;

		PosSuwakX = -0.27f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -1.15f;
		OpcjeDzwiekuX[1] = -1.18f;
	}

	else if (rozdzielczosc_w >= 1600.0f && rozdzielczosc_w < 1680.0f && rozdzielczosc_h >= 900.0f && rozdzielczosc_h < 1050.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.39f;
		vGraNapisy.x2 = 1.25;
		x_dl_zycia = -1.26f;
		xBroniki = 1.33;

		MenuStworzLabX[0] = -1.26f;
		MenuStworzLabX[1] = -1.32f;
		MenuStworzLabX[2] = -1.22f;
		MenuStworzLabX[3] = -1.28f;
		MenuStworzLabX[4] = -0.707f;
		MenuStworzLabX[5] = -0.57f;

		ObjektyScen12X[0] = -0.504f;
		ObjektyScen12X[1] = -0.785f;

		PozGuz[0].x1 = -0.712f;
		PozGuz[1].x1 = -0.577f;
		PozGuz[2].x1 = -0.712f;
		PozGuz[3].x1 = -0.577f;
		PozGuz[4].x1 = -0.712f;
		PozGuz[5].x1 = -0.577f;
		PozGuz[6].x1 = -0.712f;
		PozGuz[7].x1 = -0.577f;
		PozGuz[8].x1 = -0.712f;
		PozGuz[9].x1 = -0.577f;

		IloPunkt = 574;
		ZaznaczGuziczek = 566;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.79f; ty > -1.117f; ty -= 0.076f)
				for (FLOAT tx = -0.464f; tx < 1.437f; tx += 0.076f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.076f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.77f; py > -1.037f; py -= 0.076f)
				for (FLOAT px = -0.464; px < 1.437f; px += 0.076f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 22.73f;
			while (yy1 < 500.06f && yy2 < 522.79f)
			{
				FLOAT xx1 = 332.5f, xx2 = 358.17f;
				while (xx1 < 999.92f && xx2 < 1025.59f)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 25.67f;
					xx2 += 25.67f;
				}
				yy1 += 22.73f;
				yy2 += 22.73f;
			}
		}

		OpcjeNapX[0] = -1.35f;
		OpcjeNapX[1] = 1.1f;
		OpcjeNapX[2] = -1.21f;
		OpcjeNapX[3] = -1.16f;
		OpcjeNapX[4] = -1.06f;

		Grafika1Pos[0] = 0.71f;
		Grafika1Pos[1] = 0.96f;
		Grafika1Pos[2] = 1.07f;
		Grafika1Pos[3] = 0.9f;

		PosSuwakX = -0.14f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -1.25f;
		OpcjeDzwiekuX[1] = -1.28f;
	}

	else if (rozdzielczosc_w >= 1680.0f && rozdzielczosc_w < 1920.0f && rozdzielczosc_h >= 1050.0f && rozdzielczosc_h < 1080.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.24f;
		vGraNapisy.x2 = 1.11f;
		x_dl_zycia = -1.11f;
		xBroniki = 1.19f;

		MenuStworzLabX[0] = -1.17f;
		MenuStworzLabX[1] = -1.22f;
		MenuStworzLabX[2] = -1.11f;
		MenuStworzLabX[3] = -1.17f;
		MenuStworzLabX[4] = -0.647f;
		MenuStworzLabX[5] = -0.51f;

		ObjektyScen12X[0] = -0.443f;
		ObjektyScen12X[1] = -0.723f;

		PozGuz[0].x1 = -0.65f;
		PozGuz[1].x1 = -0.515f;
		PozGuz[2].x1 = -0.65f;
		PozGuz[3].x1 = -0.515f;
		PozGuz[4].x1 = -0.65f;
		PozGuz[5].x1 = -0.515f;
		PozGuz[6].x1 = -0.65f;
		PozGuz[7].x1 = -0.515f;
		PozGuz[8].x1 = -0.65f;
		PozGuz[9].x1 = -0.515f;

		IloPunkt = 652;
		ZaznaczGuziczek = 644;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.794f; ty > -1.117f; ty -= 0.068f)
				for (FLOAT tx = -0.405f; tx < 1.296f; tx += 0.068f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.068f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.78f; py > -1.047f; py -= 0.068f)
				for (FLOAT px = -0.405f; px < 1.296f; px += 0.068f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 20.0f;
			while (yy1 < 500.0f && yy2 < 525.0f)
			{
				FLOAT xx1 = 336.9f, xx2 = 362.4f;
				while (xx1 < 999.9f && xx2 < 1025.4f)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 25.5f;
					xx2 += 25.5f;
				}
				yy1 += 20.0f;
				yy2 += 20.0f;
			}
		}

		OpcjeNapX[0] = -1.25f;
		OpcjeNapX[1] = 0.99f;
		OpcjeNapX[2] = -1.16f;
		OpcjeNapX[3] = -1.11f;
		OpcjeNapX[4] = -1.0f;

		Grafika1Pos[0] = 0.56f;
		Grafika1Pos[1] = 0.82f;
		Grafika1Pos[2] = 0.93f;
		Grafika1Pos[3] = 0.77f;

		PosSuwakX = -0.28f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -1.13f;
		OpcjeDzwiekuX[1] = -1.16f;
	}

	else if (rozdzielczosc_w >= 1920.0f && rozdzielczosc_w < 2000.0f && rozdzielczosc_h >= 1080.0f && rozdzielczosc_h < 1100.0f)
	{
		nap[0][0].x1 = -0.15f;
		nap[0][1].x1 = -0.3f;
		nap[0][2].x1 = -0.43f;
		nap[0][0].y1 = 0.47f;
		nap[0][1].y1 = -0.09f;
		nap[0][2].y1 = -0.69f;

		nap[1][0].x1 = -0.15f;
		nap[1][1].x1 = -0.36f;
		nap[1][2].x1 = -0.5f;
		nap[1][3].x1 = -0.7f;
		nap[1][4].x1 = -0.27f;
		nap[1][0].y1 = 0.37f;
		nap[1][1].y1 = 0.17f;
		nap[1][2].y1 = -0.07f;
		nap[1][3].y1 = -0.28f;
		nap[1][4].y1 = -0.65f;

		nap[2][0].x1 = -0.31f;
		nap[2][1].x1 = -0.27f;
		nap[2][2].x1 = -0.31f;
		nap[2][3].x1 = -0.45f;
		nap[2][4].x1 = -0.27f;
		nap[2][0].y1 = 0.37f;
		nap[2][1].y1 = 0.17f;
		nap[2][2].y1 = -0.07f;
		nap[2][3].y1 = -0.28f;
		nap[2][4].y1 = -0.65f;

		nap[3][0].x1 = -1.03f;
		nap[3][0].x2 = -1.03f;
		nap[3][1].x1 = -0.61f;
		nap[3][2].x1 = -0.75f;
		nap[3][3].x1 = -0.27f;
		nap[3][0].y1 = 0.37f;
		nap[3][1].y1 = 0.05f;
		nap[3][2].y1 = -0.19f;
		nap[3][3].y1 = -0.65f;

		ScaleMenu4 = 0.0033f;

		nap[4][0].x1 = -0.73f;
		nap[4][1].x1 = -0.3f;
		nap[4][2].x1 = -0.27f;
		nap[4][0].y1 = 0.37f;
		nap[4][1].y1 = -0.07f;
		nap[4][2].y1 = -0.65f;

		nap[5][0].x1 = -0.55f;
		nap[5][1].x1 = -0.2f;
		nap[5][2].x1 = -0.27f;
		nap[5][0].y1 = 0.37f;
		nap[5][1].y1 = -0.07f;
		nap[5][2].y1 = -0.65f;

		vGraNapisy.x1 = -1.38f;
		vGraNapisy.x2 = 1.26f;
		x_dl_zycia = -1.25f;
		xBroniki = 1.34f;

		MenuStworzLabX[0] = -1.26f;
		MenuStworzLabX[1] = -1.32f;
		MenuStworzLabX[2] = -1.22f;
		MenuStworzLabX[3] = -1.28f;
		MenuStworzLabX[4] = -0.708f;
		MenuStworzLabX[5] = -0.569f;

		ObjektyScen12X[0] = -0.505f;
		ObjektyScen12X[1] = -0.786f;

		PozGuz[0].x1 = -0.713f;
		PozGuz[1].x1 = -0.578f;
		PozGuz[2].x1 = -0.713f;
		PozGuz[3].x1 = -0.578f;
		PozGuz[4].x1 = -0.713f;
		PozGuz[5].x1 = -0.578f;
		PozGuz[6].x1 = -0.713f;
		PozGuz[7].x1 = -0.578f;
		PozGuz[8].x1 = -0.713f;
		PozGuz[9].x1 = -0.578f;

		IloPunkt = 548;
		ZaznaczGuziczek = 540;

		INT poz = 1;
		while (poz < IloPunkt - 1)
		{
			for (FLOAT ty = 0.79f; ty > -1.117f; ty -= 0.076f)
				for (FLOAT tx = -0.467f; tx < 1.434f; tx += 0.076f)
				{
					pozycja_sciany[1][poz].x1 = tx;
					pozycja_sciany[1][poz].y1 = ty;
					poz++;
				}
		}

		ScaleGuzik = 0.076f;

		INT pu = 1;
		while (pu < IloPunkt - 1)
		{
			for (FLOAT py = 0.77f; py > -1.037f; py -= 0.076f)
				for (FLOAT px = -0.467f; px < 1.434f; px += 0.076f)
				{
					punkcior[pu].x1 = px;
					punkcior[pu].y1 = py;
					pu++;
				}
		}

		INT poz1 = 1;
		while (poz1 < IloPunkt - 1)
		{
			FLOAT yy1 = 0, yy2 = 23.8f;
			while (yy1 < 499.8f && yy2 < 523.6f)
			{
				FLOAT xx1 = 330.2f, xx2 = 355.96f;
				while (xx1 < 999.96f && xx2 < 1025.72f)
				{
					pozycja_sciany[0][poz1].x1 = xx1;
					pozycja_sciany[0][poz1].x2 = xx2;
					pozycja_sciany[0][poz1].y1 = yy1;
					pozycja_sciany[0][poz1].y2 = yy2;
					poz1++;
					xx1 += 25.76f;
					xx2 += 25.76f;
				}
				yy1 += 23.8f;
				yy2 += 23.8f;
			}
		}

		OpcjeNapX[0] = -1.39f;
		OpcjeNapX[1] = 1.14f;
		OpcjeNapX[2] = -1.22f;
		OpcjeNapX[3] = -1.17f;
		OpcjeNapX[4] = -1.08f;

		Grafika1Pos[0] = 0.69f;
		Grafika1Pos[1] = 0.96f;
		Grafika1Pos[2] = 1.06f;
		Grafika1Pos[3] = 0.91f;

		PosSuwakX = -0.14f;
		ScaleSuwak = -0.02f;

		OpcjeDzwiekuX[0] = -1.25f;
		OpcjeDzwiekuX[1] = -1.28f;
	}
}

void LoadSave::WczytPost(INT i, SkinningModels *Potworek)
{
	if (((tab[i - 25] == 1) && (tab[i - 26] != 1)) ||
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
		Potworek[i].Pozycja[0].x1 = boxx[i].x1;
		Potworek[i].Pozycja[0].y1 = boxx[i].y1;
		Potworek[i].Pozycja[0].z1 = boxx[i].z1;

		Potworek[i].Kat[0] = 0.5f;

		Potworek[i].Pozycja[1].x1 = Potworek[i].Pozycja[0].x1;
		Potworek[i].Pozycja[1].y1 = Potworek[i].Pozycja[0].y1;
		Potworek[i].Pozycja[1].z1 = Potworek[i].Pozycja[0].z1;
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
		Potworek[i].Pozycja[0].x1 = boxx[i].x1;
		Potworek[i].Pozycja[0].y1 = boxx[i].y1;
		Potworek[i].Pozycja[0].z1 = boxx[i].z1;

		Potworek[i].Kat[0] = 4.0f;

		Potworek[i].Pozycja[1].x1 = Potworek[i].Pozycja[0].x1;
		Potworek[i].Pozycja[1].y1 = Potworek[i].Pozycja[0].y1;
		Potworek[i].Pozycja[1].z1 = Potworek[i].Pozycja[0].z1;
		Potworek[i].Kat[1] = Potworek[i].Kat[0];
	}
}

void LoadSave::WczytLab(string sciezka, string nazwa)
{
	string txt = ".txt";
	string nazwa2 = sciezka + nazwa + txt;

	ifstream plik(nazwa2);

	if (plik.good())
	{
		FLOAT TempPositionX = 0, TempPositionZ = 0;
		for (INT i = 0; i < IloElLab; i++)
			tab[i] = 0;

		while (!plik.eof())
		{
			if (WczytLabiryntu % 2 == 1)
			{
				for (INT i = 0; i < IloElLab; i++)
					plik >> tab[i];
			}
			else if (WczytLabiryntu % 2 == 0)
			{
				plik >> punkty;
				plik >> strzaly;
				plik >> plansza;
				plik >> dl_zycia;
				plik >> x_dl_zycia;
				plik >> TempPositionX;
				TPCam.charPosition = XMVectorSetX(TPCam.charPosition, TempPositionX);
				plik >> TempPositionZ;
				TPCam.charPosition = XMVectorSetZ(TPCam.charPosition, TempPositionZ);
				XMMATRIX Translate = XMMatrixTranslation(TempPositionX, 0.1f, TempPositionZ);
				TPCam.playerCharWorld = Translate;
				plik >> TPCam.charDirAngle;
				plik >> TempTime;
				plik >> weapons[0];
				plik >> weapons[1];
				plik >> weapons[2];
				plik >> ilo_bron;

				for (INT i = 0; i < IloElLab; i++)
					plik >> tab[i];
			}
		}
		plik.close();
	}

	for (INT i = 0; i < IloElLab; i++)
	{
		if (tab[i] == 8)
			WczytPost(i, Potwor[0]);

		if (tab[i] == 9)
			WczytPost(i, Potwor[1]);

		if (tab[i] == 10)
			WczytPost(i, Potwor[2]);

		if (tab[i] == 11)
			WczytPost(i, Potwor[3]);
	}

	if (scen[0] == 19 && WczytLabiryntu % 2 == 1)
	{
		pozycja_startowa();
		czekanie = state = 0;
		start = clock();
	}
}

void LoadSave::WczytUstSter(string ZapisUstSterowania, INT klaw1, INT klaw2, INT pierw1, INT pierw2, INT ster)
{
	ifstream plik(ZapisUstSterowania);

	if (plik.good() == true)
	{
		for (INT kl = klaw1; kl < klaw2; kl++)
			klaw[kl].klawisz = klaw[kl].ustawiony_klaw = 0;

		while (!plik.eof())
		{
			for (INT kl = klaw1; kl < klaw2; kl++)
			{
				plik >> klaw[kl].klawisz;
				plik >> klaw[kl].ustawiony_klaw;
			}
		}
		plik.close();
		for (INT pier = pierw1; pier < pierw2; pier++)
			pierwszy[pier] = 1;

		wczyt[ster] = true;
	}
}

void LoadSave::WczytUstDzw()
{
	ifstream plik("zapisane_ustawienia_dzwieku.txt");

	if (plik.good() == true)
	{
		Tlx[1] = Tlx[2] = 0;
		glosnosc_efektow = glosnosc_tla = 0;

		while (!plik.eof())
		{
			plik >> Tlx[1];
			plik >> Tlx[2];
			plik >> glosnosc_tla;
			plik >> glosnosc_efektow;
		}
		plik.close();

		wczyt[2] = true;
	}
}

void LoadSave::WczytUstGraf()
{
	ifstream plik("zapisane_ustawienia_grafiki.txt");

	if (plik.good() == true)
	{
		while (!plik.eof())
		{
			plik >> Tlx[0];
			plik >> jas_sw;
			plik >> full;
			plik >> rozdzielczosc_w;
			plik >> rozdzielczosc_h;
		}
		plik.close();
		wczyt[3] = true;
	}

	roz_w = rozdzielczosc_w;
	roz_h = rozdzielczosc_h;
	PozARozdz();
}

void LoadSave::ZapUstSter(string ZapisUstSterowania, INT klaw1, INT klaw2, INT ster)
{
	fstream plik;

	plik.open(ZapisUstSterowania, ios::out | ios::trunc);
	if (plik.good())
	{
		for (INT kl = klaw1; kl < klaw2; kl++)
		{
			plik << klaw[kl].klawisz << endl;
			plik << klaw[kl].ustawiony_klaw << endl;
		}
		plik.close();
	}

	zapis[ster] = TRUE;
}

void LoadSave::ZapUstDzw()
{
	fstream plik;

	plik.open("zapisane_ustawienia_dzwieku.txt", ios::out | ios::trunc);
	if (plik.good())
	{
		plik << Tlx[1] << endl;
		plik << Tlx[2] << endl;
		plik << glosnosc_tla << endl;
		plik << glosnosc_efektow << endl;

		plik.close();
	}

	zapis[2] = TRUE;
}

void LoadSave::ZapUstGraf()
{
	fstream plik;

	plik.open("zapisane_ustawienia_grafiki.txt", ios::out | ios::trunc);
	if (plik.good())
	{
		plik << Tlx[0] << endl;
		plik << jas_sw << endl;
		plik << full << endl;
		plik << rozdzielczosc_w << endl;
		plik << rozdzielczosc_h << endl;

		plik.close();
	}

	roz_w = rozdzielczosc_w;
	roz_h = rozdzielczosc_h;

	zapis[3] = TRUE;
	//zapisany_full = full;
	zmiana_full = FALSE;
}

void LoadSave::ZapLab(string nazwa)
{
	string sciezka;
	string txt = ".txt";

	if (scen[0] == 20 && ZapiszGiere % 2 == 1)
		sciezka = "ZapisaneLabirynty\\";

	else if (ZapiszGiere % 2 == 0)
		sciezka = "ZapisanaGra\\";

	string nazwa2 = sciezka + nazwa + txt;
	fstream plik;
	plik.open(nazwa2.c_str(), ios::out | ios::trunc);

	if (scen[0] == 20 && ZapiszGiere % 2 == 1)
		for (INT g = 0; g < IloElLab; g++)
			plik << tab[g] << endl;

	else if (ZapiszGiere % 2 == 0)
	{
		plik << punkty << endl;
		plik << strzaly << endl;
		plik << plansza << endl;
		plik << dl_zycia << endl;
		plik << x_dl_zycia << endl;
		plik << XMVectorGetX(TPCam.charPosition) << endl;
		plik << XMVectorGetZ(TPCam.charPosition) << endl;
		plik << TPCam.charDirAngle << endl;
		plik << sekundy << endl;
		plik << weapons[0] << endl;
		plik << weapons[1] << endl;
		plik << weapons[2] << endl;
		plik << ilo_bron << endl;

		for (INT g = 0; g < IloElLab; g++)
			plik << tab[g] << endl;
	}

	plik.close();
}