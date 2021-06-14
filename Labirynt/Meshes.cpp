#include "Meshes.h"

Meshes::Meshes(HINSTANCE hInstance) : D3DApp(hInstance), mLightCount(3)
{
	//BOOL
	Postac.Animation = new BOOL[PersonAnimation];
	for (INT i = 0; i < PersonAnimation; i++)
		Postac.Animation[i] = FALSE;

	//INT
	Anim[0] = 0; Anim[1] = 16; Anim[2] = 32; Anim[3] = 47; Anim[4] = 63; Anim[5] = 82; Anim[6] = 98; Anim[7] = 0;

	ZywoplotT[0].x1 = 0.0f; ZywoplotT[1].x1 = 0.0f; ZywoplotT[2].x1 = 0.0f; ZywoplotT[3].x1 = 0.0f; ZywoplotT[0].y1 = 7.0f;
	ZywoplotT[0].z1 = 0.0f; ZywoplotT[1].z1 = 0.0f; ZywoplotT[2].z1 = 0.0f; ZywoplotT[3].z1 = 0.0f;

	for (INT i = 0; i < 13; i++)
		klawisze[i] = FALSE;

	for (INT i = 0; i < 4; i++)
		walks[i] = FALSE;
}

Meshes::~Meshes()
{
}

void Meshes::Init(ID3D11Device *device, ID3D11DeviceContext *dc, ThirdPersonCamera &camera, FLOAT width, FLOAT height)
{
	mTexMgr.Init(device);
	camera.CamProjection = XMMatrixPerspectiveFovLH(0.25f * XM_PI, width / height, 1.0f, 10000.0f);
	XMMATRIX Scale = XMMatrixScaling(0.0f, 0.0f, 0.0f);
	XMMATRIX RotX = XMMatrixRotationX(0.0f);
	XMMATRIX RotY = XMMatrixRotationY(0.0f);
	XMMATRIX RotZ = XMMatrixRotationZ(0.0f);
	XMMATRIX Translate = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	camera.playerCharWorld = Scale * RotY * RotX * RotZ * Translate;
	mSsao = new Ssao(device, dc, (INT)width, (INT)height, 1.0f, 10000.0f);

	AnglePostac.x1 = 1.35f;
	AnglePostac.y1 = 1.5f;
	AnglePostac.z1 = 0.5f;

	Postac.Ins = new SkinnedModelInstance[PersonAnimation];

	for (INT jj = 0; jj < 4; jj++)
	{
		for (INT i = 0; i < IloElLab; i++)
			Potwor[jj][i].Ins = new SkinnedModelInstance[MonstersAnimation];
		for (INT j = 0; j < 10; j++)
		{
			Potwor[jj]->M[j] = new SkinnedModel(device, mTexMgr, "..\\Release\\SkeletalModels\\" + to_string(jj) + "\\" + to_string(j) + ".m3d", L"..\\Release\\SkeletalModels\\");
			for (INT i = 0; i < IloElLab; i++)
			{
				Potwor[jj][i].Ins[j].Model = Potwor[jj]->M[j];
				Potwor[jj][i].Ins[j].TimePos = 0.0f;
				Potwor[jj][i].Ins[j].ClipName = "MD5Action";
				Potwor[jj][i].Ins[j].FinalTransforms.resize(Potwor[jj]->M[j]->SkinnedData.BoneCount());
			}
		}
	}

	for (INT i = 0; i < 128; i++)
	{
		Postac.M[i] = new SkinnedModel(device, mTexMgr, "..\\Release\\SkeletalModels\\postac\\" + to_string(i) + ".m3d", L"..\\Release\\SkeletalModels\\postac\\");

		Postac.Ins[i].Model = Postac.M[i];
		Postac.Ins[i].TimePos = 0.0f;
		Postac.Ins[i].ClipName = "MD5Action";
		Postac.Ins[i].FinalTransforms.resize(Postac.M[i]->SkinnedData.BoneCount());
	}

	for (INT i = 0; i < Znaki; i++)
	{
		Czarny[i] = new Models[IloLiter];
		Czerwony[i] = new Models[IloLiter];

		Czarny[i]->M = new BasicModel(device, mTexMgr, "..\\Release\\Alfabet\\Algiers\\Czarny\\" + to_string(i) + ".m3d", L"..\\Release\\Alfabet\\");
		Czerwony[i]->M = new BasicModel(device, mTexMgr, "..\\Release\\Alfabet\\Algiers\\Czerwony\\" + to_string(i) + ".m3d", L"..\\Release\\Alfabet\\");

		for (INT iloi = 0; iloi < IloLiter; iloi++)
		{
			Czarny[i][iloi].Model = Czarny[i]->M;
			Czerwony[i][iloi].Model = Czerwony[i]->M;
		}
	}

	Modele[0]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\zywoplot\\kaw_lab\\kaw_lab.m3d", L"..\\Release\\StaticModels\\zywoplot\\kaw_lab\\");
	Modele[1]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\zywoplot\\rog_lab\\rog_lab.m3d", L"..\\Release\\StaticModels\\zywoplot\\rog_lab\\");
	Modele[2]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\zywoplot\\t_lab\\t_lab.m3d", L"..\\Release\\StaticModels\\zywoplot\\t_lab\\");
	Modele[3]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\zywoplot\\krzyzyk_lab\\krzyzyk_lab.m3d", L"..\\Release\\StaticModels\\zywoplot\\krzyzyk_lab\\");
	Modele[4]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\luk\\luk.m3d", L"..\\Release\\StaticModels\\luk\\");
	Modele[5]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\strzala\\strzala.m3d", L"..\\Release\\StaticModels\\strzala\\");
	Modele[6]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\miecz\\miecz.m3d", L"..\\Release\\StaticModels\\miecz\\");
	Modele[7]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\mieczyk\\mieczyk.m3d", L"..\\Release\\StaticModels\\mieczyk\\");
	Modele[8]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\moneta\\moneta.m3d", L"..\\Release\\StaticModels\\moneta\\");
	Modele[9]->M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\diament\\diament.m3d", L"..\\Release\\StaticModels\\diament\\");
	Podloze.M = new BasicModel(device, mTexMgr, "..\\Release\\StaticModels\\podloze\\podloze.m3d", L"..\\Release\\StaticModels\\podloze\\");

	Podloze.Model = Podloze.M;
	StrzalaShoting.Model = Modele[5]->M;

	for (INT ilo = 0; ilo < 10; ilo++)
	{
		for (INT i = 0; i < IloElLab; i++)
			Modele[ilo][i].Model = Modele[ilo]->M;
	}

	for (INT i = 0; i < MaxCountArrows; i++)
		StrzalaShot[i].Model = Modele[5]->M;

	Czarny[0]->nazwa_klawisza[0] = Czerwony[0]->nazwa_klawisza[0] = '#';
	Czarny[1]->nazwa_klawisza[0] = Czerwony[1]->nazwa_klawisza[0] = '$';
	Czarny[2]->nazwa_klawisza[0] = Czerwony[2]->nazwa_klawisza[0] = '%';
	Czarny[3]->nazwa_klawisza[0] = Czerwony[3]->nazwa_klawisza[0] = '&';
	Czarny[4]->nazwa_klawisza[0] = Czerwony[4]->nazwa_klawisza[0] = '(';
	Czarny[5]->nazwa_klawisza[0] = Czerwony[5]->nazwa_klawisza[0] = ')';
	Czarny[6]->nazwa_klawisza[0] = Czerwony[6]->nazwa_klawisza[0] = ',';
	Czarny[7]->nazwa_klawisza[0] = Czerwony[7]->nazwa_klawisza[0] = '\'';
	Czarny[8]->nazwa_klawisza[0] = Czerwony[8]->nazwa_klawisza[0] = '-';
	Czarny[9]->nazwa_klawisza[0] = Czerwony[9]->nazwa_klawisza[0] = ';';
	Czarny[10]->nazwa_klawisza[0] = Czerwony[10]->nazwa_klawisza[0] = '@';
	Czarny[11]->nazwa_klawisza[0] = Czerwony[11]->nazwa_klawisza[0] = '[';
	Czarny[12]->nazwa_klawisza[0] = Czerwony[12]->nazwa_klawisza[0] = ']';
	Czarny[13]->nazwa_klawisza[0] = Czerwony[13]->nazwa_klawisza[0] = '^';
	Czarny[14]->nazwa_klawisza[0] = Czerwony[14]->nazwa_klawisza[0] = '_';
	Czarny[15]->nazwa_klawisza[0] = Czerwony[15]->nazwa_klawisza[0] = '`';
	Czarny[16]->nazwa_klawisza[0] = Czerwony[16]->nazwa_klawisza[0] = '{';
	Czarny[17]->nazwa_klawisza[0] = Czerwony[17]->nazwa_klawisza[0] = '}';
	Czarny[18]->nazwa_klawisza[0] = Czerwony[18]->nazwa_klawisza[0] = '~';
	Czarny[19]->nazwa_klawisza[0] = Czerwony[19]->nazwa_klawisza[0] = '+';
	Czarny[20]->nazwa_klawisza[0] = Czerwony[20]->nazwa_klawisza[0] = '=';
	Czarny[21]->nazwa_klawisza[0] = Czerwony[21]->nazwa_klawisza[0] = '0';
	Czarny[22]->nazwa_klawisza[0] = Czerwony[22]->nazwa_klawisza[0] = '1';
	Czarny[23]->nazwa_klawisza[0] = Czerwony[23]->nazwa_klawisza[0] = '2';
	Czarny[24]->nazwa_klawisza[0] = Czerwony[24]->nazwa_klawisza[0] = '3';
	Czarny[25]->nazwa_klawisza[0] = Czerwony[25]->nazwa_klawisza[0] = '4';
	Czarny[26]->nazwa_klawisza[0] = Czerwony[26]->nazwa_klawisza[0] = '5';
	Czarny[27]->nazwa_klawisza[0] = Czerwony[27]->nazwa_klawisza[0] = '6';
	Czarny[28]->nazwa_klawisza[0] = Czerwony[28]->nazwa_klawisza[0] = '7';
	Czarny[29]->nazwa_klawisza[0] = Czerwony[29]->nazwa_klawisza[0] = '8';
	Czarny[30]->nazwa_klawisza[0] = Czerwony[30]->nazwa_klawisza[0] = '9';
	Czarny[31]->nazwa_klawisza[0] = Czerwony[31]->nazwa_klawisza[0] = 'A';	Czarny[31]->nazwa_klawisza[1] = Czerwony[31]->nazwa_klawisza[1] = 'a';
	Czarny[32]->nazwa_klawisza[0] = Czerwony[32]->nazwa_klawisza[0] = '•';	Czarny[32]->nazwa_klawisza[1] = Czerwony[32]->nazwa_klawisza[1] = 'π';
	Czarny[33]->nazwa_klawisza[0] = Czerwony[33]->nazwa_klawisza[0] = 'B';	Czarny[33]->nazwa_klawisza[1] = Czerwony[33]->nazwa_klawisza[1] = 'b';
	Czarny[34]->nazwa_klawisza[0] = Czerwony[34]->nazwa_klawisza[0] = '\\';
	Czarny[35]->nazwa_klawisza[0] = Czerwony[35]->nazwa_klawisza[0] = 'C';	Czarny[35]->nazwa_klawisza[1] = Czerwony[35]->nazwa_klawisza[1] = 'c';
	Czarny[36]->nazwa_klawisza[0] = Czerwony[36]->nazwa_klawisza[0] = '∆';	Czarny[36]->nazwa_klawisza[1] = Czerwony[36]->nazwa_klawisza[1] = 'Ê';
	Czarny[37]->nazwa_klawisza[0] = Czerwony[37]->nazwa_klawisza[0] = '"';
	Czarny[38]->nazwa_klawisza[0] = Czerwony[38]->nazwa_klawisza[0] = 'D';	Czarny[38]->nazwa_klawisza[1] = Czerwony[38]->nazwa_klawisza[1] = 'd';
	Czarny[39]->nazwa_klawisza[0] = Czerwony[39]->nazwa_klawisza[0] = ':';
	Czarny[40]->nazwa_klawisza[0] = Czerwony[40]->nazwa_klawisza[0] = 'E';	Czarny[40]->nazwa_klawisza[1] = Czerwony[40]->nazwa_klawisza[1] = 'e';
	Czarny[41]->nazwa_klawisza[0] = Czerwony[41]->nazwa_klawisza[0] = ' ';	Czarny[41]->nazwa_klawisza[1] = Czerwony[41]->nazwa_klawisza[1] = 'Í';
	Czarny[42]->nazwa_klawisza[0] = Czerwony[42]->nazwa_klawisza[0] = 'F';	Czarny[42]->nazwa_klawisza[1] = Czerwony[42]->nazwa_klawisza[1] = 'f';
	Czarny[43]->nazwa_klawisza[0] = Czerwony[43]->nazwa_klawisza[0] = 'G';	Czarny[43]->nazwa_klawisza[1] = Czerwony[43]->nazwa_klawisza[1] = 'g';
	Czarny[44]->nazwa_klawisza[0] = Czerwony[44]->nazwa_klawisza[0] = '*';
	Czarny[45]->nazwa_klawisza[0] = Czerwony[45]->nazwa_klawisza[0] = 'H';	Czarny[45]->nazwa_klawisza[1] = Czerwony[45]->nazwa_klawisza[1] = 'h';
	Czarny[46]->nazwa_klawisza[0] = Czerwony[46]->nazwa_klawisza[0] = 'I';	Czarny[46]->nazwa_klawisza[1] = Czerwony[46]->nazwa_klawisza[1] = 'i';
	Czarny[47]->nazwa_klawisza[0] = Czerwony[47]->nazwa_klawisza[0] = 'J';	Czarny[47]->nazwa_klawisza[1] = Czerwony[47]->nazwa_klawisza[1] = 'j';
	Czarny[48]->nazwa_klawisza[0] = Czerwony[48]->nazwa_klawisza[0] = 'K';	Czarny[48]->nazwa_klawisza[1] = Czerwony[48]->nazwa_klawisza[1] = 'k';
	Czarny[49]->nazwa_klawisza[0] = Czerwony[49]->nazwa_klawisza[0] = '.';
	Czarny[50]->nazwa_klawisza[0] = Czerwony[50]->nazwa_klawisza[0] = 'L';	Czarny[50]->nazwa_klawisza[1] = Czerwony[50]->nazwa_klawisza[1] = 'l';
	Czarny[51]->nazwa_klawisza[0] = Czerwony[51]->nazwa_klawisza[0] = '£';	Czarny[51]->nazwa_klawisza[1] = Czerwony[51]->nazwa_klawisza[1] = '≥';
	Czarny[52]->nazwa_klawisza[0] = Czerwony[52]->nazwa_klawisza[0] = 'M';	Czarny[52]->nazwa_klawisza[1] = Czerwony[52]->nazwa_klawisza[1] = 'm';
	Czarny[53]->nazwa_klawisza[0] = Czerwony[53]->nazwa_klawisza[0] = '<';
	Czarny[54]->nazwa_klawisza[0] = Czerwony[54]->nazwa_klawisza[0] = 'N';	Czarny[54]->nazwa_klawisza[1] = Czerwony[54]->nazwa_klawisza[1] = 'n';
	Czarny[55]->nazwa_klawisza[0] = Czerwony[55]->nazwa_klawisza[0] = '—';	Czarny[55]->nazwa_klawisza[1] = Czerwony[55]->nazwa_klawisza[1] = 'Ò';
	Czarny[56]->nazwa_klawisza[0] = Czerwony[56]->nazwa_klawisza[0] = 'O';	Czarny[56]->nazwa_klawisza[1] = Czerwony[56]->nazwa_klawisza[1] = 'o';
	Czarny[57]->nazwa_klawisza[0] = Czerwony[57]->nazwa_klawisza[0] = '”';	Czarny[57]->nazwa_klawisza[1] = Czerwony[57]->nazwa_klawisza[1] = 'Û';
	Czarny[58]->nazwa_klawisza[0] = Czerwony[58]->nazwa_klawisza[0] = 'P';	Czarny[58]->nazwa_klawisza[1] = Czerwony[58]->nazwa_klawisza[1] = 'p';
	Czarny[59]->nazwa_klawisza[0] = Czerwony[59]->nazwa_klawisza[0] = '|';
	Czarny[60]->nazwa_klawisza[0] = Czerwony[60]->nazwa_klawisza[0] = '?';
	Czarny[61]->nazwa_klawisza[0] = Czerwony[61]->nazwa_klawisza[0] = 'Q';	Czarny[61]->nazwa_klawisza[1] = Czerwony[61]->nazwa_klawisza[1] = 'q';
	Czarny[62]->nazwa_klawisza[0] = Czerwony[62]->nazwa_klawisza[0] = 'R';	Czarny[62]->nazwa_klawisza[1] = Czerwony[62]->nazwa_klawisza[1] = 'r';
	Czarny[63]->nazwa_klawisza[0] = Czerwony[63]->nazwa_klawisza[0] = 'S';	Czarny[63]->nazwa_klawisza[1] = Czerwony[63]->nazwa_klawisza[1] = 's';
	Czarny[64]->nazwa_klawisza[0] = Czerwony[64]->nazwa_klawisza[0] = 'å';	Czarny[64]->nazwa_klawisza[1] = Czerwony[64]->nazwa_klawisza[1] = 'ú';
	Czarny[65]->nazwa_klawisza[0] = Czerwony[65]->nazwa_klawisza[0] = '/';
	Czarny[66]->nazwa_klawisza[0] = Czerwony[66]->nazwa_klawisza[0] = 'T';	Czarny[66]->nazwa_klawisza[1] = Czerwony[66]->nazwa_klawisza[1] = 't';
	Czarny[67]->nazwa_klawisza[0] = Czerwony[67]->nazwa_klawisza[0] = 'U';	Czarny[67]->nazwa_klawisza[1] = Czerwony[67]->nazwa_klawisza[1] = 'u';
	Czarny[68]->nazwa_klawisza[0] = Czerwony[68]->nazwa_klawisza[0] = 'V';	Czarny[68]->nazwa_klawisza[1] = Czerwony[68]->nazwa_klawisza[1] = 'v';
	Czarny[69]->nazwa_klawisza[0] = Czerwony[69]->nazwa_klawisza[0] = 'W';	Czarny[69]->nazwa_klawisza[1] = Czerwony[69]->nazwa_klawisza[1] = 'w';
	Czarny[70]->nazwa_klawisza[0] = Czerwony[70]->nazwa_klawisza[0] = '>';
	Czarny[71]->nazwa_klawisza[0] = Czerwony[71]->nazwa_klawisza[0] = '!';
	Czarny[72]->nazwa_klawisza[0] = Czerwony[72]->nazwa_klawisza[0] = 'X';	Czarny[72]->nazwa_klawisza[1] = Czerwony[72]->nazwa_klawisza[1] = 'x';
	Czarny[73]->nazwa_klawisza[0] = Czerwony[73]->nazwa_klawisza[0] = 'Y';	Czarny[73]->nazwa_klawisza[1] = Czerwony[73]->nazwa_klawisza[1] = 'y';
	Czarny[74]->nazwa_klawisza[0] = Czerwony[74]->nazwa_klawisza[0] = 'Z';	Czarny[74]->nazwa_klawisza[1] = Czerwony[74]->nazwa_klawisza[1] = 'z';
	Czarny[75]->nazwa_klawisza[0] = Czerwony[75]->nazwa_klawisza[0] = 'è';	Czarny[75]->nazwa_klawisza[1] = Czerwony[75]->nazwa_klawisza[1] = 'ü';
	Czarny[76]->nazwa_klawisza[0] = Czerwony[76]->nazwa_klawisza[0] = 'Ø';	Czarny[76]->nazwa_klawisza[1] = Czerwony[76]->nazwa_klawisza[1] = 'ø';
}

