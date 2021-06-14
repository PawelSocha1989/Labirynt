#ifndef SSAOMESHES_H
#define SSAOMESHES_H

#include "d3dApp.h"
#include "FX11\inc\d3dx11effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "TextureMgr.h"
#include "Sky.h"
#include "Ssao.h"
#include "Structures.h"

class SsaoMeshes : public Structures
{
protected:
	Ssao* mSsao;
	INT WczytLabiryntu = 1, ZapiszGiere = 1;
	BOOL Shooting = FALSE, wygrana = FALSE;
	FLOAT WspolFunKwLotuStrzaly[3], *ppTrajShotHedge, *AngleXTrajShotHedge, *yyTrajShotHedge, **ppTrajShot, **AngleXTrajShot, **yyTrajShot, ppTrajShoting, ixic = 45.0f;
	Vect *CTrajShotHedge, **CTrajShot, CTrajShoting, TrajShoting, AngleStrzalka;

public:
	SsaoMeshes();
	~SsaoMeshes();

	void TrajLotuStrzaly();
	void LotStrzaly(ThirdPersonCamera &camera);

	void DrawSsao(ID3D11DeviceContext *dc, XMMATRIX &mProj, XMMATRIX &mView, ThirdPersonCamera &camera, INT pierw, INT scen, INT obr_sciany, string punkty, string strzaly, string sekundy_s[3], INT po, INT nowy[2], INT *poz_scian, INT kla, string klawiszek, INT pierwszy[31], INT ustaw_klaw[31], BOOL ustaw_rozdzielczosc, string OpcjaRozdzielczosc[2][2], FLOAT rozdzielczosc_w, FLOAT rozdzielczosc_h, BOOL full, string fullekr[2], BOOL ustaw_fullekran, INT scen2, INT wczyt, INT IloscPlikow[2][2], string *pliki[2], string pliki2[2][17], INT zapxx[19], INT CzerwonyNap[17][18], INT *koloreczek[2], INT Anim[7], BOOL weapon[3], INT strzalki, FLOAT ScaleMenuTitle, Vect vGraNapy, FLOAT MenuStworzLabX[6], FLOAT OpcjePosX[5], FLOAT Grafika1Pos[4], FLOAT OpcjeDzwiekuX[2], Vect ZywT[4]);

	void DrawSsaoIns(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Models Ins, Vect Scaling, Vect Angle, Vect Translate);
	void DrawSsaoInsSkin(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, SkinnedModelInstance Model, Vect Scaling, Vect Angle, Vect Translate);
	void DrawSsaoNap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT Color, string text, Vect FontSize, Vect Position);
	void DrawSsaoNapMen1(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony1[3]);
	void DrawSsaoNapMen2(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony2[4]);
	void DrawSsaoNapMen3(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony3[4]);
	void DrawSsaoNapMen4(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony4[3], FLOAT ScaleMenuTitle);
	void DrawSsaoNapMen5(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony5[2]);
	void DrawSsaoNapMen6(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony6[2]);
	void DrawSsaoGraNap(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string punkty, string strzaly, Vect vGraNapy);
	void DrawSsaoCzas(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string sekundy_s[3]);
	void DrawSsaoMenPomNap(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT kolor[6]);
	void DrawSsaoNapMenStwLab(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony7[12], INT czerwony8[4], FLOAT MenuStworzLabX[6]);
	void DrawSsaoNapPA(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, Vect Pos);
	void DrawSsaoNapPB(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, Vect Pos);
	void DrawSsaoNapSterMen(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT CzerwonyKlaw[18], string tytul, Vect Pos, FLOAT OpcjePosX[2]);
	void DrawSsaoNapSterMen1(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string klawiszek, INT kol, INT pier, FLOAT yklaw);
	void DrawSsaoNapOpGraf(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony9[6], FLOAT OpcjePosX[5]);
	void DrawSsaoNapOpRozdz(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string OpcjaRozdzielczosc[2][2], INT czer[2]);
	void DrawSsaoNapOpGraf1(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string Rozdz1, string Rozdz2, string fullekran, INT czerw[2], FLOAT Grafika1Pos[4]);
	void DrawSsaoNapOpFullEkr(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT kolo[2]);
	void DrawSsaoNapOpDzw(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony10[5], FLOAT OpcjePosX[2], FLOAT OpcjeDzwiekuX[2]);
	void DrawSsaoNapNieZapZm(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT kolorek[2]);
	void DrawSsaoNapNieWszFunKl(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p);
	void DrawSsaoWcz(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT ko);
	void DrawSsaoWcz2(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string plik, INT koloreczek, FLOAT wysokosc);
	void DrawSsaoZap(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT czerwonka[2]);
	void DrawSsaoZap2(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, string klawiszek, FLOAT zapx);
	void DrawSsaoSter(ID3D11DeviceContext *dc, XMMATRIX &mView, XMMATRIX &mProj, ID3DX11EffectTechnique* tech, UINT p, INT color[4], FLOAT OpcjePosX);
	void DrawSsaoArrowInThere(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p);
	void DrawSsaoWygr(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p);
	void DrawSsaoKoniecGry(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p);
	void DrawSsaoZapGre(ID3D11DeviceContext * dc, XMMATRIX & mView, XMMATRIX & mProj, ID3DX11EffectTechnique * tech, UINT p, INT *color, INT zapxx[19]);
};

#endif