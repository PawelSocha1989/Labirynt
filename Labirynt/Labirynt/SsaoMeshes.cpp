#include "SsaoMeshes.h"

SsaoMeshes::SsaoMeshes() : mSsao(0)
{
	CTrajShotHedge = new Vect[MaxCountArrows];
	ppTrajShotHedge = new FLOAT[MaxCountArrows];
	AngleXTrajShotHedge = new FLOAT[MaxCountArrows];
	yyTrajShotHedge = new FLOAT[MaxCountArrows];

	ppTrajShot = new FLOAT*[IloElLab];
	AngleXTrajShot = new FLOAT*[IloElLab];
	CTrajShot = new Vect*[IloElLab];
	yyTrajShot = new FLOAT*[IloElLab];

	//FLOAT
	for (INT j = 0; j < IloElLab; j++)
	{
		ppTrajShot[j] = new FLOAT[MaxCountArrows];
		AngleXTrajShot[j] = new FLOAT[MaxCountArrows];
		CTrajShot[j] = new Vect[MaxCountArrows];
		yyTrajShot[j] = new FLOAT[MaxCountArrows];
	}

	TrajShoting.x1 = ixic;

	AngleStrzalka.x1 = 1.35f; AngleStrzalka.z1 = 1.35f;

	for (INT i = 0; i < 3; i++)
		WspolFunKwLotuStrzaly[i] = 0.0f;
}

SsaoMeshes::~SsaoMeshes()
{
}

void SsaoMeshes::TrajLotuStrzaly()
{
	FLOAT xm = PozycjaStrzaly.x2 - PozycjaStrzaly.x1;
	FLOAT ym = PozycjaStrzaly.y1;

	WspolFunKwLotuStrzaly[0] = -1 / (-PozycjaStrzaly.x2 * PozycjaStrzaly.x1 + PozycjaStrzaly.x2 * xm + xm * PozycjaStrzaly.x1 - xm * xm) * PozycjaStrzaly.y1;

	WspolFunKwLotuStrzaly[1] = (PozycjaStrzaly.x1 + PozycjaStrzaly.x2) / (-PozycjaStrzaly.x2 * PozycjaStrzaly.x1 + PozycjaStrzaly.x2 * xm + xm * PozycjaStrzaly.x1 - xm * xm) * PozycjaStrzaly.y1;

	WspolFunKwLotuStrzaly[2] = -PozycjaStrzaly.x2 * PozycjaStrzaly.x1 / (-PozycjaStrzaly.x2 * PozycjaStrzaly.x1 + PozycjaStrzaly.x2 * xm + xm * PozycjaStrzaly.x1 - xm * xm) * PozycjaStrzaly.y1;
}