void Meshes::DestroyAll()
{
	for (INT j = 0; j < 4; j++)
	{
		for (INT monster = 0; monster < MonstersAnimation; monster++)
			SafeDelete(Potwor[j]->M[monster]);

		for (INT i = 0; i < IloElLab; i++)
		{
			SafeDelete(Potwor[j][i].M);
			SafeDelete(Potwor[j][i].Ins);
			SafeDelete(Potwor[j][i].Animation);
		}
	}

	for (INT ilo = 0; ilo < 10; ilo++)
	{
		for (INT i = 0; i < IloElLab; i++)
			SafeDelete(Modele[ilo][i].Model);
	}

	for (INT i = 0; i < MaxCountArrows; i++)
		SafeDelete(StrzalaShot[i].Model);

	SafeDelete(mSsao);
	SafeDelete(Podloze.M);
	SafeDelete(StrzalaShoting.M);

	for (INT ilo = 0; ilo < 10; ilo++)
		SafeDelete(Modele[ilo]->M);

	for (INT j = 0; j < 4; j++)
		SafeDelete(Potwor[j]);

	SafeDelete(Postac.Ins);
	SafeDelete(Postac.M);

	for (INT j = 0; j < MaxCountArrows; j++)
	{
		SafeDelete(CTrajShot[j]);
		SafeDelete(yyTrajShot[j]);
		SafeDelete(ppTrajShot[j]);
		SafeDelete(AngleXTrajShot[j]);
	}

	SafeDelete(CTrajShotHedge);
	SafeDelete(yyTrajShotHedge);
	SafeDelete(ppTrajShotHedge);
	SafeDelete(AngleXTrajShotHedge);
}

void Meshes::CollisionArrowWithHedge(ThirdPersonCamera &camera)
{
	for (INT ilo = 0; ilo < IloElLab; ilo++)
	{
		if (tab[ilo] == 1)
		{
			if (!ShootingArrow)
			{
				if ((CTrajShoting.x1 >= boxx[ilo].x1 - 10.0f) &&
					(CTrajShoting.x1 <= boxx[ilo].x1 + 10.0f) &&
					(CTrajShoting.z1 >= boxx[ilo].z1 - 9.0f) &&
					(CTrajShoting.z1 <= boxx[ilo].z1 - 7.0f))
				{
					CTrajShoting.z1 -= 2.0f;
					ShowArrowOnHedge[trafiony] = TRUE;
					CTrajShotHedge[trafiony].x1 = CTrajShoting.x1;
					CTrajShotHedge[trafiony].z1 = CTrajShoting.z1;
					yyTrajShotHedge[trafiony] = TrajShoting.y1;
					ppTrajShotHedge[trafiony] = ppTrajShoting;
					AngleXTrajShotHedge[trafiony] = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;
					trafiony++;
					if (trafiony >= MaxCountArrows - 2)
					{
						for (INT si = 0; si < MaxCountArrows; si++)
						{
							if (ShowArrowOnHedge[si] == TRUE)
								ShowArrowOnHedge[si] == FALSE;
						}
						trafiony = 0;
					}
					ShootingArrow = TRUE;
					TrajShoting.x1 = ixic;
					Shooting = FALSE;
				}

				else if ((CTrajShoting.x1 >= boxx[ilo].x1 - 10.0f) &&
					(CTrajShoting.x1 <= boxx[ilo].x1 + 10.0f) &&
					(CTrajShoting.z1 <= boxx[ilo].z1 + 9.0f) &&
					(CTrajShoting.z1 >= boxx[ilo].z1 + 7.0f))
				{
					CTrajShoting.z1 += 2.0f;
					ShowArrowOnHedge[trafiony] = TRUE;
					CTrajShotHedge[trafiony].x1 = CTrajShoting.x1;
					CTrajShotHedge[trafiony].z1 = CTrajShoting.z1;
					yyTrajShotHedge[trafiony] = TrajShoting.y1;
					ppTrajShotHedge[trafiony] = ppTrajShoting;
					AngleXTrajShotHedge[trafiony] = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;
					trafiony++;
					if (trafiony >= MaxCountArrows - 2)
					{
						for (INT si = 0; si < MaxCountArrows; si++)
						{
							if (ShowArrowOnHedge[si] == TRUE)
								ShowArrowOnHedge[si] == FALSE;
						}
						trafiony = 0;
					}
					ShootingArrow = TRUE;
					TrajShoting.x1 = ixic;
					Shooting = FALSE;
				}

				else if ((CTrajShoting.z1 >= boxx[ilo].z1 - 10.0f) &&
					(CTrajShoting.z1 <= boxx[ilo].z1 + 10.0f) &&
					(CTrajShoting.x1 >= boxx[ilo].x1 - 9.0f) &&
					(CTrajShoting.x1 <= boxx[ilo].x1 - 7.0f))
				{
					CTrajShoting.x1 -= 2.0f;
					ShowArrowOnHedge[trafiony] = TRUE;
					CTrajShotHedge[trafiony].x1 = CTrajShoting.x1;
					CTrajShotHedge[trafiony].z1 = CTrajShoting.z1;
					yyTrajShotHedge[trafiony] = TrajShoting.y1;
					ppTrajShotHedge[trafiony] = ppTrajShoting;
					AngleXTrajShotHedge[trafiony] = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;
					trafiony++;
					if (trafiony >= MaxCountArrows - 2)
					{
						for (INT si = 0; si < MaxCountArrows; si++)
						{
							if (ShowArrowOnHedge[si] == TRUE)
								ShowArrowOnHedge[si] == FALSE;
						}
						trafiony = 0;
					}
					ShootingArrow = TRUE;
					TrajShoting.x1 = ixic;
					Shooting = FALSE;
				}

				else if ((CTrajShoting.z1 >= boxx[ilo].z1 - 10.0f) &&
					(CTrajShoting.z1 <= boxx[ilo].z1 + 10.0f) &&
					(CTrajShoting.x1 <= boxx[ilo].x1 + 9.0f) &&
					(CTrajShoting.x1 >= boxx[ilo].x1 + 7.0))
				{
					CTrajShoting.x1 += 2.0f;
					ShowArrowOnHedge[trafiony] = TRUE;
					CTrajShotHedge[trafiony].x1 = CTrajShoting.x1;
					CTrajShotHedge[trafiony].z1 = CTrajShoting.z1;
					yyTrajShotHedge[trafiony] = TrajShoting.y1;
					ppTrajShotHedge[trafiony] = ppTrajShoting;
					AngleXTrajShotHedge[trafiony] = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;
					trafiony++;
					if (trafiony >= MaxCountArrows - 2)
					{
						for (INT si = 0; si < MaxCountArrows; si++)
						{
							if (ShowArrowOnHedge[si] == TRUE)
								ShowArrowOnHedge[si] == FALSE;
						}
						trafiony = 0;
					}
					ShootingArrow = TRUE;
					TrajShoting.x1 = ixic;
					Shooting = FALSE;
				}
			}
		}
	}
}

void Meshes::CollisionArrowWithMonster(INT model, ThirdPersonCamera &camera, SkinningModels *Potwor)
{
	for (INT ilo = 0; ilo < IloElLab; ilo++)
	{
		if (tab[ilo] == model)
		{
			if (!ShootingArrow)
			{
				if ((CTrajShoting.x1 >= Potwor[ilo].Pozycja[0].x1 - 0.5f) &&
					(CTrajShoting.x1 <= Potwor[ilo].Pozycja[0].x1 + 0.5f) &&
					(CTrajShoting.z1 >= Potwor[ilo].Pozycja[0].z1 - 1.5f) &&
					(CTrajShoting.z1 <= Potwor[ilo].Pozycja[0].z1 - 1.0f))
				{
					CTrajShoting.z1 -= 0.25f;
					Potwor[ilo].trafienie = TRUE;
					Potwor[ilo].traf++;
					ShowArrowOnMonster[ilo][trafiony] = TRUE;
					CTrajShot[ilo][trafiony].x1 = CTrajShoting.x1;
					CTrajShot[ilo][trafiony].z1 = CTrajShoting.z1;
					yyTrajShot[ilo][trafiony] = TrajShoting.y1;
					ppTrajShot[ilo][trafiony] = ppTrajShoting;
					AngleXTrajShot[ilo][trafiony] = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;
					trafiony++;
					if (trafiony >= MaxCountArrows - 2)
					{
						for (INT si = 0; si < MaxCountArrows; si++)
						{
							if (ShowArrowOnMonster[ilo][si] == TRUE)
								ShowArrowOnMonster[ilo][si] == FALSE;
						}
						trafiony = 0;
					}
					ShootingArrow = TRUE;
					TrajShoting.x1 = ixic;
					Shooting = FALSE;
				}

				else if ((CTrajShoting.x1 >= Potwor[ilo].Pozycja[0].x1 - 0.5f) &&
					(CTrajShoting.x1 <= Potwor[ilo].Pozycja[0].x1 + 0.5f) &&
					(CTrajShoting.z1 >= Potwor[ilo].Pozycja[0].z1 + 1.0f) &&
					(CTrajShoting.z1 <= Potwor[ilo].Pozycja[0].z1 + 1.5f))
				{
					CTrajShoting.z1 += 0.25f;
					Potwor[ilo].trafienie = TRUE;
					Potwor[ilo].traf++;
					ShowArrowOnMonster[ilo][trafiony] = TRUE;
					CTrajShot[ilo][trafiony].x1 = CTrajShoting.x1;
					CTrajShot[ilo][trafiony].z1 = CTrajShoting.z1;
					yyTrajShot[ilo][trafiony] = TrajShoting.y1;
					ppTrajShot[ilo][trafiony] = ppTrajShoting;
					AngleXTrajShot[ilo][trafiony] = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;
					trafiony++;
					if (trafiony >= MaxCountArrows - 2)
					{
						for (INT si = 0; si < MaxCountArrows; si++)
						{
							if (ShowArrowOnMonster[ilo][si] == TRUE)
								ShowArrowOnMonster[ilo][si] == FALSE;
						}
						trafiony = 0;
					}
					ShootingArrow = TRUE;
					TrajShoting.x1 = ixic;
					Shooting = FALSE;
				}

				else if ((CTrajShoting.z1 >= Potwor[ilo].Pozycja[0].z1 - 0.5f) &&
					(CTrajShoting.z1 <= Potwor[ilo].Pozycja[0].z1 + 0.5f) &&
					(CTrajShoting.x1 >= Potwor[ilo].Pozycja[0].x1 - 1.5f) &&
					(CTrajShoting.x1 <= Potwor[ilo].Pozycja[0].x1 - 1.0f))
				{
					CTrajShoting.x1 -= 0.25f;
					Potwor[ilo].trafienie = TRUE;
					Potwor[ilo].traf++;
					ShowArrowOnMonster[ilo][trafiony] = TRUE;
					CTrajShot[ilo][trafiony].x1 = CTrajShoting.x1;
					CTrajShot[ilo][trafiony].z1 = CTrajShoting.z1;
					yyTrajShot[ilo][trafiony] = TrajShoting.y1;
					ppTrajShot[ilo][trafiony] = ppTrajShoting;
					AngleXTrajShot[ilo][trafiony] = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;
					trafiony++;
					if (trafiony >= MaxCountArrows - 2)
					{
						for (INT si = 0; si < MaxCountArrows; si++)
						{
							if (ShowArrowOnMonster[ilo][si] == TRUE)
								ShowArrowOnMonster[ilo][si] == FALSE;
						}
						trafiony = 0;
					}
					ShootingArrow = TRUE;
					TrajShoting.x1 = ixic;
					Shooting = FALSE;
				}

				else if ((CTrajShoting.z1 >= Potwor[ilo].Pozycja[0].z1 - 0.5f) &&
					(CTrajShoting.z1 <= Potwor[ilo].Pozycja[0].z1 + 0.5f) &&
					(CTrajShoting.x1 >= Potwor[ilo].Pozycja[0].x1 + 1.0f) &&
					(CTrajShoting.x1 <= Potwor[ilo].Pozycja[0].x1 + 1.5f))
				{
					CTrajShoting.x1 += 0.25f;
					Potwor[ilo].trafienie = TRUE;
					Potwor[ilo].traf++;
					ShowArrowOnMonster[ilo][trafiony] = TRUE;
					CTrajShot[ilo][trafiony].x1 = CTrajShoting.x1;
					CTrajShot[ilo][trafiony].z1 = CTrajShoting.z1;
					yyTrajShot[ilo][trafiony] = TrajShoting.y1;
					ppTrajShot[ilo][trafiony] = ppTrajShoting;
					AngleXTrajShot[ilo][trafiony] = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;
					trafiony++;
					if (trafiony >= MaxCountArrows - 2)
					{
						for (INT si = 0; si < MaxCountArrows; si++)
						{
							if (ShowArrowOnMonster[ilo][si] == TRUE)
								ShowArrowOnMonster[ilo][si] == FALSE;
						}
						trafiony = 0;
					}
					ShootingArrow = TRUE;
					TrajShoting.x1 = ixic;
					Shooting = FALSE;
				}
			}
		}
	}
}

