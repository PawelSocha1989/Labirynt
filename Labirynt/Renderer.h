#ifndef RENDERER_H
#define RENDERER_H

#include "FX11\inc\d3dx11effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Sky.h"
#include "Napisy.h"
#include "Renders.h"

#include <process.h>
#include <sstream>
#include <iostream>

#include <windows.h>

#define KEYDOWN(name, key) (name[key] & 0x80)
#define BUTTONDOWN(name, key) ( name.rgbButtons[key] & 0x80)
#define SAMPLE_BUFFER_SIZE 16

#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
	return(0);
}
x_input_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
	return(0);
}
x_input_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

class Renderer : public Renders
{
	struct BoundingSphere
	{
		BoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
		XMFLOAT3 Center;
		FLOAT Radius;
	};

public:
	Renderer(HINSTANCE hInstance);
	~Renderer();

	BOOL Init();
	void OnResize();
	void CreateBufferViews();
	void ResizeBuffers();
	void SetFullscreen(UINT width, UINT height, BOOL fullscreen);
	void Resizable();
	void WheelUp();
	void WheelDown();
	void DrawScene(FLOAT dt);
	
	void DetectInput();
	void OnMouseMove(WPARAM btnState, INT x, INT y);
	
	void suwak(Vect Tb, Vect Tl, XMMATRIX &mProj, XMMATRIX &mView, FLOAT ScSuw);

	void BeginD3D();
	void EndD3D();
	void EndzikD3D(XMMATRIX &proj, XMMATRIX &view);
	
	void ObjektyScen12(XMMATRIX &proj, XMMATRIX &view, FLOAT ObjScen12X[2], FLOAT ScGuz);

	void InitKeyboard();
	void DetectInputKeyboard();
	void DestroyKeyboard();
	void InputGamepad();

	void InitMouse();
	void DetectInputMouse();
	void DestroyMouse();

private:
	Napisy napis;

	Sky *mSky;
	Sky *Skydome;

	XMFLOAT4X4 mLightView;
	XMFLOAT4X4 mLightProj;

	DirectionalLight mDirLights[3];
	XMFLOAT3 mOriginalLightDir[3];

	BoundingSphere mSceneBounds;
	POINT mLastMousePos;

	string pliki2[2][17], punkty_string2 = "0", strzaly_string2 = "0", sekundy_s[3], OpRozdz[2][2], fullekr[2];

	INT obr_sciany, lr[2], wczytyw, pierw, kla = 0, nowy2[10], ZaznPoz = 0, WczytGre = 0,

	CzerwonyNapis[18][18], *CzerwonyPliki[2];

	BOOL ustawienia[4], keysPressed[256], ustaw_rozdzielczosc = FALSE, ustaw_fullekran = FALSE, MouseButton[4], frami = FALSE;

	FLOAT jas_diff;

	Vect obr;

	XMFLOAT4X4 mView, mProj;

	LPDIRECTINPUT8 DI;
	LPDIRECTINPUTDEVICE8 DIKeyboard;
	DIMOUSESTATE mouse_state;
	DIMOUSESTATE mouseLastState;
	UCHAR keys[256];
	LPDIRECTINPUTDEVICE8 DIMouse;
	HANDLE	DIMouseEvent;
	RECT R;
};

#endif