void SsaoMeshes::LotStrzaly(ThirdPersonCamera &camera)
{
	TrajShoting.y1 = (WspolFunKwLotuStrzaly[0] * TrajShoting.x1 * TrajShoting.x1) + (WspolFunKwLotuStrzaly[1] * TrajShoting.x1) + WspolFunKwLotuStrzaly[2];
	ppTrajShoting = (2 * WspolFunKwLotuStrzaly[0] * TrajShoting.x1) + WspolFunKwLotuStrzaly[1];
	FLOAT ArrowAngle = (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson;

	FLOAT ArrowAngle1 = -1.42f, ArrowAngle2 = -1.41f, AngleStrzalkaX1 = 6.15f;
	while (ArrowAngle1 <= -1.14f)
	{
		if (ArrowAngle >= ArrowAngle1 && ArrowAngle < ArrowAngle2)
		{
			AngleStrzalka.x1 = AngleStrzalkaX1;
			AngleStrzalka.z1 = -1.45f;
		}
		ArrowAngle1 += 0.01f;
		ArrowAngle2 += 0.01f;
		AngleStrzalkaX1 -= 0.1f;
	}

	if (ArrowAngle >= -1.13f && ArrowAngle < -1.12f)
	{
		AngleStrzalka.x1 = 3.15f;
		AngleStrzalka.z1 = -1.45f;
	}

	FLOAT ArrowAngle1_1 = -1.12f, ArrowAngle2_1 = -1.11f, AngleStrzalkaX1_1 = 2.95f;
	while (ArrowAngle1_1 <= -1.07f)
	{
		if (ArrowAngle >= ArrowAngle1_1 && ArrowAngle < ArrowAngle2_1)
		{
			AngleStrzalka.x1 = AngleStrzalkaX1_1;
			AngleStrzalka.z1 = -1.45f;
		}
		ArrowAngle1_1 += 0.01f;
		ArrowAngle2_1 += 0.01f;
		AngleStrzalkaX1_1 -= 0.3f;
	}

	if (ArrowAngle >= -1.06f && ArrowAngle < -1.05f)
	{
		AngleStrzalka.x1 = 1.25f;
		AngleStrzalka.z1 = -1.35f;
	}
	else if (ArrowAngle >= -1.05f && ArrowAngle < -1.04f)
	{
		AngleStrzalka.x1 = 1.15f;
		AngleStrzalka.z1 = -1.25f;
	}
	else if (ArrowAngle >= -1.04f && ArrowAngle < -1.03f)
	{
		AngleStrzalka.x1 = 2.05f;
		AngleStrzalka.z1 = -1.05f;
	}
	else if (ArrowAngle >= -1.03f && ArrowAngle < -1.02f)
	{
		AngleStrzalka.x1 = 2.05f;
		AngleStrzalka.z1 = -1.05f;
	}
	else if (ArrowAngle >= -1.02f && ArrowAngle < -1.01f)
	{
		AngleStrzalka.x1 = 2.05f;
		AngleStrzalka.z1 = -0.95f;
	}
	else if (ArrowAngle >= -1.01f && ArrowAngle < -1.00f)
	{
		AngleStrzalka.x1 = 2.35f;
		AngleStrzalka.z1 = -0.85f;
	}
	else if (ArrowAngle >= -1.00f && ArrowAngle < -0.99f)
	{
		AngleStrzalka.x1 = 2.85f;
		AngleStrzalka.z1 = -0.75f;
	}
	else if (ArrowAngle >= -0.99f && ArrowAngle < -0.98f)
	{
		AngleStrzalka.x1 = 2.85f;
		AngleStrzalka.z1 = -0.75f;
	}
	else if (ArrowAngle >= -0.98f && ArrowAngle < -0.97f)
	{
		AngleStrzalka.x1 = 2.85f;
		AngleStrzalka.z1 = -0.65f;
	}
	else if (ArrowAngle >= -0.97f && ArrowAngle < -0.96f)
	{
		AngleStrzalka.x1 = 2.85f;
		AngleStrzalka.z1 = -0.65f;
	}
	else if (ArrowAngle >= -0.96f && ArrowAngle < -0.95f)
	{
		AngleStrzalka.x1 = 2.85f;
		AngleStrzalka.z1 = -0.65f;
	}
	else if (ArrowAngle >= -0.95f && ArrowAngle < -0.94f)
	{
		AngleStrzalka.x1 = 2.35f;
		AngleStrzalka.z1 = -0.55f;
	}
	else if (ArrowAngle >= -0.94f && ArrowAngle < -0.93f)
	{
		AngleStrzalka.x1 = 2.35f;
		AngleStrzalka.z1 = -0.55f;
	}
	else if (ArrowAngle >= -0.93f && ArrowAngle < -0.92f)
	{
		AngleStrzalka.x1 = 2.05f;
		AngleStrzalka.z1 = -0.45f;
	}
	else if (ArrowAngle >= -0.92f && ArrowAngle < -0.91f)
	{
		AngleStrzalka.x1 = 1.55f;
		AngleStrzalka.z1 = -0.35f;
	}
	else if (ArrowAngle >= -0.91f && ArrowAngle < -0.90f)
	{
		AngleStrzalka.x1 = 1.35f;
		AngleStrzalka.z1 = -0.35f;
	}
	else if (ArrowAngle >= -0.90f && ArrowAngle < -0.89f)
	{
		AngleStrzalka.x1 = 1.35f;
		AngleStrzalka.z1 = -0.25f;
	}
	else if (ArrowAngle >= -0.89f && ArrowAngle < -0.88f)
	{
		AngleStrzalka.x1 = 1.35f;
		AngleStrzalka.z1 = -0.15f;
	}
	else if (ArrowAngle >= -0.88f && ArrowAngle < -0.87f)
	{
		AngleStrzalka.x1 = 1.15f;
		AngleStrzalka.z1 = -0.05f;
	}
	else if (ArrowAngle >= -0.87f && ArrowAngle < -0.86f)
	{
		AngleStrzalka.x1 = 0.95f;
		AngleStrzalka.z1 = -0.05f;
	}
	else if (ArrowAngle >= -0.86f && ArrowAngle < -0.85f)
	{
		AngleStrzalka.x1 = 0.95f;
		AngleStrzalka.z1 = 0.15f;
	}
	else if (ArrowAngle >= -0.85f && ArrowAngle < -0.84f)
	{
		AngleStrzalka.x1 = 0.95f;
		AngleStrzalka.z1 = 0.25f;
	}
	else if (ArrowAngle >= -0.84f && ArrowAngle < -0.83f)
	{
		AngleStrzalka.x1 = 0.95f;
		AngleStrzalka.z1 = 0.35f;
	}
	else if (ArrowAngle >= -0.83f && ArrowAngle < -0.82f)
	{
		AngleStrzalka.x1 = 0.95f;
		AngleStrzalka.z1 = 0.35f;
	}
	else if (ArrowAngle >= -0.82f && ArrowAngle < -0.81f)
	{
		AngleStrzalka.x1 = 0.85f;
		AngleStrzalka.z1 = 0.45f;
	}
	else if (ArrowAngle >= -0.81f && ArrowAngle < -0.80f)
	{
		AngleStrzalka.x1 = 0.85f;
		AngleStrzalka.z1 = 0.65f;
	}
	else if (ArrowAngle >= -0.80f && ArrowAngle < -0.79f)
	{
		AngleStrzalka.x1 = 0.65f;
		AngleStrzalka.z1 = 0.95f;
	}
	else if (ArrowAngle >= -0.79f && ArrowAngle < -0.78f)
	{
		AngleStrzalka.x1 = 0.55f;
		AngleStrzalka.z1 = 1.15f;
	}
	else if (ArrowAngle >= -0.78f && ArrowAngle < -0.77f)
	{
		AngleStrzalka.x1 = 0.45f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.77f && ArrowAngle < -0.76f)
	{
		AngleStrzalka.x1 = 0.45f;
		AngleStrzalka.z1 = 1.65f;
	}
	else if (ArrowAngle >= -0.76f && ArrowAngle < -0.75f)
	{
		AngleStrzalka.x1 = 0.45f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.75f && ArrowAngle < -0.74f)
	{
		AngleStrzalka.x1 = 0.65f;
		AngleStrzalka.z1 = 2.05f;
	}
	else if (ArrowAngle >= -0.74f && ArrowAngle < -0.73f)
	{
		AngleStrzalka.x1 = 0.75f;
		AngleStrzalka.z1 = 2.25f;
	}
	else if (ArrowAngle >= -0.73f && ArrowAngle < -0.72f)
	{
		AngleStrzalka.x1 = 0.95f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.72f && ArrowAngle < -0.71f)
	{
		AngleStrzalka.x1 = 1.05f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.71f && ArrowAngle < -0.70f)
	{
		AngleStrzalka.x1 = 1.15f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.70f && ArrowAngle < -0.69f)
	{
		AngleStrzalka.x1 = 1.35f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.69f && ArrowAngle < -0.68f)
	{
		AngleStrzalka.x1 = 1.55f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.68f && ArrowAngle < -0.67f)
	{
		AngleStrzalka.x1 = 1.65f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.67f && ArrowAngle < -0.66f)
	{
		AngleStrzalka.x1 = 1.85f;
		AngleStrzalka.z1 = 2.05f;
	}
	else if (ArrowAngle >= -0.66f && ArrowAngle < -0.65f)
	{
		AngleStrzalka.x1 = 1.95f;
		AngleStrzalka.z1 = 2.05f;
	}
	else if (ArrowAngle >= -0.65f && ArrowAngle < -0.64f)
	{
		AngleStrzalka.x1 = 2.05f;
		AngleStrzalka.z1 = 2.05f;
	}
	else if (ArrowAngle >= -0.64f && ArrowAngle < -0.63f)
	{
		AngleStrzalka.x1 = 2.15f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.63f && ArrowAngle < -0.62f)
	{
		AngleStrzalka.x1 = 2.25f;
		AngleStrzalka.z1 = 2.15f;
	}
	else if (ArrowAngle >= -0.62f && ArrowAngle < -0.61f)
	{
		AngleStrzalka.x1 = 2.35f;
		AngleStrzalka.z1 = 1.95f;
	}
	else if (ArrowAngle >= -0.61f && ArrowAngle < -0.60f)
	{
		AngleStrzalka.x1 = 2.45f;
		AngleStrzalka.z1 = 1.95f;
	}
	else if (ArrowAngle >= -0.60f && ArrowAngle < -0.59f)
	{
		AngleStrzalka.x1 = 2.55f;
		AngleStrzalka.z1 = 1.95f;
	}
	else if (ArrowAngle >= -0.59f && ArrowAngle < -0.58f)
	{
		AngleStrzalka.x1 = 2.65f;
		AngleStrzalka.z1 = 1.95f;
	}
	else if (ArrowAngle >= -0.58f && ArrowAngle < -0.57f)
	{
		AngleStrzalka.x1 = 2.75f;
		AngleStrzalka.z1 = 1.95f;
	}
	else if (ArrowAngle >= -0.57f && ArrowAngle < -0.56f)
	{
		AngleStrzalka.x1 = 2.85f;
		AngleStrzalka.z1 = 1.95f;
	}
	else if (ArrowAngle >= -0.56f && ArrowAngle < -0.55f)
	{
		AngleStrzalka.x1 = 2.95f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.55f && ArrowAngle < -0.54f)
	{
		AngleStrzalka.x1 = 3.05f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.54f && ArrowAngle < -0.53f)
	{
		AngleStrzalka.x1 = 3.05f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.53f && ArrowAngle < -0.52f)
	{
		AngleStrzalka.x1 = 3.15f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.52f && ArrowAngle < -0.51f)
	{
		AngleStrzalka.x1 = 3.25f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.51f && ArrowAngle < -0.50f)
	{
		AngleStrzalka.x1 = 3.35f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.50f && ArrowAngle < -0.49f)
	{
		AngleStrzalka.x1 = 3.45f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.49f && ArrowAngle < -0.48f)
	{
		AngleStrzalka.x1 = 3.45f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.48f && ArrowAngle < -0.47f)
	{
		AngleStrzalka.x1 = 3.55f;
		AngleStrzalka.z1 = 1.45f;
	}
	else if (ArrowAngle >= -0.47f && ArrowAngle < -0.46f)
	{
		AngleStrzalka.x1 = 3.65f;
		AngleStrzalka.z1 = 2.25f;
	}
	else if (ArrowAngle >= -0.46f && ArrowAngle < -0.45f)
	{
		AngleStrzalka.x1 = 3.75f;
		AngleStrzalka.z1 = 2.25f;
	}
	else if (ArrowAngle >= -0.45f && ArrowAngle < -0.44f)
	{
		AngleStrzalka.x1 = 3.85f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.44f && ArrowAngle < -0.43f)
	{
		AngleStrzalka.x1 = 3.95f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.43f && ArrowAngle < -0.42f)
	{
		AngleStrzalka.x1 = 4.05f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.42f && ArrowAngle < -0.41f)
	{
		AngleStrzalka.x1 = 4.15f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.41f && ArrowAngle < -0.40f)
	{
		AngleStrzalka.x1 = 4.25f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.40f && ArrowAngle < -0.39f)
	{
		AngleStrzalka.x1 = 4.35f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.39f && ArrowAngle < -0.38f)
	{
		AngleStrzalka.x1 = 4.45f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.38f && ArrowAngle < -0.37f)
	{
		AngleStrzalka.x1 = 4.55f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.37f && ArrowAngle < -0.36f)
	{
		AngleStrzalka.x1 = 4.75f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.36f && ArrowAngle < -0.35f)
	{
		AngleStrzalka.x1 = 4.95f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.35f && ArrowAngle < -0.34f)
	{
		AngleStrzalka.x1 = 5.15f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.34f && ArrowAngle < -0.33f)
	{
		AngleStrzalka.x1 = 5.25f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.33f && ArrowAngle < -0.32f)
	{
		AngleStrzalka.x1 = 5.35f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.32f && ArrowAngle < -0.31f)
	{
		AngleStrzalka.x1 = 5.65f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.31f && ArrowAngle < -0.30f)
	{
		AngleStrzalka.x1 = 5.85f;
		AngleStrzalka.z1 = 3.05f;
	}
	else if (ArrowAngle >= -0.30f && ArrowAngle < -0.29f)
	{
		AngleStrzalka.x1 = 6.15f;
		AngleStrzalka.z1 = 3.15f;
	}
	else if (ArrowAngle >= -0.29f && ArrowAngle < -0.28f)
	{
		AngleStrzalka.x1 = 6.55f;
		AngleStrzalka.z1 = 3.15f;
	}
	else if (ArrowAngle >= -0.28f && ArrowAngle < -0.27f)
	{
		AngleStrzalka.x1 = 7.15f;
		AngleStrzalka.z1 = 3.15f;
	}
	else if (ArrowAngle >= -0.27f && ArrowAngle < -0.26f)
	{
		AngleStrzalka.x1 = 7.05f;
		AngleStrzalka.z1 = 3.85f;
	}
	else if (ArrowAngle >= -0.26f && ArrowAngle < -0.25f)
	{
		AngleStrzalka.x1 = 7.25f;
		AngleStrzalka.z1 = 4.45f;
	}
	else if (ArrowAngle >= -0.25f && ArrowAngle < -0.24f)
	{
		AngleStrzalka.x1 = 7.25f;
		AngleStrzalka.z1 = 4.75f;
	}
	else if (ArrowAngle >= -0.24f && ArrowAngle < -0.23f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 5.05f;
	}
	else if (ArrowAngle >= -0.23f && ArrowAngle < -0.22f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 5.15f;
	}
	else if (ArrowAngle >= -0.22f && ArrowAngle < -0.21f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 5.35f;
	}
	else if (ArrowAngle >= -0.21f && ArrowAngle < -0.20f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 5.45f;
	}
	else if (ArrowAngle >= -0.20f && ArrowAngle < -0.19f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 5.55f;
	}
	else if (ArrowAngle >= -0.19f && ArrowAngle < -0.18f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 5.75f;
	}
	else if (ArrowAngle >= -0.18f && ArrowAngle < -0.17f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 5.85f;
	}
	else if (ArrowAngle >= -0.17f && ArrowAngle < -0.16f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.05f;
	}
	else if (ArrowAngle >= -0.16f && ArrowAngle < -0.15f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.15f;
	}
	else if (ArrowAngle >= -0.15f && ArrowAngle < -0.14f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.25f;
	}
	else if (ArrowAngle >= -0.14f && ArrowAngle < -0.13f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.35f;
	}
	else if (ArrowAngle >= -0.13f && ArrowAngle < -0.12f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.45f;
	}
	else if (ArrowAngle >= -0.12f && ArrowAngle < -0.11f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.55f;
	}
	else if (ArrowAngle >= -0.11f && ArrowAngle < -0.10f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.65f;
	}
	else if (ArrowAngle >= -0.10f && ArrowAngle < -0.09f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.75f;
	}
	else if (ArrowAngle >= -0.09f && ArrowAngle < -0.08f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.75f;
	}
	else if (ArrowAngle >= -0.08f && ArrowAngle < -0.07f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.85f;
	}
	else if (ArrowAngle >= -0.07f && ArrowAngle < -0.06f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 6.95f;
	}
	else if (ArrowAngle >= -0.06f && ArrowAngle < -0.05f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.05f;
	}
	else if (ArrowAngle >= -0.05f && ArrowAngle < -0.04f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.15f;
	}
	else if (ArrowAngle >= -0.04f && ArrowAngle < -0.03f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.15f;
	}
	else if (ArrowAngle >= -0.03f && ArrowAngle < -0.02f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.25f;
	}
	else if (ArrowAngle >= -0.02f && ArrowAngle < -0.01f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.25f;
	}
	else if (ArrowAngle >= -0.01f && ArrowAngle < 0.00f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.35f;
	}
	else if (ArrowAngle >= 0.00f && ArrowAngle < 0.01f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.45f;
	}
	else if (ArrowAngle >= 0.01f && ArrowAngle < 0.02f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.45f;
	}
	else if (ArrowAngle >= 0.02f && ArrowAngle < 0.03f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.55f;
	}
	else if (ArrowAngle >= 0.03f && ArrowAngle < 0.04f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 7.65f;
	}

	FLOAT ArrowAngle1_2 = 0.04f, ArrowAngle2_2 = 0.05f, AngleStrzalkaZ1_2 = 7.65f;
	while (ArrowAngle1_2 <= 0.13f)
	{
		if (ArrowAngle >= ArrowAngle1_2 && ArrowAngle < ArrowAngle2_2)
		{
			AngleStrzalka.x1 = 7.45f;
			AngleStrzalka.z1 = AngleStrzalkaZ1_2;
		}
		ArrowAngle1_2 += 0.01f;
		ArrowAngle2_2 += 0.01f;
		AngleStrzalkaZ1_2 += 0.1f;
	}

	if (ArrowAngle >= 0.14f && ArrowAngle < 0.15f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 8.75f;
	}
	else if (ArrowAngle >= 0.15f && ArrowAngle < 0.16f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 8.85f;
	}
	else if (ArrowAngle >= 0.16f && ArrowAngle < 0.17f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 9.05f;
	}
	else if (ArrowAngle >= 0.17f && ArrowAngle < 0.18f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 9.25f;
	}
	else if (ArrowAngle >= 0.18f && ArrowAngle < 0.19f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 9.45f;
	}
	else if (ArrowAngle >= 0.19f && ArrowAngle < 0.20f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 9.65f;
	}
	else if (ArrowAngle >= 0.20f && ArrowAngle < 0.21f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 9.85f;
	}
	else if (ArrowAngle >= 0.21f && ArrowAngle < 0.22f)
	{
		AngleStrzalka.x1 = 7.45f;
		AngleStrzalka.z1 = 10.05f;
	}
	else if (ArrowAngle >= 0.22f && ArrowAngle < 0.23f)
	{
		AngleStrzalka.x1 = 7.75f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.23f && ArrowAngle < 0.24f)
	{
		AngleStrzalka.x1 = 8.15f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.24f && ArrowAngle < 0.25f)
	{
		AngleStrzalka.x1 = 8.55f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.25f && ArrowAngle < 0.26f)
	{
		AngleStrzalka.x1 = 8.85f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.26f && ArrowAngle < 0.27f)
	{
		AngleStrzalka.x1 = 9.05f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.27f && ArrowAngle < 0.28f)
	{
		AngleStrzalka.x1 = 9.25f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.28f && ArrowAngle < 0.29f)
	{
		AngleStrzalka.x1 = 9.45f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.29f && ArrowAngle < 0.30f)
	{
		AngleStrzalka.x1 = 9.65f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.30f && ArrowAngle < 0.31f)
	{
		AngleStrzalka.x1 = 9.75f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.31f && ArrowAngle < 0.32f)
	{
		AngleStrzalka.x1 = 9.85f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.32f && ArrowAngle < 0.33f)
	{
		AngleStrzalka.x1 = 9.95f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.33f && ArrowAngle < 0.34f)
	{
		AngleStrzalka.x1 = 10.05f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.34f && ArrowAngle < 0.35f)
	{
		AngleStrzalka.x1 = 10.15f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.35f && ArrowAngle < 0.36f)
	{
		AngleStrzalka.x1 = 10.35f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.36f && ArrowAngle < 0.37f)
	{
		AngleStrzalka.x1 = 10.45f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.37f && ArrowAngle < 0.38f)
	{
		AngleStrzalka.x1 = 10.55f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.38f && ArrowAngle < 0.39f)
	{
		AngleStrzalka.x1 = 10.65f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.39f && ArrowAngle < 0.40f)
	{
		AngleStrzalka.x1 = 10.75f;
		AngleStrzalka.z1 = 10.35f;
	}
	else if (ArrowAngle >= 0.40f)
	{
		AngleStrzalka.x1 = 10.75f;
		AngleStrzalka.z1 = 10.35f;
	}

	CTrajShoting.x1 = ((TrajShoting.x1 - 44.0f) * sinf((camera.charDirAngle + AngleStrzalka.x1 - XM_PI) / camera.AnglePerson)) + (XMVectorGetX(camera.charPosition));
	CTrajShoting.z1 = ((TrajShoting.x1 - 44.0f) * cosf((camera.charDirAngle + AngleStrzalka.z1 - XM_PI) / camera.AnglePerson)) + (XMVectorGetZ(camera.charPosition));
}

void SsaoMeshes::DrawSsaoInsSkin(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, SkinnedModelInstance Model, Vect Scaling, Vect Angle, Vect Translate)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&Model.World, I);
	XMMATRIX Scale = XMMatrixScaling(Scaling.x1, Scaling.y1, Scaling.z1);
	XMMATRIX RotX = XMMatrixRotationX(Angle.x1*XM_PI);
	XMMATRIX RotY = XMMatrixRotationY(Angle.y1*XM_PI);
	XMMATRIX RotZ = XMMatrixRotationZ(Angle.z1*XM_PI);
	XMMATRIX Trans = XMMatrixTranslation(Translate.x1, Translate.y1, Translate.z1);
	XMStoreFloat4x4(&Model.World, Scale*RotY*RotX*RotZ*Trans);

	XMMATRIX world = XMLoadFloat4x4(&Model.World);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldView = world*view;
	XMMATRIX worldInvTransposeView = worldInvTranspose*view;
	XMMATRIX worldViewProj = world*view*proj;

	Effects::SsaoNormalDepthFX->SetWorldView(worldView);
	Effects::SsaoNormalDepthFX->SetWorldInvTransposeView(worldInvTransposeView);
	Effects::SsaoNormalDepthFX->SetWorldViewProj(worldViewProj);
	Effects::SsaoNormalDepthFX->SetTexTransform(XMMatrixIdentity());
	Effects::SsaoNormalDepthFX->SetBoneTransforms(&Model.FinalTransforms[0], Model.FinalTransforms.size());

	tech->GetPassByIndex(p)->Apply(0, dc);
	for (UINT subset = 0; subset < Model.Model->SubsetCount; ++subset)
		Model.Model->ModelMesh.Draw(dc, subset);
}