void Meshes::Animations(FLOAT dt, ThirdPersonCamera &camera, INT Animacja, INT Stojka, INT JogStart, INT Jogging, INT JogStop, INT Run, INT Jump, INT Punch, INT Kick, INT JumpKick, INT JumpToForward, INT KickZPolobrotu, INT JumpKickPunch, INT FlySpinKick, INT Oberwanie, INT Upadek, INT Boxing, INT po)
{
	//przÛd || tyl || lewo || prawo || przÛd i lewo || przÛd i prawo || tyl i lewo || ty≥ i prawo
	if ((klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) || (!klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) || (!klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) || (klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) || (!klawisze[0] && klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		if (Anim[Animacja] == Stojka || (Anim[Animacja] == JogStop && Postac.Ins[JogStop].TimePos >= Postac.Ins[JogStop].Model->SkinnedData.GetClipEndTime(Postac.Ins[JogStop].ClipName)))
		{
			camera.SpeedCamera = 3.0f;
			if (!Postac.Animation[JogStart])
			{
				Postac.Ins[JogStart].TimePos = 0.0f;
				Postac.Animation[JogStart] = TRUE;
			}
			Anim[Animacja] = JogStart;
			box = 0;
		}

		else if ((Anim[Animacja] == JogStart && Postac.Ins[JogStart].TimePos >= Postac.Ins[JogStart].Model->SkinnedData.GetClipEndTime(Postac.Ins[JogStart].ClipName)) ||
			Anim[Animacja] == Run ||
			(Anim[Animacja] == JumpToForward && Postac.Ins[JumpToForward].TimePos >= Postac.Ins[JumpToForward].Model->SkinnedData.GetClipEndTime(Postac.Ins[JumpToForward].ClipName) - 1.0f) ||
			(Anim[Animacja] == JumpKick && Postac.Ins[JumpKick].TimePos >= Postac.Ins[JumpKick].Model->SkinnedData.GetClipEndTime(Postac.Ins[JumpKick].ClipName)) ||
			(Anim[Animacja] == Punch && Postac.Ins[Punch].TimePos >= Postac.Ins[Punch].Model->SkinnedData.GetClipEndTime(Postac.Ins[Punch].ClipName)) ||
			(Anim[Animacja] == Boxing && Postac.Ins[Boxing].TimePos >= Postac.Ins[Boxing].Model->SkinnedData.GetClipEndTime(Postac.Ins[Boxing].ClipName)) ||
			(Anim[Animacja] == Kick && Postac.Ins[Kick].TimePos >= Postac.Ins[Kick].Model->SkinnedData.GetClipEndTime(Postac.Ins[Kick].ClipName)) ||
			(Anim[Animacja] == JumpKickPunch && Postac.Ins[JumpKickPunch].TimePos >= Postac.Ins[JumpKickPunch].Model->SkinnedData.GetClipEndTime(Postac.Ins[JumpKickPunch].ClipName)) ||
			(Anim[Animacja] == KickZPolobrotu && Postac.Ins[KickZPolobrotu].TimePos >= Postac.Ins[KickZPolobrotu].Model->SkinnedData.GetClipEndTime(Postac.Ins[KickZPolobrotu].ClipName)) ||
			(Anim[Animacja] == FlySpinKick && Postac.Ins[FlySpinKick].TimePos >= Postac.Ins[FlySpinKick].Model->SkinnedData.GetClipEndTime(Postac.Ins[FlySpinKick].ClipName)))
		{
			camera.SpeedCamera = 8.0f;
			if (!Postac.Animation[Jogging])
			{
				Postac.Ins[Jogging].TimePos = 0.0f;
				Postac.Animation[Jogging] = TRUE;
			}
			Anim[Animacja] = Jogging;
			Anim[7] = 3;
			box = 0;
		}

		for (int i = 0; i < PersonAnimation; i++)
		{
			if (i != 1 && i != 2 && i != 17 && i != 18 && i != 33 && i != 34 && i != 48 && i != 49 && i != 64 && i != 65 && i != 83 && i != 84 && i != 99 && i != 100)
				Postac.Animation[i] = FALSE;
		}
	}

	////kick z pÛ≥obrotu przÛd ||kick z pÛ≥obrotu tyl ||kick z pÛ≥obrotu lewo ||kick z pÛ≥obrotu prawo ||kick z pÛ≥obrotu przÛd i lewo ||kick z pÛ≥obrotu przÛd i prawo ||kick z pÛ≥obrotu tyl i lewo ||kick z pÛ≥obrotu ty≥ i prawo
	else if ((walks[0] && walks[2] && !walks[3] && !walks[1] && klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(walks[0] && walks[2] && !walks[3] && !walks[1] && !klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(walks[0] && !walks[2] && walks[3] && !walks[1] && klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(walks[0] && !walks[2] && walks[3] && !walks[1] && !klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!walks[0] && walks[2] && !walks[3] && walks[1] && !klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!walks[0] && walks[2] && !walks[3] && walks[1] && !klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!walks[0] && !walks[2] && walks[3] && walks[1] && !klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!walks[0] && !walks[2] && walks[3] && walks[1] && !klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]))
	{
		if (Postac.Animation[JumpToForward])
		{
			camera.SpeedCamera = 1.0f;
			if (!Postac.Animation[FlySpinKick])
			{
				Postac.Ins[FlySpinKick].TimePos = 0.0f;
				Postac.Animation[FlySpinKick] = TRUE;
			}
			Anim[Animacja] = FlySpinKick;
			box = 0;
		}
		else if (!Postac.Animation[JumpToForward])
		{
			camera.SpeedCamera = 1.0f;
			if (!Postac.Animation[KickZPolobrotu])
			{
				Postac.Ins[KickZPolobrotu].TimePos = 0.0f;
				Postac.Animation[KickZPolobrotu] = TRUE;
			}
			Anim[Animacja] = KickZPolobrotu;
			box = 0;
		}

		for (int i = 0; i < PersonAnimation; i++)
		{
			if (i != 9 && i != 10 && i != 12 && i != 25 && i != 26 && i != 28 && i != 41 && i != 42 && i != 43 && i != 56 && i != 57 && i != 59 && i != 72 && i != 73 && i != 75 && i != 91 && i != 92 && i != 94 && i != 107 && i != 108 && i != 109)
				Postac.Animation[i] = FALSE;
		}
	}

	//skok w przÛd || skok w tyl || skok w lewo || skok w prawo || skok w przÛd i w lewo | skok w przÛd i w prawo || skok w tyl i w lewo || skok w ty≥ i w prawo
	else if ((klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		camera.SpeedCamera = 10.0f;
		if (!Postac.Animation[JumpToForward])
		{
			Postac.Ins[JumpToForward].TimePos = 0.0f;
			Postac.Animation[JumpToForward] = TRUE;
		}
		Anim[Animacja] = JumpToForward;
		box = 0;

		for (int i = 0; i < PersonAnimation; i++)
		{
			if (i != 9 && i != 25 && i != 41 && i != 56 && i != 72 && i != 91 && i != 107)
				Postac.Animation[i] = FALSE;
		}
	}

	//bieg w przÛd || bieg w tyl || bieg w lewo || bieg w prawo || bieg w przÛd i w lewo || bieg w przÛd  i w prawo || bieg w ty≥ i w lewo || bieg w ty≥ i w prawo 
	else if ((klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && klawisze[2] && !klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && klawisze[3] && klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7]))
	{
		camera.SpeedCamera = 16.0f;
		if (Anim[Animacja] == Jogging)
		{
			if (!Postac.Animation[Run])
			{
				Postac.Ins[Run].TimePos = 0.0f;
				Postac.Animation[Run] = TRUE;
			}
		}
		Anim[Animacja] = Run;
		box = 0;

		for (int i = 0; i < PersonAnimation; i++)
		{
			if (i != 4 && i != 20 && i != 36 && i != 51 && i != 67 && i != 86 && i != 102)
				Postac.Animation[i] = FALSE;
		}
	}

	//JumpKick w przÛd || JumpKick w tyl || JumpKick w lewo || JumpKick w prawo || JumpKick w przÛd i w lewo || JumpKick w przÛd  i w prawo || JumpKick w ty≥ i w lewo || JumpKick w ty≥ i w prawo 
	else if ((!klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && klawisze[7]))
	{
		if (Postac.Animation[JumpToForward] || Postac.Animation[Jump])
		{
			camera.SpeedCamera = 8.0f;
			if (!Postac.Animation[JumpKick])
			{
				Postac.Ins[JumpKick].TimePos = 0.0f;
				Postac.Animation[JumpKick] = TRUE;
			}
			Anim[Animacja] = JumpKick;
			box = 0;
		}

		else if (!Postac.Animation[JumpToForward] && !Postac.Animation[Jump])
		{
			camera.SpeedCamera = 1.0f;
			if (!Postac.Animation[Kick])
			{
				Postac.Ins[Kick].TimePos = 0.0f;
				Postac.Animation[Kick] = TRUE;
			}
			Anim[Animacja] = Kick;
			box = 0;
		}

		for (int i = 0; i < PersonAnimation; i++)
		{
			if (i != 5 && i != 7 && i != 8 && i != 9 && i != 21 && i != 23 && i != 24 && i != 25 && i != 37 && i != 39 && i != 40 && i != 41 && i != 52 && i != 54 && i != 55 && i != 56 && i != 68 && i != 70 && i != 71 && i != 72 && i != 87 && i != 89 && i != 90 && i != 91 && i != 103 && i != 105 && i != 106 && i != 107)
				Postac.Animation[i] = FALSE;
		}
	}

	//JumpKickPunch w przÛd || JumpKickPunch w tyl || JumpKickPunch w lewo || JumpKickPunch w prawo || JumpKickPunch w przÛd i w lewo || JumpKickPunch w przÛd  i w prawo || JumpKickPunch w ty≥ i w lewo || JumpKickPunch w ty≥ i w prawo 
	else if ((!klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]) ||
		(klawisze[0] && !klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]) ||
		(!klawisze[0] && klawisze[1] && !klawisze[2] && klawisze[3] && !klawisze[4] && !klawisze[5] && klawisze[6] && !klawisze[7]))
	{
		if ((Postac.Animation[JumpToForward] && Postac.Animation[JumpKick]) || (Postac.Animation[Jump] && Postac.Animation[JumpKick]))
		{
			camera.SpeedCamera = 8.0f;
			if (!Postac.Animation[JumpKickPunch])
			{
				Postac.Ins[JumpKickPunch].TimePos = 0.0f;
				Postac.Animation[JumpKickPunch] = TRUE;
			}
			Anim[Animacja] = JumpKickPunch;
			box = 0;
		}

		else if (!Postac.Animation[JumpToForward] && !Postac.Animation[JumpKick] && !Postac.Animation[Jump] && box % 2 == 0)
		{
			camera.SpeedCamera = 1.0f;
			if (!Postac.Animation[Boxing])
			{
				Postac.Ins[Boxing].TimePos = 0.0f;
				Postac.Animation[Boxing] = TRUE;
			}
			Anim[Animacja] = Boxing;
		}

		else if (!Postac.Animation[JumpToForward] && !Postac.Animation[JumpKick] && !Postac.Animation[Jump] && box % 2 == 1)
		{
			camera.SpeedCamera = 1.0f;
			if (!Postac.Animation[Punch])
			{
				Postac.Ins[Punch].TimePos = 0.0f;
				Postac.Animation[Punch] = TRUE;
			}
			Anim[Animacja] = Punch;
		}

		if (Anim[0] == 46)
		{
			if (!Postac.Animation[127])
			{
				Postac.Ins[127].TimePos = 0.0f;
				Postac.Animation[127] = TRUE;
			}
			Anim[7] = 127;
		}

		for (int i = 0; i < PersonAnimation; i++)
		{
			if (i != 5 && i != 6 && i != 8 && i != 9 && i != 11 && i != 15 && i != 21 && i != 22 && i != 24 && i != 25 && i != 27 && i != 31 && i != 37 && i != 38 && i != 46 && i != 40 && i != 41 && i != 52 && i != 53 && i != 55 && i != 56 && i != 58 && i != 62 && i != 68 && i != 69 && i != 79 && i != 71 && i != 72 && i != 74 && i != 80 && i != 78 && i != 81 && i != 87 && i != 88 && i != 90 && i != 91 && i != 93 && i != 97 && i != 103 && i != 104 && i != 106 && i != 107 && i != 112 && i != 113 && i != 114 && i != 115 && i != 116 && i != 117 && i != 118 && i != 119 && i != 120 && i != 121 && i != 122 && i != 123 && i != 124 && i != 125 && i != 126 && i != 127)
				Postac.Animation[i] = FALSE;
		}
	}

	// skok 
	else if (!klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && klawisze[5] && !klawisze[6] && !klawisze[7])
	{
		if (!Postac.Animation[Jump])
		{
			Postac.Ins[Jump].TimePos = 0.0f;
			Postac.Animation[Jump] = TRUE;
		}
		Anim[Animacja] = Jump;
		box = 0;

		for (int i = 0; i < PersonAnimation; i++)
		{
			if (i != 5 && i != 21 && i != 37 && i != 52 && i != 68 && i != 87 && i != 103)
				Postac.Animation[i] = FALSE;
		}
	}

	//wszystkie klawisze wy≥πczone
	else if (!klawisze[0] && !klawisze[1] && !klawisze[2] && !klawisze[3] && !klawisze[4] && !klawisze[5] && !klawisze[6] && !klawisze[7])
	{
		if (dl_zycia >= 0.01f)
		{
			for (INT il = 0; il < IloElLab; il++)
			{
				if ((Potwor[0][il].Anim == 2 && Potwor[0][il].Ins[2].TimePos < Potwor[0][il].Ins[2].Model->SkinnedData.GetClipEndTime(Potwor[0][il].Ins[2].ClipName)) ||
					(Potwor[1][il].Anim == 2 && Potwor[1][il].Ins[2].TimePos < Potwor[1][il].Ins[2].Model->SkinnedData.GetClipEndTime(Potwor[1][il].Ins[2].ClipName)) ||
					(Potwor[2][il].Anim == 2 && Potwor[2][il].Ins[2].TimePos < Potwor[2][il].Ins[2].Model->SkinnedData.GetClipEndTime(Potwor[2][il].Ins[2].ClipName)) ||
					(Potwor[3][il].Anim == 2 && Potwor[3][il].Ins[2].TimePos < Potwor[3][il].Ins[2].Model->SkinnedData.GetClipEndTime(Potwor[3][il].Ins[2].ClipName)))
				{
					if (!Postac.Animation[Oberwanie])
					{
						camera.SpeedCamera = 1.0f;
						Postac.Ins[Oberwanie].TimePos = 0.0f;

						if (Postac.Ins[Oberwanie].TimePos == 0.0f)
						{
							x_dl_zycia -= 0.005f;
							dl_zycia -= 0.01f;
						}

						Postac.Animation[Oberwanie] = TRUE;
						Postac.Speed[0] = 1.012f;
					}
					Anim[Animacja] = Oberwanie;
				}

				else if ((Potwor[0][il].Anim == 3) && (Potwor[0][il].Ins[3].TimePos < Potwor[0][il].Ins[3].Model->SkinnedData.GetClipEndTime(Potwor[0][il].Ins[3].ClipName)) ||
					(Potwor[1][il].Anim == 3) && (Potwor[1][il].Ins[3].TimePos < Potwor[1][il].Ins[3].Model->SkinnedData.GetClipEndTime(Potwor[1][il].Ins[3].ClipName)) ||
					(Potwor[2][il].Anim == 3) && (Potwor[2][il].Ins[3].TimePos < Potwor[2][il].Ins[3].Model->SkinnedData.GetClipEndTime(Potwor[2][il].Ins[3].ClipName)) ||
					(Potwor[3][il].Anim == 3) && (Potwor[3][il].Ins[3].TimePos < Potwor[3][il].Ins[3].Model->SkinnedData.GetClipEndTime(Potwor[3][il].Ins[3].ClipName)))
				{
					if (!Postac.Animation[Oberwanie])
					{
						camera.SpeedCamera = 1.0f;
						Postac.Speed[0] = 1.01f;
						if (Potwor[0][il].Ins[3].TimePos > 0.5f || Potwor[1][il].Ins[3].TimePos > 0.5f || Potwor[2][il].Ins[3].TimePos > 0.5f ||
							Potwor[3][il].Ins[3].TimePos > 0.5f)
						{
							Postac.Ins[Oberwanie].TimePos = 0.0f;

							if (Postac.Ins[Oberwanie].TimePos == 0.0f)
							{
								x_dl_zycia -= 0.005f;
								dl_zycia -= 0.01f;
							}

							Anim[Animacja] = Oberwanie;
							Postac.Animation[Oberwanie] = TRUE;
						}
					}
				}

				else if ((Potwor[0][il].Anim == 4 && Potwor[0][il].Ins[4].TimePos < Potwor[0][il].Ins[4].Model->SkinnedData.GetClipEndTime(Potwor[0][il].Ins[4].ClipName)) ||
					(Potwor[1][il].Anim == 4 && Potwor[1][il].Ins[4].TimePos < Potwor[1][il].Ins[4].Model->SkinnedData.GetClipEndTime(Potwor[1][il].Ins[4].ClipName)) ||
					(Potwor[2][il].Anim == 4 && Potwor[2][il].Ins[4].TimePos < Potwor[2][il].Ins[4].Model->SkinnedData.GetClipEndTime(Potwor[2][il].Ins[4].ClipName)) ||
					(Potwor[3][il].Anim == 4 && Potwor[3][il].Ins[4].TimePos < Potwor[3][il].Ins[4].Model->SkinnedData.GetClipEndTime(Potwor[3][il].Ins[4].ClipName)))
				{
					if (!Postac.Animation[Oberwanie])
					{
						camera.SpeedCamera = 1.0f;
						Postac.Speed[0] = 1.01f;
						Postac.Ins[Oberwanie].TimePos = 0.0f;

						if (Postac.Ins[Oberwanie].TimePos == 0.0f)
						{
							x_dl_zycia -= 0.005f;
							dl_zycia -= 0.01f;
						}

						Postac.Animation[Oberwanie] = TRUE;
					}
					Anim[Animacja] = Oberwanie;
				}

				else if ((Potwor[0][il].Anim == 5) && (Potwor[0][il].Ins[5].TimePos < Potwor[0][il].Ins[5].Model->SkinnedData.GetClipEndTime(Potwor[0][il].Ins[5].ClipName)) ||
					(Potwor[1][il].Anim == 5) && (Potwor[1][il].Ins[5].TimePos < Potwor[1][il].Ins[5].Model->SkinnedData.GetClipEndTime(Potwor[1][il].Ins[5].ClipName)) ||
					(Potwor[2][il].Anim == 5) && (Potwor[2][il].Ins[5].TimePos < Potwor[2][il].Ins[5].Model->SkinnedData.GetClipEndTime(Potwor[2][il].Ins[5].ClipName)) ||
					(Potwor[3][il].Anim == 5) && (Potwor[3][il].Ins[5].TimePos < Potwor[3][il].Ins[5].Model->SkinnedData.GetClipEndTime(Potwor[3][il].Ins[5].ClipName)))
				{
					if (!Postac.Animation[Oberwanie])
					{
						camera.SpeedCamera = 1.0f;
						Postac.Speed[0] = 1.01f;
						Postac.Ins[Oberwanie].TimePos = 0.0f;

						if (Postac.Ins[Oberwanie].TimePos == 0.0f)
						{
							x_dl_zycia -= 0.005f;
							dl_zycia -= 0.01f;
						}

						Postac.Animation[Oberwanie] = TRUE;
					}
					Anim[Animacja] = Oberwanie;
				}

				else if ((Potwor[0][il].Anim == 6) && (Potwor[0][il].Ins[6].TimePos < Potwor[0][il].Ins[6].Model->SkinnedData.GetClipEndTime(Potwor[0][il].Ins[6].ClipName)) ||
					(Potwor[1][il].Anim == 6) && (Potwor[1][il].Ins[6].TimePos < Potwor[1][il].Ins[6].Model->SkinnedData.GetClipEndTime(Potwor[1][il].Ins[6].ClipName)) ||
					(Potwor[2][il].Anim == 6) && (Potwor[2][il].Ins[6].TimePos < Potwor[2][il].Ins[6].Model->SkinnedData.GetClipEndTime(Potwor[2][il].Ins[6].ClipName)) ||
					(Potwor[3][il].Anim == 6) && (Potwor[3][il].Ins[6].TimePos < Potwor[3][il].Ins[6].Model->SkinnedData.GetClipEndTime(Potwor[3][il].Ins[6].ClipName)))
				{
					if (!Postac.Animation[Oberwanie])
					{
						camera.SpeedCamera = 1.0f;
						Postac.Speed[0] = 1.01f;
						Postac.Ins[Oberwanie].TimePos = 0.0f;

						if (Postac.Ins[Oberwanie].TimePos == 0.0f)
						{
							x_dl_zycia -= 0.005f;
							dl_zycia -= 0.01f;
						}

						Postac.Animation[Oberwanie] = TRUE;
					}
					Anim[Animacja] = Oberwanie;
				}
			}
		}

		else if (dl_zycia < 0.01f)
		{
			if (!Postac.Animation[Upadek])
			{
				camera.SpeedCamera = 1.0f;
				Postac.Speed[0] = 1.0f;
				Postac.Ins[Upadek].TimePos = 0.0f;

				if (Postac.Ins[Upadek].TimePos == 0.0f)
				{
					x_dl_zycia -= 0.005f;
					dl_zycia -= 0.01f;
				}

				Postac.Animation[Upadek] = TRUE;
			}
			Anim[Animacja] = Upadek;
		}

		if (Anim[Animacja] == Jogging || Anim[Animacja] == Run || (Anim[Animacja] == JogStart && Postac.Ins[JogStart].TimePos >= Postac.Ins[JogStart].Model->SkinnedData.GetClipEndTime(Postac.Ins[JogStart].ClipName)))
		{
			if (!Postac.Animation[JogStop])
			{
				Postac.Speed[0] = 1.0f;
				camera.SpeedCamera = 3.0f;
				Postac.Ins[JogStop].TimePos = 0.0f;
				Postac.Animation[JogStop] = TRUE;
			}
			Anim[Animacja] = JogStop;
			box = 0;
		}

		else if ((Anim[Animacja] == JogStop && Postac.Ins[JogStop].TimePos < Postac.Ins[JogStop].Model->SkinnedData.GetClipEndTime(Postac.Ins[JogStop].ClipName)) ||
			(Anim[Animacja] == JogStart && Postac.Ins[JogStart].TimePos < Postac.Ins[JogStart].Model->SkinnedData.GetClipEndTime(Postac.Ins[JogStart].ClipName)) ||
			(Anim[Animacja] == JumpToForward && Postac.Ins[JumpToForward].TimePos < Postac.Ins[JumpToForward].Model->SkinnedData.GetClipEndTime(Postac.Ins[JumpToForward].ClipName) - 1.0f))
		{
			if (walks[0] && !walks[1] && !walks[2] && !walks[3])
				camera.WalkForward(dt);

			else if (!walks[0] && walks[1] && !walks[2] && !walks[3])
				camera.WalkBack(dt);

			else if (!walks[0] && !walks[1] && walks[2] && !walks[3])
				camera.WalkLeft(dt);

			else if (!walks[0] && !walks[1] && !walks[2] && walks[3])
				camera.WalkRight(dt);

			else if (walks[0] && !walks[1] && walks[2] && !walks[3])
			{
				camera.WalkForward(dt);
				camera.WalkLeft(dt);
			}

			else if (walks[0] && !walks[1] && !walks[2] && walks[3])
			{
				camera.WalkForward(dt);
				camera.WalkRight(dt);
			}

			else if (!walks[0] && walks[1] && walks[2] && !walks[3])
			{
				camera.WalkBack(dt);
				camera.WalkLeft(dt);
			}

			else if (!walks[0] && walks[1] && !walks[2] && walks[3])
			{
				camera.WalkBack(dt);
				camera.WalkRight(dt);
			}
		}

		else if ((Anim[Animacja] == JogStop && Postac.Ins[JogStop].TimePos >= Postac.Ins[JogStop].Model->SkinnedData.GetClipEndTime(Postac.Ins[JogStop].ClipName)) ||
			(Anim[Animacja] == Punch && Postac.Ins[Punch].TimePos >= Postac.Ins[Punch].Model->SkinnedData.GetClipEndTime(Postac.Ins[Punch].ClipName)) ||
			(Anim[Animacja] == Kick && Postac.Ins[Kick].TimePos >= Postac.Ins[Kick].Model->SkinnedData.GetClipEndTime(Postac.Ins[Kick].ClipName)) ||
			(Anim[Animacja] == Jump && Postac.Ins[Jump].TimePos >= Postac.Ins[Jump].Model->SkinnedData.GetClipEndTime(Postac.Ins[Jump].ClipName)) ||
			(Anim[Animacja] == KickZPolobrotu && Postac.Ins[KickZPolobrotu].TimePos >= Postac.Ins[KickZPolobrotu].Model->SkinnedData.GetClipEndTime(Postac.Ins[KickZPolobrotu].ClipName)) ||
			(Anim[Animacja] == JumpKickPunch && Postac.Ins[JumpKickPunch].TimePos >= Postac.Ins[JumpKickPunch].Model->SkinnedData.GetClipEndTime(Postac.Ins[JumpKickPunch].ClipName)) ||
			(Anim[Animacja] == FlySpinKick && Postac.Ins[FlySpinKick].TimePos >= Postac.Ins[FlySpinKick].Model->SkinnedData.GetClipEndTime(Postac.Ins[FlySpinKick].ClipName)) ||
			(Anim[Animacja] == JumpToForward && Postac.Ins[JumpToForward].TimePos >= Postac.Ins[JumpToForward].Model->SkinnedData.GetClipEndTime(Postac.Ins[JumpToForward].ClipName)) ||
			(Anim[Animacja] == JumpKick && Postac.Ins[JumpKick].TimePos >= Postac.Ins[JumpKick].Model->SkinnedData.GetClipEndTime(Postac.Ins[JumpKick].ClipName)) ||
			(Anim[Animacja] == Boxing && Postac.Ins[Boxing].TimePos >= Postac.Ins[Boxing].Model->SkinnedData.GetClipEndTime(Postac.Ins[Boxing].ClipName)) ||
			(Anim[Animacja] == Oberwanie && Postac.Ins[Oberwanie].TimePos >= Postac.Ins[Oberwanie].Model->SkinnedData.GetClipEndTime(Postac.Ins[Oberwanie].ClipName)))
		{
			if (!Postac.Animation[Stojka])
			{
				Postac.Speed[0] = 1.0f;
				Postac.Ins[Stojka].TimePos = 0.0f;
				Postac.Animation[Stojka] = TRUE;
			}
			camera.SpeedCamera = 0.0f;
			Anim[Animacja] = Stojka;
			Anim[7] = 0;
			box = 0;
		}

		for (int i = 0; i < PersonAnimation; i++)
		{
			if (i != 0 && i != 3 && i != 13 && i != 14 && i != 16 && i != 19 && i != 29 && i != 30 && i != 32 && i != 35 && i != 44 && i != 45 && i != 47 && i != 50 && i != 60 && i != 61 && i != 63 && i != 66 && i != 76 && i != 77 && i != 82 && i != 85 && i != 95 && i != 96 && i != 98 && i != 101 && i != 110 && i != 111 && i != 130 && i != 131 && i != 132 && i != 133 && i != 134 && i != 135 && i != 136 && i != 137 && i != 138 && i != 139 && i != 140 && i != 141)
				Postac.Animation[i] = FALSE;
		}
	}

	if ((Anim[Animacja] == Stojka && !wygrana && po % 2 == 1 && Anim[0] != 14) ||
		(Anim[Animacja] == Jogging && !wygrana && po % 2 == 1 && Anim[0] != 14) ||
		(Anim[Animacja] == Run && !wygrana && po % 2 == 1 && Anim[0] != 14))
		Postac.Ins[Anim[Animacja]].Loop();
}

void Meshes::DrawInsSkin(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, SkinnedModelInstance Model, FLOAT jas_sw, FLOAT jas_diff, Vect Scaling, Vect Angle, Vect Translate, BOOL enableBlend)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&Model.World, I);
	XMMATRIX Scale = XMMatrixScaling(Scaling.x1, Scaling.y1, Scaling.z1);
	XMMATRIX RotX = XMMatrixRotationX(Angle.x1*XM_PI);
	XMMATRIX RotY = XMMatrixRotationY(Angle.y1*XM_PI);
	XMMATRIX RotZ = XMMatrixRotationZ(Angle.z1*XM_PI);
	XMMATRIX Trans = XMMatrixTranslation(Translate.x1, Translate.y1, Translate.z1);
	XMStoreFloat4x4(&Model.World, Scale*RotY*RotX*RotZ*Trans);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (enableBlend)
		dc->OMSetBlendState(mAlphaEBlendState, blendFactor, 0xff00ff);
	else
		dc->OMSetBlendState(mAlphaDBlendState, blendFactor, 0xff00ff);

	XMMATRIX toTexSpace(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX world = XMLoadFloat4x4(&Model.World);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldViewProj = world * view*proj;

	Effects::NormalMapFX->SetWorld(world);
	Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
	Effects::NormalMapFX->SetWorldViewProjTex(worldViewProj*toTexSpace);
	Effects::NormalMapFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
	Effects::NormalMapFX->SetBoneTransforms(&Model.FinalTransforms[0], Model.FinalTransforms.size());

	for (UINT subset = 0; subset < Model.Model->SubsetCount; ++subset)
	{
		Model.Model->Mat[subset].Ambient = XMFLOAT4(jas_sw, jas_sw, jas_sw, jas_sw);
		Model.Model->Mat[subset].Diffuse = XMFLOAT4(jas_diff, jas_diff, jas_diff, jas_diff);
		Model.Model->Mat[subset].Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
		Model.Model->Mat[subset].Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

		Effects::NormalMapFX->SetMaterial(Model.Model->Mat[subset]);
		Effects::NormalMapFX->SetDiffuseMap(Model.Model->DiffuseMapSRV[subset]);
		Effects::NormalMapFX->SetNormalMap(Model.Model->NormalMapSRV[subset]);

		tech->GetPassByIndex(p)->Apply(0, dc);
		Model.Model->ModelMesh.Draw(dc, subset);
	}
}

