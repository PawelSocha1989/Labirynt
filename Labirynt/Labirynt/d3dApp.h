#ifndef D3DAPP_H
#define D3DAPP_H

#include "d3dUtil.h"
#include "GameTimer.h"
#include <string>

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE AppInst()const;
	HWND      MainWnd()const;
	FLOAT     AspectRatio()const;

	INT Run();
	void BuildDynCubeMapViews();

	virtual BOOL Init();
	virtual void OnResize();
	virtual void DetectInput() = 0;
	virtual void DrawScene(FLOAT dt) = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseMove(WPARAM btnState, INT myszkaX, INT myszkaY) {}
	virtual void WheelUp(){}
	virtual void WheelDown(){}

	FLOAT rozdzielczosc_w, rozdzielczosc_h;
protected:
	BOOL InitMainWindow();
	BOOL InitDirect3D();

	void CalculateFrameStats();

	HINSTANCE mhAppInst;
	HWND      mhMainWnd;
	BOOL      mAppPaused;
	BOOL      mMinimized;
	BOOL      mMaximized;
	BOOL      mResizing;
	UINT      m4xMsaaQuality;

	GameTimer mTimer;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;

	std::wstring mMainWndCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	BOOL mEnable4xMsaa;
	LPCWSTR	  Cursor;
	INT dir;

	ID3D11DepthStencilView *mDynamicCubeMapDSV;
	ID3D11RenderTargetView *mDynamicCubeMapRTV[6];
	ID3D11ShaderResourceView *mDynamicCubeMapSRV;
	D3D11_VIEWPORT mCubeMapViewport;
	static const INT CubeMapSize = 256;
};

#endif