void SsaoMeshes::DrawSsaoIns(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Models Ins, Vect Scaling, Vect Angle, Vect Translate)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&Ins.World, I);
	XMMATRIX Scale = XMMatrixScaling(Scaling.x1, Scaling.y1, Scaling.z1);
	XMMATRIX RotX = XMMatrixRotationX(Angle.x1*XM_PI);
	XMMATRIX RotY = XMMatrixRotationY(Angle.y1*XM_PI);
	XMMATRIX RotZ = XMMatrixRotationZ(Angle.z1*XM_PI);
	XMMATRIX Trans = XMMatrixTranslation(Translate.x1, Translate.y1, Translate.z1);
	XMStoreFloat4x4(&Ins.World, Scale*RotY*RotX*RotZ*Trans);

	XMMATRIX world = XMLoadFloat4x4(&Ins.World);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldView = world*view;
	XMMATRIX worldInvTransposeView = worldInvTranspose*view;
	XMMATRIX worldViewProj = world*view*proj;

	Effects::SsaoNormalDepthFX->SetWorldView(worldView);
	Effects::SsaoNormalDepthFX->SetWorldInvTransposeView(worldInvTransposeView);
	Effects::SsaoNormalDepthFX->SetWorldViewProj(worldViewProj);
	Effects::SsaoNormalDepthFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

	tech->GetPassByIndex(p)->Apply(0, dc);
	for (UINT subset = 0; subset < Ins.Model->SubsetCount; ++subset)
		Ins.Model->ModelMesh.Draw(dc, subset);
}