void Meshes::DrawNap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Vect FontSize, Vect Position, INT Color, string text, FLOAT jas_sw, FLOAT jas_diff)
{
	for (INT i = 0; i < text.size(); i++)
	{
		for (INT zna = 0; zna < Znaki; zna++)
		{
			if ((text[i] == Czarny[zna]->nazwa_klawisza[0] && Color == 0) || (text[i] == Czarny[zna]->nazwa_klawisza[1] && Color == 0))
				DrawStatMeshLab(dc, view, proj, tech, p, FontSize, { 0.0f, 1.0f, 0.0f }, { Position.x1 + (i * (FontSize.x1 / 0.022f)), Position.y1, 2.0f }, Czarny[zna][i], jas_sw, jas_diff, false);

			else if ((text[i] == Czerwony[zna]->nazwa_klawisza[0] && Color == 1) || (text[i] == Czerwony[zna]->nazwa_klawisza[1] && Color == 1))
				DrawStatMeshLab(dc, view, proj, tech, p, FontSize, { 0.0f, 1.0f, 0.0f }, { Position.x1 + (i * (FontSize.x1 / 0.022f)), Position.y1, 2.0f }, Czerwony[zna][i], jas_sw, jas_diff, false);
		}
	}
}

void Meshes::DrawNapMen1(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony1[3], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, nap[0][0], czerwony1[0], "GRA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, nap[0][1], czerwony1[1], "OPCJE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, nap[0][2], czerwony1[2], "WYJåCIE", jas_sw, jas_diff);
}

