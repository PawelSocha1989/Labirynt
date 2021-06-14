#ifndef MESHES_H
#define MESHES_H

#include "SsaoMeshes.h"
#include "d3dApp.h"

class Meshes : public D3DApp, public SsaoMeshes
{
protected:
	TextureMgr mTexMgr;
	UINT mLightCount;

	INT Anim[8], box = 0, trafiony = 0;
	BOOL klawisze[13], walks[4], ShootingArrow = FALSE;
	FLOAT dl_zycia = 0.3f, x_dl_zycia = -1.59f;
	Vect ZywoplotT[4];

public:
	Meshes(HINSTANCE hInstance);
	~Meshes();

	void Init(ID3D11Device *device, ID3D11DeviceContext *dc, ThirdPersonCamera &camera , FLOAT width, FLOAT height);
	void DestroyAll();
	void Animations(FLOAT dt, ThirdPersonCamera &camera, INT Animacja, INT Stojka, INT JogStart, INT Jogging, INT JogStop, INT Run, INT Jump, INT Punch, INT Kick, INT JumpKick, INT JumpToForward, INT KickZPolobrotu, INT JumpKickPunch, INT FlySpinKick, INT Oberwanie, INT Upadek, INT Boxing, INT po);
	
	void CollisionArrowWithHedge(ThirdPersonCamera &camera);
	void CollisionArrowWithMonster(INT model, ThirdPersonCamera &camera, SkinningModels *Potwor);

	void Draw(ID3D11DeviceContext *dc, XMFLOAT4X4 LightView, XMFLOAT4X4 LightProj, ID3D11DepthStencilView* DepthStencilView, D3D11_VIEWPORT ScreenViewport, ID3D11RenderTargetView* RenderTargetView, XMMATRIX &mProj, XMMATRIX &mView, ThirdPersonCamera &camera, Sky *mSky, INT pierw, INT scen, INT obr_sciany, string punkty, string strzaly, string sekundy_s[3], INT po, INT nowy[2], INT *poz_scian, INT kla, string klawiszek, INT pierwszy[31], INT ustaw_klaw[31], BOOL ustaw_rozdzielczosc, string OpcjaRozdzielczosc[2][2], FLOAT rozdzielczosc_w, FLOAT rozdzielczosc_h, BOOL full, string fullekr[2], BOOL ustaw_fullekran, INT scen2, INT wczyt, INT IloscPlikow[2][2], string *pliki[2], string pliki2[2][17], INT zapxx[19], INT CzerwonyNap[17][18], INT *koloreczek[2], FLOAT dt, DirectionalLight DirLights[3], FLOAT jas_sw, FLOAT jas_diff, INT Anim[7], BOOL weapon[3], INT strzalki, FLOAT ScaleMenuTitle, Vect vGraNapy, FLOAT MenuStworzLabX[6], FLOAT OpcjePosX[5], FLOAT Grafika1Pos[4], FLOAT OpcjeDzwiekuX[2], Vect ZywT[4]);

	void DrawInsSkin(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, SkinnedModelInstance Model, FLOAT jas_sw, FLOAT jas_diff, Vect Scaling, Vect Angle, Vect Translate, BOOL enableBlend);
	void DrawNap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Vect FontSize, Vect Position, INT Color, string text, FLOAT jas_sw, FLOAT jas_diff);
	void DrawStatMeshLab(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Vect Scaling, Vect Angle, Vect Translate, Models Ins, FLOAT jas_sw, FLOAT jas_diff, BOOL enableBlend);
	void DrawNapMen1(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony1[3], FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapMen2(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony2[4], FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapMen3(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony3[4], FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapMen4(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj,  ID3DX11EffectTechnique* tech, UINT p, INT czerwony4[3], FLOAT jas_sw, FLOAT jas_diff, FLOAT ScaleMenuTitle);
	void DrawNapMen5(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony5[2], FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapMen6(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony6[2], FLOAT jas_sw, FLOAT jas_diff);
	void DrawGraNap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, string punkty, string strzaly, FLOAT jas_sw, FLOAT jas_diff, Vect vGraNapy);
	void DrawCzas(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, string sekundy_s[3], FLOAT jas_sw, FLOAT jas_diff);
	void DrawMenPomNap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT kolor[6], FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapMenStwLab(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony7[12], INT czerwony8[4], FLOAT jas_sw, FLOAT jas_diff, FLOAT MenuStworzLabX[6]);
	void DrawNapPA(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Vect Pos, FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapPB(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, Vect Pos, FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapSterMen(ID3D11DeviceContext *dc,XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT CzerwonyKlaw[18], string tytul, Vect Pos, FLOAT jas_sw, FLOAT jas_diff, FLOAT OpcjePosX[2]);
	void DrawNapSterMen1(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, FLOAT yklaw, string klawiszek, INT kol, FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapOpGraf(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony9[6], FLOAT jas_sw, FLOAT jas_diff, FLOAT OpcjePosX[5]);
	void DrawNapOpRozdz(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, string OpcjaRozdzielczosc[2][2], INT czer[2], FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapOpGraf1(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, string Rozdz1, string Rozdz2, string fullekran, INT czerw[2], FLOAT jas_sw, FLOAT jas_diff, FLOAT Grafika1Pos[4]);
	void DrawNapOpFullEkr(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT kolo[2], FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapOpDzw(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwony10[5], FLOAT jas_sw, FLOAT jas_diff, FLOAT OpcjePosX[2], FLOAT OpcjeDzwiekuX[2]);
	void DrawNapNieZapZm(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT kolorek[2], FLOAT jas_sw, FLOAT jas_diff);
	void DrawNapNieWszFunKl(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, FLOAT jas_sw, FLOAT jas_diff);
	void DrawWcz(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT ko, FLOAT jas_sw, FLOAT jas_diff);
	void DrawWcz2(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT ilosc, FLOAT wysokosc, string plik, INT koloreczek, FLOAT jas_sw, FLOAT jas_diff);
	void DrawZap(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT czerwonka[2], FLOAT jas_sw, FLOAT jas_diff);
	void DrawZap2(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, FLOAT zapx, string klawiszek, FLOAT jas_sw, FLOAT jas_diff);
	void DrawSter(ID3D11DeviceContext *dc,XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique* tech, UINT p, INT color[4], FLOAT jas_sw, FLOAT jas_diff, FLOAT OpcjePosX);
	void DrawZapGre(ID3D11DeviceContext *dc, XMMATRIX &mProj, XMMATRIX &mView, ID3DX11EffectTechnique *tech, UINT p, FLOAT jas_sw, FLOAT jas_diff, INT *color, INT zapxx[19]);
	void DrawArrowInThere(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique *tech, UINT p, FLOAT jas_sw, FLOAT jas_diff);
	void DrawWygr(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique *tech, UINT p, FLOAT jas_sw, FLOAT jas_diff);
	void DrawKoniecGry(ID3D11DeviceContext *dc, XMMATRIX &view, XMMATRIX &proj, ID3DX11EffectTechnique *tech, UINT p, FLOAT jas_sw, FLOAT jas_diff);
};

#endif