void SsaoMeshes::DrawSsaoNap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT Color, string text, Vect FontSize, Vect Position)
{
	for (INT i = 0; i < text.size(); i++)
	{
		for (INT zna = 0; zna < Znaki; zna++)
		{
			if ((text[i] == Czarny[zna]->nazwa_klawisza[0] && Color == 0) || (text[i] == Czarny[zna]->nazwa_klawisza[1] && Color == 0))
				DrawSsaoIns(dc, view, proj, tech, p, Czarny[zna][i], FontSize, { 0.0f, 1.0f, 0.0f }, { Position.x1 + (i * (FontSize.x1 / 0.022f)), Position.y1, 2.0f });

			else if ((text[i] == Czerwony[zna]->nazwa_klawisza[0] && Color == 1) || (text[i] == Czerwony[zna]->nazwa_klawisza[1] && Color == 1))
				DrawSsaoIns(dc, view, proj, tech, p, Czerwony[zna][i], FontSize, { 0.0f, 1.0f, 0.0f }, { Position.x1 + (i * (FontSize.x1 / 0.022f)), Position.y1, 2.0f });
		}
	}
}

void SsaoMeshes::DrawSsaoNapMen1(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony1[3])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony1[0], "GRA", { 0.0033f, 0.0033f, 0.0033f }, nap[0][0]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony1[1], "OPCJE", { 0.0033f, 0.0033f, 0.0033f }, nap[0][1]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony1[2], "WYJåCIE", { 0.0033f, 0.0033f, 0.0033f }, nap[0][2]);
}

void SsaoMeshes::DrawSsaoNapMen2(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony2[4])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "GRA", { 0.0033f, 0.0033f, 0.0033f }, nap[1][0]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony2[0], "NOWA GRA", { 0.0022f, 0.0022f, 0.0022f }, nap[1][1]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony2[1], "WCZYTAJ GR ", { 0.0022f, 0.0022f, 0.0022f }, nap[1][2]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony2[2], "STW”RZ LABIRYNT", { 0.0022f, 0.0022f, 0.0022f }, nap[1][3]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony2[3], "POWR”T", { 0.0022f, 0.0022f, 0.0022f }, nap[1][4]);
}

void SsaoMeshes::DrawSsaoNapMen3(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony3[4])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "OPCJE", { 0.0033f, 0.0033f, 0.0033f }, nap[2][0]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony3[0], "DèWI K", { 0.0022f, 0.0022f, 0.0022f }, nap[2][1]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony3[1], "GRAFIKA", { 0.0022f, 0.0022f, 0.0022f }, nap[2][2]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony3[2], "STEROWANIE", { 0.0022f, 0.0022f, 0.0022f }, nap[2][3]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony3[3], "POWR”T", { 0.0022f, 0.0022f, 0.0022f }, nap[2][4]);
}

void SsaoMeshes::DrawSsaoNapMen4(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony4[3], FLOAT ScaleMenuTitle)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "STW”RZ LABIRYNT", { ScaleMenuTitle, ScaleMenuTitle, ScaleMenuTitle }, nap[3][0]);
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "NOWY LABIRYNT", { 0.0022f, 0.0022f, 0.0022f }, nap[3][1]);
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "WCZYTAJ LABIRYNT", { 0.0022f, 0.0022f, 0.0022f }, nap[3][2]);
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "POWR”T", { 0.0022f, 0.0022f, 0.0022f }, nap[3][3]);
}

void SsaoMeshes::DrawSsaoNapMen5(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony5[2])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "WCZYTAJ GR ", { 0.0033f, 0.0033f, 0.0033f }, nap[4][0]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony5[0], "WCZYTAJ", { 0.0022f, 0.0022f, 0.0022f }, nap[4][1]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony5[1], "POWR”T", { 0.0022f, 0.0022f, 0.0022f }, nap[4][2]);
}

void SsaoMeshes::DrawSsaoNapMen6(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony6[2])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "NOWA GRA", { 0.0033f, 0.0033f, 0.0033f }, nap[5][0]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony6[0], "GRAJ", { 0.0022f, 0.0022f, 0.0022f }, nap[5][1]);
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony6[1], "POWR”T", { 0.0022f, 0.0022f, 0.0022f }, nap[5][2]);
}

void SsaoMeshes::DrawSsaoGraNap(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string punkty, string strzaly, Vect vGraNapisy)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, punkty, { 0.0008f, 0.0008f, 0.0008f }, { vGraNapisy.x1, 0.67f });

	if (bron == 1)
		DrawSsaoNap(dc, mView, mProj, tech, p, 0, strzaly, { 0.0008f, 0.0008f, 0.0008f }, { vGraNapisy.x2, 0.52f });
}