void Meshes::DrawNapMen2(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony2[4], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, nap[1][0], 0, "GRA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[1][1], czerwony2[0], "NOWA GRA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[1][2], czerwony2[1], "WCZYTAJ GR ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[1][3], czerwony2[2], "STW”RZ LABIRYNT", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[1][4], czerwony2[3], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawNapMen3(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony3[4], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, nap[2][0], 0, "OPCJE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[2][1], czerwony3[0], "DèWI K", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[2][2], czerwony3[1], "GRAFIKA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[2][3], czerwony3[2], "STEROWANIE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[2][4], czerwony3[3], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawNapMen4(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony4[3], FLOAT jas_sw, FLOAT jas_diff, FLOAT ScaleMenuTitle)
{
	DrawNap(dc, view, proj, tech, p, { ScaleMenuTitle, ScaleMenuTitle, ScaleMenuTitle }, nap[3][0], 0, "STW”RZ LABIRYNT", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[3][1], czerwony4[0], "NOWY LABIRYNT", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[3][2], czerwony4[1], "WCZYTAJ LABIRYNT", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[3][3], czerwony4[2], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawNapMen5(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony5[2], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, nap[4][0], 0, "WCZYTAJ GR ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[4][1], czerwony5[0], "WCZYTAJ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[4][2], czerwony5[1], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawNapMen6(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony6[2], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, nap[5][0], 0, "NOWA GRA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[5][1], czerwony6[0], "GRAJ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, nap[5][2], czerwony6[1], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawGraNap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, string punkty, string strzaly, FLOAT jas_sw, FLOAT jas_diff, Vect vGraNapisy)
{
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { vGraNapisy.x1, 0.67f }, 0, punkty, jas_sw, jas_diff);

	if (bron == 1)
		DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { vGraNapisy.x2, 0.52f }, 0, strzaly, jas_sw, jas_diff);
}

void Meshes::DrawCzas(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, string sekundy_s[3], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.102f, 0.75f }, 0, sekundy_s[0], jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.01f, 0.75f }, 0, ":", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { 0.02f, 0.75f }, 0, sekundy_s[1], jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { 0.13f, 0.75f }, 0, ":", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { 0.16f, 0.75f }, 0, sekundy_s[2], jas_sw, jas_diff);
}

void Meshes::DrawMenPomNap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT kolor[6], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.07f, 0.25f }, kolor[0], "WZN”W", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.1f, 0.15f }, kolor[1], "RESTART", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.1f, 0.05f }, kolor[2], "WCZYTAJ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.1f, -0.05f }, kolor[3], "ZAPISZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.09f, -0.13f }, kolor[4], "OPCJE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.11f, -0.225f }, kolor[5], "POWR”∆", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.18f, -0.3f }, kolor[5], "DO G£. MENU", jas_sw, jas_diff);
}

void Meshes::DrawNapMenStwLab(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony7[12], INT czerwony8[4], FLOAT jas_sw, FLOAT jas_diff, FLOAT MenuStworzLabX[6])
{
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[0], 0.62f }, 0, "STW”RZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[1], 0.49f }, 0, "LABIRYNT", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[2], 0.17f }, czerwony8[0], "NOWY", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[3], 0.03f }, czerwony8[1], "OTW”RZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[3], -0.1f }, czerwony8[2], "ZAPISZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[3], -0.75f }, czerwony8[3], "POWR”T", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0018f, 0.0018f, 0.0018f }, { MenuStworzLabX[4], -0.64f }, czerwony7[10], "A", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0018f, 0.0018f, 0.0018f }, { MenuStworzLabX[5], -0.64f }, czerwony7[11], "B", jas_sw, jas_diff);
}

void Meshes::DrawNapPA(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Vect Pos, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, Pos, 0, "A", jas_sw, jas_diff);
}

void Meshes::DrawNapPB(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Vect Pos, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, Pos, 0, "B", jas_sw, jas_diff);
}

void Meshes::DrawNapSterMen(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT CzerwonyKlaw[18], string tytul, Vect Pos, FLOAT jas_sw, FLOAT jas_diff, FLOAT OpcjePosX[2])
{
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, Pos, 0, tytul, jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.8f, 0.56f }, 0, "FUNKCJA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { 0.25f, 0.56f }, 0, "KLAWISZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.49f }, CzerwonyKlaw[0], "IDè DO PRZODU", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.42f }, CzerwonyKlaw[1], "IDè DO TY£U", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.35f }, CzerwonyKlaw[2], "IDè W LEWO", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.28f }, CzerwonyKlaw[3], "IDè W PRAWO", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.21f }, CzerwonyKlaw[4], "PATRZ W G”R ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.14f }, CzerwonyKlaw[5], "PATRZ W D”£", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.07f }, CzerwonyKlaw[6], "PATRZ W LEWO", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.0f }, CzerwonyKlaw[7], "PATRZ W PRAWO", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.07f }, CzerwonyKlaw[8], "BIEGNIJ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.14f }, CzerwonyKlaw[9], "CIOS", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.21f }, CzerwonyKlaw[10], "KOPNIAK", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.28f }, CzerwonyKlaw[11], "SKACZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.35f }, CzerwonyKlaw[12], "CELOWNIK", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.42f }, CzerwonyKlaw[13], "POPRZEDNIA BRO—", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.49f }, CzerwonyKlaw[14], "NAST PNA BRO—", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.7f }, CzerwonyKlaw[15], "USTAWIENIA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.8f }, CzerwonyKlaw[15], "PODSTAWOWE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.7f }, CzerwonyKlaw[16], "ZAPISZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.8f }, CzerwonyKlaw[16], "ZMIANY", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[1], -0.8f }, CzerwonyKlaw[17], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawNapSterMen1(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, FLOAT yklaw, string klawiszek, INT kol, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { 0.26f, 0.49f - yklaw }, kol, klawiszek, jas_sw, jas_diff);
}

void Meshes::DrawNapOpGraf(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony9[6], FLOAT jas_sw, FLOAT jas_diff, FLOAT OpcjePosX[5])
{
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, { -0.3f, 0.66f }, 0, "GRAFIKA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[2], 0.1f }, czerwony9[0], "ROZDZIELCZOå∆", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[3], -0.07f }, czerwony9[1], "PE£NY EKRAN", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[4], -0.24f }, czerwony9[2], "JASNOå∆", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.7f }, czerwony9[3], "USTAWIENIA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.8f }, czerwony9[3], "PODSTAWOWE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.7f }, czerwony9[4], "ZAPISZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.8f }, czerwony9[4], "ZMIANY", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[1], -0.8f }, czerwony9[5], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawNapOpRozdz(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, string OpcjaRozdzielczosc[2][2], INT czer[2], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.2f, 0.06f }, czer[0], OpcjaRozdzielczosc[0][0], jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { 0.02f, 0.06f }, czer[0], "X", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { 0.08f, 0.06f }, czer[0], OpcjaRozdzielczosc[0][1], jas_sw, jas_diff);

	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.2f, -0.105f }, czer[1], OpcjaRozdzielczosc[1][0], jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { 0.02f, -0.105f }, czer[1], "X", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { 0.08f, -0.105f }, czer[1], OpcjaRozdzielczosc[1][1], jas_sw, jas_diff);
}

void Meshes::DrawNapOpGraf1(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, string Rozdz1, string Rozdz2, string fullekran, INT czerw[2], FLOAT jas_sw, FLOAT jas_diff, FLOAT Grafika1Pos[4])
{
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { Grafika1Pos[0], 0.09f }, czerw[0], Rozdz1, jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.0008f }, { Grafika1Pos[1], 0.09f }, czerw[0], "X", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { Grafika1Pos[2], 0.09f }, czerw[0], Rozdz2, jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { Grafika1Pos[3], -0.07f }, czerw[1], fullekran, jas_sw, jas_diff);
}

void Meshes::DrawNapOpFullEkr(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT kolo[2], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.02f, 0.06f }, kolo[1], "W£", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.05f, -0.1f }, kolo[2], "WY£", jas_sw, jas_diff);
}

void Meshes::DrawNapOpDzw(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony10[5], FLOAT jas_sw, FLOAT jas_diff, FLOAT OpcjePosX[2], FLOAT OpcjeDzwiekuX[2])
{
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, { -0.6f, 0.63f }, 0, "OPCJE DèWI KU", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjeDzwiekuX[0], -0.04f }, czerwony10[0], "G£OåNOå∆ MUZYKI", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjeDzwiekuX[1], -0.23f }, czerwony10[1], "G£OåNOå∆ EFEKT”W", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.7f }, czerwony10[2], "USTAWIENIA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.8f }, czerwony10[2], "PODSTAWOWE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.7f }, czerwony10[3], "ZAPISZ", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.8f }, czerwony10[3], "ZMIANY", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[1], -0.8f }, czerwony10[4], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawNapNieZapZm(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT kolorek[2], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0015f, 0.0015f, 0.0015f }, { -0.89f, 0.03f }, 0, "WSZELKIE NIEZAPISANE ZMIANY", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0015f, 0.0015f, 0.0015f }, { -0.51f, -0.06f }, 0, "ZOSTAN• UTRACONE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0015f, 0.0015f, 0.0015f }, { -0.36f, -0.16f }, 0, "KONTYNUOWA∆?", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.002f, 0.002f, 0.002f }, { -0.83f, -0.31f }, kolorek[0], "TAK", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.002f, 0.002f, 0.002f }, { 0.65f, -0.31f }, kolorek[1], "NIE", jas_sw, jas_diff);
}

void Meshes::DrawNapNieWszFunKl(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0015f, 0.0015f, 0.0015f }, { -0.85f, 0.03f }, 0, "NIEZAPISANO Z POWODU BRAKU", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0015f, 0.0015f, 0.0015f }, { -0.65f, -0.06f }, 0, "WYBRANYCH WSZYSTKICH", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0015f, 0.0015f, 0.0015f }, { -0.51f, -0.16f }, 0, "FUNKCJI KLAWISZY", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.002f, 0.002f, 0.002f }, { -0.1f, -0.31f }, 1, "OK", jas_sw, jas_diff);
}

void Meshes::DrawWcz(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT ko, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.005f }, { -0.28f, 0.59f }, 0, "WCZYTYWANIE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.005f }, { -0.12f, -0.67f }, ko, "ANULUJ", jas_sw, jas_diff);
}

void Meshes::DrawWcz2(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT ilosc, FLOAT wysokosc, string plik, INT koloreczek, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.005f }, { -0.28f, 0.48f - wysokosc }, koloreczek, plik, jas_sw, jas_diff);
}

void Meshes::DrawZap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwonka[2], FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.004f }, { -0.28f, 0.12f }, 0, "Zapisywanie", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.003f }, { -0.83f, -0.03f }, 0, "Zapisz jako:", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.002f }, { -0.83f, -0.36f }, czerwonka[0], "Zapisz", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.002f }, { 0.65f, -0.36f }, czerwonka[1], "Anuluj", jas_sw, jas_diff);
}

void Meshes::DrawZap2(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, FLOAT zapx, string klawiszek, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0008f, 0.0008f, 0.003f }, { -0.39f + zapx, -0.03f }, 0, klawiszek, jas_sw, jas_diff);
}

void Meshes::DrawSter(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique *tech, UINT p, INT color[4], FLOAT jas_sw, FLOAT jas_diff, FLOAT OpcjePosX)
{
	DrawNap(dc, view, proj, tech, p, { 0.0022f, 0.0022f, 0.0022f }, { -0.46f, 0.66f }, 0, "STEROWANIE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0018f, 0.0018f, 0.0018f }, { -1.0f, 0.06f }, color[0], "KLAWIATURA", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0018f, 0.0018f, 0.0018f }, { 0.45f, 0.06f }, color[1], "GAMEPAD", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX, -0.8f }, color[2] | color[3], "POWR”T", jas_sw, jas_diff);
}

void Meshes::DrawZapGre(ID3D11DeviceContext *dc, XMMATRIX &mProj, XMMATRIX &mView, ID3DX11EffectTechnique *tech, UINT p, FLOAT jas_sw, FLOAT jas_diff, INT *color, INT zapxx[19])
{
	DrawZap(dc, mView, mProj, tech, p, color, jas_sw, jas_diff);

	for (INT kla = 0; kla < KLA - 25; kla++)
	{
		if ((kla >= 16 && kla <= 28) || (kla >= 31 && kla <= 43) || (kla >= 45 && kla <= 55) || (kla >= 58 && kla <= 67) || (kla == 71))
		{
			FLOAT zapx = 0.0f;
			for (INT dlug_zap = 0; dlug_zap < 19; dlug_zap++)
			{
				if (zapxx[dlug_zap] >= 1 && zapxx[dlug_zap] <= 3)
				{
					if (klaw[kla].zapis[dlug_zap] == dlug_zap + 1)
						DrawZap2(dc, mView, mProj, tech, p, zapx, klaw[kla].nazwa_klawisza, jas_sw, jas_diff);

					zapx += 0.04f;
				}

				else if (zapxx[dlug_zap] == 4)
					zapx += 0.04f;
			}
		}
	}
}

void Meshes::DrawArrowInThere(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique *tech, UINT p, FLOAT jas_sw, FLOAT jas_diff)
{
	for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
	{
		if (ShowArrowOnHedge[ilo2])
		{
			DrawStatMeshLab(dc, view, proj, tech, p, { 1.0f, 1.0f, 1.0f }, { AngleXTrajShotHedge[ilo2], 0.0f, ppTrajShotHedge[ilo2] + 0.4f }, { CTrajShotHedge[ilo2].x1, yyTrajShotHedge[ilo2], CTrajShotHedge[ilo2].z1 }, StrzalaShot[ilo2], jas_sw, jas_diff, false);
			ShootingArrow = FALSE;
		}
	}

	for (INT ill = 0; ill < IloElLab; ill++)
	{
		for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
		{
			if (ShowArrowOnMonster[ill][ilo2])
			{
				DrawStatMeshLab(dc, view, proj, tech, p, { 1.0f, 1.0f, 1.0f }, { AngleXTrajShot[ill][ilo2], 0.0f, ppTrajShot[ill][ilo2] + 0.4f }, { CTrajShot[ill][ilo2].x1, yyTrajShot[ill][ilo2], CTrajShot[ill][ilo2].z1 }, StrzalaShot[ilo2], jas_sw, jas_diff, false);
				ShootingArrow = FALSE;
			}
		}
	}
}

void Meshes::DrawWygr(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique *tech, UINT p, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, { -0.67f, 0.1f }, 0, "GRATULACJE", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, { -0.52f, -0.15f }, 0, "WYGRA£Eå", jas_sw, jas_diff);
}

void Meshes::DrawKoniecGry(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique *tech, UINT p, FLOAT jas_sw, FLOAT jas_diff)
{
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, { -0.37f, 0.1f }, 0, "KONIEC", jas_sw, jas_diff);
	DrawNap(dc, view, proj, tech, p, { 0.0033f, 0.0033f, 0.0033f }, { -0.15f, -0.15f }, 0, "GRY", jas_sw, jas_diff);
}

