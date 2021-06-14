#include "Structures.h"

Structures::Structures()
{
	mAlphaEBlendState = 0;
	mAlphaDBlendState = 0;

	pozycja_sciany[0] = new Vect[IloPunkt];
	pozycja_sciany[1] = new Vect[IloPunkt];
	punkcior = new Vect[IloPunkt];
	boxx = new Vect[2604];

	klaw = new klawisze[256];
	for (INT i = 0; i < 4; i++)
		Potwor[i] = new SkinningModels[IloElLab];

	poz_scian = new INT[IloPunkt];
	odleglosc = new FLOAT[IloElLab];

	for(int ilo = 0; ilo < 10; ilo++)
		Modele[ilo] = new Models[IloElLab];
	
	StrzalaShot = new Models[MaxCountArrows];

	Czarny = new Models*[Znaki];
	Czerwony = new Models*[Znaki];

	tab = new INT[2604];
	ShowArrowOnHedge = new BOOL[MaxCountArrows];

	ShowArrowOnMonster = new BOOL*[IloElLab];

	for (INT j = 0; j < IloElLab; j++)
		ShowArrowOnMonster[j] = new BOOL[MaxCountArrows];

	for (INT j = 0; j < 4; j++)
	{
		for (INT i = 0; i < IloElLab; i++)
		{
			Potwor[j][i] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1 };
			Potwor[j][i].Speed[0] = 1.0f;
			Potwor[j][i].Speed[1] = 7.0f;
			Potwor[j][i].Animation = new BOOL[10];

			for(INT an = 0; an < 10; an++)
			Potwor[j][i].Animation[an] = FALSE;

			Potwor[j][i].traf = 0;
			Potwor[j][i].trafienie = FALSE;
		}

		Potwor[j]->M = new SkinnedModel*[MonstersAnimation];
	}

	Postac.M = new SkinnedModel*[PersonAnimation];

	for (INT kla = 0; kla < KLA; kla++)
		for (INT i = 0; i < 19; i++)
			klaw[kla].zapis[i] = 0;

	for (INT i = 0; i < 50; i++)
	{
		rozdz[i].ind = 0;
		rozdz[i].height = 0;
		rozdz[i].width = 0;
	}

	Postac.Speed[0] = 1.0f;
	PozycjaStrzaly.x1 = 5.6f; PozycjaStrzaly.x2 = 200.0f; PozycjaStrzaly.y1 = 0.9f;

	ilosc[0] = 0; ilosc[1] = 0;

	//klawiatura i myszka
	klaw[0] = { 0, 0, "" };
	klaw[1] = { 0, 0, "ESC", DIK_ESCAPE };
	klaw[2] = { 0, 0, "F1", DIK_F1 };
	klaw[3] = { 0, 0, "F2", DIK_F2 };
	klaw[4] = { 0, 0, "F3", DIK_F3 };
	klaw[5] = { 0, 0, "F4", DIK_F4 };
	klaw[6] = { 0, 0, "F5", DIK_F5 };
	klaw[7] = { 0, 0, "F6", DIK_F6 };
	klaw[8] = { 0, 0, "F7", DIK_F7 };
	klaw[9] = { 0, 0, "F8", DIK_F8 };
	klaw[10] = { 0, 0, "F9", DIK_F9 };
	klaw[11] = { 0, 0, "F10", DIK_F10 };
	klaw[12] = { 0, 0, "F11", DIK_F11 };
	klaw[13] = { 0, 0, "F12", DIK_F12 };
	klaw[14] = { 0, 0, "INSERT", DIK_INSERT };
	klaw[15] = { 0, 0, "DELETE", DIK_DELETE };
	klaw[16] = { 0, 0, "`", DIK_GRAVE };
	klaw[17] = { 0, 0, "1", DIK_1 };
	klaw[18] = { 0, 0, "2", DIK_2 };
	klaw[19] = { 0, 0, "3", DIK_3 };
	klaw[20] = { 0, 0, "4", DIK_4 };
	klaw[21] = { 0, 0, "5", DIK_5 };
	klaw[22] = { 0, 0, "6", DIK_6 };
	klaw[23] = { 0, 0, "7", DIK_7 };
	klaw[24] = { 0, 0, "8", DIK_8 };
	klaw[25] = { 0, 0, "9", DIK_9 };
	klaw[26] = { 0, 0, "0", DIK_0 };
	klaw[27] = { 0, 0, "-", DIK_MINUS };
	klaw[28] = { 0, 0, "=", DIK_EQUALS };
	klaw[29] = { 0, 0, "BACKSPACE", DIK_BACKSPACE };
	klaw[30] = { 0, 13, "TAB", DIK_TAB };
	klaw[31] = { 0, 11, "q", DIK_Q };
	klaw[32] = { 0, 1, "w", DIK_W };
	klaw[33] = { 0, 10, "e", DIK_E };
	klaw[34] = { 0, 0, "r", DIK_R };
	klaw[35] = { 0, 0, "t", DIK_T };
	klaw[36] = { 0, 0, "y", DIK_Y };
	klaw[37] = { 0, 0, "u", DIK_U };
	klaw[38] = { 0, 0, "i", DIK_I };
	klaw[39] = { 0, 0, "o", DIK_O };
	klaw[40] = { 0, 0, "p", DIK_P };
	klaw[41] = { 0, 0, "[", DIK_LBRACKET };
	klaw[42] = { 0, 0, "]", DIK_RBRACKET };
	klaw[43] = { 0, 0, "\\", DIK_BACKSLASH };
	klaw[44] = { 0, 0, "CAPSLOCK", DIK_CAPSLOCK };
	klaw[45] = { 0, 3, "a", DIK_A };
	klaw[46] = { 0, 2, "s", DIK_S };
	klaw[47] = { 0, 4, "d", DIK_D };
	klaw[48] = { 0, 0, "f", DIK_F };
	klaw[49] = { 0, 0, "g", DIK_G };
	klaw[50] = { 0, 0, "h", DIK_H };
	klaw[51] = { 0, 0, "j", DIK_J };
	klaw[52] = { 0, 0, "k", DIK_K };
	klaw[53] = { 0, 0, "l", DIK_L };
	klaw[54] = { 0, 0, ";", DIK_SEMICOLON };
	klaw[55] = { 0, 0, "'", DIK_APOSTROPHE };
	klaw[56] = { 0, 0, "ENTER", DIK_RETURN };
	klaw[57] = { 0, 9, "L_SHIFT", DIK_LSHIFT };
	klaw[58] = { 0, 0, "z", DIK_Z };
	klaw[59] = { 0, 0, "x", DIK_X };
	klaw[60] = { 0, 0, "c", DIK_C };
	klaw[61] = { 0, 0, "v", DIK_V };
	klaw[62] = { 0, 0, "b", DIK_B };
	klaw[63] = { 0, 0, "n", DIK_N };
	klaw[64] = { 0, 0, "m", DIK_M };
	klaw[65] = { 0, 0, ",", DIK_COMMA };
	klaw[66] = { 0, 0, ".", DIK_PERIOD };
	klaw[67] = { 0, 0, "/", DIK_SLASH };
	klaw[68] = { 0, 0, "P_SHIFT", DIK_RSHIFT };
	klaw[69] = { 0, 0, "L_CTRL", DIK_LCONTROL };
	klaw[70] = { 0, 0, "L_ALT", DIK_LALT };
	klaw[71] = { 0, 12, "SPACE", DIK_SPACE };
	klaw[72] = { 0, 0, "P_ALT", DIK_RALT };
	klaw[73] = { 0, 5, "GÓRA", DIK_UP };
	klaw[74] = { 0, 6, "DÓ£", DIK_DOWN };
	klaw[75] = { 0, 7, "LEWO", DIK_LEFT };
	klaw[76] = { 0, 8, "PRAWO", DIK_RIGHT };
	klaw[77] = { 0, 0, "HOME", DIK_HOME };
	klaw[78] = { 0, 0, "END", DIK_END };
	klaw[79] = { 0, 0, "PGUP", DIK_PRIOR };
	klaw[80] = { 0, 0, "PGDOWN", DIK_NEXT };
	klaw[81] = { 0, 0, "P_CTRL", DIK_RCONTROL };
	klaw[82] = { 0, 0, "LMB", (UCHAR)DIMOUSE_BUTTON0 };
	klaw[83] = { 0, 0, "RMB", (UCHAR)DIMOUSE_BUTTON1 };
	klaw[84] = { 0, 14, "SCROLL_GÓRA", WHEEL_DELTA };
	klaw[85] = { 0, 15, "SCROLL_DÓ£",WHEEL_DELTA };

	// gamepad
	klaw[86] = { 0, 16, "LEWY_ANALOG_GÓRA" };
	klaw[87] = { 0, 17, "LEWY_ANALOG_DÓ£" };
	klaw[88] = { 0, 18, "LEWY_ANALOG_LEWO" };
	klaw[89] = { 0, 19, "LEWY_ANALOG_PRAWO" };
	klaw[90] = { 0, 0, "LEWY_ANALOG" };
	klaw[91] = { 0, 20, "PRAWY_ANALOG_GÓRA" };
	klaw[92] = { 0, 21, "PRAWY_ANALOG_DÓ£" };
	klaw[93] = { 0, 22, "PRAWY_ANALOG_LEWO" };
	klaw[94] = { 0, 23, "PRAWY_ANALOG_PRAWO" };
	klaw[95] = { 0, 0, "PRAWY_ANALOG" };
	klaw[96] = { 0, 29, "KRZYZYK_GÓRA" };
	klaw[97] = { 0, 30, "KRZYZYK_DÓ£" };
	klaw[98] = { 0, 0, "KRZYZYK_LEWO" };
	klaw[99] = { 0, 0, "KRZYZYK_PRAWO" };
	klaw[100] = { 0, 25, "A" };
	klaw[101] = { 0, 26, "B" };
	klaw[102] = { 0, 0, "X" };
	klaw[103] = { 0, 0, "Y" };
	klaw[104] = { 0, 0, "START" };
	klaw[105] = { 0, 0, "BACK" };
	klaw[106] = { 0, 0, "L1" };
	klaw[107] = { 0, 28, "R1" };
	klaw[108] = { 0, 24, "L2" };
	klaw[109] = { 0, 27, "R2" };

	klaw[120] = { 0, 0, "" };

	UstWszKl();

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

	INT bo2 = 0;
	while (bo2 < IloElLab - 2)
	{
		FLOAT zz1 = 480.0f, zz2 = 500.0f;
		while (zz1 > -504.0f && zz2 > -488.0f)
		{
			FLOAT xx1 = -260.0f, xx2 = -240.0f;
			while (xx1 < 244.0f && xx2 < 264.0f)
			{
				boxx[bo2].x1 = xx1;
				boxx[bo2].x2 = xx2;
				boxx[bo2].y1 = 0.1f;
				boxx[bo2].y2 = 1.0f;
				boxx[bo2].z1 = zz1;
				boxx[bo2].z2 = zz2;
				bo2++;
				xx1 += 20.0f;
				xx2 += 20.0f;
			}
			zz1 -= 20.0f;
			zz2 -= 20.0f;
		}
	}

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

	for (INT ilo = 0; ilo < 3; ilo++)
	{
		nap[0][ilo].x1 = 0.0f;
		nap[0][ilo].y1 = 0.0f;
		nap[4][ilo].x1 = 0.0f;
		nap[4][ilo].y1 = 0.0f;
		nap[5][ilo].x1 = 0.0f;
		nap[5][ilo].y1 = 0.0f;
	}

	for (INT ilo = 0; ilo < 4; ilo++)
	{
		nap[3][ilo].x1 = 0.0f;
		nap[3][ilo].y1 = 0.0f;
	}
	
	for (INT ii = 0; ii < MaxCountArrows; ii++)
		ShowArrowOnHedge[ii] = FALSE;

	for (int j = 0; j < IloElLab; j++)
	{
		for (INT ii = 0; ii < MaxCountArrows; ii++)
			ShowArrowOnMonster[j][ii] = FALSE;
	}
		
	for (INT ilo = 0; ilo < 5; ilo++)
	{
		nap[1][ilo].x1 = 0.0f;
		nap[2][ilo].x1 = 0.0f;
		nap[1][ilo].y1 = 0.0f;
		nap[2][ilo].y1 = 0.0f;
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

	nap[0][0].x2 = -0.15f;
	nap[0][1].x2 = -0.3f;
	nap[0][2].x2 = -0.43f;
	nap[0][0].y2 = 0.47f;
	nap[0][1].y2 = -0.09f;
	nap[0][2].y2 = -0.69f;

	nap[1][0].x2 = -0.15f;
	nap[1][1].x2 = -0.36f;
	nap[1][2].x2 = -0.5f;
	nap[1][3].x2 = -0.7f;
	nap[1][4].x2 = -0.27f;
	nap[1][0].y2 = 0.37f;
	nap[1][1].y2 = 0.17f;
	nap[1][2].y2 = -0.07f;
	nap[1][3].y2 = -0.28f;
	nap[1][4].y2 = -0.65f;

	nap[2][0].x2 = -0.31f;
	nap[2][1].x2 = -0.27f;
	nap[2][2].x2 = -0.31f;
	nap[2][3].x2 = -0.45f;
	nap[2][4].x2 = -0.27f;
	nap[2][0].y2 = 0.37f;
	nap[2][1].y2 = 0.17f;
	nap[2][2].y2 = -0.07f;
	nap[2][3].y2 = -0.28f;
	nap[2][4].y2 = -0.65f;

	nap[3][0].x2 = -1.05f;
	nap[3][1].x2 = -0.61f;
	nap[3][2].x2 = -0.75f;
	nap[3][3].x2 = -0.27f;
	nap[3][0].y2 = 0.37f;
	nap[3][1].y2 = 0.05f;
	nap[3][2].y2 = -0.19f;
	nap[3][3].y2 = -0.65f;

	nap[4][0].x2 = -0.73f;
	nap[4][1].x2 = -0.3f;
	nap[4][2].x2 = -0.27f;
	nap[4][0].y2 = 0.37f;
	nap[4][1].y2 = -0.07f;
	nap[4][2].y2 = -0.65f;

	nap[5][0].x2 = -0.55f;
	nap[5][1].x2 = -0.2f;
	nap[5][2].x2 = -0.27f;
	nap[5][0].y2 = 0.37f;
	nap[5][1].y2 = -0.07f;
	nap[5][2].y2 = -0.65f;
}