void SsaoMeshes::DrawSsaoCzas(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string sekundy_s[3])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, sekundy_s[0], { 0.0008f, 0.0008f, 0.0008f }, { -0.102f, 0.75f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, ":", { 0.0008f, 0.0008f, 0.0008f }, { -0.01f, 0.75f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, sekundy_s[1], { 0.0008f, 0.0008f, 0.0008f }, { 0.02f, 0.75f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, ":", { 0.0008f, 0.0008f, 0.0008f }, { 0.13f, 0.75f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, sekundy_s[2], { 0.0008f, 0.0008f, 0.0008f }, { 0.16f, 0.75f });
}

void SsaoMeshes::DrawSsaoMenPomNap(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT kolor[6])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, kolor[0], "WZN”W", { 0.0008f, 0.0008f, 0.0008f }, { -0.07f, 0.25f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolor[1], "RESTART", { 0.0008f, 0.0008f, 0.0008f }, { -0.1f, 0.15f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolor[2], "WCZYTAJ", { 0.0008f, 0.0008f, 0.0008f }, { -0.1f, 0.05f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolor[3], "ZAPISZ", { 0.0008f, 0.0008f, 0.0008f }, { -0.1f, -0.05f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolor[4], "OPCJE", { 0.0008f, 0.0008f, 0.0008f }, { -0.09f, -0.13f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolor[5], "POWR”∆", { 0.0008f, 0.0008f, 0.0008f }, { -0.11f, -0.225f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolor[5], "DO G£. MENU", { 0.0008f, 0.0008f, 0.0008f }, { -0.18f, -0.3f });
}

void SsaoMeshes::DrawSsaoNapMenStwLab(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony7[12], INT czerwony8[4], FLOAT MenuStworzLabX[6])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "STW”RZ", { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[0], 0.62f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "LABIRYNT", { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[1], 0.49f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony8[0], "NOWY", { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[2], 0.17f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony8[1], "OTW”RZ", { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[3], 0.03f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony8[2], "ZAPISZ", { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[3], -0.1f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony8[3], "POWR”T", { 0.0012f, 0.0012f, 0.0012f }, { MenuStworzLabX[3], -0.75f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony7[10], "A", { 0.0018f, 0.0018f, 0.0018f }, { MenuStworzLabX[4], -0.64f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony7[11], "B", { 0.0018f, 0.0018f, 0.0018f }, { MenuStworzLabX[5], -0.64f });
}

void SsaoMeshes::DrawSsaoNapPA(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, Vect Pos)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "A", { 0.0008f, 0.0008f, 0.0008f }, Pos);
}

void SsaoMeshes::DrawSsaoNapPB(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, Vect Pos)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "B", { 0.0008f, 0.0008f, 0.0008f }, Pos);
}

void SsaoMeshes::DrawSsaoNapSterMen(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT CzerwonyKlaw[18], string tytul, Vect Pos, FLOAT OpcjePosX[2])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, tytul, { 0.0022f, 0.0022f, 0.0022f }, Pos);
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "FUNKCJA", { 0.0012f, 0.0012f, 0.0012f }, { -0.8f, 0.56f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "KLAWISZ", { 0.0012f, 0.0012f, 0.0012f }, { 0.25f, 0.56f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[0], "IDè DO PRZODU", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.49f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[1], "IDè DO TY£U", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.42f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[2], "IDè W LEWO", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.35f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[3], "IDè W PRAWO", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.28f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[4], "PATRZ W G”R ", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.21f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[5], "PATRZ W D”£", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.14f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[6], "PATRZ W LEWO", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.07f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[7], "PATRZ W PRAWO", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, 0.0f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[8], "BIEGNIJ", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.07f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[9], "CIOS", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.14f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[10], "KOPNIAK", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.21f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[11], "SKACZ", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.28f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[12], "CELOWNIK", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.35f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[13], "POPRZEDNIA BRO—", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.42f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[14], "NAST PNA BRO—", { 0.0008f, 0.0008f, 0.0008f }, { -0.81f, -0.49f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[15], "USTAWIENIA", { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.7f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[15], "PODSTAWOWE", { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.8f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[16], "ZAPISZ", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.7f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[16], "ZMIANY", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.8f });
	DrawSsaoNap(dc, mView, mProj, tech, p, CzerwonyKlaw[17], "POWR”T", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[1], -0.8f });
}

void SsaoMeshes::DrawSsaoNapSterMen1(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string klawiszek, INT kol, INT pier, FLOAT yklaw)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, kol, klawiszek, { 0.0008f, 0.0008f, 0.0008f }, { 0.26f, 0.49f - yklaw });
}

void SsaoMeshes::DrawSsaoNapOpGraf(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony9[6], FLOAT OpcjePosX[5])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "GRAFIKA", { 0.0022f, 0.0022f, 0.0022f }, { -0.3f, 0.66f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony9[0], "ROZDZIELCZOå∆", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[2], 0.1f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony9[1], "PE£NY EKRAN", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[3], -0.07f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony9[2], "JASNOå∆", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[4], -0.24f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony9[3], "USTAWIENIA", { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.7f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony9[3], "PODSTAWOWE", { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.8f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony9[4], "ZAPISZ", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.7f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony9[4], "ZMIANY", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.8f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony9[5], "POWR”T", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[1], -0.8f });
}

void SsaoMeshes::DrawSsaoNapOpRozdz(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string OpcjaRozdzielczosc[2][2], INT czer[2])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, czer[0], OpcjaRozdzielczosc[0][0], { 0.0012f, 0.0012f, 0.0012f }, { -0.2f, 0.06f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czer[0], "X", { 0.0008f, 0.0008f, 0.0008f }, { 0.02f, 0.06f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czer[0], OpcjaRozdzielczosc[0][1], { 0.0012f, 0.0012f, 0.0012f }, { 0.08f, 0.06f });

	DrawSsaoNap(dc, mView, mProj, tech, p, czer[1], OpcjaRozdzielczosc[1][0], { 0.0012f, 0.0012f, 0.0012f }, { -0.2f, -0.105f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czer[1], "X", { 0.0008f, 0.0008f, 0.0008f }, { 0.02f, -0.105f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czer[1], OpcjaRozdzielczosc[1][1], { 0.0012f, 0.0012f, 0.0012f }, { 0.08f, -0.105f });
}

void SsaoMeshes::DrawSsaoNapOpGraf1(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string Rozdz1, string Rozdz2, string fullekran, INT czerw[2], FLOAT Grafika1Pos[4])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, czerw[0], Rozdz1, { 0.0012f, 0.0012f, 0.0012f }, { Grafika1Pos[0], 0.09f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerw[0], "X", { 0.0008f, 0.0008f, 0.0008f }, { Grafika1Pos[1], 0.09f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerw[0], Rozdz2, { 0.0012f, 0.0012f, 0.0012f }, { Grafika1Pos[2], 0.09f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerw[1], fullekran, { 0.0012f, 0.0012f, 0.0012f }, { Grafika1Pos[3], -0.07f });
}

void SsaoMeshes::DrawSsaoNapOpFullEkr(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT kolo[2])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, kolo[1], "W£", { 0.0012f, 0.0012f, 0.0012f }, { -0.02f, 0.06f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolo[2], "WY£", { 0.0012f, 0.0012f, 0.0012f }, { -0.05f, -0.1f });
}

void SsaoMeshes::DrawSsaoNapOpDzw(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony10[5], FLOAT OpcjePosX[2], FLOAT OpcjeDzwiekuX[2])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "OPCJE DèWI KU", { 0.0022f, 0.0022f, 0.0022f }, { -0.6f, 0.63f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony10[0], "G£OåNOå∆ MUZYKI", { 0.0012f, 0.0012f, 0.0012f }, { OpcjeDzwiekuX[0], -0.04f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony10[1], "G£OåNOå∆ EFEKT”W", { 0.0012f, 0.0012f, 0.0012f }, { OpcjeDzwiekuX[1], -0.23f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony10[2], "USTAWIENIA", { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.7f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony10[2], "PODSTAWOWE", { 0.0012f, 0.0012f, 0.0012f }, { -0.3f, -0.8f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony10[3], "ZAPISZ", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.7f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony10[3], "ZMIANY", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[0], -0.8f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwony10[3], "POWR”T", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX[1], -0.8f });
}

void SsaoMeshes::DrawSsaoNapNieZapZm(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT kolorek[2])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "WSZELKIE NIEZAPISANE ZMIANY", { 0.0015f, 0.0015f, 0.0015f }, { -0.89f, 0.03f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "ZOSTAN• UTRACONE", { 0.0015f, 0.0015f, 0.0015f }, { -0.51f, -0.06f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "KONTYNUOWA∆?", { 0.0015f, 0.0015f, 0.0015f }, { -0.36f, -0.16f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolorek[0], "TAK", { 0.002f, 0.002f, 0.002f }, { -0.83f, -0.31f });
	DrawSsaoNap(dc, mView, mProj, tech, p, kolorek[1], "NIE", { 0.002f, 0.002f, 0.002f }, { 0.65f, -0.31f });
}

void SsaoMeshes::DrawSsaoNapNieWszFunKl(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "NIEZAPISANO Z POWODU BRAKU", { 0.0015f, 0.0015f, 0.0015f }, { -0.85f, 0.03f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "WYBRANYCH WSZYSTKICH", { 0.0015f, 0.0015f, 0.0015f }, { -0.65f, -0.06f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "FUNKCJI KLAWISZY", { 0.0015f, 0.0015f, 0.0015f }, { -0.51f, -0.16f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 1, "OK", { 0.002f, 0.002f, 0.002f }, { -0.1f, -0.31f });
}

void SsaoMeshes::DrawSsaoWcz(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT ko)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "WCZYTYWANIE", { 0.0012f, 0.0012f, 0.005f }, { -0.28f, 0.59f });
	DrawSsaoNap(dc, mView, mProj, tech, p, ko, "ANULUJ", { 0.0012f, 0.0012f, 0.005f }, { -0.12f, -0.67f });
}

void SsaoMeshes::DrawSsaoWcz2(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string plik, INT koloreczek, FLOAT wysokosc)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, koloreczek, plik, { 0.0008f, 0.0008f, 0.005f }, { -0.28f, 0.48f - wysokosc });
}

void SsaoMeshes::DrawSsaoZap(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwonka[2])
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "Zapisywanie", { 0.0012f, 0.0012f, 0.004f }, { -0.28f, 0.12f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "Zapisz jako:", { 0.0008f, 0.0008f, 0.003f }, { -0.83f, -0.03f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwonka[0], "Zapisz", { 0.0012f, 0.0012f, 0.002f }, { -0.83f, -0.36f });
	DrawSsaoNap(dc, mView, mProj, tech, p, czerwonka[1], "Anuluj", { 0.0012f, 0.0012f, 0.002f }, { 0.65f, -0.36f });
}

void SsaoMeshes::DrawSsaoZap2(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string klawiszek, FLOAT zapx)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, klawiszek, { 0.0008f, 0.0008f, 0.003f }, { -0.39f + zapx, -0.03f });
}

void SsaoMeshes::DrawSsaoSter(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p, INT color[4], FLOAT OpcjePosX)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "STEROWANIE", { 0.0022f, 0.0022f, 0.0022f }, { -0.46f, 0.66f });
	DrawSsaoNap(dc, mView, mProj, tech, p, color[0], "KLAWIATURA", { 0.0018f, 0.0018f, 0.0018f }, { -1.0f, 0.06f });
	DrawSsaoNap(dc, mView, mProj, tech, p, color[1], "GAMEPAD", { 0.0018f, 0.0018f, 0.0018f }, { 0.45f, 0.06f });
	DrawSsaoNap(dc, mView, mProj, tech, p, color[2] | color[3], "POWR”T", { 0.0012f, 0.0012f, 0.0012f }, { OpcjePosX, -0.8f });
}

void SsaoMeshes::DrawSsaoArrowInThere(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p)
{
	for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
	{
		if (ShowArrowOnHedge[ilo2])
			DrawSsaoIns(dc, mView, mProj, tech, p, StrzalaShot[ilo2], { 1.0f, 1.0f, 1.0f }, { AngleXTrajShotHedge[ilo2], 0.0f, ppTrajShotHedge[ilo2] + 0.4f }, { CTrajShotHedge[ilo2].x1, yyTrajShotHedge[ilo2], CTrajShotHedge[ilo2].z1 });
	}

	for (INT ill = 0; ill < IloElLab; ill++)
	{
		for (INT ilo2 = 0; ilo2 < MaxCountArrows; ilo2++)
		{
			if (ShowArrowOnMonster[ill][ilo2])
				DrawSsaoIns(dc, mView, mProj, tech, p, StrzalaShot[ilo2], { 1.0f, 1.0f, 1.0f }, { AngleXTrajShot[ill][ilo2], 0.0f, ppTrajShot[ill][ilo2] + 0.4f }, { CTrajShot[ill][ilo2].x1, yyTrajShot[ill][ilo2], CTrajShot[ill][ilo2].z1 });
		}
	}
}

void SsaoMeshes::DrawSsaoWygr(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "GRATULACJE", { 0.0033f, 0.0033f, 0.0033f }, { -0.67f, 0.1f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "WYGRA£Eå", { 0.0033f, 0.0033f, 0.0033f }, { -0.52f, -0.15f });
}

void SsaoMeshes::DrawSsaoKoniecGry(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p)
{
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "KONIEC", { 0.0033f, 0.0033f, 0.0033f }, { -0.37f, 0.1f });
	DrawSsaoNap(dc, mView, mProj, tech, p, 0, "GRY", { 0.0033f, 0.0033f, 0.0033f }, { -0.15f, -0.15f });
}

void SsaoMeshes::DrawSsaoZapGre(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique *tech, UINT p, INT *color, INT zapxx[19])
{
	DrawSsaoZap(dc, mView, mProj, tech, p, color);

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
						DrawSsaoZap2(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, zapx);

					zapx += 0.04f;
				}

				else if (zapxx[dlug_zap] == 4)
					zapx += 0.04f;
			}
		}
	}
}