void Meshes::Draw(ID3D11DeviceContext *dc, XMFLOAT4X4 LightView, XMFLOAT4X4 LightProj, ID3D11DepthStencilView* DepthStencilView, D3D11_VIEWPORT ScreenViewport, ID3D11RenderTargetView* RenderTargetView, XMMATRIX &mProj, XMMATRIX &mView, ThirdPersonCamera &camera, Sky *mSky, INT pierw, INT scen, INT obr_sciany, string punkty, string strzaly, string sekundy_s[3], INT po, INT nowy[2], INT *poz_scian, INT kla, string klawiszek, INT pierwszy[31], INT ustaw_klaw[31], BOOL ustaw_rozdzielczosc, string OpcjaRozdzielczosc[2][2], FLOAT rozdzielczosc_w, FLOAT rozdzielczosc_h, BOOL full, string fullekr[2], BOOL ustaw_fullekran, INT scen2, INT wczyt, INT IloscPlikow[2][2], string *pliki[2], string pliki2[2][17], INT zapxx[19], INT CzerwonyNapis[17][18], INT *koloreczek[2], FLOAT dt, DirectionalLight DirLights[3], FLOAT jas_sw, FLOAT jas_diff, INT Anim[7], BOOL weapon[3], INT strzalki, FLOAT ScaleMenuTitle, Vect vGraNapisy, FLOAT MenuStworzLabX[6], FLOAT OpcjePosX[5], FLOAT Grafika1Pos[4], FLOAT OpcjeDzwiekuX[2], Vect ZywT[4])
{
	dc->RSSetState(0);
	dc->RSSetViewports(1, &ScreenViewport);
	mSsao->SetNormalDepthRenderTarget(DepthStencilView);

	DrawSsao(dc, mProj, mView, camera, pierw, scen, obr_sciany, punkty, strzaly, sekundy_s, po, nowy, poz_scian, kla, klawiszek, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpcjaRozdzielczosc, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen2, wczyt, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, koloreczek, Anim, weapon, strzalki, ScaleMenuTitle, vGraNapisy, MenuStworzLabX, OpcjePosX, Grafika1Pos, OpcjeDzwiekuX, ZywT);

	mSsao->ComputeSsao(camera);

	ID3D11RenderTargetView* renderTargets[1] = { RenderTargetView };
	dc->OMSetRenderTargets(1, renderTargets, DepthStencilView);
	dc->RSSetViewports(1, &ScreenViewport);

	dc->OMSetDepthStencilState(RenderStates::EqualsDSS, 0);

	XMMATRIX view = camera.CamView;
	XMMATRIX proj = camera.CamProjection;
	XMMATRIX viewProj = XMMatrixMultiply(view, proj);

	Effects::NormalMapFX->SetDirLights(DirLights);
	Effects::NormalMapFX->SetEyePosW(camera.CamPosition);
	Effects::NormalMapFX->SetCubeMap(mSky->CubeMapSRV());
	Effects::NormalMapFX->SetSsaoMap(mSsao->AmbientSRV());

	ID3DX11EffectTechnique* activeTech = Effects::NormalMapFX->Light3TexTech;
	ID3DX11EffectTechnique* activeSkinnedTech = Effects::NormalMapFX->Light3TexSkinnedTech;

	dc->IASetInputLayout(InputLayouts::PosNormalTexTan);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex::PosNormalTexTan);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		if (scen == 0)
			DrawNapMen1(dc, mView, mProj, activeTech, p, CzerwonyNapis[0], jas_sw, jas_diff);

		if (scen == 1)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen1(dc, mView, mProj, activeTech, p, CzerwonyNapis[0], jas_sw, jas_diff);
				DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
			}

			DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
		}

		if (scen == 2)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen1(dc, mView, mProj, activeTech, p, CzerwonyNapis[0], jas_sw, jas_diff);
				DrawNapMen3(dc, mView, mProj, activeTech, p, CzerwonyNapis[2], jas_sw, jas_diff);
			}

			DrawNapMen3(dc, mView, mProj, activeTech, p, CzerwonyNapis[2], jas_sw, jas_diff);
		}

		if (scen == 3)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
				DrawNapMen1(dc, mView, mProj, activeTech, p, CzerwonyNapis[0], jas_sw, jas_diff);
			}

			DrawNapMen1(dc, mView, mProj, activeTech, p, CzerwonyNapis[0], jas_sw, jas_diff);
		}

		if (scen == 4)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen3(dc, mView, mProj, activeTech, p, CzerwonyNapis[2], jas_sw, jas_diff);
				DrawNapMen1(dc, mView, mProj, activeTech, p, CzerwonyNapis[0], jas_sw, jas_diff);
			}

			DrawNapMen1(dc, mView, mProj, activeTech, p, CzerwonyNapis[0], jas_sw, jas_diff);
		}

		if (scen == 5)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
				DrawNapMen4(dc, mView, mProj, activeTech, p, CzerwonyNapis[3], jas_sw, jas_diff, ScaleMenuTitle);
			}

			DrawNapMen4(dc, mView, mProj, activeTech, p, CzerwonyNapis[3], jas_sw, jas_diff, ScaleMenuTitle);
		}

		if (scen == 6)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen4(dc, mView, mProj, activeTech, p, CzerwonyNapis[3], jas_sw, jas_diff, ScaleMenuTitle);
				DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
			}

			DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
		}

		if (scen == 7)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
				DrawNapMen5(dc, mView, mProj, activeTech, p, CzerwonyNapis[4], jas_sw, jas_diff);
			}

			DrawNapMen5(dc, mView, mProj, activeTech, p, CzerwonyNapis[4], jas_sw, jas_diff);

			if (WczytLabiryntu % 2 == 0)
			{
				DrawWcz(dc, mView, mProj, activeTech, p, koloreczek[1][IloscPlikow[1][0]], jas_sw, jas_diff);

				ilosc[1] = 0;
				FLOAT WysokoscPliki = 0.0f;

				while (ilosc[1] < IloscPlikow[1][0] && ilosc[1] < 17)
				{
					pliki2[1][ilosc[1]] = pliki[1][ilosc[1] + IloscPlikow[1][1]];
					DrawWcz2(dc, mView, mProj, activeTech, p, ilosc[1], WysokoscPliki, pliki2[1][ilosc[1]], koloreczek[1][ilosc[1]], jas_sw, jas_diff);
					WysokoscPliki += 0.066f;
					ilosc[1]++;
				}
			}
		}

		if (scen == 8)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen5(dc, mView, mProj, activeTech, p, CzerwonyNapis[4], jas_sw, jas_diff);
				DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
			}

			DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
		}

		if (scen == 9)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
				DrawNapMen6(dc, mView, mProj, activeTech, p, CzerwonyNapis[5], jas_sw, jas_diff);
			}

			DrawNapMen6(dc, mView, mProj, activeTech, p, CzerwonyNapis[5], jas_sw, jas_diff);
		}

		if (scen == 10)
		{
			if (obr_sciany == 0)
			{
				DrawNapMen6(dc, mView, mProj, activeTech, p, CzerwonyNapis[5], jas_sw, jas_diff);
				DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
			}

			DrawNapMen2(dc, mView, mProj, activeTech, p, CzerwonyNapis[1], jas_sw, jas_diff);
		}

		if (scen == 11)
		{
			DrawStatMeshLab(dc, view, proj, activeTech, p, { 100.0f, 100.0f, 100.0f }, { 0.5f, -1.0f, 0.0f }, { 1.0f, 0.0f, 1.0f }, Podloze, jas_sw, jas_diff, false);

			ZywoplotT[0].x1 = 1.39f; ZywoplotT[1].x1 = 0.72f; ZywoplotT[2].x1 = -1.28f; ZywoplotT[3].x1 = -0.66f; ZywoplotT[0].y1 = 5.85f;
			ZywoplotT[0].z1 = -0.49f; ZywoplotT[1].z1 = 1.46f; ZywoplotT[2].z1 = 0.77f; ZywoplotT[3].z1 = -1.34f;

			for (INT i = 0; i < IloElLab; i++)
			{
				if (tab[i] == 1)
				{
					if (pierw == 0)
					{
						if ((tab[i + 1] == 1) && (tab[i + 26] == 1))
							DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[1][i], jas_sw, jas_diff, false);

						else if (tab[i + 1] == 1)
							DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.5f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[0][i], jas_sw, jas_diff, false);

						else if (tab[i + 26] == 1)
							DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.5f, 0.5f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[0][i], jas_sw, jas_diff, false);

						pierw = 1;
					}

					else if ((tab[i + 1] == 1) && (tab[i + 26] == 1) && (tab[i - 1] == 1) && (tab[i - 26] == 1))
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[3][i], jas_sw, jas_diff, false);

					else if ((tab[i - 26] == 1) && (tab[i + 26] == 1) && (tab[i + 1] == 1))//lewe te
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.5f, 0.0f }, { boxx[i].x1 + ZywT[0].x1, ZywT[0].y1, boxx[i].z1 + ZywT[0].z1 }, Modele[2][i], jas_sw, jas_diff, false);

					else if ((tab[i - 26] == 1) && (tab[i + 1] == 1) && (tab[i - 1] == 1))//dolne te
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { boxx[i].x1 + ZywT[1].x1, ZywT[0].y1, boxx[i].z1 + ZywT[1].z1 }, Modele[2][i], jas_sw, jas_diff, false);

					else if ((tab[i - 26] == 1) && (tab[i + 26] == 1) && (tab[i - 1] == 1))//prawe te
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, -0.5f, 0.0f }, { boxx[i].x1 + ZywT[2].x1, ZywT[0].y1, boxx[i].z1 + ZywT[2].z1 }, Modele[2][i], jas_sw, jas_diff, false);

					else if ((tab[i + 26] == 1) && (tab[i + 1] == 1) && (tab[i - 1] == 1))//gÛrne te
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 1.0f, 0.0f }, { boxx[i].x1 + ZywT[3].x1, ZywT[0].y1, boxx[i].z1 + ZywT[3].z1 }, Modele[2][i], jas_sw, jas_diff, false);

					else if ((tab[i - 1] == 1) && (tab[i + 26] == 1))//prawy gÛrny rÛg
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.5f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[1][i], jas_sw, jas_diff, false);

					else if ((tab[i + 26] == 1) && (tab[i + 1] == 1))//lewy gÛrny rÛg
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[1][i], jas_sw, jas_diff, false);

					else if ((tab[i - 26] == 1) && (tab[i + 1] == 1))//lewy dolny rÛg
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, -0.5f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[1][i], jas_sw, jas_diff, false);

					else if ((tab[i - 26] == 1) && (tab[i - 1] == 1))//prawy dolny rÛg
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 1.0f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[1][i], jas_sw, jas_diff, false);

					else if ((tab[i - 1] == 1) || (tab[i + 1] == 1))
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.5f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[0][i], jas_sw, jas_diff, false);

					else if ((tab[i - 26] == 1) || (tab[i + 26] == 1))
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.5f, 0.5f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[0][i], jas_sw, jas_diff, false);

					else if ((tab[i - 26] != 1) && (tab[i + 26] != 1) && (tab[i - 1] != 1) && (tab[i + 1] != 1))
						DrawStatMeshLab(dc, view, proj, activeTech, p, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.5f }, { boxx[i].x1, 7.0f, boxx[i].z1 }, Modele[0][i], jas_sw, jas_diff, false);
				}

				else if (tab[i] == 4)
					DrawStatMeshLab(dc, view, proj, activeTech, p, { 0.104f, 0.104f, 0.104f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 5.0f, boxx[i].z1 }, Modele[4][i], jas_sw, jas_diff, false);

				else if (tab[i] == 6)
					DrawStatMeshLab(dc, view, proj, activeTech, p, { 4.0f, 4.0f, 4.0f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 5.0f, boxx[i].z1 }, Modele[5][i], jas_sw, jas_diff, false);

				else if (tab[i] == 5)
					DrawStatMeshLab(dc, view, proj, activeTech, p, { 0.125f, 0.125f, 0.125f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 6.0f, boxx[i].z1 }, Modele[6][i], jas_sw, jas_diff, false);

				else if (tab[i] == 7)
					DrawStatMeshLab(dc, view, proj, activeTech, p, { 0.016f, 0.016f, 0.016f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 3.0f, boxx[i].z1 }, Modele[7][i], jas_sw, jas_diff, false);

				else if (tab[i] == 12)
					DrawStatMeshLab(dc, view, proj, activeTech, p, { 1.2f, 1.2f, 1.2f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 0.5f, boxx[i].z1 }, Modele[8][i], jas_sw, jas_diff, false);

				else if (tab[i] == 3)
					DrawStatMeshLab(dc, view, proj, activeTech, p, { 0.11f, 0.11f, 0.11f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 0.6f, boxx[i].z1 }, Modele[9][i], jas_sw, jas_diff, false);
			}

			if (Shooting && strzalki >= 0)
			{
				TrajLotuStrzaly();
				LotStrzaly(camera);

				DrawStatMeshLab(dc, view, proj, activeTech, p, { 1.0f, 1.0f, 1.0f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, 0.0f, ppTrajShoting + 0.4f }, { CTrajShoting.x1, TrajShoting.y1, CTrajShoting.z1 }, StrzalaShoting, jas_sw, jas_diff, false);

				CollisionArrowWithHedge(camera);
				for (INT si = 0; si < 4; si++)
					CollisionArrowWithMonster(si + 8, camera, Potwor[si]);
			}

			DrawArrowInThere(dc, view, proj, activeTech, p, jas_sw, jas_diff);

			DrawGraNap(dc, mView, mProj, activeTech, p, punkty, strzaly, jas_sw, jas_diff, vGraNapisy);
			DrawCzas(dc, mView, mProj, activeTech, p, sekundy_s, jas_sw, jas_diff);

			if (Postac.Ins[14].TimePos >= Postac.Ins[14].Model->SkinnedData.GetClipEndTime(Postac.Ins[14].ClipName) && Anim[0] == 14 && po % 2 == 1)
				DrawKoniecGry(dc, mView, mProj, activeTech, p, jas_sw, jas_diff);

			if (wygrana && po % 2 == 1)
				DrawWygr(dc, mView, mProj, activeTech, p, jas_sw, jas_diff);

			if (po % 2 == 0 && ZapiszGiere % 2 == 1)
				DrawMenPomNap(dc, mView, mProj, activeTech, p, CzerwonyNapis[9], jas_sw, jas_diff);

			else if (po % 2 == 1 && ZapiszGiere % 2 == 1 && !wygrana && Anim[0] != 14)
			{
				if (Shooting && strzalki >= 0)
				{
					TrajShoting.x1 += (PozycjaStrzaly.x2 / 100);
					if (TrajShoting.x1 > PozycjaStrzaly.x2)
					{
						TrajShoting.x1 = ixic;
						Shooting = FALSE;
					}
				}

				angle += 0.01f;
				if (angle >= 2.0f)
					angle = 0.0f;
			}

			else if (po % 2 == 0 && ZapiszGiere % 2 == 0)
				DrawZapGre(dc, mProj, mView, activeTech, p, jas_sw, jas_diff, CzerwonyNapis[15], zapxx);
		}

		if (scen == 12)
		{
			DrawNapMenStwLab(dc, mView, mProj, activeTech, p, CzerwonyNapis[7], CzerwonyNapis[6], jas_sw, jas_diff, MenuStworzLabX);

			if (nowy[0] == 1)
			{
				if (nowy[1] > 0)
				{
					for (INT g = 1; g < IloPunkt - 1; g++)
					{
						if (poz_scian[g] == 2)
						{
							if (g > 0)
								DrawNapPA(dc, mView, mProj, activeTech, p, punkcior[g], jas_sw, jas_diff);
						}

						else if (poz_scian[g] == 3)
						{
							if (g > 0)
								DrawNapPB(dc, mView, mProj, activeTech, p, punkcior[g], jas_sw, jas_diff);
						}
					}
				}
			}
		}

		if (scen == 13)
			DrawNapMen4(dc, mView, mProj, activeTech, p, CzerwonyNapis[3], jas_sw, jas_diff, ScaleMenuTitle);

		if (scen == 14)
		{
			DrawNapSterMen(dc, mView, mProj, activeTech, p, CzerwonyNapis[8], "KLAWIATURA I MYSZKA", { -0.96f, 0.66f }, jas_sw, jas_diff, OpcjePosX);

			for (kla = 0; kla < KLA - 25; kla++)
			{
				INT pier = 0; FLOAT ysc = 0.0f;
				while (pier < 15 && ysc < 0.99f)
				{
					if (pierwszy[pier] == 0)
					{
						if (klaw[kla].ustawiony_klaw == pier + 1)
							DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], jas_sw, jas_diff);
					}

					else if ((pierwszy[pier] == 1 || ustaw_klaw[pier] == 1) || (pierwszy[pier] == 1 && ustaw_klaw[pier] == 1))
					{
						if (klaw[kla].ustawiony_klaw == pier + 1)
							DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], jas_sw, jas_diff);
					}
					pier++;
					ysc += 0.07f;
				}
			}
		}

		if (scen == 15)
			DrawNapMen3(dc, mView, mProj, activeTech, p, CzerwonyNapis[2], jas_sw, jas_diff);

		if (scen == 16)
		{
			DrawNapOpGraf(dc, mView, mProj, activeTech, p, CzerwonyNapis[10], jas_sw, jas_diff, OpcjePosX);

			if (ustaw_rozdzielczosc)
				DrawNapOpRozdz(dc, mView, mProj, activeTech, p, OpcjaRozdzielczosc, CzerwonyNapis[13], jas_sw, jas_diff);

			stringstream Rozdzielczosc[2];
			Rozdzielczosc[0] << rozdzielczosc_w;
			Rozdzielczosc[1] << rozdzielczosc_h;

			string Rozdz1 = Rozdzielczosc[0].str();
			string Rozdz2 = Rozdzielczosc[1].str();

			if (!full)
				DrawNapOpGraf1(dc, mView, mProj, activeTech, p, Rozdz1, Rozdz2, fullekr[1], CzerwonyNapis[10], jas_sw, jas_diff, Grafika1Pos);

			else
				DrawNapOpGraf1(dc, mView, mProj, activeTech, p, Rozdz1, Rozdz2, fullekr[0], CzerwonyNapis[10], jas_sw, jas_diff, Grafika1Pos);

			if (ustaw_fullekran)
				DrawNapOpFullEkr(dc, mView, mProj, activeTech, p, CzerwonyNapis[12], jas_sw, jas_diff);
		}

		if (scen == 17)
			DrawNapOpDzw(dc, mView, mProj, activeTech, p, CzerwonyNapis[11], jas_sw, jas_diff, OpcjePosX, OpcjeDzwiekuX);

		if (scen == 18)
		{
			DrawNapNieZapZm(dc, mView, mProj, activeTech, p, CzerwonyNapis[14], jas_sw, jas_diff);

			if (scen2 == 1)
			{
				DrawNapSterMen(dc, mView, mProj, activeTech, p, CzerwonyNapis[8], "KLAWIATURA I MYSZKA", { -0.96f, 0.66f }, jas_sw, jas_diff, OpcjePosX);

				for (kla = 0; kla < KLA - 25; kla++)
				{
					INT pier = 0; FLOAT ysc = 0.0f;
					while (pier < 15 && ysc < 0.99f)
					{
						if (pierwszy[pier] == 0)
						{
							if (klaw[kla].ustawiony_klaw == pier + 1)
								DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], jas_sw, jas_diff);
						}

						else if ((pierwszy[pier] == 1 || ustaw_klaw[pier] == 1) || (pierwszy[pier] == 1 && ustaw_klaw[pier] == 1))
						{
							if (klaw[kla].ustawiony_klaw == pier + 1)
								DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], jas_sw, jas_diff);
						}
						pier++;
						ysc += 0.07f;
					}
				}
			}

			else if (scen2 == 2)
			{
				DrawNapOpGraf(dc, mView, mProj, activeTech, p, CzerwonyNapis[10], jas_sw, jas_diff, OpcjePosX);

				stringstream Rozdzielczosc1, Rozdzielczosc2;
				Rozdzielczosc1 << rozdzielczosc_w;
				Rozdzielczosc2 << rozdzielczosc_h;

				string Rozdz1 = Rozdzielczosc1.str();
				string Rozdz2 = Rozdzielczosc2.str();

				if (!full)
					DrawNapOpGraf1(dc, mView, mProj, activeTech, p, Rozdz1, Rozdz2, fullekr[1], CzerwonyNapis[10], jas_sw, jas_diff, Grafika1Pos);

				else
					DrawNapOpGraf1(dc, mView, mProj, activeTech, p, Rozdz1, Rozdz2, fullekr[0], CzerwonyNapis[10], jas_sw, jas_diff, Grafika1Pos);
			}

			else if (scen2 == 3)
				DrawNapOpDzw(dc, mView, mProj, activeTech, p, CzerwonyNapis[11], jas_sw, jas_diff, OpcjePosX, OpcjeDzwiekuX);

			else if (scen2 == 4)
			{
				DrawNapSterMen(dc, mView, mProj, activeTech, p, CzerwonyNapis[17], "GAMEPAD", { -0.36f, 0.66f }, jas_sw, jas_diff, OpcjePosX);

				for (kla = 86; kla < KLA; kla++)
				{
					INT pier = 0; FLOAT ysc = 0.0f;
					while (pier < 15 && ysc < 0.99f)
					{
						if (pierwszy[pier + 15] == 0)
						{
							if (klaw[kla].ustawiony_klaw == pier + 16)
								DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], jas_sw, jas_diff);
						}

						else if ((pierwszy[pier + 15] == 1 || ustaw_klaw[pier + 15] == 1) || (pierwszy[pier + 15] == 1 && ustaw_klaw[pier + 15] == 1))
						{
							if (klaw[kla].ustawiony_klaw == pier + 16)
								DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], jas_sw, jas_diff);
						}
						pier++;
						ysc += 0.07f;
					}
				}
			}
		}

		if (scen == 19)
		{
			if (wczyt == 0)
			{
				DrawNapMenStwLab(dc, mView, mProj, activeTech, p, CzerwonyNapis[7], CzerwonyNapis[6], jas_sw, jas_diff, MenuStworzLabX);

				if (nowy[0] == 1)
				{
					if (nowy[1] > 0)
					{
						for (INT g = 1; g < IloPunkt - 1; g++)
						{
							if (poz_scian[g] == 2)
							{
								if (g > 0)
									DrawNapPA(dc, mView, mProj, activeTech, p, punkcior[g], jas_sw, jas_diff);
							}

							if (poz_scian[g] == 3)
							{
								if (g > 0)
									DrawNapPB(dc, mView, mProj, activeTech, p, punkcior[g], jas_sw, jas_diff);
							}
						}
					}
				}
			}

			else if (wczyt == 1)
				DrawNapMen4(dc, mView, mProj, activeTech, p, CzerwonyNapis[3], jas_sw, jas_diff, ScaleMenuTitle);

			DrawWcz(dc, mView, mProj, activeTech, p, koloreczek[0][IloscPlikow[0][0]], jas_sw, jas_diff);

			ilosc[0] = 0;
			FLOAT WysokoscPliki = 0.0f;

			while (ilosc[0] < IloscPlikow[0][0] && ilosc[0] < 17)
			{
				pliki2[0][ilosc[0]] = pliki[0][ilosc[0] + IloscPlikow[0][1]];
				DrawWcz2(dc, mView, mProj, activeTech, p, ilosc[0], WysokoscPliki, pliki2[0][ilosc[0]], koloreczek[0][ilosc[0]], jas_sw, jas_diff);
				WysokoscPliki += 0.066f;
				ilosc[0]++;
			}
		}

		if (scen == 20)
		{
			DrawNapMenStwLab(dc, mView, mProj, activeTech, p, CzerwonyNapis[7], CzerwonyNapis[6], jas_sw, jas_diff, MenuStworzLabX);

			if (nowy[0] == 1)
			{
				if (nowy[1] > 0)
				{
					for (INT g = 1; g < IloPunkt - 1; g++)
					{
						if (poz_scian[g] == 2)
						{
							if (g > 0)
								DrawNapPA(dc, mView, mProj, activeTech, p, punkcior[g], jas_sw, jas_diff);
						}

						if (poz_scian[g] == 3)
						{
							if (g > 0)
								DrawNapPB(dc, mView, mProj, activeTech, p, punkcior[g], jas_sw, jas_diff);
						}
					}
				}
			}

			DrawZapGre(dc, mProj, mView, activeTech, p, jas_sw, jas_diff, CzerwonyNapis[15], zapxx);
		}

		if (scen == 21)
		{
			DrawSter(dc, mView, mProj, activeTech, p, CzerwonyNapis[16], jas_sw, jas_diff, OpcjePosX[1]);
		}

		if (scen == 22)
		{
			DrawNapSterMen(dc, mView, mProj, activeTech, p, CzerwonyNapis[17], "GAMEPAD", { -0.36f, 0.66f }, jas_sw, jas_diff, OpcjePosX);

			for (kla = 86; kla < KLA; kla++)
			{
				INT pier = 0; FLOAT ysc = 0.0f;
				while (pier < 15 && ysc < 0.99f)
				{
					if (pierwszy[pier + 15] == 0)
					{
						if (klaw[kla].ustawiony_klaw == pier + 16)
							DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], jas_sw, jas_diff);
					}

					else if ((pierwszy[pier + 15] == 1 || ustaw_klaw[pier + 15] == 1) || (pierwszy[pier + 15] == 1 && ustaw_klaw[pier + 15] == 1))
					{
						if (klaw[kla].ustawiony_klaw == pier + 16)
							DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], jas_sw, jas_diff);
					}
					pier++;
					ysc += 0.07f;
				}
			}
		}

		if (scen == 23)
		{
			DrawNapNieWszFunKl(dc, mView, mProj, activeTech, p, jas_sw, jas_diff);

			if (scen2 == 1)
			{
				DrawNapSterMen(dc, mView, mProj, activeTech, p, CzerwonyNapis[8], "KLAWIATURA I MYSZKA", { -0.96f, 0.66f }, jas_sw, jas_diff, OpcjePosX);

				for (kla = 0; kla < KLA - 25; kla++)
				{
					INT pier = 0; FLOAT ysc = 0.0f;
					while (pier < 15 && ysc < 0.99f)
					{
						if (pierwszy[pier] == 0)
						{
							if (klaw[kla].ustawiony_klaw == pier + 1)
								DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], jas_sw, jas_diff);
						}

						else if ((pierwszy[pier] == 1 || ustaw_klaw[pier] == 1) || (pierwszy[pier] == 1 && ustaw_klaw[pier] == 1))
						{
							if (klaw[kla].ustawiony_klaw == pier + 1)
								DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], jas_sw, jas_diff);
						}
						pier++;
						ysc += 0.07f;
					}
				}
			}

			else if (scen2 == 4)
			{
				DrawNapSterMen(dc, mView, mProj, activeTech, p, CzerwonyNapis[17], "GAMEPAD", { -0.36f, 0.66f }, jas_sw, jas_diff, OpcjePosX);

				for (kla = 86; kla < KLA; kla++)
				{
					INT pier = 0; FLOAT ysc = 0.0f;
					while (pier < 15 && ysc < 0.99f)
					{
						if (pierwszy[pier + 15] == 0)
						{
							if (klaw[kla].ustawiony_klaw == pier + 16)
								DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], jas_sw, jas_diff);
						}

						else if ((pierwszy[pier + 15] == 1 || ustaw_klaw[pier + 15] == 1) || (pierwszy[pier + 15] == 1 && ustaw_klaw[pier + 15] == 1))
						{
							if (klaw[kla].ustawiony_klaw == pier + 16)
								DrawNapSterMen1(dc, mView, mProj, activeTech, p, ysc, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], jas_sw, jas_diff);
						}
						pier++;
						ysc += 0.07f;
					}
				}
			}
		}
	}

	stride = sizeof(Vertex::Basic32);
	offset = 0;

	// Draw the animated CHARacters.
	if (scen == 11)
	{
		dc->IASetInputLayout(InputLayouts::PosNormalTexTanSkinned);
		activeSkinnedTech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			if (!weapon[0] && !weapon[1] && !weapon[2] && bron == 0)
			{
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && !weapon[1] && !weapon[2] && bron == 0)
			{
				if (Postac.Ins[Anim[1]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[1]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[1]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, po);
				Animations(dt, camera, 1, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[1]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && !weapon[1] && !weapon[2] && bron == 1)
			{
				if (Postac.Ins[Anim[2]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[2]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[2]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 46, 7, 8, 9, 10, 8, 12, 13, 14, 46, po);
				Animations(dt, camera, 2, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 40, 43, 44, 45, 38, po);

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[2]].Update(dt, Postac.Speed[0]);

				if (Anim[7] == 127 && Postac.Ins[127].TimePos >= 0.4f && Postac.Ins[127].TimePos <= 1.3f)
				{
					DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[127], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);

					if (Postac.Ins[127].TimePos > 1.25f && Postac.Ins[127].TimePos <= 1.3f)
						Shooting = TRUE;
				}
				Postac.Ins[127].Update(dt, Postac.Speed[0]);
			}

			else if (!weapon[0] && weapon[1] && !weapon[2] && bron == 0)
			{
				if (Postac.Ins[Anim[3]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[3]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[3]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, po);
				Animations(dt, camera, 3, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[3]].Update(dt, Postac.Speed[0]);
			}

			else if (!weapon[0] && weapon[1] && !weapon[2] && bron == 2)
			{
				if (Postac.Ins[Anim[4]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[4]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[4]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 79, 7, 8, 9, 10, 80, 12, 13, 14, 81, po);
				Animations(dt, camera, 4, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, po);

				if (Anim[0] == 79 && Anim[4] == 69)
					Postac.Speed[0] = 1.01f;
				else
					Postac.Speed[0] = 1.0f;

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[4]].Update(dt, Postac.Speed[0]);

			}

			else if (!weapon[0] && !weapon[1] && weapon[2] && bron == 0)
			{
				if (Postac.Ins[Anim[5]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[5]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[5]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, po);
				Animations(dt, camera, 5, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[5]].Update(dt, Postac.Speed[0]);
			}

			else if (!weapon[0] && !weapon[1] && weapon[2] && bron == 3)
			{
				if (Postac.Ins[Anim[6]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[6]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[6]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 113, 7, 8, 9, 10, 8, 12, 13, 14, 114, po);
				Animations(dt, camera, 6, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 106, 109, 110, 111, 112, po);

				if (Anim[0] == 113 && Anim[6] == 104)
					Postac.Speed[0] = 1.01f;
				else
					Postac.Speed[0] = 1.0f;

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[6]].Update(dt, Postac.Speed[0]);
			}

			else if (!weapon[0] && weapon[1] && weapon[2] && bron == 0)
			{
				if (Postac.Ins[Anim[3]].TimePos != Postac.Ins[Anim[5]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[3]].TimePos = Postac.Ins[Anim[5]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[3]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[5]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, po);
				Animations(dt, camera, 3, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, po);
				Animations(dt, camera, 5, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[3]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[5]].Update(dt, Postac.Speed[0]);
			}

			else if (!weapon[0] && weapon[1] && weapon[2] && bron == 3)
			{
				if (Postac.Ins[Anim[3]].TimePos != Postac.Ins[Anim[6]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[3]].TimePos = Postac.Ins[Anim[6]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[3]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[6]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 113, 7, 8, 9, 10, 8, 12, 13, 14, 114, po);
				Animations(dt, camera, 3, 47, 48, 49, 50, 51, 52, 115, 54, 55, 56, 57, 55, 59, 60, 61, 116, po);
				Animations(dt, camera, 6, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 106, 109, 110, 111, 112, po);
				
				if (Anim[0] == 113 && Anim[3] == 115 && Anim[6] == 104)
					Postac.Speed[0] = 1.01f;
				else
					Postac.Speed[0] = 1.0f;

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[3]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[6]].Update(dt, Postac.Speed[0]);
			}

			else if (!weapon[0] && weapon[1] && weapon[2] && bron == 2)
			{
				if (Postac.Ins[Anim[4]].TimePos != Postac.Ins[Anim[5]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[4]].TimePos = Postac.Ins[Anim[5]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[4]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[5]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 79, 7, 8, 9, 10, 80, 12, 13, 14, 81, po);
				Animations(dt, camera, 4, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, po);
				Animations(dt, camera, 5, 82, 83, 84, 85, 86, 87, 117, 89, 90, 91, 92, 118, 94, 95, 96, 119, po);
				
				if (Anim[0] == 79 && Anim[4] == 69 && Anim[5] == 117)
					Postac.Speed[0] = 1.01f;
				else
					Postac.Speed[0] = 1.0f;

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[4]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[5]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && weapon[1] && !weapon[2] && bron == 0)
			{
				if (Postac.Ins[Anim[1]].TimePos != Postac.Ins[Anim[3]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[1]].TimePos = Postac.Ins[Anim[3]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[1]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[3]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, po);
				Animations(dt, camera, 1, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, po);
				Animations(dt, camera, 3, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[1]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[3]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && weapon[1] && !weapon[2] && bron == 2)
			{
				if (Postac.Ins[Anim[1]].TimePos != Postac.Ins[Anim[4]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[1]].TimePos = Postac.Ins[Anim[4]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[1]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[4]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 79, 7, 8, 9, 10, 80, 12, 13, 14, 81, po);
				Animations(dt, camera, 1, 16, 17, 18, 19, 20, 21, 120, 23, 24, 25, 26, 121, 28, 29, 30, 122, po);
				Animations(dt, camera, 4, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, po);

				if (Anim[0] == 79 && Anim[1] == 120 && Anim[4] == 69)
					Postac.Speed[0] = 1.01f;
				else
					Postac.Speed[0] = 1.0f;

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[1]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[4]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && weapon[1] && !weapon[2] && bron == 1)
			{
				if (Postac.Ins[Anim[2]].TimePos != Postac.Ins[Anim[3]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[2]].TimePos = Postac.Ins[Anim[3]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[2]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[3]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 46, 7, 8, 9, 10, 8, 12, 13, 14, 46, po);
				Animations(dt, camera, 2, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 40, 43, 44, 45, 38, po);
				Animations(dt, camera, 3, 47, 48, 49, 50, 51, 52, 123, 54, 55, 56, 57, 55, 59, 60, 61, 123, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[2]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[3]].Update(dt, Postac.Speed[0]);

				if (Anim[7] == 127 && Postac.Ins[127].TimePos >= 0.4f && Postac.Ins[127].TimePos <= 1.3f)
				{
					DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[127], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);

					if (Postac.Ins[127].TimePos > 1.25f && Postac.Ins[127].TimePos <= 1.3f)
						Shooting = TRUE;
				}
				Postac.Ins[127].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && !weapon[1] && weapon[2] && bron == 0)
			{
				if (Postac.Ins[Anim[1]].TimePos != Postac.Ins[Anim[5]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[1]].TimePos = Postac.Ins[Anim[5]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[1]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[5]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, po);
				Animations(dt, camera, 1, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, po);
				Animations(dt, camera, 5, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[1]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[5]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && !weapon[1] && weapon[2] && bron == 3)
			{
				if (Postac.Ins[Anim[1]].TimePos != Postac.Ins[Anim[6]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[1]].TimePos = Postac.Ins[Anim[6]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[1]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[6]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 113, 7, 8, 9, 10, 8, 12, 13, 14, 114, po);
				Animations(dt, camera, 1, 16, 17, 18, 19, 20, 21, 124, 23, 24, 25, 26, 24, 28, 29, 30, 125, po);
				Animations(dt, camera, 6, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 106, 109, 110, 111, 112, po);

				if (Anim[0] == 113 && Anim[1] == 124 && Anim[6] == 104)
					Postac.Speed[0] = 1.01f;
				else
					Postac.Speed[0] = 1.0f;

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[1]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[6]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && !weapon[1] && weapon[2] && bron == 1)
			{
				if (Postac.Ins[Anim[2]].TimePos != Postac.Ins[Anim[5]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[2]].TimePos = Postac.Ins[Anim[5]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[2]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[5]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 46, 7, 8, 9, 10, 8, 12, 13, 14, 46, po);
				Animations(dt, camera, 2, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 40, 43, 44, 45, 38, po);
				Animations(dt, camera, 5, 82, 83, 84, 85, 86, 87, 126, 89, 90, 91, 92, 90, 94, 95, 96, 126, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[2]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[5]].Update(dt, Postac.Speed[0]);

				if (Anim[7] == 127 && Postac.Ins[127].TimePos >= 0.4f && Postac.Ins[127].TimePos <= 1.3f)
				{
					DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[127], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);

					if (Postac.Ins[127].TimePos > 1.25f && Postac.Ins[127].TimePos <= 1.3f)
						Shooting = TRUE;
				}
				Postac.Ins[127].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && weapon[1] && weapon[2] && bron == 0)
			{
				if (Postac.Ins[Anim[1]].TimePos != Postac.Ins[Anim[3]].TimePos != Postac.Ins[Anim[5]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[1]].TimePos = Postac.Ins[Anim[3]].TimePos = Postac.Ins[Anim[5]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[1]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[3]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[5]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, po);
				Animations(dt, camera, 1, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, po);
				Animations(dt, camera, 3, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, po);
				Animations(dt, camera, 5, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[1]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[3]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[5]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && weapon[1] && weapon[2] && bron == 3)
			{
				if (Postac.Ins[Anim[1]].TimePos != Postac.Ins[Anim[3]].TimePos != Postac.Ins[Anim[6]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[1]].TimePos = Postac.Ins[Anim[3]].TimePos = Postac.Ins[Anim[6]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[1]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[3]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[6]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 113, 7, 8, 9, 10, 8, 12, 13, 14, 114, po);
				Animations(dt, camera, 1, 16, 17, 18, 19, 20, 21, 124, 23, 24, 25, 26, 24, 28, 29, 30, 125, po);
				Animations(dt, camera, 3, 47, 48, 49, 50, 51, 52, 115, 54, 55, 56, 57, 55, 59, 60, 61, 116, po);
				Animations(dt, camera, 6, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 106, 109, 110, 111, 112, po);

				if (Anim[0] == 113 && Anim[1] == 124 && Anim[3] == 115 && Anim[6] == 104)
					Postac.Speed[0] = 1.01f;
				else
					Postac.Speed[0] = 1.0f;

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[1]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[3]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[6]].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && weapon[1] && weapon[2] && bron == 1)
			{
				if (Postac.Ins[Anim[2]].TimePos != Postac.Ins[Anim[3]].TimePos != Postac.Ins[Anim[5]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[2]].TimePos = Postac.Ins[Anim[3]].TimePos = Postac.Ins[Anim[5]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[2]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[3]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[5]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 46, 7, 8, 9, 10, 8, 12, 13, 14, 46, po);
				Animations(dt, camera, 2, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 40, 43, 44, 45, 38, po);
				Animations(dt, camera, 3, 47, 48, 49, 50, 51, 52, 123, 54, 55, 56, 57, 55, 59, 60, 61, 123, po);
				Animations(dt, camera, 5, 82, 83, 84, 85, 86, 87, 126, 89, 90, 91, 92, 90, 94, 95, 96, 126, po);
				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[2]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[3]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[5]].Update(dt, Postac.Speed[0]);

				if (Anim[7] == 127 && Postac.Ins[127].TimePos >= 0.4f && Postac.Ins[127].TimePos <= 1.3f)
				{
					DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[127], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);

					if (Postac.Ins[127].TimePos > 1.25f && Postac.Ins[127].TimePos <= 1.3f)
						Shooting = TRUE;
				}
				Postac.Ins[127].Update(dt, Postac.Speed[0]);
			}

			else if (weapon[0] && weapon[1] && weapon[2] && bron == 2)
			{
				if (Postac.Ins[Anim[1]].TimePos != Postac.Ins[Anim[4]].TimePos != Postac.Ins[Anim[5]].TimePos != Postac.Ins[Anim[0]].TimePos)
					Postac.Ins[Anim[1]].TimePos = Postac.Ins[Anim[4]].TimePos = Postac.Ins[Anim[5]].TimePos = Postac.Ins[Anim[0]].TimePos;

				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[0]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[1]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[4]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Postac.Ins[Anim[5]], jas_sw, jas_diff, { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) }, false);
				Animations(dt, camera, 0, 0, 1, 2, 3, 4, 5, 79, 7, 8, 9, 10, 80, 12, 13, 14, 81, po);
				Animations(dt, camera, 1, 16, 17, 18, 19, 20, 21, 120, 23, 24, 25, 26, 121, 28, 29, 30, 122, po);
				Animations(dt, camera, 4, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, po);
				Animations(dt, camera, 5, 82, 83, 84, 85, 86, 87, 117, 89, 90, 91, 92, 118, 94, 95, 96, 119, po);

				if (Anim[0] == 79 && Anim[1] == 120 && Anim[4] == 69 && Anim[5] == 117)
					Postac.Speed[0] = 1.01f;
				else
					Postac.Speed[0] = 1.0f;

				Postac.Ins[Anim[0]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[1]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[4]].Update(dt, Postac.Speed[0]);
				Postac.Ins[Anim[5]].Update(dt, Postac.Speed[0]);
			}

			for (INT i = 0; i < IloElLab; i++)
			{
				if (tab[i] == 8)
				{
					DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Potwor[0][i].Ins[Potwor[0][i].Anim], jas_sw, jas_diff, { 0.06f, 0.06f, 0.06f }, { Potwor[0][i].Kat[0], -0.5f, 1.5f }, Potwor[0][i].Pozycja[0], false);

					if (Potwor[0][i].Anim == 7 || Potwor[0][i].Anim == 9)
						Potwor[0][i].Speed[0] = 1.014f;
					else
						Potwor[0][i].Speed[0] = 1.0f;

					Potwor[0][i].Ins[Potwor[0][i].Anim].Update(dt, Potwor[0][i].Speed[0]);

					if (po % 2 == 1 && !wygrana && Anim[0] != 14)
					{
						if (Potwor[0][i].Anim == 0 || Potwor[0][i].Anim == 1)
							Potwor[0][i].Ins[Potwor[0][i].Anim].Loop();
					}
				}

				if (tab[i] == 9)
				{
					DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Potwor[1][i].Ins[Potwor[1][i].Anim], jas_sw, jas_diff, { 0.06f, 0.06f, -0.06f }, { Potwor[1][i].Kat[0], 0.5f, -0.5f }, Potwor[1][i].Pozycja[0], false);

					if (Potwor[1][i].Anim == 7 || Potwor[1][i].Anim == 9)
						Potwor[1][i].Speed[0] = 1.014f;
					else
						Potwor[1][i].Speed[0] = 1.0f;

					Potwor[1][i].Ins[Potwor[1][i].Anim].Update(dt, Potwor[1][i].Speed[0]);

					if (po % 2 == 1 && !wygrana && Anim[0] != 14)
					{
						if (Potwor[1][i].Anim == 0 || Potwor[1][i].Anim == 1)
							Potwor[1][i].Ins[Potwor[1][i].Anim].Loop();
					}
				}

				if (tab[i] == 10)
				{
					DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Potwor[2][i].Ins[Potwor[2][i].Anim], jas_sw, jas_diff, { 0.06f, 0.06f, -0.06f }, { Potwor[2][i].Kat[0], 0.5f, -0.5f }, Potwor[2][i].Pozycja[0], false);

					if (Potwor[2][i].Anim == 7 || Potwor[2][i].Anim == 9)
						Potwor[2][i].Speed[0] = 1.014f;
					else
						Potwor[2][i].Speed[0] = 1.0f;

					Potwor[2][i].Ins[Potwor[2][i].Anim].Update(dt, Potwor[2][i].Speed[0]);

					if (po % 2 == 1 && !wygrana && Anim[0] != 14)
					{
						if (Potwor[2][i].Anim == 0 || Potwor[2][i].Anim == 1)
							Potwor[2][i].Ins[Potwor[2][i].Anim].Loop();
					}
				}

				if (tab[i] == 11)
				{
					DrawInsSkin(dc, view, proj, activeSkinnedTech, p, Potwor[3][i].Ins[Potwor[3][i].Anim], jas_sw, jas_diff, { 0.06f, 0.06f, -0.06f }, { Potwor[3][i].Kat[0], 0.5f, -0.5f }, Potwor[3][i].Pozycja[0], false);

					if (Potwor[3][i].Anim == 7 || Potwor[3][i].Anim == 9)
						Potwor[3][i].Speed[0] = 1.014f;
					else
						Potwor[3][i].Speed[0] = 1.0f;

					Potwor[3][i].Ins[Potwor[3][i].Anim].Update(dt, Potwor[3][i].Speed[0]);

					if (po % 2 == 1 && !wygrana && Anim[0] != 14)
					{
						if (Potwor[3][i].Anim == 0 || Potwor[3][i].Anim == 1)
							Potwor[3][i].Ins[Potwor[3][i].Anim].Loop();
					}
				}
			}
		}
	}

	dc->RSSetState(0);
	dc->OMSetDepthStencilState(0, 0);

	ID3D11ShaderResourceView* nullSRV[16] = { 0 };
	dc->PSSetShaderResources(0, 16, nullSRV);
}

void Meshes::DrawStatMeshLab(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Vect Scaling, Vect Angle, Vect Translate, Models Ins, FLOAT jas_sw, FLOAT jas_diff, BOOL enableBlend)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&Ins.World, I);

	XMMATRIX Scale = XMMatrixScaling(Scaling.x1, Scaling.y1, Scaling.z1);
	XMMATRIX RotX = XMMatrixRotationX(Angle.x1*XM_PI);
	XMMATRIX RotY = XMMatrixRotationY(Angle.y1*XM_PI);
	XMMATRIX RotZ = XMMatrixRotationZ(Angle.z1*XM_PI);
	XMMATRIX Trans = XMMatrixTranslation(Translate.x1, Translate.y1, Translate.z1);
	XMStoreFloat4x4(&Ins.World, Scale*RotY*RotX*RotZ*Trans);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (enableBlend)
		dc->OMSetBlendState(mAlphaEBlendState, blendFactor, 0xff00ff);
	else
		dc->OMSetBlendState(mAlphaDBlendState, blendFactor, 0xff00ff);

	XMMATRIX toTexSpace(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX world = XMLoadFloat4x4(&Ins.World);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldViewProj = world * view*proj;

	Effects::NormalMapFX->SetWorld(world);
	Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
	Effects::NormalMapFX->SetWorldViewProjTex(worldViewProj*toTexSpace);
	Effects::NormalMapFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

	for (UINT subset = 0; subset < Ins.Model->SubsetCount; ++subset)
	{
		Ins.Model->Mat[subset].Ambient = XMFLOAT4(jas_sw, jas_sw, jas_sw, jas_sw);
		Ins.Model->Mat[subset].Diffuse = XMFLOAT4(jas_diff, jas_diff, jas_diff, jas_diff);
		Ins.Model->Mat[subset].Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
		Ins.Model->Mat[subset].Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

		Effects::NormalMapFX->SetMaterial(Ins.Model->Mat[subset]);
		Effects::NormalMapFX->SetDiffuseMap(Ins.Model->DiffuseMapSRV[subset]);
		Effects::NormalMapFX->SetNormalMap(Ins.Model->NormalMapSRV[subset]);

		tech->GetPassByIndex(p)->Apply(0, dc);
		Ins.Model->ModelMesh.Draw(dc, subset);
	}
}