Structures::~Structures()
{
	SafeDelete(pozycja_sciany[0]);
	SafeDelete(pozycja_sciany[1]);
	SafeDelete(punkcior);
	SafeDelete(boxx);
	SafeDelete(klaw);
	
	for (INT i = 0; i < 4; i++)
	{
		SafeDelete(Potwor[i]);
		SafeDelete(Potwor[i]->M);
	}
		
	SafeDelete(poz_scian);
	SafeDelete(odleglosc);

	for(int ilo = 0; ilo < 10; ilo++)
		SafeDelete(Modele[ilo]);
	
	SafeDelete(StrzalaShot);
	SafeDelete(tab);
	SafeDelete(Czarny);
	SafeDelete(Czerwony);
	SafeDelete(Postac.M);
	SafeDelete(ShowArrowOnHedge);
	for(INT j = 0; j < IloElLab; j++)
		SafeDelete(ShowArrowOnMonster[j]);

	if (mAlphaEBlendState)
	{
		mAlphaEBlendState->Release();
		mAlphaEBlendState = 0;
	}

	if (mAlphaDBlendState)
	{
		mAlphaDBlendState->Release();
		mAlphaDBlendState = 0;
	}
}

void Structures::UstWszKl()
{
	ustaw = TRUE;

	for (INT ust = 0; ust < 31; ust++)
		ustawiono[ust] = FALSE;

	for (INT klawi = 0; klawi < KLA; klawi++)
	{
		if (klaw[klawi].ustawiony_klaw >= 1 && klaw[klawi].ustawiony_klaw < 31)
			ustawiono[klaw[klawi].ustawiony_klaw] = TRUE;
	}

	for (int ust = 1; ust < 31; ust++)
	{
		if (ustawiono[ust] && ustaw)
			ustaw = TRUE;

		else if (!ustawiono[ust])
			ustaw = FALSE;
	}
}

void Structures::InitStructures(ID3D11Device* device)
{
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&blendStateDesc, &mAlphaEBlendState));
	blendStateDesc.RenderTarget[0].BlendEnable = false;

	HR(device->CreateBlendState(&blendStateDesc, &mAlphaDBlendState));
}