void SsaoMeshes::DrawSsao(ID3D11DeviceContext *dc, XMMATRIX &mProj, XMMATRIX &mView, ThirdPersonCamera &camera, INT pierw, INT scen, INT obr_sciany, string punkty, string strzaly, string sekundy_s[3], INT po, INT nowy[2], INT *poz_scian, INT kla, string klawiszek, INT pierwszy[31], INT ustaw_klaw[31], BOOL ustaw_rozdzielczosc, string OpcjaRozdzielczosc[2][2], FLOAT rozdzielczosc_w, FLOAT rozdzielczosc_h, BOOL full, string fullekr[2], BOOL ustaw_fullekran, INT scen2, INT wczyt, INT IloscPlikow[2][2], string *pliki[2], string pliki2[2][17], INT zapxx[19], INT CzerwonyNapis[17][18], INT *koloreczek[2], INT Anim[7], BOOL weapon[3], INT strzalki, FLOAT ScaleMenuTitle, Vect vGraNapisy, FLOAT MenuStworzLabX[6], FLOAT OpcjePosX[5], FLOAT Grafika1Pos[4], FLOAT OpcjeDzwiekuX[2], Vect ZywT[4])
{
	XMMATRIX 	view = camera.CamView;
	XMMATRIX 	proj = camera.CamProjection;

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);

	ID3DX11EffectTechnique* tech = Effects::SsaoNormalDepthFX->NormalDepthTech;
	ID3DX11EffectTechnique* animatedTech = Effects::SsaoNormalDepthFX->NormalDepthSkinnedTech;

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->IASetInputLayout(InputLayouts::PosNormalTexTan);

	UINT stride = sizeof(Vertex::PosNormalTexTan);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		if (scen == 0)
			DrawSsaoNapMen1(dc, mView, mProj, tech, p, CzerwonyNapis[0]);

		if (scen == 1)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen1(dc, mView, mProj, tech, p, CzerwonyNapis[0]);
				DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
			}

			DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
		}

		if (scen == 2)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen1(dc, mView, mProj, tech, p, CzerwonyNapis[0]);
				DrawSsaoNapMen3(dc, mView, mProj, tech, p, CzerwonyNapis[2]);
			}

			DrawSsaoNapMen3(dc, mView, mProj, tech, p, CzerwonyNapis[2]);
		}

		if (scen == 3)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
				DrawSsaoNapMen1(dc, mView, mProj, tech, p, CzerwonyNapis[0]);
			}

			DrawSsaoNapMen1(dc, mView, mProj, tech, p, CzerwonyNapis[0]);
		}

		if (scen == 4)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen3(dc, mView, mProj, tech, p, CzerwonyNapis[2]);
				DrawSsaoNapMen1(dc, mView, mProj, tech, p, CzerwonyNapis[0]);
			}

			DrawSsaoNapMen1(dc, mView, mProj, tech, p, CzerwonyNapis[0]);
		}

		if (scen == 5)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
				DrawSsaoNapMen4(dc, mView, mProj, tech, p, CzerwonyNapis[3], ScaleMenuTitle);
			}

			DrawSsaoNapMen4(dc, mView, mProj, tech, p, CzerwonyNapis[3], ScaleMenuTitle);
		}

		if (scen == 6)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen4(dc, mView, mProj, tech, p, CzerwonyNapis[3], ScaleMenuTitle);
				DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
			}

			DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
		}

		if (scen == 7)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
				DrawSsaoNapMen5(dc, mView, mProj, tech, p, CzerwonyNapis[4]);
			}

			DrawSsaoNapMen5(dc, mView, mProj, tech, p, CzerwonyNapis[4]);

			if (WczytLabiryntu % 2 == 0)
			{
				DrawSsaoWcz(dc, mView, mProj, tech, p, koloreczek[1][IloscPlikow[1][0]]);

				ilosc[1] = 0;
				FLOAT WysokoscPliki = 0.0f;
				while (ilosc[1] < IloscPlikow[1][0] && ilosc[1] < 17)
				{
					pliki2[1][ilosc[1]] = pliki[1][ilosc[1] + IloscPlikow[1][1]];
					DrawSsaoWcz2(dc, mView, mProj, tech, p, pliki2[1][ilosc[1]], koloreczek[1][ilosc[1]], WysokoscPliki);
					WysokoscPliki += 0.066f;
					ilosc[1]++;
				}
			}
		}

		if (scen == 8)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen5(dc, mView, mProj, tech, p, CzerwonyNapis[4]);
				DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
			}

			DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
		}

		if (scen == 9)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
				DrawSsaoNapMen6(dc, mView, mProj, tech, p, CzerwonyNapis[5]);
			}

			DrawSsaoNapMen6(dc, mView, mProj, tech, p, CzerwonyNapis[5]);
		}

		if (scen == 10)
		{
			if (obr_sciany == 0)
			{
				DrawSsaoNapMen6(dc, mView, mProj, tech, p, CzerwonyNapis[5]);
				DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
			}

			DrawSsaoNapMen2(dc, mView, mProj, tech, p, CzerwonyNapis[1]);
		}

		if (scen == 11)
		{
			DrawSsaoIns(dc, view, proj, tech, p, Podloze, { 100.0f, 100.0f, 100.0f }, { 0.5f, -1.0f, 0.0f }, { 1.0f, 0.0f, 1.0f });

			for (INT i = 0; i < IloElLab; i++)
			{
				if (tab[i] == 1)
				{
					if (pierw == 0)
					{
						if ((tab[i + 1] == 1) && (tab[i + 26] == 1))
							DrawSsaoIns(dc, view, proj, tech, p, Modele[1][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

						else if (tab[i + 1] == 1)
							DrawSsaoIns(dc, view, proj, tech, p, Modele[0][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.5f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

						else if (tab[i + 26] == 1)
							DrawSsaoIns(dc, view, proj, tech, p, Modele[0][i], { 2.0f, 2.0f, 2.0f }, { 0.5f, 0.5f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

						pierw = 1;
					}

					else if ((tab[i + 1] == 1) && (tab[i + 26] == 1) && (tab[i - 1] == 1) && (tab[i - 26] == 1))
						DrawSsaoIns(dc, view, proj, tech, p, Modele[3][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

					else if ((tab[i - 26] == 1) && (tab[i + 26] == 1) && (tab[i + 1] == 1))//lewe te
						DrawSsaoIns(dc, view, proj, tech, p, Modele[2][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.5f, 0.0f }, { boxx[i].x1 + ZywT[0].x1, ZywT[0].y1, boxx[i].z1 + ZywT[0].z1 });

					else if ((tab[i - 26] == 1) && (tab[i + 1] == 1) && (tab[i - 1] == 1))//dolne te
						DrawSsaoIns(dc, view, proj, tech, p, Modele[2][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { boxx[i].x1 + ZywT[1].x1, ZywT[0].y1, boxx[i].z1 + ZywT[1].z1 });

					else if ((tab[i - 26] == 1) && (tab[i + 26] == 1) && (tab[i - 1] == 1))//prawe te
						DrawSsaoIns(dc, view, proj, tech, p, Modele[2][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, -0.5f, 0.0f }, { boxx[i].x1 + ZywT[2].x1, ZywT[0].y1, boxx[i].z1 + ZywT[2].z1 });

					else if ((tab[i + 26] == 1) && (tab[i + 1] == 1) && (tab[i - 1] == 1))//gÛrne te
						DrawSsaoIns(dc, view, proj, tech, p, Modele[2][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 1.0f, 0.0f }, { boxx[i].x1 + ZywT[3].x1, ZywT[0].y1, boxx[i].z1 + ZywT[3].z1 });

					else if ((tab[i - 1] == 1) && (tab[i + 26] == 1))//prawy gÛrny rÛg
						DrawSsaoIns(dc, view, proj, tech, p, Modele[1][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.5f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

					else if ((tab[i + 26] == 1) && (tab[i + 1] == 1))//lewy gÛrny rÛg
						DrawSsaoIns(dc, view, proj, tech, p, Modele[1][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

					else if ((tab[i - 26] == 1) && (tab[i + 1] == 1))//lewy dolny rÛg
						DrawSsaoIns(dc, view, proj, tech, p, Modele[1][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, -0.5f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

					else if ((tab[i - 26] == 1) && (tab[i - 1] == 1))//prawy dolny rÛg
						DrawSsaoIns(dc, view, proj, tech, p, Modele[1][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 1.0f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

					else if ((tab[i - 1] == 1) || (tab[i + 1] == 1))
						DrawSsaoIns(dc, view, proj, tech, p, Modele[0][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.5f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

					else if ((tab[i - 26] == 1) || (tab[i + 26] == 1))
						DrawSsaoIns(dc, view, proj, tech, p, Modele[0][i], { 2.0f, 2.0f, 2.0f }, { 0.5f, 0.5f, 0.0f }, { boxx[i].x1, 7.0f, boxx[i].z1 });

					else if ((tab[i - 26] != 1) && (tab[i + 26] != 1) && (tab[i - 1] != 1) && (tab[i + 1] != 1))
						DrawSsaoIns(dc, view, proj, tech, p, Modele[0][i], { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.5f }, { boxx[i].x1, 7.0f, boxx[i].z1 });
				}

				else if (tab[i] == 4)
					DrawSsaoIns(dc, view, proj, tech, p, Modele[4][i], { 0.104f, 0.104f, 0.104f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 5.0f, boxx[i].z1 });

				else if (tab[i] == 6)
					DrawSsaoIns(dc, view, proj, tech, p, Modele[5][i], { 4.0f, 4.0f, 4.0f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 5.0f, boxx[i].z1 });

				else if (tab[i] == 5)
					DrawSsaoIns(dc, view, proj, tech, p, Modele[6][i], { 0.125f, 0.125f, 0.125f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 6.0f, boxx[i].z1 });

				else if (tab[i] == 7)
					DrawSsaoIns(dc, view, proj, tech, p, Modele[7][i], { 0.016f, 0.016f, 0.016f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 3.0f, boxx[i].z1 });

				else if (tab[i] == 12)
					DrawSsaoIns(dc, view, proj, tech, p, Modele[8][i], { 1.2f, 1.2f, 1.2f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 0.5f, boxx[i].z1 });

				else if (tab[i] == 3)
					DrawSsaoIns(dc, view, proj, tech, p, Modele[9][i], { 0.11f, 0.11f, 0.11f }, { 0.0f, angle, 0.0f }, { boxx[i].x1, 0.6f, boxx[i].z1 });
			}

			if (Shooting && strzalki >= 0)
			{
				TrajLotuStrzaly();
				LotStrzaly(camera);
				DrawSsaoIns(dc, view, proj, tech, p, StrzalaShoting, { 1.0f, 1.0f, 1.0f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, 0.0f, ppTrajShoting + 0.4f }, { CTrajShoting.x1, TrajShoting.y1, CTrajShoting.z1 });
			}

			DrawSsaoArrowInThere(dc, view, proj, tech, p);

			DrawSsaoGraNap(dc, mView, mProj, tech, p, punkty, strzaly, vGraNapisy);
			DrawSsaoCzas(dc, mView, mProj, tech, p, sekundy_s);

			if (Postac.Ins[14].TimePos >= Postac.Ins[14].Model->SkinnedData.GetClipEndTime(Postac.Ins[14].ClipName) && Anim[0] == 14 && po % 2 == 1)
				DrawSsaoKoniecGry(dc, mView, mProj, tech, p);

			if (wygrana && po % 2 == 1)
				DrawSsaoWygr(dc, mView, mProj, tech, p);

			if (po % 2 == 0 && ZapiszGiere % 2 == 1)
				DrawSsaoMenPomNap(dc, mView, mProj, tech, p, CzerwonyNapis[9]);

			else if (po % 2 == 0 && ZapiszGiere % 2 == 0)
				DrawSsaoZapGre(dc, mView, mProj, tech, p, CzerwonyNapis[15], zapxx);
		}

		if (scen == 12)
		{
			DrawSsaoNapMenStwLab(dc, mView, mProj, tech, p, CzerwonyNapis[7], CzerwonyNapis[6], MenuStworzLabX);

			if (nowy[0] == 1)
			{
				if (nowy[1] > 0)
				{
					for (INT g = 1; g < IloPunkt - 1; g++)
					{
						if (poz_scian[g] == 2)
						{
							if (g > 0)
								DrawSsaoNapPA(dc, mView, mProj, tech, p, punkcior[g]);
						}

						else if (poz_scian[g] == 3)
						{
							if (g > 0)
								DrawSsaoNapPB(dc, mView, mProj, tech, p, punkcior[g]);
						}
					}
				}
			}
		}

		if (scen == 13)
			DrawSsaoNapMen4(dc, mView, mProj, tech, p, CzerwonyNapis[3], ScaleMenuTitle);

		if (scen == 14)
		{
			DrawSsaoNapSterMen(dc, mView, mProj, tech, p, CzerwonyNapis[8], "KLAWIATURA I MYSZKA", { -0.96f, 0.66f }, OpcjePosX);

			for (kla = 0; kla < KLA - 25; kla++)
			{
				INT pier = 0; FLOAT ysc = 0.0f;
				while (pier < 15 && ysc < 0.99f)
				{
					if (pierwszy[pier] == 0)
					{
						if (klaw[kla].ustawiony_klaw == pier + 1)
							DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], pier, ysc);
					}

					else if ((pierwszy[pier] == 1 || ustaw_klaw[pier] == 1) || (pierwszy[pier] == 1 && ustaw_klaw[pier] == 1))
					{
						if (klaw[kla].ustawiony_klaw == pier + 1)
							DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], pier, ysc);
					}
					pier++;
					ysc += 0.07f;
				}
			}
		}

		if (scen == 15)
			DrawSsaoNapMen3(dc, mView, mProj, tech, p, CzerwonyNapis[2]);

		if (scen == 16)
		{
			DrawSsaoNapOpGraf(dc, mView, mProj, tech, p, CzerwonyNapis[10], OpcjePosX);

			if (ustaw_rozdzielczosc)
				DrawSsaoNapOpRozdz(dc, mView, mProj, tech, p, OpcjaRozdzielczosc, CzerwonyNapis[13]);

			stringstream Rozdzielczosc1, Rozdzielczosc2;
			Rozdzielczosc1 << rozdzielczosc_w;
			Rozdzielczosc2 << rozdzielczosc_h;

			string Rozdz1 = Rozdzielczosc1.str();
			string Rozdz2 = Rozdzielczosc2.str();

			if (!full)
				DrawSsaoNapOpGraf1(dc, mView, mProj, tech, p, Rozdz1, Rozdz2, fullekr[1], CzerwonyNapis[10], Grafika1Pos);

			else
				DrawSsaoNapOpGraf1(dc, mView, mProj, tech, p, Rozdz1, Rozdz2, fullekr[0], CzerwonyNapis[10], Grafika1Pos);

			if (ustaw_fullekran)
				DrawSsaoNapOpFullEkr(dc, mView, mProj, tech, p, CzerwonyNapis[12]);
		}

		if (scen == 17)
			DrawSsaoNapOpDzw(dc, mView, mProj, tech, p, CzerwonyNapis[11], OpcjePosX, OpcjeDzwiekuX);

		if (scen == 18)
		{
			DrawSsaoNapNieZapZm(dc, mView, mProj, tech, p, CzerwonyNapis[14]);

			if (scen2 == 1)
			{
				DrawSsaoNapSterMen(dc, mView, mProj, tech, p, CzerwonyNapis[8], "KLAWIATURA I MYSZKA", { -0.96f, 0.66f }, OpcjePosX);

				for (kla = 0; kla < KLA - 25; kla++)
				{
					INT pier = 0; FLOAT ysc = 0.0f;
					while (pier < 15 && ysc < 0.99f)
					{
						if (pierwszy[pier] == 0)
						{
							if (klaw[kla].ustawiony_klaw == pier + 1)
								DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], pier, ysc);
						}

						else if ((pierwszy[pier] == 1 || ustaw_klaw[pier] == 1) || (pierwszy[pier] == 1 && ustaw_klaw[pier] == 1))
						{
							if (klaw[kla].ustawiony_klaw == pier + 1)
								DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], pier, ysc);
						}
						pier++;
						ysc += 0.07f;
					}
				}
			}

			else if (scen2 == 2)
			{
				DrawSsaoNapOpGraf(dc, mView, mProj, tech, p, CzerwonyNapis[10], OpcjePosX);

				stringstream Rozdzielczosc1, Rozdzielczosc2;
				Rozdzielczosc1 << rozdzielczosc_w;
				Rozdzielczosc2 << rozdzielczosc_h;

				string Rozdz1 = Rozdzielczosc1.str();
				string Rozdz2 = Rozdzielczosc2.str();

				if (!full)
					DrawSsaoNapOpGraf1(dc, mView, mProj, tech, p, Rozdz1, Rozdz2, fullekr[1], CzerwonyNapis[10], Grafika1Pos);

				else
					DrawSsaoNapOpGraf1(dc, mView, mProj, tech, p, Rozdz1, Rozdz2, fullekr[0], CzerwonyNapis[10], Grafika1Pos);
			}

			else if (scen2 == 3)
				DrawSsaoNapOpDzw(dc, mView, mProj, tech, p, CzerwonyNapis[11], OpcjePosX, OpcjeDzwiekuX);

			else if (scen2 == 4)
			{
				DrawSsaoNapSterMen(dc, mView, mProj, tech, p, CzerwonyNapis[17], "GAMEPAD", { -0.36f, 0.66f }, OpcjePosX);

				for (kla = 86; kla < KLA; kla++)
				{
					INT pier = 0; FLOAT ysc = 0.0f;
					while (pier < 15 && ysc < 0.99f)
					{
						if (pierwszy[pier + 15] == 0)
						{
							if (klaw[kla].ustawiony_klaw == pier + 16)
								DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], pier, ysc);
						}

						else if ((pierwszy[pier + 15] == 1 || ustaw_klaw[pier + 15] == 1) || (pierwszy[pier + 15] == 1 && ustaw_klaw[pier + 15] == 1))
						{
							if (klaw[kla].ustawiony_klaw == pier + 16)
								DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], pier, ysc);
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
				DrawSsaoNapMenStwLab(dc, mView, mProj, tech, p, CzerwonyNapis[7], CzerwonyNapis[6], MenuStworzLabX);

				if (nowy[0] == 1)
				{
					if (nowy[1] > 0)
					{
						for (INT g = 1; g < IloPunkt - 1; g++)
						{
							if (poz_scian[g] == 2)
							{
								if (g > 0)
									DrawSsaoNapPA(dc, mView, mProj, tech, p, punkcior[g]);
							}

							else if (poz_scian[g] == 3)
							{
								if (g > 0)
									DrawSsaoNapPB(dc, mView, mProj, tech, p, punkcior[g]);
							}
						}
					}
				}
			}

			else if (wczyt == 1)
				DrawSsaoNapMen4(dc, mView, mProj, tech, p, CzerwonyNapis[3], ScaleMenuTitle);

			DrawSsaoWcz(dc, mView, mProj, tech, p, koloreczek[0][IloscPlikow[0][0]]);

			ilosc[0] = 0;
			FLOAT WysokoscPliki = 0.0f;
			while (ilosc < IloscPlikow[0] && ilosc[0] < 17)
			{
				pliki2[0][ilosc[0]] = pliki[0][ilosc[0] + IloscPlikow[0][1]];
				DrawSsaoWcz2(dc, mView, mProj, tech, p, pliki2[0][ilosc[0]], koloreczek[0][ilosc[0]], WysokoscPliki);
				WysokoscPliki += 0.066f;
				ilosc[0]++;
			}
		}

		if (scen == 20)
		{
			DrawSsaoNapMenStwLab(dc, mView, mProj, tech, p, CzerwonyNapis[7], CzerwonyNapis[6], MenuStworzLabX);

			if (nowy[0] == 1)
			{
				if (nowy[1] > 0)
				{
					for (INT g = 1; g < IloPunkt - 1; g++)
					{
						if (poz_scian[g] == 2)
						{
							if (g > 0)
								DrawSsaoNapPA(dc, mView, mProj, tech, p, punkcior[g]);
						}

						else if (poz_scian[g] == 3)
						{
							if (g > 0)
								DrawSsaoNapPB(dc, mView, mProj, tech, p, punkcior[g]);
						}
					}
				}
			}

			DrawSsaoZapGre(dc, mView, mProj, tech, p, CzerwonyNapis[15], zapxx);
		}

		if (scen == 21)
		{
			DrawSsaoSter(dc, mView, mProj, tech, p, CzerwonyNapis[16], OpcjePosX[1]);
		}

		if (scen == 22)
		{
			DrawSsaoNapSterMen(dc, mView, mProj, tech, p, CzerwonyNapis[17], "GAMEPAD", { -0.36f, 0.66f }, OpcjePosX);

			for (kla = 86; kla < KLA; kla++)
			{
				INT pier = 0; FLOAT ysc = 0.0f;
				while (pier < 15 && ysc < 0.99f)
				{
					if (pierwszy[pier + 15] == 0)
					{
						if (klaw[kla].ustawiony_klaw == pier + 16)
							DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], pier, ysc);
					}

					else if ((pierwszy[pier + 15] == 1 || ustaw_klaw[pier + 15] == 1) || (pierwszy[pier + 15] == 1 && ustaw_klaw[pier + 15] == 1))
					{
						if (klaw[kla].ustawiony_klaw == pier + 16)
							DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], pier, ysc);
					}
					pier++;
					ysc += 0.07f;
				}
			}
		}

		if (scen == 23)
		{
			DrawSsaoNapNieWszFunKl(dc, mView, mProj, tech, p);

			if (scen2 == 1)
			{
				DrawSsaoNapSterMen(dc, mView, mProj, tech, p, CzerwonyNapis[8], "KLAWIATURA I MYSZKA", { -0.96f, 0.66f }, OpcjePosX);

				for (kla = 0; kla < KLA - 25; kla++)
				{
					INT pier = 0; FLOAT ysc = 0.0f;
					while (pier < 15 && ysc < 0.99f)
					{
						if (pierwszy[pier] == 0)
						{
							if (klaw[kla].ustawiony_klaw == pier + 1)
								DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], pier, ysc);
						}

						else if ((pierwszy[pier] == 1 || ustaw_klaw[pier] == 1) || (pierwszy[pier] == 1 && ustaw_klaw[pier] == 1))
						{
							if (klaw[kla].ustawiony_klaw == pier + 1)
								DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[8][pier], pier, ysc);
						}
						pier++;
						ysc += 0.07f;
					}
				}
			}

			else if (scen2 == 4)
			{
				DrawSsaoNapSterMen(dc, mView, mProj, tech, p, CzerwonyNapis[17], "GAMEPAD", { -0.36f, 0.66f }, OpcjePosX);

				for (kla = 86; kla < KLA; kla++)
				{
					INT pier = 0; FLOAT ysc = 0.0f;
					while (pier < 15 && ysc < 0.99f)
					{
						if (pierwszy[pier + 15] == 0)
						{
							if (klaw[kla].ustawiony_klaw == pier + 16)
								DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], pier, ysc);
						}

						else if ((pierwszy[pier + 15] == 1 || ustaw_klaw[pier + 15] == 1) || (pierwszy[pier + 15] == 1 && ustaw_klaw[pier + 15] == 1))
						{
							if (klaw[kla].ustawiony_klaw == pier + 16)
								DrawSsaoNapSterMen1(dc, mView, mProj, tech, p, klaw[kla].nazwa_klawisza, CzerwonyNapis[17][pier], pier, ysc);
						}
						pier++;
						ysc += 0.07f;
					}
				}
			}
		}
	}

	// Draw the animated CHARacters.
	if (scen == 11)
	{
		dc->IASetInputLayout(InputLayouts::PosNormalTexTan);
		dc->IASetInputLayout(InputLayouts::PosNormalTexTanSkinned);
		animatedTech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			if (!weapon[0] && !weapon[1] && !weapon[2] && bron == 0)
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });

			else if (weapon[0] && !weapon[1] && !weapon[2] && bron == 0)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[1]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (weapon[0] && !weapon[1] && !weapon[2] && bron == 1)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[2]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });

				if (Anim[7] == 127 && Postac.Ins[127].TimePos >= 0.4f && Postac.Ins[127].TimePos <= 1.3f)
				{
					DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[7]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				}
			}

			else if (!weapon[0] && weapon[1] && !weapon[2] && bron == 0)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[3]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (!weapon[0] && weapon[1] && !weapon[2] && bron == 2)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[4]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (!weapon[0] && !weapon[1] && weapon[2] && bron == 0)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[5]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (!weapon[0] && !weapon[1] && weapon[2] && bron == 3)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[6]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (!weapon[0] && weapon[1] && weapon[2] && bron == 0)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[5]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[3]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (!weapon[0] && weapon[1] && weapon[2] && bron == 3)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[6]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[3]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (!weapon[0] && weapon[1] && weapon[2] && bron == 2)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[5]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[4]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (weapon[0] && weapon[1] && !weapon[2] && bron == 0)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[1]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[3]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (weapon[0] && weapon[1] && !weapon[2] && bron == 2)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[4]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[1]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (weapon[0] && weapon[1] && !weapon[2] && bron == 1)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[2]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[3]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });

				if (Anim[7] == 127 && Postac.Ins[127].TimePos >= 0.4f && Postac.Ins[127].TimePos <= 1.3f)
				{
					DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[7]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				}
			}

			else if (weapon[0] && !weapon[1] && weapon[2] && bron == 0)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[1]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[5]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (weapon[0] && !weapon[1] && weapon[2] && bron == 3)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[1]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[6]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (weapon[0] && !weapon[1] && weapon[2] && bron == 1)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[5]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[2]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });

				if (Anim[7] == 127 && Postac.Ins[127].TimePos >= 0.4f && Postac.Ins[127].TimePos <= 1.3f)
				{
					DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[7]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				}
			}

			else if (weapon[0] && weapon[1] && weapon[2] && bron == 0)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[1]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[5]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[3]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (weapon[0] && weapon[1] && weapon[2] && bron == 3)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[1]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[3]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[6]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			else if (weapon[0] && weapon[1] && weapon[2] && bron == 1)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[2]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[5]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[3]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });

				if (Anim[7] == 127 && Postac.Ins[127].TimePos >= 0.4f && Postac.Ins[127].TimePos <= 1.3f)
				{
					DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[7]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				}
			}

			else if (weapon[0] && weapon[1] && weapon[2] && bron == 2)
			{
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[0]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[4]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[1]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
				DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Postac.Ins[Anim[5]], { 0.05f, 0.05f, -0.05f }, { (camera.charDirAngle + AnglePostac.x1 - XM_PI) / camera.AnglePerson, AnglePostac.y1, AnglePostac.z1 }, { XMVectorGetX(camera.charPosition), 0.1f, XMVectorGetZ(camera.charPosition) });
			}

			for (INT i = 0; i < IloElLab; i++)
			{
				if (tab[i] == 8)
					DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Potwor[0][i].Ins[Potwor[0][i].Anim], { 0.06f, 0.06f, 0.06f }, { Potwor[0][i].Kat[0], -0.5f, 1.5f }, Potwor[0][i].Pozycja[0]);

				if (tab[i] == 9)
					DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Potwor[1][i].Ins[Potwor[1][i].Anim], { 0.06f, 0.06f, -0.06f }, { Potwor[1][i].Kat[0], 0.5f, -0.5f }, Potwor[1][i].Pozycja[0]);

				if (tab[i] == 10)
					DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Potwor[2][i].Ins[Potwor[2][i].Anim], { 0.06f, 0.06f, -0.06f }, { Potwor[2][i].Kat[0], 0.5f, -0.5f }, Potwor[2][i].Pozycja[0]);

				if (tab[i] == 11)
					DrawSsaoInsSkin(dc, view, proj, animatedTech, p, Potwor[3][i].Ins[Potwor[3][i].Anim], { 0.06f, 0.06f, -0.06f }, { Potwor[3][i].Kat[0], 0.5f, -0.5f }, Potwor[3][i].Pozycja[0]);
			}
		}
	}

	dc->RSSetState(0);
}