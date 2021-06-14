#include "Renderer.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, INT showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	Renderer theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}

Renderer::Renderer(HINSTANCE hInstance) : Renders(hInstance), mSky(0), Skydome(0)
{
	mSceneBounds.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mSceneBounds.Radius = sqrtf(10.0f*10.0f + 15.0f*15.0f);

	mMainWndCaption = L"Labirynt";

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	//BOOL
	for (INT j = 0; j < 4; j++) 
		ustawienia[j] = FALSE; 

	//string
	for (INT j = 0; j < 3; j++) 
		sekundy_s[j] = "0";

	OpRozdz[0][0] = "0"; OpRozdz[0][1] = "0"; OpRozdz[1][0] = "0"; OpRozdz[1][1] = "0";

	fullekr[0] = "W£"; fullekr[1] = "WY£";

	//INT
	obr_sciany = 0; wczytyw = 0; pierw = 0; lr[0] = 0; lr[1] = 1; 

	//FLOAT
	obr.x1 = 0; obr.y1 = 0; jas_diff = 0.8f; 

	for (INT j = 0; j < 256; j++)
		keysPressed[j] = FALSE;

	for (INT ilo = 0; ilo < 4; ilo++)
		MouseButton[ilo] = TRUE;

	for (INT ilo = 0; ilo < 10; ilo++)
		nowy2[ilo] = 0;

	for (INT ilo = 0; ilo < 18; ilo++)
		for (INT ilo2 = 0; ilo2 < 18; ilo2++)
			CzerwonyNapis[ilo][ilo2] = 0;

	for (INT j = 0; j < 2; j++)
		CzerwonyPliki[j] = new INT[1000];
		
	for (INT j = 0; j < 2; j++)
		for (INT ilo = 0; ilo < 1000; ilo++)
			CzerwonyPliki[j][ilo] = 0;

	for (INT i = 0; i < IloElLab; i++)
	{
		odleglosc[i] = 0.0f;
		tab[i] = 0;
	}

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	mDirLights[0].Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	for (INT i = 0; i < 3; i++)
	{
		mDirLights[i].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
		mOriginalLightDir[i] = mDirLights[i].Direction;
	}
	
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DI, NULL);

	HMODULE XInputLibrary = LoadLibraryA("xinput1_3.dll");
	if (XInputLibrary)
	{
		XInputGetState = (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
		XInputSetState = (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
	}
}

Renderer::~Renderer()
{
	SafeDelete(mSky);
	SafeDelete(Skydome);

	ReleaseCOM(mDynamicCubeMapDSV);
	ReleaseCOM(mDynamicCubeMapSRV);
	for (INT i = 0; i < 6; ++i)
		ReleaseCOM(mDynamicCubeMapRTV[i]);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();

	DestroyAll();
	HR(mSwapChain->SetFullscreenState(FALSE, NULL));

	if (DI)
	{
		DestroyMouse();
		DestroyKeyboard();

		DI->Release();
		DI = NULL;
	}
}

BOOL Renderer::Init()
{
	if (!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);

	InitMouse();
	InitKeyboard();

	BronieInit(jas_sw, jas_diff, md3dDevice);
	CreateBufferQuad(md3dDevice);

	mSky = new Sky(md3dDevice, L"../Release/Textures/szescian.dds");
	Skydome = new Sky(md3dDevice, L"../Release/Textures/Skydome1.dds");

	Meshes::Init(md3dDevice, md3dImmediateContext, TPCam, rozdzielczosc_w, rozdzielczosc_h);
	
	WczytUstDzw();
	WczytUstGraf();
	WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
	WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);

	muza.Initialize(mhMainWnd);
	muza.muza_tlo(glosnosc_tla);

	BuildDynCubeMapViews();

	SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);
	Resizable();

	return true;
}

void Renderer::OnResize()
{
	D3DApp::OnResize();

	if (mSsao)
		mSsao->OnSize((INT)rozdzielczosc_w, (INT)rozdzielczosc_h, 1.0f, 10000.0f);

	TPCam.CamView = XMMatrixLookAtLH(TPCam.CamPosition, TPCam.CamTarget, TPCam.CamUp);
	TPCam.CamProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, rozdzielczosc_w / rozdzielczosc_h, 1.0f, 10000.0f);

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 10000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void Renderer::WheelUp()
{
	if (scen[0] == 7 && WczytLabiryntu % 2 == 0)
	{
		if (IloscPlikow[1][1] > 1)
			IloscPlikow[1][1]--;
	}
		
	if (scen[0] == 11)
		klawa(84);

	if (scen[0] == 12)
	{
		if (nowy[1] > 0)
		{
			w -= 26;
			if (w <= 1) w = 1;
		}
	}

	if (scen[0] == 14)
	{
		ustawianie_klawiszy(84);
		klik[0] = 0;
	}

	if (scen[0] == 16 && ustaw_rozdzielczosc)
	{
		for (INT j = 0; j < 2; j++)
		{
			lr[j]--;
			if (lr[j] <= j) lr[j] = j;
			rozdz[lr[j]].ind = j + 1;
		}
	}

	if (scen[0] == 16 && klik[1] == 1)
	{
		zapis[3] = false;
		jas_sw += 0.1f;
		if (jas_sw >= 2.4f) jas_sw = 2.4f;

		if (jas_sw == 0.6f || jas_sw == 0.7f || jas_sw == 1.7f || jas_sw == 1.8f)
			Tlx[0] += 0.015f;
		else
			Tlx[0] += 0.03f;

		if (Tlx[0] >= 1.4f) Tlx[0] = 1.4f;
	}

	if (scen[0] == 17)
	{
		if (klik[2] == 1)
		{
			zapis[2] = false;

			if (glosnosc_tla == -7496)
			{
				Tlx[1] += 0.015f;
				glosnosc_tla += 840;
			}

			else if (glosnosc_tla == -7080 || glosnosc_tla == -2504 || glosnosc_tla == -2920)
			{
				Tlx[1] += 0.015f;
				glosnosc_tla += 416;
			}

			else
			{
				glosnosc_tla += 416;
				Tlx[1] += 0.03f;
			}

			if (glosnosc_tla >= 0) glosnosc_tla = 0;
			if (Tlx[1] >= 1.4f) Tlx[1] = 1.4f;

			muza.muzyka_tlo_volume(glosnosc_tla);
		}

		else if (klik[3] == 1)
		{
			zapis[2] = false;

			if (glosnosc_efektow == -7496)
			{
				Tlx[2] += 0.015f;
				glosnosc_efektow += 840;
			}

			else if (glosnosc_efektow == -7080 || glosnosc_efektow == -2504 || glosnosc_efektow == -2920)
			{
				Tlx[2] += 0.015f;
				glosnosc_efektow += 416;
			}

			else
			{
				glosnosc_efektow += 416;
				Tlx[2] += 0.03f;
			}

			if (glosnosc_efektow >= 0) glosnosc_efektow = 0;
			if (Tlx[2] >= 1.4f) Tlx[2] = 1.4f;

			muza.dzwiek_potwierdzenie(glosnosc_efektow);
		}
	}

	if (scen[0] == 19)
		if (IloscPlikow[0][1] > 1)
			IloscPlikow[0][1]--;
}

void Renderer::WheelDown()
{
	if (scen[0] == 7 && WczytLabiryntu % 2 == 0)
	{
		if (IloscPlikow[1][1] < IloscPlikow[1][0] - 17)
			IloscPlikow[1][1]++;
	}
		

	if (scen[0] == 11)
		klawa(85);

	if (scen[0] == 12)
	{
		if (nowy[1] > 0)
		{
			w += 26;
			if (w >= 781) w = 781;
		}
	}

	if (scen[0] == 14)
	{
		ustawianie_klawiszy(85);
		klik[0] = 0;
	}

	if (scen[0] == 16 && ustaw_rozdzielczosc)
	{
		lr[0]++;
		lr[1]++;

		if (lr[0] >= index2 - 2)  lr[0] = index2 - 2;
		if (lr[1] >= index2 - 1)  lr[1] = index2 - 1;

		rozdz[lr[0]].ind = 2;
		rozdz[lr[1]].ind = 1;
	}

	if (scen[0] == 16 && klik[1] == 1)
	{
		zapis[3] = false;
		jas_sw -= 0.1f;
		if (jas_sw <= 0.2f) jas_sw = 0.2f;

		if (jas_sw == 0.7f || jas_sw == 0.8f || jas_sw == 1.9f || jas_sw == 1.8f)
			Tlx[0] -= 0.015f;
		else
			Tlx[0] -= 0.03f;

		if (Tlx[0] <= 0.8f) Tlx[0] = 0.8f;
	}

	if (scen[0] == 17)
	{
		if (klik[2] == 1)
		{
			zapis[2] = false;

			if (glosnosc_tla == -7496)
			{
				Tlx[1] -= 0.015f;
				glosnosc_tla -= 840;
			}

			else if (glosnosc_tla == -7080 || glosnosc_tla == -2504 || glosnosc_tla == -2920)
			{
				Tlx[1] -= 0.015f;
				glosnosc_tla -= 416;
			}

			else
			{
				glosnosc_tla -= 416;
				Tlx[1] -= 0.03f;
			}

			if (glosnosc_tla <= -10000) glosnosc_tla = -10000;
			if (Tlx[1] <= 0.8f) Tlx[1] = 0.8f;

			muza.muzyka_tlo_volume(glosnosc_tla);
		}

		if (klik[3] == 1)
		{
			zapis[2] = false;

			if (glosnosc_efektow == -7496)
			{
				Tlx[2] -= 0.015f;
				glosnosc_efektow -= 840;
			}

			else if (glosnosc_efektow == -7080 || glosnosc_efektow == -2504 || glosnosc_efektow == -2920)
			{
				Tlx[2] -= 0.015f;
				glosnosc_efektow -= 416;
			}

			else
			{
				glosnosc_efektow -= 416;
				Tlx[2] -= 0.03f;
			}

			if (glosnosc_efektow <= -10000) glosnosc_efektow = -10000;
			if (Tlx[2] <= 0.8f) Tlx[2] = 0.8f;

			muza.dzwiek_potwierdzenie(glosnosc_efektow);
		}
	}

	if (scen[0] == 19)
	{
		if (IloscPlikow[0][1] < IloscPlikow[0][0] - 17)
			IloscPlikow[0][1]++;
	}
}

void Renderer::OnMouseMove(WPARAM btnState, INT myszkaX, INT myszkaY)
{
	DIMOUSESTATE mouseCurrState;
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 420 && myszkaX <= rozdzielczosc_w / 1000 * 550 && myszkaY >= rozdzielczosc_h / 500 * 60 && myszkaY <= rozdzielczosc_h / 500 * 110)
		{
			frame[0] = 0;
			OdtwDzwMen[0][0] = 1;
			if (Odtw[0][1] == 0 || Odtw[0][2] == 0)
				Odtw[0][1] = Odtw[0][2] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 390 && myszkaX <= rozdzielczosc_w / 1000 * 570 && myszkaY >= rozdzielczosc_h / 500 * 225 && myszkaY <= rozdzielczosc_h / 500 * 280)
		{
			frame[0] = 1;
			OdtwDzwMen[0][1] = 1;
			if (Odtw[0][0] == 0 || Odtw[0][2] == 0)
				Odtw[0][0] = Odtw[0][2] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 345 && myszkaX <= rozdzielczosc_w / 1000 * 610 && myszkaY >= rozdzielczosc_h / 500 * 390 && myszkaY <= rozdzielczosc_h / 500 * 463)
		{
			frame[0] = 2;
			OdtwDzwMen[0][2] = 1;
			if (Odtw[0][1] == 0 || Odtw[0][0] == 0)
				Odtw[0][1] = Odtw[0][0] = 1;
		}
	}

	if ((scen[0] == 1) || (scen[0] == 6) || (scen[0] == 8) || (scen[0] == 10))
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 380 && myszkaX <= rozdzielczosc_w / 1000 * 600 && myszkaY >= rozdzielczosc_h / 500 * 160 && myszkaY <= rozdzielczosc_h / 500 * 200)
		{
			frame[1] = 0;
			OdtwDzwMen[1][0] = 1;
			if (Odtw[1][1] == 0 || Odtw[1][2] == 0 || Odtw[1][3] == 0)
				Odtw[1][1] = Odtw[1][2] = Odtw[1][3] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 335 && myszkaX <= rozdzielczosc_w / 1000 * 630 && myszkaY >= rozdzielczosc_h / 500 * 230 && myszkaY <= rozdzielczosc_h / 500 * 270)
		{
			frame[1] = 1;
			OdtwDzwMen[1][1] = 1;
			if (Odtw[1][0] == 0 || Odtw[1][2] == 0 || Odtw[1][3] == 0)
				Odtw[1][0] = Odtw[1][2] = Odtw[1][3] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 285 && myszkaX <= rozdzielczosc_w / 1000 * 690 && myszkaY >= rozdzielczosc_h / 500 * 300 && myszkaY <= rozdzielczosc_h / 500 * 340)
		{
			frame[1] = 2;
			OdtwDzwMen[1][2] = 1;
			if (Odtw[1][1] == 0 || Odtw[1][0] == 0 || Odtw[1][3] == 0)
				Odtw[1][1] = Odtw[1][0] = Odtw[1][3] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 570 && myszkaY >= rozdzielczosc_h / 500 * 410 && myszkaY <= rozdzielczosc_h / 500 * 450)
		{
			frame[1] = 3;
			OdtwDzwMen[1][3] = 1;
			if (Odtw[1][1] == 0 || Odtw[1][2] == 0 || Odtw[1][0] == 0)
				Odtw[1][1] = Odtw[1][2] = Odtw[1][0] = 1;
		}
	}

	if ((scen[0] == 2) || (scen[0] == 15))
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 415 && myszkaX <= rozdzielczosc_w / 1000 * 565 && myszkaY >= rozdzielczosc_h / 500 * 160 && myszkaY <= rozdzielczosc_h / 500 * 200)
		{
			frame[2] = 0;
			OdtwDzwMen[2][0] = 1;
			if (Odtw[2][1] == 0 || Odtw[2][2] == 0 || Odtw[2][3] == 0)
				Odtw[2][1] = Odtw[2][2] = Odtw[2][3] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 395 && myszkaX <= rozdzielczosc_w / 1000 * 580 && myszkaY >= rozdzielczosc_h / 500 * 235 && myszkaY <= rozdzielczosc_h / 500 * 270)
		{
			frame[2] = 1;
			OdtwDzwMen[2][1] = 1;
			if (Odtw[2][0] == 0 || Odtw[2][2] == 0 || Odtw[2][3] == 0)
				Odtw[2][0] = Odtw[2][2] = Odtw[2][3] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 345 && myszkaX <= rozdzielczosc_w / 1000 * 620 && myszkaY >= rozdzielczosc_h / 500 * 300 && myszkaY <= rozdzielczosc_h / 500 * 340)
		{
			frame[2] = 2;
			OdtwDzwMen[2][2] = 1;
			if (Odtw[2][1] == 0 || Odtw[2][0] == 0 || Odtw[2][3] == 0)
				Odtw[2][1] = Odtw[2][0] = Odtw[2][3] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 570 && myszkaY >= rozdzielczosc_h / 500 * 410 && myszkaY <= rozdzielczosc_h / 500 * 450)
		{
			frame[2] = 3;
			OdtwDzwMen[2][3] = 1;
			if (Odtw[2][1] == 0 || Odtw[2][2] == 0 || Odtw[2][0] == 0)
				Odtw[2][1] = Odtw[2][2] = Odtw[2][0] = 1;
		}
	}

	if ((scen[0] == 5) || (scen[0] == 13))
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 315 && myszkaX <= rozdzielczosc_w / 1000 * 660 && myszkaY >= rozdzielczosc_h / 500 * 200 && myszkaY <= rozdzielczosc_h / 500 * 240)
		{
			frame[3] = 0;
			OdtwDzwMen[3][0] = 1;
			if (Odtw[3][1] == 0 || Odtw[3][2] == 0)
				Odtw[3][1] = Odtw[3][2] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 270 && myszkaX <= rozdzielczosc_w / 1000 * 700 && myszkaY >= rozdzielczosc_h / 500 * 270 && myszkaY <= rozdzielczosc_h / 500 * 310)
		{
			frame[3] = 1;
			OdtwDzwMen[3][1] = 1;
			if (Odtw[3][0] == 0 || Odtw[3][2] == 0)
				Odtw[3][0] = Odtw[3][2] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 570 && myszkaY >= rozdzielczosc_h / 500 * 410 && myszkaY <= rozdzielczosc_h / 500 * 450)
		{
			frame[3] = 2;
			OdtwDzwMen[3][2] = 1;
			if (Odtw[3][1] == 0 || Odtw[3][0] == 0)
				Odtw[3][1] = Odtw[3][0] = 1;
		}
	}

	if (scen[0] == 7)
	{
		if (WczytLabiryntu % 2 == 1)
		{
			if (myszkaX >= rozdzielczosc_w / 1000 * 425 && myszkaX <= rozdzielczosc_w / 1000 * 565 && myszkaY >= rozdzielczosc_h / 500 * 230 && myszkaY <= rozdzielczosc_h / 500 * 270)
			{
				frame[4] = 0;
				OdtwDzwMen[4][0] = 1;
				if (Odtw[4][1] == 0)
					Odtw[4][1] = 1;
			}

			if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 570 && myszkaY >= rozdzielczosc_h / 500 * 410 && myszkaY <= rozdzielczosc_h / 500 * 450)
			{
				frame[4] = 1;
				OdtwDzwMen[4][1] = 1;
				if (Odtw[4][0] == 0)
					Odtw[4][0] = 1;
			}
		}

		else if (WczytLabiryntu % 2 == 0)
		{
			ilosc[1] = 0; FLOAT WysokoscY1 = 90, WysokoscY2 = 110;
			while (ilosc[1] < IloscPlikow[1][0] && ilosc[1] < 17)
			{
				if (myszkaX >= rozdzielczosc_w / 1000 * 420 && myszkaX <= rozdzielczosc_w / 1000 * 600 && myszkaY >= rozdzielczosc_h / 500 * WysokoscY1 && myszkaY <= rozdzielczosc_h / 500 * WysokoscY2)
				{
					frame[16] = ilosc[1];

					if (ilosc == 0)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 1)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][0] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][0] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 2)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][0] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][0] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 3)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][0] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][0] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 4)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][0] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][0] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 5)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][0] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][0] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 6)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][0] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][0] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 7)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][0] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][0] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 8)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][0] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][0] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 9)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][0] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][0] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 10)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][0] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][0] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 11)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][0] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][0] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 12)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][0] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][0] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 13)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][0] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][0] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 14)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][0] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][0] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 15)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][0] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][0] = Odtw[16][16] = Odtw[16][17] = 1;
					}

					if (ilosc[1] == 16)
					{
						OdtwDzwMen[16][ilosc[1]] = 1;
						if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
							Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
							Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
							Odtw[16][15] == 0 || Odtw[16][0] == 0 || Odtw[16][17] == 0)
							Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
							Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
							Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][0] = Odtw[16][17] = 1;
					}
				}
				WysokoscY1 += 20;
				WysokoscY2 += 20;
				ilosc[1]++;
			}

			if (myszkaX >= rozdzielczosc_w / 1000 * 450 && myszkaX <= rozdzielczosc_w / 1000 * 600 && myszkaY >= rozdzielczosc_h / 500 * 430 && myszkaY <= rozdzielczosc_h / 500 * 465)
			{
				frame[16] = IloscPlikow[1][0];
				OdtwDzwMen[16][17] = 1;
				if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
					Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
					Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
					Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][0] == 0)
					Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
					Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
					Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][0] = 1;
			}
		}
	}

	if (scen[0] == 9)
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 425 && myszkaX <= rozdzielczosc_w / 1000 * 565 && myszkaY >= rozdzielczosc_h / 500 * 230 && myszkaY <= rozdzielczosc_h / 500 * 270)
		{
			frame[5] = 0;
			OdtwDzwMen[5][0] = 1;
			if (Odtw[5][1] == 0)
				Odtw[5][1] = 1;	
		}
			
		if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 570 && myszkaY >= rozdzielczosc_h / 500 * 410 && myszkaY <= rozdzielczosc_h / 500 * 450)
		{
			frame[5] = 1;
			OdtwDzwMen[5][1] = 1;
			if (Odtw[5][0] == 0)
				Odtw[5][0] = 1;
		}
	}

	if (scen[0] == 11)
	{
		if (po % 2 == 1 && ZapiszGiere % 2 == 1 && !wygrana && Anim[0] != 14)
			TPCam.ControllMouse(mouseCurrState, mouseLastState);

		if (po % 2 == 1 && ZapiszGiere % 2 == 1)
			SetCursor(LoadCursor(NULL, NULL));

		else if (po % 2 == 0 && ZapiszGiere % 2 == 1)
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));

			if (myszkaX >= rozdzielczosc_w / 1000 * 470 && myszkaX <= rozdzielczosc_w / 1000 * 530 && myszkaY >= rozdzielczosc_h / 500 * 155 && myszkaY <= rozdzielczosc_h / 500 * 175)
			{
				frame[9] = 0;
				reset_czasu = 0;
				OdtwDzwMen[9][0] = 1;
				if (Odtw[9][1] == 0 || Odtw[9][2] == 0 || Odtw[9][3] == 0 || Odtw[9][4] == 0 || Odtw[9][5] == 0)
					Odtw[9][1] = Odtw[9][2] = Odtw[9][3] = Odtw[9][4] = Odtw[9][5] = 1;
			}

			if (myszkaX >= rozdzielczosc_w / 1000 * 450 && myszkaX <= rozdzielczosc_w / 1000 * 540 && myszkaY >= rozdzielczosc_h / 500 * 195 && myszkaY <= rozdzielczosc_h / 500 * 215)
			{
				frame[9] = 1;
				reset_czasu = 0;
				OdtwDzwMen[9][1] = 1;
				if (Odtw[9][0] == 0 || Odtw[9][2] == 0 || Odtw[9][3] == 0 || Odtw[9][4] == 0 || Odtw[9][5] == 0)
					Odtw[9][0] = Odtw[9][2] = Odtw[9][3] = Odtw[9][4] = Odtw[9][5] = 1;
			}

			if (myszkaX >= rozdzielczosc_w / 1000 * 460 && myszkaX <= rozdzielczosc_w / 1000 * 520 && myszkaY >= rozdzielczosc_h / 500 * 225 && myszkaY <= rozdzielczosc_h / 500 * 245)
			{
				frame[9] = 2;
				reset_czasu = 0;
				OdtwDzwMen[9][2] = 1;
				if (Odtw[9][1] == 0 || Odtw[9][0] == 0 || Odtw[9][3] == 0 || Odtw[9][4] == 0 || Odtw[9][5] == 0)
					Odtw[9][1] = Odtw[9][0] = Odtw[9][3] = Odtw[9][4] = Odtw[9][5] = 1;
			}

			if (myszkaX >= rozdzielczosc_w / 1000 * 445 && myszkaX <= rozdzielczosc_w / 1000 * 520 && myszkaY >= rozdzielczosc_h / 500 * 255 && myszkaY <= rozdzielczosc_h / 500 * 275)
			{
				frame[9] = 3;
				reset_czasu = 0;
				OdtwDzwMen[9][3] = 1;
				if (Odtw[9][1] == 0 || Odtw[9][2] == 0 || Odtw[9][0] == 0 || Odtw[9][4] == 0 || Odtw[9][5] == 0)
					Odtw[9][1] = Odtw[9][2] = Odtw[9][0] = Odtw[9][4] = Odtw[9][5] = 1;
			}

			if (myszkaX >= rozdzielczosc_w / 1000 * 445 && myszkaX <= rozdzielczosc_w / 1000 * 520 && myszkaY >= rozdzielczosc_h / 500 * 285 && myszkaY <= rozdzielczosc_h / 500 * 295)
			{
				frame[9] = 4;
				reset_czasu = 0;
				OdtwDzwMen[9][4] = 1;
				if (Odtw[9][1] == 0 || Odtw[9][2] == 0 || Odtw[9][3] == 0 || Odtw[9][0] == 0 || Odtw[9][5] == 0)
					Odtw[9][1] = Odtw[9][2] = Odtw[9][3] = Odtw[9][0] = Odtw[9][5] = 1;
			}

			if (myszkaX >= rozdzielczosc_w / 1000 * 445 && myszkaX <= rozdzielczosc_w / 1000 * 520 && myszkaY >= rozdzielczosc_h / 500 * 315 && myszkaY <= rozdzielczosc_h / 500 * 360)
			{
				frame[9] = 5;
				reset_czasu = 0;
				OdtwDzwMen[9][5] = 1;
				if (Odtw[9][1] == 0 || Odtw[9][2] == 0 || Odtw[9][3] == 0 || Odtw[9][4] == 0 || Odtw[9][0] == 0)
					Odtw[9][1] = Odtw[9][2] = Odtw[9][3] = Odtw[9][4] = Odtw[9][0] = 1;
			}
		}
	}

	if (scen[0] == 12)
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 80 && myszkaX <= rozdzielczosc_w / 1000 * 140 && myszkaY >= rozdzielczosc_h / 500 * 180 && myszkaY <= rozdzielczosc_h / 500 * 200)
		{
			frame[6] = 0;

			OdtwDzwMen[6][0] = 1;
			if (Odtw[6][1] == 0 || Odtw[6][2] == 0 || Odtw[6][3] == 0)
				Odtw[6][1] = Odtw[6][2] = Odtw[6][3] = 1;

			frame[7] = 20;

			for (INT fr = 0; fr < 1400; fr++)
				frejmik[fr] = false;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 65 && myszkaX <= rozdzielczosc_w / 1000 * 150 && myszkaY >= rozdzielczosc_h / 500 * 220 && myszkaY <= rozdzielczosc_h / 500 * 240)
		{
			frame[6] = 1;

			OdtwDzwMen[6][1] = 1;
			if (Odtw[6][0] == 0 || Odtw[6][2] == 0 || Odtw[6][3] == 0)
				Odtw[6][0] = Odtw[6][2] = Odtw[6][3] = 1;

			frame[7] = 20;

			for (INT fr = 0; fr < 1400; fr++)
				frejmik[fr] = false;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 75 && myszkaX <= rozdzielczosc_w / 1000 * 150 && myszkaY >= rozdzielczosc_h / 500 * 260 && myszkaY <= rozdzielczosc_h / 500 * 280)
		{
			frame[6] = 2;

			OdtwDzwMen[6][2] = 1;
			if (Odtw[6][1] == 0 || Odtw[6][0] == 0 || Odtw[6][3] == 0)
				Odtw[6][1] = Odtw[6][0] = Odtw[6][3] = 1;

			frame[7] = 20;

			for (INT fr = 0; fr < 1400; fr++)
				frejmik[fr] = false;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 75 && myszkaX <= rozdzielczosc_w / 1000 * 160 && myszkaY >= rozdzielczosc_h / 500 * 455 && myszkaY <= rozdzielczosc_h / 500 * 480)
		{
			frame[6] = 3;

			OdtwDzwMen[6][3] = 1;
			if (Odtw[6][1] == 0 || Odtw[6][2] == 0 || Odtw[6][0] == 0)
				Odtw[6][1] = Odtw[6][2] = Odtw[6][0] = 1;

			frame[7] = 20;

			for (INT fr = 0; fr < 1400; fr++)
				frejmik[fr] = false;
		}

		if (nowy[0] == 1)
		{
			if (myszkaX >= rozdzielczosc_w / 1000 * 243 && myszkaX <= rozdzielczosc_w / 1000 * 280 && myszkaY >= rozdzielczosc_h / 500 * 0 && myszkaY <= rozdzielczosc_h / 500 * 40)
			{
				frame[7] = 0;

				OdtwDzwMen[7][0] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 0;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[0] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 281 && myszkaX <= rozdzielczosc_w / 1000 * 315 && myszkaY >= rozdzielczosc_h / 500 * 0 && myszkaY <= rozdzielczosc_h / 500 * 40)
			{
				frame[7] = 1;

				OdtwDzwMen[7][1] = 1;
				if (Odtw[7][0] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][0] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 1;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[1] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 243 && myszkaX <= rozdzielczosc_w / 1000 * 280 && myszkaY >= rozdzielczosc_h / 500 * 41 && myszkaY <= rozdzielczosc_h / 500 * 80)
			{
				frame[7] = 2;

				OdtwDzwMen[7][2] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][0] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][0] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 2;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[2] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 281 && myszkaX <= rozdzielczosc_w / 1000 * 315 && myszkaY >= rozdzielczosc_h / 500 * 41 && myszkaY <= rozdzielczosc_h / 500 * 80)
			{
				frame[7] = 3;

				OdtwDzwMen[7][3] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][0] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][0] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 3;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[3] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 243 && myszkaX <= rozdzielczosc_w / 1000 * 280 && myszkaY >= rozdzielczosc_h / 500 * 81 && myszkaY <= rozdzielczosc_h / 500 * 120)
			{
				frame[7] = 4;

				OdtwDzwMen[7][4] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][0] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][0] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 4;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[4] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 281 && myszkaX <= rozdzielczosc_w / 1000 * 315 && myszkaY >= rozdzielczosc_h / 500 * 81 && myszkaY <= rozdzielczosc_h / 500 * 120)
			{
				frame[7] = 5;

				OdtwDzwMen[7][5] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][0] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][0] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 5;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[5] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 243 && myszkaX <= rozdzielczosc_w / 1000 * 280 && myszkaY >= rozdzielczosc_h / 500 * 121 && myszkaY <= rozdzielczosc_h / 500 * 160)
			{
				frame[7] = 6;

				OdtwDzwMen[7][6] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][0] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][0] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 6;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[6] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 281 && myszkaX <= rozdzielczosc_w / 1000 * 315 && myszkaY >= rozdzielczosc_h / 500 * 121 && myszkaY <= rozdzielczosc_h / 500 * 160)
			{
				frame[7] = 7;

				OdtwDzwMen[7][7] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][0] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][0] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 7;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[7] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 243 && myszkaX <= rozdzielczosc_w / 1000 * 280 && myszkaY >= rozdzielczosc_h / 500 * 161 && myszkaY <= rozdzielczosc_h / 500 * 200)
			{
				frame[7] = 8;

				OdtwDzwMen[7][8] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][0] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][0] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 8;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[8] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 281 && myszkaX <= rozdzielczosc_w / 1000 * 315 && myszkaY >= rozdzielczosc_h / 500 * 161 && myszkaY <= rozdzielczosc_h / 500 * 200)
			{
				frame[7] = 9;

				OdtwDzwMen[7][9] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][0] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][0] =
					Odtw[7][10] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 9;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[9] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 243 && myszkaX <= rozdzielczosc_w / 1000 * 281 && myszkaY >= rozdzielczosc_h / 500 * 410 && myszkaY <= rozdzielczosc_h / 500 * 450)
			{
				frame[7] = 10;

				OdtwDzwMen[7][10] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][0] == 0 || Odtw[7][11] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][0] = Odtw[7][11] = 1;

				frame[6] = 10;
				ZaznGuz = 10;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[10] = true;
			}

			else if (myszkaX >= rozdzielczosc_w / 1000 * 281 && myszkaX <= rozdzielczosc_w / 1000 * 315 && myszkaY >= rozdzielczosc_h / 500 * 410 && myszkaY <= rozdzielczosc_h / 500 * 450)
			{
				frame[7] = 11;

				OdtwDzwMen[7][11] = 1;
				if (Odtw[7][1] == 0 || Odtw[7][2] == 0 || Odtw[7][3] == 0 ||
					Odtw[7][4] == 0 || Odtw[7][5] == 0 || Odtw[7][6] == 0 ||
					Odtw[7][7] == 0 || Odtw[7][8] == 0 || Odtw[7][9] == 0 ||
					Odtw[7][10] == 0 || Odtw[7][0] == 0)
					Odtw[7][1] = Odtw[7][2] = Odtw[7][3] =
					Odtw[7][4] = Odtw[7][5] = Odtw[7][6] =
					Odtw[7][7] = Odtw[7][8] = Odtw[7][9] =
					Odtw[7][10] = Odtw[7][0] = 1;

				frame[6] = 10;
				ZaznGuz = 11;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = false;

				frejmik[11] = true;
			}
		}

		zaznaczenie((FLOAT)myszkaX, (FLOAT)myszkaY, rozdzielczosc_w, rozdzielczosc_h);
		odznaczenie((FLOAT)myszkaX, (FLOAT)myszkaY, rozdzielczosc_w, rozdzielczosc_h);
	}

	if (scen[0] == 14 && klik[0] == 0)
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 83 && myszkaY <= rozdzielczosc_h / 500 * 98)
		{
			frame[8] = 0;

			OdtwDzwMen[8][0] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 415 && myszkaY >= rozdzielczosc_h / 500 * 105 && myszkaY <= rozdzielczosc_h / 500 * 120)
		{
			frame[8] = 1;

			OdtwDzwMen[8][1] = 1;
			if (Odtw[8][0] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][0] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 455 && myszkaY >= rozdzielczosc_h / 500 * 127 && myszkaY <= rozdzielczosc_h / 500 * 142)
		{
			frame[8] = 2;

			OdtwDzwMen[8][2] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][0] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][0] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 475 && myszkaY >= rozdzielczosc_h / 500 * 149 && myszkaY <= rozdzielczosc_h / 500 * 164)
		{
			frame[8] = 3;
			OdtwDzwMen[8][3] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][0] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][0] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 415 && myszkaY >= rozdzielczosc_h / 500 * 171 && myszkaY <= rozdzielczosc_h / 500 * 186)
		{
			frame[8] = 4;
			OdtwDzwMen[8][4] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][0] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][0] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 193 && myszkaY <= rozdzielczosc_h / 500 * 208)
		{
			frame[8] = 5;
			OdtwDzwMen[8][5] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][0] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][0] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 455 && myszkaY >= rozdzielczosc_h / 500 * 215 && myszkaY <= rozdzielczosc_h / 500 * 230)
		{
			frame[8] = 6;
			OdtwDzwMen[8][6] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][0] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][0] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 237 && myszkaY <= rozdzielczosc_h / 500 * 252)
		{
			frame[8] = 7;
			OdtwDzwMen[8][7] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][0] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][0] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 415 && myszkaY >= rozdzielczosc_h / 500 * 259 && myszkaY <= rozdzielczosc_h / 500 * 274)
		{
			frame[8] = 8;
			OdtwDzwMen[8][8] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][0] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][0] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 281 && myszkaY <= rozdzielczosc_h / 500 * 296)
		{
			frame[8] = 9;
			OdtwDzwMen[8][9] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][0] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][0] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 303 && myszkaY <= rozdzielczosc_h / 500 * 318)
		{
			frame[8] = 10;
			OdtwDzwMen[8][10] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][0] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][0] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 325 && myszkaY <= rozdzielczosc_h / 500 * 340)
		{
			frame[8] = 11;
			OdtwDzwMen[8][11] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][0] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][0] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 347 && myszkaY <= rozdzielczosc_h / 500 * 362)
		{
			frame[8] = 12;
			OdtwDzwMen[8][12] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][0] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][0] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 369 && myszkaY <= rozdzielczosc_h / 500 * 384)
		{
			frame[8] = 13;
			OdtwDzwMen[8][13] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][0] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][0] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 391 && myszkaY <= rozdzielczosc_h / 500 * 406)
		{
			frame[8] = 14;
			OdtwDzwMen[8][14] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][0] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][0] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 535 && myszkaY >= rozdzielczosc_h / 500 * 435 && myszkaY <= rozdzielczosc_h / 500 * 475)
		{
			frame[8] = 15;
			OdtwDzwMen[8][15] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][0] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][0] =
				Odtw[8][16] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 10 && myszkaX <= rozdzielczosc_w / 1000 * 100 && myszkaY >= rozdzielczosc_h / 500 * 435 && myszkaY <= rozdzielczosc_h / 500 * 475)
		{
			frame[8] = 16;
			OdtwDzwMen[8][16] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][0] == 0 ||
				Odtw[8][17] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][0] = Odtw[8][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 840 && myszkaX <= rozdzielczosc_w / 1000 * 930 && myszkaY >= rozdzielczosc_h / 500 * 455 && myszkaY <= rozdzielczosc_h / 500 * 475)
		{
			frame[8] = 17;
			OdtwDzwMen[8][17] = 1;
			if (Odtw[8][1] == 0 || Odtw[8][2] == 0 || Odtw[8][3] == 0 || Odtw[8][4] == 0 ||
				Odtw[8][5] == 0 || Odtw[8][6] == 0 || Odtw[8][7] == 0 || Odtw[8][8] == 0 ||
				Odtw[8][9] == 0 || Odtw[8][10] == 0 || Odtw[8][11] == 0 || Odtw[8][12] == 0 ||
				Odtw[8][13] == 0 || Odtw[8][14] == 0 || Odtw[8][15] == 0 || Odtw[8][16] == 0 ||
				Odtw[8][0] == 0)
				Odtw[8][1] = Odtw[8][2] = Odtw[8][3] = Odtw[8][4] = Odtw[8][5] =
				Odtw[8][6] = Odtw[8][7] = Odtw[8][8] = Odtw[8][9] = Odtw[8][10] =
				Odtw[8][11] = Odtw[8][12] = Odtw[8][13] = Odtw[8][14] = Odtw[8][15] =
				Odtw[8][16] = Odtw[8][0] = 1;
		}
	}

	if (scen[0] == 16)
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 50 && myszkaX <= rozdzielczosc_w / 1000 * 350 && myszkaY >= rozdzielczosc_h / 500 * 200 && myszkaY <= rozdzielczosc_h / 500 * 225)
		{
			frame[10] = 0;
			OdtwDzwMen[10][0] = 1;
			if (Odtw[10][1] == 0 || Odtw[10][2] == 0 || Odtw[10][3] == 0 || Odtw[10][4] == 0 || Odtw[10][5] == 0)
				Odtw[10][1] = Odtw[10][2] = Odtw[10][3] = Odtw[10][4] = Odtw[10][5] = 1;

			frame[12] = frame[13] = 4;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 60 && myszkaX <= rozdzielczosc_w / 1000 * 330 && myszkaY >= rozdzielczosc_h / 500 * 250 && myszkaY <= rozdzielczosc_h / 500 * 275)
		{
			frame[10] = 1;
			OdtwDzwMen[10][1] = 1;
			if (Odtw[10][0] == 0 || Odtw[10][2] == 0 || Odtw[10][3] == 0 || Odtw[10][4] == 0 || Odtw[10][5] == 0)
				Odtw[10][0] = Odtw[10][2] = Odtw[10][3] = Odtw[10][4] = Odtw[10][5] = 1;

			frame[12] = frame[13] = 4;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 90 && myszkaX <= rozdzielczosc_w / 1000 * 260 && myszkaY >= rozdzielczosc_h / 500 * 300 && myszkaY <= rozdzielczosc_h / 500 * 325)
		{
			frame[10] = 2;
			OdtwDzwMen[10][2] = 1;
			if (Odtw[10][1] == 0 || Odtw[10][0] == 0 || Odtw[10][3] == 0 || Odtw[10][4] == 0 || Odtw[10][5] == 0)
				Odtw[10][1] = Odtw[10][0] = Odtw[10][3] = Odtw[10][4] = Odtw[10][5] = 1;

			frame[12] = frame[13] = 4;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 650 && myszkaY >= rozdzielczosc_h / 500 * 440 && myszkaY <= rozdzielczosc_h / 500 * 490)
		{
			frame[10] = 3;
			OdtwDzwMen[10][3] = 1;
			if (Odtw[10][1] == 0 || Odtw[10][2] == 0 || Odtw[10][0] == 0 || Odtw[10][4] == 0 || Odtw[10][5] == 0)
				Odtw[10][1] = Odtw[10][2] = Odtw[10][0] = Odtw[10][4] = Odtw[10][5] = 1;

			frame[12] = frame[13] = 4;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 10 && myszkaX <= rozdzielczosc_w / 1000 * 150 && myszkaY >= rozdzielczosc_h / 500 * 440 && myszkaY <= rozdzielczosc_h / 500 * 490)
		{
			frame[10] = 4;
			OdtwDzwMen[10][4] = 1;
			if (Odtw[10][1] == 0 || Odtw[10][2] == 0 || Odtw[10][3] == 0 || Odtw[10][0] == 0 || Odtw[10][5] == 0)
				Odtw[10][1] = Odtw[10][2] = Odtw[10][3] = Odtw[10][0] = Odtw[10][5] = 1;

			frame[12] = frame[13] = 4;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 840 && myszkaX <= rozdzielczosc_w / 1000 * 990 && myszkaY >= rozdzielczosc_h / 500 * 460 && myszkaY <= rozdzielczosc_h / 500 * 490)
		{
			frame[10] = 5;
			OdtwDzwMen[10][5] = 1;
			if (Odtw[10][1] == 0 || Odtw[10][2] == 0 || Odtw[10][3] == 0 || Odtw[10][4] == 0 || Odtw[10][0] == 0)
				Odtw[10][1] = Odtw[10][2] = Odtw[10][3] = Odtw[10][4] = Odtw[10][0] = 1;

			frame[12] = frame[13] = 4;
		}

		if (ustaw_fullekran)
		{
			if (myszkaX >= rozdzielczosc_w / 1000 * 480 && myszkaX <= rozdzielczosc_w / 1000 * 520 && myszkaY >= rozdzielczosc_h / 500 * 210 && myszkaY <= rozdzielczosc_h / 500 * 235)
			{
				frame[12] = 1;
				OdtwDzwMen[12][0] = 1;
				if (Odtw[12][1] == 0)
					Odtw[12][1] = 1;

				frame[10] = 10;
				frame[13] = 4;
			}

			if (myszkaX >= rozdzielczosc_w / 1000 * 470 && myszkaX <= rozdzielczosc_w / 1000 * 530 && myszkaY >= rozdzielczosc_h / 500 * 260 && myszkaY <= rozdzielczosc_h / 500 * 285)
			{
				frame[12] = 2;
				OdtwDzwMen[12][1] = 1;
				if (Odtw[12][0] == 0)
					Odtw[12][0] = 1;

				frame[10] = 10;
				frame[13] = 4;
			}
		}

		if (ustaw_rozdzielczosc)
		{
			for (INT i = 0, ii = 2, iii = 210; i < 2, ii > 0, iii <= 260; i++, ii--, iii += 50)
				if (myszkaX >= rozdzielczosc_w / 1000 * 440 && myszkaX <= rozdzielczosc_w / 1000 * 560 && myszkaY >= rozdzielczosc_h / 500 * iii && myszkaY <= rozdzielczosc_h / 500 * iii + 30)
				{
					frame[13] = i;
					frame[12] = 2;
					frame[10] = 10;

					rozdz[lr[0]].ind = i + 1;
					rozdz[lr[1]].ind = ii;

					OdtwDzwMen[13][i] = 1;
					if (Odtw[13][ii - 1] == 0)
						Odtw[13][ii - 1] = 1;
				}
		}
	}

	if (scen[0] == 17)
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 60 && myszkaX <= rozdzielczosc_w / 1000 * 420 && myszkaY >= rozdzielczosc_h / 500 * 240 && myszkaY <= rozdzielczosc_h / 500 * 265)
		{
			frame[11] = 0;
			OdtwDzwMen[11][0] = 1;
			if (Odtw[11][1] == 0 || Odtw[11][2] == 0 || Odtw[11][3] == 0 || Odtw[11][4] == 0)
				Odtw[11][1] = Odtw[11][2] = Odtw[11][3] = Odtw[11][4] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 50 && myszkaX <= rozdzielczosc_w / 1000 * 420 && myszkaY >= rozdzielczosc_h / 500 * 300 && myszkaY <= rozdzielczosc_h / 500 * 325)
		{
			frame[11] = 1;
			OdtwDzwMen[11][1] = 1;
			if (Odtw[11][0] == 0 || Odtw[11][2] == 0 || Odtw[11][3] == 0 || Odtw[11][4] == 0)
				Odtw[11][0] = Odtw[11][2] = Odtw[11][3] = Odtw[11][4] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 650 && myszkaY >= rozdzielczosc_h / 500 * 440 && myszkaY <= rozdzielczosc_h / 500 * 490)
		{
			frame[11] = 2;
			OdtwDzwMen[11][2] = 1;
			if (Odtw[11][1] == 0 || Odtw[11][0] == 0 || Odtw[11][3] == 0 || Odtw[11][4] == 0)
				Odtw[11][1] = Odtw[11][0] = Odtw[11][3] = Odtw[11][4] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 10 && myszkaX <= rozdzielczosc_w / 1000 * 150 && myszkaY >= rozdzielczosc_h / 500 * 440 && myszkaY <= rozdzielczosc_h / 500 * 490)
		{
			frame[11] = 3;
			OdtwDzwMen[11][3] = 1;
			if (Odtw[11][1] == 0 || Odtw[11][2] == 0 || Odtw[11][0] == 0 || Odtw[11][4] == 0)
				Odtw[11][1] = Odtw[11][2] = Odtw[11][0] = Odtw[11][4] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 840 && myszkaX <= rozdzielczosc_w / 1000 * 990 && myszkaY >= rozdzielczosc_h / 500 * 460 && myszkaY <= rozdzielczosc_h / 500 * 490)
		{
			frame[11] = 4;
			OdtwDzwMen[11][4] = 1;
			if (Odtw[11][1] == 0 || Odtw[11][2] == 0 || Odtw[11][3] == 0 || Odtw[11][0] == 0)
				Odtw[11][1] = Odtw[11][2] = Odtw[11][3] = Odtw[11][0] = 1;
		}
	}

	if (scen[0] == 18)
	{
		for (INT i = 0, ii = 1, iii = 250; i < 2, ii >= 0, iii < 701; i++, ii--, iii += 450)
		{
			if (myszkaX >= rozdzielczosc_w / 1000 * iii && myszkaX <= rozdzielczosc_w / 1000 * iii + 70 && myszkaY >= rozdzielczosc_h / 500 * 310 && myszkaY <= rozdzielczosc_h / 500 * 360)
			{
				frame[14] = i;
				OdtwDzwMen[14][i] = 1;
				if (Odtw[14][ii] == 0)
					Odtw[14][ii] = 1;
			}
		}
	}

	if (scen[0] == 19)
	{
		ilosc[0] = 0; FLOAT WysokoscY1 = 90, WysokoscY2 = 110;
		while (ilosc[0] < IloscPlikow[0][0] && ilosc[0] < 17)
		{
			if (myszkaX >= rozdzielczosc_w / 1000 * 420 && myszkaX <= rozdzielczosc_w / 1000 * 600 && myszkaY >= rozdzielczosc_h / 500 * WysokoscY1 && myszkaY <= rozdzielczosc_h / 500 * WysokoscY2)
			{
				frame[16] = ilosc[0];

				if (ilosc[0] == 0)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 1)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][0] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][0] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 2)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][0] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][0] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 3)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][0] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][0] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 4)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][0] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][0] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 5)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][0] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][0] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 6)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][0] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][0] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 7)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][0] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][0] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 8)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][0] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][0] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 9)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][0] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][0] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 10)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][0] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][0] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 11)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][0] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][0] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 12)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][0] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][0] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 13)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][0] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][0] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 14)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][0] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][0] = Odtw[16][15] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 15)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][0] == 0 || Odtw[16][16] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][0] = Odtw[16][16] = Odtw[16][17] = 1;
				}

				if (ilosc[0] == 16)
				{
					OdtwDzwMen[16][ilosc[0]] = 1;
					if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
						Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
						Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
						Odtw[16][15] == 0 || Odtw[16][0] == 0 || Odtw[16][17] == 0)
						Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
						Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
						Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][0] = Odtw[16][17] = 1;
				}
			}
			WysokoscY1 += 20;
			WysokoscY2 += 20;
			ilosc[0]++;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 450 && myszkaX <= rozdzielczosc_w / 1000 * 600 && myszkaY >= rozdzielczosc_h / 500 * 430 && myszkaY <= rozdzielczosc_h / 500 * 465)
		{
			frame[16] = IloscPlikow[0][0];
			OdtwDzwMen[16][17] = 1;
			if (Odtw[16][1] == 0 || Odtw[16][2] == 0 || Odtw[16][3] == 0 || Odtw[16][4] == 0 || Odtw[16][5] == 0 ||
				Odtw[16][6] == 0 || Odtw[16][7] == 0 || Odtw[16][8] == 0 || Odtw[16][9] == 0 || Odtw[16][10] == 0 ||
				Odtw[16][11] == 0 || Odtw[16][12] == 0 || Odtw[16][13] == 0 || Odtw[16][14] == 0 ||
				Odtw[16][15] == 0 || Odtw[16][16] == 0 || Odtw[16][0] == 0)
				Odtw[16][1] = Odtw[16][2] = Odtw[16][3] = Odtw[16][4] = Odtw[16][5] = Odtw[16][6] =
				Odtw[16][7] = Odtw[16][8] = Odtw[16][9] = Odtw[16][10] = Odtw[16][11] = Odtw[16][12] =
				Odtw[16][13] = Odtw[16][14] = Odtw[16][15] = Odtw[16][16] = Odtw[16][0] = 1;
		}
	}

	if ((scen[0] == 20) || (po % 2 == 0 && ZapiszGiere % 2 == 0))
	{
		for (INT i = 0, ii = 1, iii = 250; i < 2, ii >= 0, iii < 681; i++, ii--, iii += 430)
		{
			if (myszkaX >= rozdzielczosc_w / 1000 * iii && myszkaX <= rozdzielczosc_w / 1000 * iii + 120 && myszkaY >= rozdzielczosc_h / 500 * 340 && myszkaY <= rozdzielczosc_h / 500 * 370)
			{
				frame[15] = i;
				OdtwDzwMen[15][i] = 1;
				if (Odtw[15][ii] == 0)
					Odtw[15][ii] = 1;
			}
		}
	}

	if (scen[0] == 21)
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 200 && myszkaX <= rozdzielczosc_w / 1000 * 450 && myszkaY >= rozdzielczosc_h / 500 * 200 && myszkaY <= rozdzielczosc_h / 500 * 240)
		{
			frame[17] = 0;
			OdtwDzwMen[17][0] = 1;
			if (Odtw[17][1] == 0 || Odtw[17][2] == 0 || Odtw[17][3] == 0 || Odtw[17][4] == 0 || Odtw[17][5] == 0) Odtw[17][1] = Odtw[17][2] = Odtw[17][3] = Odtw[17][4] = Odtw[17][5] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 600 && myszkaX <= rozdzielczosc_w / 1000 * 800 && myszkaY >= rozdzielczosc_h / 500 * 200 && myszkaY <= rozdzielczosc_h / 500 * 240)
		{
			frame[17] = 1;
			OdtwDzwMen[17][1] = 1;
			if (Odtw[17][0] == 0 || Odtw[17][2] == 0 || Odtw[17][3] == 0 || Odtw[17][4] == 0 || Odtw[17][5] == 0) Odtw[17][0] = Odtw[17][2] = Odtw[17][3] = Odtw[17][4] = Odtw[17][5] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 840 && myszkaX <= rozdzielczosc_w / 1000 * 990 && myszkaY >= rozdzielczosc_h / 500 * 460 && myszkaY <= rozdzielczosc_h / 500 * 490)
		{
			frame[17] = 2;
			OdtwDzwMen[17][4] = 1;
			if (Odtw[17][0] == 0 || Odtw[17][1] == 0 || Odtw[17][2] == 0 || Odtw[17][3] == 0 || Odtw[17][5] == 0) Odtw[17][0] = Odtw[17][1] = Odtw[17][2] = Odtw[17][3] = Odtw[17][5] = 1;
		}
	}

	if (scen[0] == 22 && klik[4] == 0)
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 83 && myszkaY <= rozdzielczosc_h / 500 * 98)
		{
			frame[18] = 0;

			OdtwDzwMen[18][0] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 415 && myszkaY >= rozdzielczosc_h / 500 * 105 && myszkaY <= rozdzielczosc_h / 500 * 120)
		{
			frame[18] = 1;

			OdtwDzwMen[18][1] = 1;
			if (Odtw[18][0] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][0] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 455 && myszkaY >= rozdzielczosc_h / 500 * 127 && myszkaY <= rozdzielczosc_h / 500 * 142)
		{
			frame[18] = 2;

			OdtwDzwMen[18][2] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][0] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][0] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 475 && myszkaY >= rozdzielczosc_h / 500 * 149 && myszkaY <= rozdzielczosc_h / 500 * 164)
		{
			frame[18] = 3;
			OdtwDzwMen[18][3] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][0] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][0] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 415 && myszkaY >= rozdzielczosc_h / 500 * 171 && myszkaY <= rozdzielczosc_h / 500 * 186)
		{
			frame[18] = 4;
			OdtwDzwMen[18][4] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][0] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][0] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 193 && myszkaY <= rozdzielczosc_h / 500 * 208)
		{
			frame[18] = 5;
			OdtwDzwMen[18][5] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][0] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][0] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 455 && myszkaY >= rozdzielczosc_h / 500 * 215 && myszkaY <= rozdzielczosc_h / 500 * 230)
		{
			frame[18] = 6;
			OdtwDzwMen[18][6] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][0] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][0] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 237 && myszkaY <= rozdzielczosc_h / 500 * 252)
		{
			frame[18] = 7;
			OdtwDzwMen[18][7] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][0] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][0] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 415 && myszkaY >= rozdzielczosc_h / 500 * 259 && myszkaY <= rozdzielczosc_h / 500 * 274)
		{
			frame[18] = 8;
			OdtwDzwMen[18][8] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][0] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][0] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 281 && myszkaY <= rozdzielczosc_h / 500 * 296)
		{
			frame[18] = 9;
			OdtwDzwMen[18][9] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][0] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][0] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 303 && myszkaY <= rozdzielczosc_h / 500 * 318)
		{
			frame[18] = 10;
			OdtwDzwMen[18][10] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][0] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][0] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 325 && myszkaY <= rozdzielczosc_h / 500 * 340)
		{
			frame[18] = 11;
			OdtwDzwMen[18][11] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][0] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][0] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 347 && myszkaY <= rozdzielczosc_h / 500 * 362)
		{
			frame[18] = 12;
			OdtwDzwMen[18][12] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][0] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][0] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 369 && myszkaY <= rozdzielczosc_h / 500 * 384)
		{
			frame[18] = 13;
			OdtwDzwMen[18][13] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][0] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][0] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 250 && myszkaX <= rozdzielczosc_w / 1000 * 435 && myszkaY >= rozdzielczosc_h / 500 * 391 && myszkaY <= rozdzielczosc_h / 500 * 406)
		{
			frame[18] = 14;
			OdtwDzwMen[18][14] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][0] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][0] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 400 && myszkaX <= rozdzielczosc_w / 1000 * 535 && myszkaY >= rozdzielczosc_h / 500 * 435 && myszkaY <= rozdzielczosc_h / 500 * 475)
		{
			frame[18] = 15;
			OdtwDzwMen[18][15] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][0] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][0] =
				Odtw[18][16] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 10 && myszkaX <= rozdzielczosc_w / 1000 * 100 && myszkaY >= rozdzielczosc_h / 500 * 435 && myszkaY <= rozdzielczosc_h / 500 * 475)
		{
			frame[18] = 16;
			OdtwDzwMen[18][16] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][0] == 0 ||
				Odtw[18][17] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][0] = Odtw[18][17] = 1;
		}

		if (myszkaX >= rozdzielczosc_w / 1000 * 840 && myszkaX <= rozdzielczosc_w / 1000 * 930 && myszkaY >= rozdzielczosc_h / 500 * 455 && myszkaY <= rozdzielczosc_h / 500 * 475)
		{
			frame[18] = 17;
			OdtwDzwMen[18][17] = 1;
			if (Odtw[18][1] == 0 || Odtw[18][2] == 0 || Odtw[18][3] == 0 || Odtw[18][4] == 0 ||
				Odtw[18][5] == 0 || Odtw[18][6] == 0 || Odtw[18][7] == 0 || Odtw[18][8] == 0 ||
				Odtw[18][9] == 0 || Odtw[18][10] == 0 || Odtw[18][11] == 0 || Odtw[18][12] == 0 ||
				Odtw[18][13] == 0 || Odtw[18][14] == 0 || Odtw[18][15] == 0 || Odtw[18][16] == 0 ||
				Odtw[18][0] == 0)
				Odtw[18][1] = Odtw[18][2] = Odtw[18][3] = Odtw[18][4] = Odtw[18][5] =
				Odtw[18][6] = Odtw[18][7] = Odtw[18][8] = Odtw[18][9] = Odtw[18][10] =
				Odtw[18][11] = Odtw[18][12] = Odtw[18][13] = Odtw[18][14] = Odtw[18][15] =
				Odtw[18][16] = Odtw[18][0] = 1;
		}
	}

	if (scen[0] == 23)
	{
		if (myszkaX >= rozdzielczosc_w / 1000 * 450 && myszkaX <= rozdzielczosc_w / 1000 * 500 && myszkaY >= rozdzielczosc_h / 500 * 310 && myszkaY <= rozdzielczosc_h / 500 * 360)
		{
			frami = TRUE;
			OdtwDzwMen[19][0] = 1;
			if (Odtw[19][1] == 0)
				Odtw[19][1] = 1;
		}
		else
		{
			frami = FALSE;
			OdtwDzwMen[19][1] = 1;
			if (Odtw[19][0] == 0)
				Odtw[19][0] = 1;
		}

	}
}

void Renderer::CreateBufferViews()
{
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	md3dDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	backBuffer->Release();
}

void Renderer::ResizeBuffers()
{
	md3dImmediateContext->ClearState();
	md3dImmediateContext->Flush();

	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = nullptr;
	}

	mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	CreateBufferViews();
}

void Renderer::SetFullscreen(UINT width, UINT height, BOOL fullscreen)
{
	DXGI_MODE_DESC mode{};
	mode.Width = width;
	mode.Height = height;
	mode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (fullscreen)
	{
		IDXGIOutput* output;
		mSwapChain->GetContainingOutput(&output);

		DXGI_MODE_DESC closestMatch;
		output->FindClosestMatchingMode(&mode, &closestMatch, md3dDevice);
		output->Release();

		mSwapChain->ResizeTarget(&closestMatch);
		ResizeBuffers();
		mSwapChain->SetFullscreenState(true, nullptr);
	}
	else
	{
		mSwapChain->SetFullscreenState(false, nullptr);
		mSwapChain->ResizeTarget(&mode);
	}
}

void Renderer::Resizable()
{
	md3dImmediateContext->OMSetRenderTargets(0, 0, 0);
	mRenderTargetView->Release();

	HR(mSwapChain->ResizeBuffers(0, (UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, DXGI_FORMAT_UNKNOWN, 0));

	ID3D11Texture2D* pBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer));
	HR(md3dDevice->CreateRenderTargetView(pBuffer, NULL, &mRenderTargetView));
	
	pBuffer->Release();
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = rozdzielczosc_w;
	vp.Height = rozdzielczosc_h;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	md3dImmediateContext->RSSetViewports(1, &vp);

	if (!full)
	{
		R = { 0, 0, (LONG)rozdzielczosc_w, (LONG)rozdzielczosc_h };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		FLOAT width = (FLOAT)(R.right - R.left);
		FLOAT height = (FLOAT)(R.bottom - R.top);
		SetWindowPos(mhMainWnd, 0, 0, 0, (INT)width, (INT)height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	
	else if (full)
		SetWindowPos(mhMainWnd, 0, 0, 0, (INT)rozdzielczosc_w, (INT)rozdzielczosc_h, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Renderer::DetectInput()
{
	DetectInputMouse();
	DetectInputKeyboard();
	InputGamepad();
}

void Renderer::ObjektyScen12(XMMATRIX &proj, XMMATRIX &view, FLOAT ObjScen12X[2], FLOAT ScGuz)
{
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 4.0f, 0.2f, 4.0f }, { 0.0f, -2.0f, 0.0f }, ziemia, mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[0].x1, -1.999f, PozGuz[0].y1 }, GuzTwScText[0], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[1].x1, -1.999f, PozGuz[1].y1 }, GuzTwScText[1], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[2].x1, -1.999f, PozGuz[2].y1 }, GuzTwScText[2], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[3].x1, -1.999f, PozGuz[3].y1 }, GuzTwScText[3], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[4].x1, -1.999f, PozGuz[4].y1 }, GuzTwScText[4], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[7].x1, -1.999f, PozGuz[7].y1 }, GuzTwScText[5], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[8].x1, -1.999f, PozGuz[8].y1 }, GuzTwScText[6], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[5].x1, -1.999f, PozGuz[5].y1 }, GuzTwScText[7], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[6].x1, -1.999f, PozGuz[6].y1 }, GuzTwScText[8], mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[9].x1, -1.999f, PozGuz[9].y1 }, GuzTwScText[10], mDirLights, false);

	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.01f, 0.2f, 1.7f }, { ObjScen12X[0], -1.999f, 0.0f }, ogien, mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.01f, 0.2f, 1.7f }, { ObjScen12X[1], -1.999f, 0.0f }, ogien, mDirLights, false);

	if (nowy[0] == 1)
	{
		if(ZaznGuz >= 0 && ZaznGuz <= 19)
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.135f, 0.2f, 0.135f }, { PozGuz[ZaznGuz].x1, -1.998f, PozGuz[ZaznGuz].y1 }, Objects::ZaznPoz, mDirLights, true);

		if (nowy[1] > 0)
		{
			for (INT g = 1; g < IloPunkt - 1; g++)
			{
				if (poz_scian[g] == 1)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[9], mDirLights, false);

				else if (poz_scian[g] == 2)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[9], mDirLights, false);

				else if (poz_scian[g] == 3)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[9], mDirLights, false);

				else if (poz_scian[g] == 4)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[1], mDirLights, false);

				else if (poz_scian[g] == 5)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[2], mDirLights, false);

				else if (poz_scian[g] == 6)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[3], mDirLights, false);

				else if (poz_scian[g] == 7)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[4], mDirLights, false);

				else if (poz_scian[g] == 8)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[7], mDirLights, false);

				else if (poz_scian[g] == 9)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[8], mDirLights, false);

				else if (poz_scian[g] == 10)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[5], mDirLights, false);

				else if (poz_scian[g] == 11)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[6], mDirLights, false);

				else if (poz_scian[g] == 12)
					Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][g].x1, -1.999f, pozycja_sciany[1][g].y1 }, GuzTwScText[10], mDirLights, false);
			}
			wysw();

			if (ZaznGuz >= 20)
				Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { ScGuz, 0.2f, ScGuz }, { pozycja_sciany[1][ZaznGuz - 19].x1, -1.998f, pozycja_sciany[1][ZaznGuz - 19].y1 }, Objects::ZaznPoz, mDirLights, true);
		}
	}
}

void Renderer::InitMouse()
{
	DI->CreateDevice(GUID_SysMouse, &DIMouse, NULL);
	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(mhMainWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	DIMouseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	DIMouse->SetEventNotification(DIMouseEvent);

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = SAMPLE_BUFFER_SIZE;
	DIMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	DIMouse->Acquire();
}

void Renderer::DestroyMouse()
{
	if (DIMouse)
	{
		DIMouse->Unacquire();
		DIMouse->Release();
		DIMouse = NULL;
	}

	if (DIMouseEvent)
	{
		CloseHandle(DIMouseEvent);
		DIMouseEvent = NULL;
	}
}

void Renderer::InitKeyboard()
{
	DI->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	DIKeyboard->SetCooperativeLevel(mhMainWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	DIKeyboard->Acquire();
}

void Renderer::DestroyKeyboard()
{
	if (DIKeyboard)
	{
		DIKeyboard->Unacquire();
		DIKeyboard->Release();
		DIKeyboard = NULL;
	}
}

void Renderer::InputGamepad()
{
	for (DWORD ControllerIndex = 0; ControllerIndex < XUSER_MAX_COUNT; ++ControllerIndex)
	{
		XINPUT_STATE ControllerState;
		if (XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
		{
			XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

			if (Pad->sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && !keysPressed[86])
			{
				keysPressed[86] = TRUE;
				if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
				{
					frame[0]--;
					if (frame[0] < 0) frame[0] = 2;
					OdtwDzwMen[0][frame[0]] = Odtw[0][frame[0]] = 1;
				}

				else if ((scen[2] == 1) || (scen[2] == 6) || (scen[2] == 8) || (scen[2] == 10))
				{
					frame[1]--;
					if (frame[1] < 0) frame[1] = 3;
					OdtwDzwMen[1][frame[1]] = Odtw[1][frame[1]] = 1;
				}

				else if ((scen[2] == 2) || (scen[2] == 15))
				{
					frame[2]--;
					if (frame[2] < 0) frame[2] = 3;
					OdtwDzwMen[2][frame[2]] = Odtw[2][frame[2]] = 1;
				}

				else if ((scen[2] == 5) || (scen[2] == 13))
				{
					frame[3]--;
					if (frame[3] < 0) frame[3] = 2;
					OdtwDzwMen[3][frame[3]] = Odtw[3][frame[3]] = 1;
				}

				else if (scen[2] == 7)
				{
					if (WczytLabiryntu % 2 == 1)
					{
						frame[4]--;
						if (frame[4] < 0) frame[4] = 1;
						OdtwDzwMen[4][frame[4]] = Odtw[4][frame[4]] = 1;
					}

					else if (WczytLabiryntu % 2 == 0)
					{
						if (frame[16] == IloLiter)
							frame[16] = 0;

						if (frame[16] < IloscPlikow[1][0])
						{
							if (frame[16] > 0)
								frame[16]--;

							else if (frame[16] <= 0)
							{
								if (IloscPlikow[1][1] > 1)
									IloscPlikow[1][1]--;

								else if (IloscPlikow[1][0] > 16)
								{
									IloscPlikow[1][1] = IloscPlikow[1][0] - 17;
									frame[16] = IloscPlikow[1][0];
								}

								else if (IloscPlikow[1][1] == 1)
									frame[16] = IloscPlikow[1][0];
							}
						}

						else if (frame[16] == IloscPlikow[1][0])
						{
							if (IloscPlikow[1][0] > 16)
								IloscPlikow[1][1] = frame[16] - 17;

							if (IloscPlikow[1][1] > 1)
								frame[16] = frame[16] - IloscPlikow[1][1] - 1;

							else if (IloscPlikow[1][1] == 1)
								frame[16] = IloscPlikow[1][0] - 1;
						}

						OdtwDzwMen[16][0] = Odtw[16][0] = 1;
					}
				}

				else if (scen[2] == 9)
				{
					frame[5]--;
					if (frame[5] < 0) frame[5] = 1;
					OdtwDzwMen[5][frame[5]] = Odtw[5][frame[5]] = 1;
				}

				else if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[86].ustawiony_klaw >= 20 && klaw[86].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[86].ustawiony_klaw >= 20 && klaw[86].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[86].ustawiony_klaw >= 20 && klaw[86].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[86].ustawiony_klaw >= 20 && klaw[86].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[86].ustawiony_klaw >= 16 && klaw[86].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[86].ustawiony_klaw >= 16 && klaw[86].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[86].ustawiony_klaw >= 16 && klaw[86].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[86].ustawiony_klaw >= 16 && klaw[86].ustawiony_klaw <= 19)))
						keysPressed[86] = FALSE;

					reset_czasu = 0;
					if (po % 2 == 0 && ZapiszGiere % 2 == 1)
					{
						keysPressed[86] = TRUE;
						frame[9]--;
						if (frame[9] < 0) frame[9] = 5;
						OdtwDzwMen[9][frame[9]] = Odtw[9][frame[9]] = 1;
					}

					klawa(86);
				}

				else if ((scen[0] == 12 && ZaznGuz == 0) ||
					(scen[0] == 12 && ZaznGuz == 2) ||
					(scen[0] == 12 && ZaznGuz == 4) ||
					(scen[0] == 12 && ZaznGuz == 6) ||
					(scen[0] == 12 && ZaznGuz == 8) ||
					(scen[0] == 12 && CzerwonyNapis[7][10] == 1 && ZaznGuz == 10))
				{
					frame[7] -= 2;
					ZaznGuz -= 2;
					if (ZaznGuz < 0) ZaznGuz = 10;
					if (frame[7] < 0) frame[7] = 10;

					if (frame[7] == 0)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 2)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 4)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 6)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 8)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 10)
					{
						CzerwonyNapis[7][11] = 0;
						CzerwonyNapis[7][10] = 1;
					}

					OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
				}

				else if ((scen[0] == 12 && ZaznGuz == 1) ||
					(scen[0] == 12 && ZaznGuz == 3) ||
					(scen[0] == 12 && ZaznGuz == 5) ||
					(scen[0] == 12 && ZaznGuz == 7) ||
					(scen[0] == 12 && ZaznGuz == 9) ||
					(scen[0] == 12 && CzerwonyNapis[7][11] == 1 && ZaznGuz == 11))
				{
					frame[7] -= 2;
					ZaznGuz -= 2;
					if (ZaznGuz < 1) ZaznGuz = 11;
					if (frame[7] < 1) frame[7] = 11;

					if (frame[7] == 1)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 3)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 5)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 7)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 9)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 11)
					{
						CzerwonyNapis[7][10] = 0;
						CzerwonyNapis[7][11] = 1;
					}

					OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
				}

				else if (scen[0] == 12 && ZaznGuz == 15)
				{
					frame[6]--;
					if (frame[6]<0) frame[6] = 3;
					OdtwDzwMen[6][frame[6]] = Odtw[6][frame[6]] = 1;
				}

				else if (scen[0] == 12 && ZaznGuz >= 20)
				{
					if (ZaznGuz <= 45)
					{
						if (nowy[1] > 0)
						{
							w -= 26;
							if (w <= 1) w = 1;
						}
					}

					else if (ZaznGuz > 45)
						ZaznGuz -= 26;

					ZaznKeys();
				}

				else if (scen[2] == 14 && klik[0] == 0)
				{
					frame[8]--;
					if (frame[8] < 0) frame[8] = 15;
					OdtwDzwMen[8][frame[8]] = Odtw[8][frame[8]] = 1;
				}

				else if (scen[2] == 16 && !ustaw_rozdzielczosc && !ustaw_fullekran)
				{
					frame[10]--;
					if (frame[10] < 0) frame[10] = 3;
					OdtwDzwMen[10][frame[10]] = Odtw[10][frame[10]] = 1;
				}

				else if (scen[2] == 16 && ustaw_fullekran)
				{
					frame[12]--;
					if (frame[12] < 1) frame[12] = 2;
					OdtwDzwMen[12][frame[12]] = Odtw[12][frame[12]] = 1;

					frame[10] = 10;
					frame[13] = 4;
				}

				else if (scen[2] == 16 && ustaw_rozdzielczosc)
				{
					if (frame[13] == 0)
					{
						lr[0]--;
						lr[1]--;
					}

					if (lr[0] <= 0) lr[0] = 0;
					if (lr[1] <= 1) lr[1] = 1;

					rozdz[lr[0]].ind = 1;
					rozdz[lr[1]].ind = 2;

					frame[13]--;
					if (frame[13] < 0) frame[13] = 0;
					OdtwDzwMen[13][frame[13]] = Odtw[13][frame[13]] = 1;

					frame[12] = 4;
					frame[10] = 10;
				}

				else if (scen[2] == 17)
				{
					frame[11]--;
					if (frame[11] < 0) frame[11] = 2;
					OdtwDzwMen[11][frame[11]] = Odtw[11][frame[11]] = 1;
				}

				else if (scen[2] == 19)
				{
					if (frame[16] == IloLiter)
						frame[16] = 0;

					if (frame[16] < IloscPlikow[0][0])
					{
						if (frame[16] > 0)
							frame[16]--;

						else if (frame[16] <= 0)
						{
							if (IloscPlikow[0][1] > 1)
								IloscPlikow[0][1]--;

							else if (IloscPlikow[0][0] > 16)
							{
								IloscPlikow[0][1] = IloscPlikow[0][0] - 17;
								frame[16] = IloscPlikow[0][0];
							}

							else if (IloscPlikow[0][1] == 1)
								frame[16] = IloscPlikow[0][0];
						}
					}

					else if (frame[16] == IloscPlikow[0][0])
					{
						if (IloscPlikow[0][0] > 16)
							IloscPlikow[0][1] = frame[16] - 17;

						if (IloscPlikow[0][1] > 1)
							frame[16] = frame[16] - IloscPlikow[0][1] - 1;

						else if (IloscPlikow[0][1] == 1)
							frame[16] = IloscPlikow[0][0] - 1;
					}

					OdtwDzwMen[16][0] = Odtw[16][0] = 1;
				}

				else if (scen[2] == 21)
				{
					if (frame[17] == 0) frame[17] = 2;
					else if (frame[17] == 1) frame[17] = 3;
					else if (frame[17] == 2) frame[17] = 0;
					else if (frame[17] == 3) frame[17] = 1;
					OdtwDzwMen[17][frame[17]] = Odtw[17][frame[17]] = 1;
				}

				else if (scen[0] == 22 && klik[4] == 0)
				{
					frame[18]--;
					if (frame[18] < 0) frame[18] = 15;
					OdtwDzwMen[18][frame[18]] = Odtw[18][frame[18]] = 1;
				}

				else if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(86);
					klik[4] = 0;
				}
			}
			else if (!(Pad->sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && keysPressed[86])
			{
				keysPressed[86] = FALSE;
				klawa_up(86);
			}

			if (-Pad->sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && !keysPressed[87])
			{
				keysPressed[87] = TRUE;
				if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
				{
					frame[0]++;
					if (frame[0] > 2) frame[0] = 0;
					OdtwDzwMen[0][frame[0]] = Odtw[0][frame[0]] = 1;
				}

				else if ((scen[2] == 1) || (scen[2] == 6) || (scen[2] == 8) || (scen[2] == 10))
				{
					frame[1]++;
					if (frame[1] > 3) frame[1] = 0;
					OdtwDzwMen[1][frame[1]] = Odtw[1][frame[1]] = 1;
				}

				else if ((scen[2] == 2) || (scen[2] == 15))
				{
					frame[2]++;
					if (frame[2] > 3) frame[2] = 0;
					OdtwDzwMen[2][frame[2]] = Odtw[2][frame[2]] = 1;
				}

				else if ((scen[2] == 5) || (scen[2] == 13))
				{
					frame[3]++;
					if (frame[3] > 2) frame[3] = 0;
					OdtwDzwMen[3][frame[3]] = Odtw[3][frame[3]] = 1;
				}

				else if (scen[2] == 7)
				{
					if (WczytLabiryntu % 2 == 1)
					{
						frame[4]++;
						if (frame[4] > 1) frame[4] = 0;
						OdtwDzwMen[4][frame[4]] = Odtw[4][frame[4]] = 1;
					}

					if (WczytLabiryntu % 2 == 0)
					{
						if (frame[16] == IloLiter)
							frame[16] = 0;

						if (frame[16] < IloscPlikow[1][0])
						{
							if (frame[16] < 16)
								frame[16]++;

							else if (frame[16] == 16 && IloscPlikow[1][1] < IloscPlikow[1][0] - 17)
								IloscPlikow[1][1]++;

							else if (IloscPlikow[1][1] == IloscPlikow[1][0] - 17)
								frame[16] += IloscPlikow[1][1] + 1;
						}

						else if (frame[16] >= IloscPlikow[1][0])
						{
							frame[16] = 0;
							IloscPlikow[1][1] = 1;
						}

						OdtwDzwMen[16][0] = Odtw[16][0] = 1;
					}
				}

				else if (scen[2] == 9)
				{
					frame[5]++;
					if (frame[5] > 1) frame[5] = 0;
					OdtwDzwMen[5][frame[5]] = Odtw[5][frame[5]] = 1;
				}

				else if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[87].ustawiony_klaw >= 20 && klaw[87].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[87].ustawiony_klaw >= 20 && klaw[87].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[87].ustawiony_klaw >= 20 && klaw[87].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[87].ustawiony_klaw >= 20 && klaw[87].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[87].ustawiony_klaw >= 16 && klaw[87].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[87].ustawiony_klaw >= 16 && klaw[87].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[87].ustawiony_klaw >= 16 && klaw[87].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[87].ustawiony_klaw >= 16 && klaw[87].ustawiony_klaw <= 19)))
						keysPressed[87] = FALSE;

					reset_czasu = 0;
					if (po % 2 == 0 && ZapiszGiere % 2 == 1)
					{
						keysPressed[87] = TRUE;
						frame[9]++;
						if (frame[9] > 5) frame[9] = 0;
						OdtwDzwMen[9][frame[9]] = Odtw[9][frame[9]] = 1;
					}

					klawa(87);
				}

				else if ((scen[0] == 12 && ZaznGuz == 0) ||
					(scen[0] == 12 && ZaznGuz == 2) ||
					(scen[0] == 12 && ZaznGuz == 4) ||
					(scen[0] == 12 && ZaznGuz == 6) ||
					(scen[0] == 12 && ZaznGuz == 8) ||
					(scen[0] == 12 && CzerwonyNapis[7][10] == 1 && ZaznGuz == 10))
				{
					frame[7] += 2;
					ZaznGuz += 2;
					if (ZaznGuz > 10) ZaznGuz = 0;
					if (frame[7] > 10) frame[7] = 0;

					if (frame[7] == 0)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 2)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 4)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 6)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 8)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 10)
					{
						CzerwonyNapis[7][11] = 0;
						CzerwonyNapis[7][10] = 1;
					}

					OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
				}

				else if ((scen[0] == 12 && ZaznGuz == 1) ||
					(scen[0] == 12 && ZaznGuz == 3) ||
					(scen[0] == 12 && ZaznGuz == 5) ||
					(scen[0] == 12 && ZaznGuz == 7) ||
					(scen[0] == 12 && ZaznGuz == 9) ||
					(scen[0] == 12 && CzerwonyNapis[7][11] == 1 && ZaznGuz == 11))
				{
					frame[7] += 2;
					ZaznGuz += 2;
					if (ZaznGuz > 11) ZaznGuz = 1;
					if (frame[7] > 11) frame[7] = 1;

					if (frame[7] == 1)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 3)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 5)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 7)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 9)
						CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

					else if (frame[7] == 11)
					{
						CzerwonyNapis[7][10] = 0;
						CzerwonyNapis[7][11] = 1;
					}

					OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
				}

				else if (scen[0] == 12 && ZaznGuz >= 20)
				{
					if (ZaznGuz < ZaznaczGuziczek)
						ZaznGuz += 26;

					else if (ZaznGuz >= ZaznaczGuziczek)
					{
						if (nowy[1] > 0)
						{
							w += 26;
							if (w >= 781) w = 781;
						}
					}

					ZaznKeys();
				}

				else if (scen[0] == 12 && ZaznGuz == 15)
				{
					frame[6]++;
					if (frame[6]>3) frame[6] = 0;
					OdtwDzwMen[6][frame[6]] = Odtw[6][frame[6]] = 1;
				}

				else if (scen[2] == 14 && klik[0] == 0)
				{
					frame[8]++;
					if (frame[8] > 15) frame[8] = 0;
					OdtwDzwMen[8][frame[8]] = Odtw[8][frame[8]] = 1;
				}

				else if (scen[2] == 16 && !ustaw_rozdzielczosc && !ustaw_fullekran)
				{
					frame[10]++;
					if (frame[10] > 3) frame[10] = 0;
					OdtwDzwMen[10][frame[10]] = Odtw[10][frame[10]] = 1;
				}

				else if (scen[2] == 16 && ustaw_fullekran)
				{
					frame[12]++;
					if (frame[12] > 2) frame[12] = 1;
					OdtwDzwMen[12][frame[12]] = Odtw[12][frame[12]] = 1;

					frame[10] = 10;
					frame[13] = 4;
				}

				else if (scen[2] == 16 && ustaw_rozdzielczosc)
				{
					if (frame[13] == 1)
					{
						lr[0]++;
						lr[1]++;
					}

					if (lr[0] >= index2 - 2)  lr[0] = index2 - 2;
					if (lr[1] >= index2 - 1)  lr[1] = index2 - 1;

					rozdz[lr[0]].ind = 2;
					rozdz[lr[1]].ind = 1;

					frame[13]++;
					if (frame[13] > 1) frame[13] = 1;

					OdtwDzwMen[13][frame[13]] = Odtw[13][frame[13]] = 1;

					frame[12] = 4;
					frame[10] = 10;
				}

				else if (scen[2] == 17)
				{
					frame[11]++;
					if (frame[11] > 2) frame[11] = 0;
					OdtwDzwMen[11][frame[11]] = Odtw[11][frame[11]] = 1;
				}

				else if (scen[2] == 19)
				{
					if (frame[16] == IloLiter)
						frame[16] = 0;

					if (frame[16] < IloscPlikow[0][0])
					{
						if (frame[16] < 16)
							frame[16]++;

						else if (frame[16] == 16 && IloscPlikow[0][1] < IloscPlikow[0][0] - 17)
							IloscPlikow[0][1]++;

						else if (IloscPlikow[0][1] == IloscPlikow[0][0] - 17)
							frame[16] += IloscPlikow[0][1] + 1;
					}

					else if (frame[16] >= IloscPlikow[0][0])
					{
						frame[16] = 0;
						IloscPlikow[0][1] = 1;
					}

					OdtwDzwMen[16][0] = Odtw[16][0] = 1;
				}

				else if (scen[2] == 21)
				{
					if (frame[17] == 0) frame[17] = 2;
					else if (frame[17] == 1) frame[17] = 3;
					else if (frame[17] == 2) frame[17] = 0;
					else if (frame[17] == 3) frame[17] = 1;
					OdtwDzwMen[17][frame[17]] = Odtw[17][frame[17]] = 1;
				}

				else if (scen[2] == 22 && klik[4] == 0)
				{
					frame[18]++;
					if (frame[18] > 15) frame[18] = 0;
					OdtwDzwMen[18][frame[18]] = Odtw[18][frame[18]] = 1;
				}

				else if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(87);
					klik[4] = 0;
				}
			}
			else if (!(-Pad->sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && keysPressed[87])
			{
				keysPressed[87] = FALSE;
				klawa_up(87);
			}

			if (-Pad->sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && !keysPressed[88])
			{
				keysPressed[88] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[88].ustawiony_klaw >= 20 && klaw[88].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[88].ustawiony_klaw >= 20 && klaw[88].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[88].ustawiony_klaw >= 20 && klaw[88].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[88].ustawiony_klaw >= 20 && klaw[88].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[88].ustawiony_klaw >= 16 && klaw[88].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[88].ustawiony_klaw >= 16 && klaw[88].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[88].ustawiony_klaw >= 16 && klaw[88].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[88].ustawiony_klaw >= 16 && klaw[88].ustawiony_klaw <= 19)))
						keysPressed[88] = FALSE;

					klawa(88);
				}

				if (scen[0] == 12 && ZaznGuz <= 19)
				{
					if ((scen[0] == 12 && ZaznGuz == 0) || (scen[0] == 12 && ZaznGuz == 1))
					{
						frame[7]--;
						if (frame[7] < 0) frame[7] = 1;

						if (frame[7] == 0)
						{
							ZaznGuz = 0;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 1)
						{
							ZaznGuz = 1;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && ZaznGuz == 2) || (scen[0] == 12 && ZaznGuz == 3))
					{
						frame[7]--;
						if (frame[7] < 2) frame[7] = 3;

						if (frame[7] == 2)
						{
							ZaznGuz = 2;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 3)
						{
							ZaznGuz = 3;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && ZaznGuz == 4) || (scen[0] == 12 && ZaznGuz == 5))
					{
						frame[7]--;
						if (frame[7] < 4) frame[7] = 5;

						if (frame[7] == 4)
						{
							ZaznGuz = 4;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 5)
						{
							ZaznGuz = 5;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && ZaznGuz == 6) || (scen[0] == 12 && ZaznGuz == 7))
					{
						frame[7]--;
						if (frame[7] < 6) frame[7] = 7;

						if (frame[7] == 6)
						{
							ZaznGuz = 6;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 7)
						{
							ZaznGuz = 7;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && ZaznGuz == 8) || (scen[0] == 12 && ZaznGuz == 9))
					{
						frame[7]--;
						if (frame[7] < 8) frame[7] = 9;

						if (frame[7] == 8)
						{
							ZaznGuz = 8;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 9)
						{
							ZaznGuz = 9;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && CzerwonyNapis[7][10] == 1 && ZaznGuz == 10) ||
						(scen[0] == 12 && CzerwonyNapis[7][11] == 1 && ZaznGuz == 11))
					{
						frame[7]--;
						if (frame[7] < 10) frame[7] = 11;

						if (frame[7] == 10)
						{
							ZaznGuz = 10;
							CzerwonyNapis[7][11] = 0;
							CzerwonyNapis[7][10] = 1;
						}

						if (frame[7] == 11)
						{
							ZaznGuz = 11;
							CzerwonyNapis[7][10] = 0;
							CzerwonyNapis[7][11] = 1;
						}
						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}
				}
				else if (scen[0] == 12 && ZaznGuz >= 20)
				{
					if (ZaznGuz != 20 && ZaznGuz != 46 && ZaznGuz != 72 && ZaznGuz != 98 && ZaznGuz != 124 && ZaznGuz != 150 && ZaznGuz != 176 && ZaznGuz != 202 && ZaznGuz != 228 && ZaznGuz != 254 && ZaznGuz != 280 && ZaznGuz != 306 && ZaznGuz != 332 && ZaznGuz != 358 && ZaznGuz != 384 && ZaznGuz != 410 && ZaznGuz != 436 && ZaznGuz != 462 && ZaznGuz != 488 && ZaznGuz != 514 && ZaznGuz != 540 && ZaznGuz != 566 && ZaznGuz != 592 && ZaznGuz != 618 && ZaznGuz != 644 && ZaznGuz != 670 && ZaznGuz != 696 && ZaznGuz != 722 && ZaznGuz != 748 && ZaznGuz != 774 && ZaznGuz != 800 && ZaznGuz != 826 && ZaznGuz != 852 && ZaznGuz != 878 && ZaznGuz != 904)
						ZaznGuz--;

					ZaznKeys();
				}

				else if (scen[2] == 14 && klik[0] == 0)
				{
					if (frame[8] == 15 || frame[8] == 16 || frame[8] == 17)
					{
						keysPressed[88] = TRUE;
						frame[8]++;
						if (frame[8] > 17) frame[8] = 15;
						OdtwDzwMen[8][frame[8]] = Odtw[8][frame[8]] = 1;
					}
				}

				if (scen[0] == 16)
				{
					keysPressed[88] = FALSE;
					if (frame[10] == 2 && klik[1] == 1)
					{
						zapis[3] = false;
						jas_sw -= 0.1f;
						if (jas_sw <= 0.2f) jas_sw = 0.2f;

						if (jas_sw == 0.7f || jas_sw == 0.8f || jas_sw == 1.9f || jas_sw == 1.8f)
							Tlx[0] -= 0.015f;
						else
							Tlx[0] -= 0.03f;

						if (Tlx[0] <= 0.8f) Tlx[0] = 0.8f;
					}

					if (frame[10] == 3 || frame[10] == 4 || frame[10] == 5)
					{
						keysPressed[88] = TRUE;
						frame[10]++;
						if (frame[10] > 5) frame[10] = 3;
						OdtwDzwMen[10][frame[10]] = Odtw[10][frame[10]] = 1;
					}
				}

				else if (scen[0] == 17)
				{
					keysPressed[88] = FALSE;
					if (frame[11] == 0 && klik[2] == 1)
					{
						zapis[2] = false;

						if (glosnosc_tla == -7496)
						{
							Tlx[1] -= 0.015f;
							glosnosc_tla -= 840;
						}

						else if (glosnosc_tla == -7080 || glosnosc_tla == -2504 || glosnosc_tla == -2920)
						{
							Tlx[1] -= 0.015f;
							glosnosc_tla -= 416;
						}

						else
						{
							glosnosc_tla -= 416;
							Tlx[1] -= 0.03f;
						}

						if (glosnosc_tla <= -10000) glosnosc_tla = -10000;
						if (Tlx[1] <= 0.8) Tlx[1] = 0.8f;

						muza.muzyka_tlo_volume(glosnosc_tla);
					}

					if (frame[11] == 1 && klik[3] == 1)
					{
						zapis[2] = false;

						if (glosnosc_efektow == -7496)
						{
							Tlx[2] -= 0.015f;
							glosnosc_efektow -= 840;
						}

						else if (glosnosc_efektow == -7080 || glosnosc_efektow == -2504 || glosnosc_efektow == -2920)
						{
							Tlx[2] -= 0.015f;
							glosnosc_efektow -= 416;
						}

						else
						{
							glosnosc_efektow -= 416;
							Tlx[2] -= 0.03f;
						}

						if (glosnosc_efektow <= -10000) glosnosc_efektow = -10000;
						if (Tlx[2] <= 0.8f) Tlx[2] = 0.8f;

						muza.dzwiek_potwierdzenie(glosnosc_efektow);
					}

					if (frame[11] == 2 || frame[11] == 3 || frame[11] == 4)
					{
						keysPressed[88] = TRUE;
						frame[11]++;
						if (frame[11] > 4) frame[11] = 2;
						OdtwDzwMen[11][frame[11]] = Odtw[11][frame[11]] = 1;
					}
				}

				else if (scen[0] == 18)
				{
					frame[14]--;
					if (frame[14] < 0) frame[14] = 1;
					OdtwDzwMen[14][frame[14]] = Odtw[14][frame[14]] = 1;
				}

				else if ((scen[0] == 20) || (po % 2 == 0 && ZapiszGiere % 2 == 0))
				{
					frame[15]--;
					if (frame[15] < 0) frame[15] = 1;
					OdtwDzwMen[15][frame[15]] = Odtw[15][frame[15]] = 1;
				}

				else if (scen[2] == 21)
				{
					if (frame[17] == 0) frame[17] = 1;
					else if (frame[17] == 1) frame[17] = 0;
					OdtwDzwMen[17][frame[17]] = Odtw[17][frame[17]] = 1;
				}

				else if (scen[2] == 22 && klik[4] == 0)
				{
					if (frame[18] == 15 || frame[18] == 16 || frame[18] == 17)
					{
						keysPressed[88] = TRUE;
						frame[18]++;
						if (frame[18] > 17) frame[18] = 15;
						OdtwDzwMen[18][frame[18]] = Odtw[18][frame[18]] = 1;
					}
				}

				else if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(88);
					klik[4] = 0;
				}
			}
			else if (!(-Pad->sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && keysPressed[88])
			{
				keysPressed[88] = FALSE;
				klawa_up(88);
			}

			if (Pad->sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && !keysPressed[89])
			{
				keysPressed[89] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[89].ustawiony_klaw >= 20 && klaw[89].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[89].ustawiony_klaw >= 20 && klaw[89].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[89].ustawiony_klaw >= 20 && klaw[89].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[89].ustawiony_klaw >= 20 && klaw[89].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[89].ustawiony_klaw >= 16 && klaw[89].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[89].ustawiony_klaw >= 16 && klaw[89].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[89].ustawiony_klaw >= 16 && klaw[89].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[89].ustawiony_klaw >= 16 && klaw[89].ustawiony_klaw <= 19)))
						keysPressed[89] = FALSE;

					klawa(89);
				}

				else if (scen[0] == 12 && ZaznGuz <= 19)
				{
					if ((scen[0] == 12 && ZaznGuz == 0) || (scen[0] == 12 && ZaznGuz == 1))
					{
						frame[7]++;
						if (frame[7] > 1) frame[7] = 0;

						if (frame[7] == 0)
						{
							ZaznGuz = 0;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 1)
						{
							ZaznGuz = 1;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && ZaznGuz == 2) || (scen[0] == 12 && ZaznGuz == 3))
					{
						frame[7]++;
						if (frame[7] > 3) frame[7] = 2;

						if (frame[7] == 2)
						{
							ZaznGuz = 2;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 3)
						{
							ZaznGuz = 3;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && ZaznGuz == 4) || (scen[0] == 12 && ZaznGuz == 5))
					{
						frame[7]++;
						if (frame[7] > 5) frame[7] = 4;

						if (frame[7] == 4)
						{
							ZaznGuz = 4;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 5)
						{
							ZaznGuz = 5;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && ZaznGuz == 6) || (scen[0] == 12 && ZaznGuz == 7))
					{
						frame[7]++;
						if (frame[7] > 7) frame[7] = 6;

						if (frame[7] == 6)
						{
							ZaznGuz = 6;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 7)
						{
							ZaznGuz = 7;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && ZaznGuz == 8) || (scen[0] == 12 && ZaznGuz == 9))
					{
						frame[7]++;
						if (frame[7] > 9) frame[7] = 8;

						if (frame[7] == 8)
						{
							ZaznGuz = 8;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						if (frame[7] == 9)
						{
							ZaznGuz = 9;
							CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if ((scen[0] == 12 && CzerwonyNapis[7][10] == 1 && ZaznGuz == 10) || (scen[0] == 12 && CzerwonyNapis[7][11] == 1 && ZaznGuz == 11))
					{
						frame[7]++;
						if (frame[7] > 11) frame[7] = 10;

						if (frame[7] == 10)
						{
							ZaznGuz = 10;
							CzerwonyNapis[7][11] = 0;
							CzerwonyNapis[7][10] = 1;
						}

						if (frame[7] == 11)
						{
							ZaznGuz = 11;
							CzerwonyNapis[7][10] = 0;
							CzerwonyNapis[7][11] = 1;
						}

						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}

					else if (scen[0] == 12 && CzerwonyNapis[7][10] == 0 && CzerwonyNapis[7][11] == 0)
					{
						frame[7] = 0;
						frame[6] = 10;
						OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
					}
				}

				else if (scen[0] == 12 && ZaznGuz >= 20)
				{
					if (ZaznGuz != 45 && ZaznGuz != 71 && ZaznGuz != 97 && ZaznGuz != 123 && ZaznGuz != 149 && ZaznGuz != 175 && ZaznGuz != 201 && ZaznGuz != 227 && ZaznGuz != 253 && ZaznGuz != 279 && ZaznGuz != 305 && ZaznGuz != 331 && ZaznGuz != 357 && ZaznGuz != 383 && ZaznGuz != 409 && ZaznGuz != 435 && ZaznGuz != 461 && ZaznGuz != 487 && ZaznGuz != 513 && ZaznGuz != 539 && ZaznGuz != 565 && ZaznGuz != 591 && ZaznGuz != 617 && ZaznGuz != 643 && ZaznGuz != 669 && ZaznGuz != 695 && ZaznGuz != 721 && ZaznGuz != 747 && ZaznGuz != 773 && ZaznGuz != 799 && ZaznGuz != 825 && ZaznGuz != 851 && ZaznGuz != 877 && ZaznGuz != 903 && ZaznGuz != 929 && ZaznGuz != 955 && ZaznGuz != 981 && ZaznGuz != 1007)
						ZaznGuz++;

					ZaznKeys();
				}

				else if (scen[2] == 14 && klik[0] == 0)
				{
					if (frame[8] == 15 || frame[8] == 16 || frame[8] == 17)
					{
						keysPressed[89] = TRUE;
						frame[8]--;
						if (frame[8] < 15) frame[8] = 17;
						OdtwDzwMen[8][frame[8]] = Odtw[8][frame[8]] = 1;
					}
				}

				if (scen[0] == 16)
				{
					keysPressed[89] = FALSE;
					if (frame[10] == 2 && klik[1] == 1)
					{
						zapis[3] = false;
						jas_sw += 0.1f;
						if (jas_sw >= 2.4f) jas_sw = 2.4f;

						if (jas_sw == 0.6f || jas_sw == 0.7f || jas_sw == 1.7f || jas_sw == 1.8f)
							Tlx[0] += 0.015f;
						else
							Tlx[0] += 0.03f;

						if (Tlx[0] >= 1.4f) Tlx[0] = 1.4f;
					}

					if (frame[10] == 3 || frame[10] == 4 || frame[10] == 5)
					{
						keysPressed[89] = TRUE;
						frame[10]--;
						if (frame[10] < 3) frame[10] = 5;
						OdtwDzwMen[10][frame[10]] = Odtw[10][frame[10]] = 1;
					}
				}

				else if (scen[0] == 17)
				{
					keysPressed[89] = FALSE;
					if (frame[11] == 0 && klik[2] == 1)
					{
						zapis[2] = false;

						if (glosnosc_tla == -7496)
						{
							Tlx[1] += 0.015f;
							glosnosc_tla += 840;
						}

						else if (glosnosc_tla == -7080 || glosnosc_tla == -2504 || glosnosc_tla == -2920)
						{
							Tlx[1] += 0.015f;
							glosnosc_tla += 416;
						}

						else
						{
							glosnosc_tla += 416;
							Tlx[1] += 0.03f;
						}

						if (glosnosc_tla >= 0) glosnosc_tla = 0;
						if (Tlx[1] >= 1.4f) Tlx[1] = 1.4f;

						muza.muzyka_tlo_volume(glosnosc_tla);
					}

					if (frame[11] == 1 && klik[3] == 1)
					{
						zapis[2] = false;

						if (glosnosc_efektow == -7496)
						{
							Tlx[2] += 0.015f;
							glosnosc_efektow += 840;
						}

						else if (glosnosc_efektow == -7080 || glosnosc_efektow == -2504 || glosnosc_efektow == -2920)
						{
							Tlx[2] += 0.015f;
							glosnosc_efektow += 416;
						}

						else
						{
							glosnosc_efektow += 416;
							Tlx[2] += 0.03f;
						}

						if (glosnosc_efektow >= 0) glosnosc_efektow = 0;
						if (Tlx[2] >= 1.4f) Tlx[2] = 1.4f;

						muza.dzwiek_potwierdzenie(glosnosc_efektow);
					}

					if (frame[11] == 2 || frame[11] == 3 || frame[11] == 4)
					{
						keysPressed[89] = TRUE;
						frame[11]--;
						if (frame[11] < 2) frame[11] = 4;
						OdtwDzwMen[11][frame[11]] = Odtw[11][frame[11]] = 1;
					}
				}

				else if (scen[0] == 18)
				{
					frame[14]++;
					if (frame[14] > 1) frame[14] = 0;
					OdtwDzwMen[14][frame[14]] = Odtw[14][frame[14]] = 1;
				}

				else if ((scen[0] == 20) || (po % 2 == 0 && ZapiszGiere % 2 == 0))
				{
					frame[15]++;
					if (frame[15] > 1) frame[15] = 0;
					OdtwDzwMen[15][frame[15]] = Odtw[15][frame[15]] = 1;
				}

				else if (scen[2] == 21)
				{
					if (frame[17] == 0) frame[17] = 1;
					else if (frame[17] == 1) frame[17] = 0;
					OdtwDzwMen[17][frame[17]] = Odtw[17][frame[17]] = 1;
				}

				else if (scen[2] == 22 && klik[4] == 0)
				{
					if (frame[18] == 15 || frame[18] == 16 || frame[18] == 17)
					{
						keysPressed[89] = TRUE;
						frame[18]--;
						if (frame[18] < 15) frame[18] = 17;
						OdtwDzwMen[18][frame[18]] = Odtw[18][frame[18]] = 1;
					}
				}

				else if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(89);
					klik[4] = 0;
				}
			}
			else if (!(Pad->sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) && keysPressed[89])
			{
				keysPressed[89] = FALSE;
				klawa_up(89);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB) && !keysPressed[90])
			{
				keysPressed[90] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[90].ustawiony_klaw >= 20 && klaw[90].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[90].ustawiony_klaw >= 20 && klaw[90].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[90].ustawiony_klaw >= 20 && klaw[90].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[90].ustawiony_klaw >= 20 && klaw[90].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[90].ustawiony_klaw >= 16 && klaw[90].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[90].ustawiony_klaw >= 16 && klaw[90].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[90].ustawiony_klaw >= 16 && klaw[90].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[90].ustawiony_klaw >= 16 && klaw[90].ustawiony_klaw <= 19)))
						keysPressed[90] = FALSE;

					klawa(90);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(90);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB) && keysPressed[90])
			{
				keysPressed[90] = FALSE;
				klawa_up(90);
			}

			if (Pad->sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && !keysPressed[91])
			{
				keysPressed[91] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[91].ustawiony_klaw >= 20 && klaw[91].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[91].ustawiony_klaw >= 20 && klaw[91].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[91].ustawiony_klaw >= 20 && klaw[91].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[91].ustawiony_klaw >= 20 && klaw[91].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[91].ustawiony_klaw >= 16 && klaw[91].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[91].ustawiony_klaw >= 16 && klaw[91].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[91].ustawiony_klaw >= 16 && klaw[91].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[91].ustawiony_klaw >= 16 && klaw[91].ustawiony_klaw <= 19)))
						keysPressed[91] = FALSE;

					klawa(91);
				}
				
				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(91);
					klik[4] = 0;
				}
			}
			else if (!(Pad->sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && keysPressed[91])
			{
				keysPressed[91] = FALSE;
				klawa_up(91);
			}

			if (-Pad->sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && !keysPressed[92])
			{
				keysPressed[92] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[92].ustawiony_klaw >= 20 && klaw[92].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[92].ustawiony_klaw >= 20 && klaw[92].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[92].ustawiony_klaw >= 20 && klaw[92].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[92].ustawiony_klaw >= 20 && klaw[92].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[92].ustawiony_klaw >= 16 && klaw[92].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[92].ustawiony_klaw >= 16 && klaw[92].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[92].ustawiony_klaw >= 16 && klaw[92].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[92].ustawiony_klaw >= 16 && klaw[92].ustawiony_klaw <= 19)))
						keysPressed[92] = FALSE;

					klawa(92);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(92);
					klik[4] = 0;
				}
			}
			else if (!(-Pad->sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && keysPressed[92])
			{
				keysPressed[92] = FALSE;
				klawa_up(92);
			}

			if (-Pad->sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && !keysPressed[93])
			{
				keysPressed[93] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[93].ustawiony_klaw >= 20 && klaw[93].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[93].ustawiony_klaw >= 20 && klaw[93].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[93].ustawiony_klaw >= 20 && klaw[93].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[93].ustawiony_klaw >= 20 && klaw[93].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[93].ustawiony_klaw >= 16 && klaw[93].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[93].ustawiony_klaw >= 16 && klaw[93].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[93].ustawiony_klaw >= 16 && klaw[93].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[93].ustawiony_klaw >= 16 && klaw[93].ustawiony_klaw <= 19)))
						keysPressed[93] = FALSE;

					klawa(93);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(93);
					klik[4] = 0;
				}
			}
			else if (!(-Pad->sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && keysPressed[93])
			{
				keysPressed[93] = FALSE;
				klawa_up(93);
			}

			if (Pad->sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && !keysPressed[94])
			{
				keysPressed[94] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[94].ustawiony_klaw >= 20 && klaw[94].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[94].ustawiony_klaw >= 20 && klaw[94].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[94].ustawiony_klaw >= 20 && klaw[94].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[94].ustawiony_klaw >= 20 && klaw[94].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[94].ustawiony_klaw >= 16 && klaw[94].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[94].ustawiony_klaw >= 16 && klaw[94].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[94].ustawiony_klaw >= 16 && klaw[94].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[94].ustawiony_klaw >= 16 && klaw[94].ustawiony_klaw <= 19)))
						keysPressed[94] = FALSE;

					klawa(94);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(94);
					klik[4] = 0;
				}
			}
			else if (!(Pad->sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) && keysPressed[94])
			{
				keysPressed[94] = FALSE;
				klawa_up(94);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) && !keysPressed[95])
			{
				keysPressed[95] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[95].ustawiony_klaw >= 20 && klaw[95].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[95].ustawiony_klaw >= 20 && klaw[95].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[95].ustawiony_klaw >= 20 && klaw[95].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[95].ustawiony_klaw >= 20 && klaw[95].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[95].ustawiony_klaw >= 16 && klaw[95].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[95].ustawiony_klaw >= 16 && klaw[95].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[95].ustawiony_klaw >= 16 && klaw[95].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[95].ustawiony_klaw >= 16 && klaw[95].ustawiony_klaw <= 19)))
						keysPressed[95] = FALSE;

					klawa(95);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(95);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) && keysPressed[95])
			{
				keysPressed[95] = FALSE;
				klawa_up(95);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP) && !keysPressed[96])
			{
				keysPressed[96] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[96].ustawiony_klaw >= 20 && klaw[96].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[96].ustawiony_klaw >= 20 && klaw[96].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[96].ustawiony_klaw >= 20 && klaw[96].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[96].ustawiony_klaw >= 20 && klaw[96].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[96].ustawiony_klaw >= 16 && klaw[96].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[96].ustawiony_klaw >= 16 && klaw[96].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[96].ustawiony_klaw >= 16 && klaw[96].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[96].ustawiony_klaw >= 16 && klaw[96].ustawiony_klaw <= 19)))
						keysPressed[96] = FALSE;

					klawa(96);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(96);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP) && keysPressed[96])
			{
				keysPressed[96] = FALSE;
				klawa_up(96);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN) && !keysPressed[97])
			{
				keysPressed[97] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[97].ustawiony_klaw >= 20 && klaw[97].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[97].ustawiony_klaw >= 20 && klaw[97].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[97].ustawiony_klaw >= 20 && klaw[97].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[97].ustawiony_klaw >= 20 && klaw[97].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[97].ustawiony_klaw >= 16 && klaw[97].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[97].ustawiony_klaw >= 16 && klaw[97].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[97].ustawiony_klaw >= 16 && klaw[97].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[97].ustawiony_klaw >= 16 && klaw[97].ustawiony_klaw <= 19)))
						keysPressed[97] = FALSE;

					klawa(97);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(97);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN) && keysPressed[97])
			{
				keysPressed[97] = FALSE;
				klawa_up(97);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT) && !keysPressed[98])
			{
				keysPressed[98] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[98].ustawiony_klaw >= 20 && klaw[98].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[98].ustawiony_klaw >= 20 && klaw[98].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[98].ustawiony_klaw >= 20 && klaw[98].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[98].ustawiony_klaw >= 20 && klaw[98].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[98].ustawiony_klaw >= 16 && klaw[98].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[98].ustawiony_klaw >= 16 && klaw[98].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[98].ustawiony_klaw >= 16 && klaw[98].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[98].ustawiony_klaw >= 16 && klaw[98].ustawiony_klaw <= 19)))
						keysPressed[98] = FALSE;

					klawa(98);
				}
				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(98);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT) && keysPressed[98])
			{
				keysPressed[98] = FALSE;
				klawa_up(98);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) && !keysPressed[99])
			{
				keysPressed[99] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[99].ustawiony_klaw >= 20 && klaw[99].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[99].ustawiony_klaw >= 20 && klaw[99].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[99].ustawiony_klaw >= 20 && klaw[99].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[99].ustawiony_klaw >= 20 && klaw[99].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[99].ustawiony_klaw >= 16 && klaw[99].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[99].ustawiony_klaw >= 16 && klaw[99].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[99].ustawiony_klaw >= 16 && klaw[99].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[99].ustawiony_klaw >= 16 && klaw[99].ustawiony_klaw <= 19)))
						keysPressed[99] = FALSE;

					klawa(99);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(99);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) && keysPressed[99])
			{
				keysPressed[99] = FALSE;
				klawa_up(99);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_A) && !keysPressed[100])
			{
				keysPressed[100] = TRUE;
				if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
				{
					for (INT i = 0; i < 2; i++)
					{
						if (frame[0] == i)
						{
							obr_sciany = 0;
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
							WczytUstDzw();
							WczytUstGraf();
							scen[0] = scen[2] = i + 1;
							fram();
						}
					}

					if (frame[0] == 2)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						muza.Shutdown();
						exit(0);
					}
				}

				else if ((scen[0] == 1) || (scen[0] == 6) || (scen[0] == 8) || (scen[0] == 10))
				{
					for (INT i = 0, ii = 9; i < 4, ii >= 3; i++, ii -= 2)
					{
						if (frame[1] == i)
						{
							obr_sciany = 0;
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							WczytLabiryntu = 1;
							WczytGre = 1;
							scen[0] = scen[2] = ii;
							fram();
						}
					}
				}

				else if ((scen[0] == 2) || (scen[0] == 15))
				{
					if (frame[2] == 0)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 17;
						ustawienia[2] = true;
						zapis[2] = true;
						fram();
					}

					if (frame[2] == 1)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 16;
						ustawienia[3] = true;
						zapis[3] = true;
						fram();
					}

					if (frame[2] == 2)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
						WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
						WczytUstDzw();
						WczytUstGraf();
						scen[0] = scen[2] = 21;
						ustawienia[0] = TRUE;
						zapis[0] = TRUE;
						ustawione = 1;
						fram();
					}

					if (frame[2] == 3)
					{
						for (INT i = 1, ii = 4; i >= 0, ii <= 11; i--, ii += 7)
						{
							if (po % 2 == i)
							{
								obr_sciany = 0;
								muza.dzwiek_potwierdzenie(glosnosc_efektow);

								if (i == 0)
									pozycja_startowa();

								scen[0] = scen[2] = ii;
								fram();
							}
						}
					}
				}

				else if ((scen[0] == 5) || (scen[0] == 13))
				{
					if (frame[3] == 0)
					{
						nowy[0] = 0;
						nowy[1] = 0; 

						for (INT i = 0; i < 5; i++)
							SingleThink[i] = 0;
							
						obr_sciany = 0;

						for (INT i = 0; i < IloElLab; i++)
							tab[i] = 1;

						for (INT i = 0; i <= IloPunkt; i++)
							poz_scian[i] = 0;

						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 12;
						fram();
					}

					if (frame[3] == 1)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						IloscPlikow[0][0] = 0;
						IloscPlikow[0][1] = 1;
						ListaLabiryntow();
						scen[0] = scen[2] = 19;
						fram();
					}

					if (frame[3] == 2)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 6;
						fram();
					}
				}

				else if (scen[0] == 7)
				{
					if (frame[4] == 0 && WczytLabiryntu % 2 == 1)
					{
						obr_sciany = 1;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						IloscPlikow[1][0] = 0;
						IloscPlikow[1][1] = 1;
						WczytLabiryntu++;
						ListaLabiryntow();
						fram();
					}

					if (frame[4] == 1 && WczytLabiryntu % 2 == 1)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);

						if (WczytGre == 1)
							scen[0] = scen[2] = 8;

						else if (WczytGre == 2)
						{
							scen[0] = scen[2] = 11;
							DystansCam = TPCam.charCamDist;
						}

						WczytLabiryntu = 1;
						fram();
					}

					if (WczytLabiryntu % 2 == 0)
					{
						if (frame[16] != IloscPlikow[1][0] && frame[16] != IloLiter)
						{
							wygrana = FALSE;
							WczytLab("ZapisanaGra\\", pliki2[1][frame[16]]);
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							po = 1;
							czekanie = state = 0;
							TempTime *= 1000;
							start = clock() + (-1 * TempTime);
							scen[0] = scen[2] = 11;
							DystansCam = TPCam.charCamDist;
							fram();
						}

						if (frame[16] == IloscPlikow[1][0])
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							WczytLabiryntu = 1;
							fram();
						}
					}
				}

				else if (scen[0] == 9)
				{
					if (frame[5] == 0)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						wygrana = FALSE;
						plansza = 1;
						WczytLab("GotowePlansze\\", "nowaplansza1");
						dl_zycia = 0.3f; 
						x_dl_zycia = -1.59f;
						pozycja_startowa();
						punkty = 0;
						strzaly = 0;
						ilo_bron = 0;
						weapons[0] = FALSE;
						weapons[1] = FALSE;
						weapons[2] = FALSE;
						czekanie = state = 0;
						start = clock();
						scen[0] = scen[2] = 11;
						DystansCam = TPCam.charCamDist;
						for (INT ii = 0; ii < MaxCountArrows; ii++)
							ShowArrowOnHedge[ii] = FALSE;

						for (int j = 0; j < IloElLab; j++)
						{
							for (INT ii = 0; ii < MaxCountArrows; ii++)
								ShowArrowOnMonster[j][ii] = FALSE;
						}
						fram();
					}

					if (frame[5] == 1)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 10;
						fram();
					}
				}

				else if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[100].ustawiony_klaw >= 20 && klaw[100].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[100].ustawiony_klaw >= 20 && klaw[100].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[100].ustawiony_klaw >= 20 && klaw[100].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[100].ustawiony_klaw >= 20 && klaw[100].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[100].ustawiony_klaw >= 16 && klaw[100].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[100].ustawiony_klaw >= 16 && klaw[100].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[100].ustawiony_klaw >= 16 && klaw[100].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[100].ustawiony_klaw >= 16 && klaw[100].ustawiony_klaw <= 19)))
						keysPressed[100] = FALSE;

					reset_czasu = 0;
					klawa(100);

					if (po % 2 == 0 && ZapiszGiere % 2 == 1)
					{
						SetCursor(LoadCursor(NULL, IDC_ARROW));
						keysPressed[100] = TRUE;
						if (frame[9] == 0)
						{
							po++;
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							fram();
						}

						else if (frame[9] == 1)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							bron = czekanie = state = punkty = ilo_bron = strzaly = 0;
							start = clock();
							po = 1;
							ZapiszGiere = 1;
							pozycja_startowa();
							fram();
						}

						else if (frame[9] == 2)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							obr_sciany = 1;
							WczytLabiryntu = 1;
							ZapiszGiere = 1;
							scen[0] = scen[2] = 7;
							fram();
						}


						else if (frame[9] == 3)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							ZapiszGiere++;
							fram();
						}

						else if (frame[9] == 4)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							obr_sciany = 0;
							WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
							WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
							WczytUstDzw();
							WczytUstGraf();
							scen[0] = scen[2] = 15;
							fram();
						}

						else if (frame[9] == 5)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							czekanie = state = scen[0] = scen[2] = obr_sciany = 0;
							obr.x1 = obr.y1 = 0.0f;
							start = clock();
							po = 1;
							ZapiszGiere = 1;
							fram();
						}
					}

					else if (po % 2 == 1 && ZapiszGiere % 2 == 1)
						SetCursor(LoadCursor(NULL, NULL));

					else if (po % 2 == 0 && ZapiszGiere % 2 == 0)
					{
						if (frame[15] != 1)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							ZapLab(NazwaZapLab);
							NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
							dl = 0;
							for (INT kla = 0; kla < KLA - 25; kla++)
								for (INT i = 0; i < 19; i++)
									klaw[kla].zapis[i] = 0;
							ZapiszGiere++;
							fram();
						}

						if (frame[15] == 1)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
							dl = 0;
							for (INT kla = 0; kla < KLA - 25; kla++)
								for (INT i = 0; i < 19; i++)
									klaw[kla].zapis[i] = 0;
							ZapiszGiere++;
							fram();
						}
					}
				}

				else if (scen[0] == 12)
				{
					if (frame[6] == 0)
					{
						nowy[0] = 1;
						nowy[1] = 0;

						for (INT i = 0; i < 7; i++)
							SingleThink[i] = 0;
							
						ZaznGuz = 0;

						for (INT i = 0; i < IloElLab; i++)
							tab[i] = 0;

						for (INT z = 0; z < IloPunkt; z++)
							poz_scian[z] = 0;

						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						fram();
						frame[6] = 10;
					}

					else if (frame[6] == 1)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						IloscPlikow[0][0] = ZaznGuz = 0;
						IloscPlikow[0][1] = 1;
						ListaLabiryntow();
						scen[0] = scen[2] = 19;
						nowy[0] = nowy[1] = 1;

						for (INT i = 0; i < IloElLab; i++)
						{
							if (tab[i] != 2)
								SingleThink[0] = 0;
							if (tab[i] != 3)
								SingleThink[1] = 0;
							if (tab[i] != 4)
								SingleThink[2] = 0;
							if (tab[i] != 5)
								SingleThink[3] = 0;
							if (tab[i] != 7)
								SingleThink[4] = 0;
							if (tab[i] != 9)
								SingleThink[5] = 0;
							if (tab[i] != 11)
								SingleThink[6] = 0;
						}
						fram();
						frame[6] = 10;
					}

					else if (frame[6] == 2)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 20;
						ZaznGuz = 0;
						nowy[0] = nowy[1] = 1;
						fram();
					}

					else if (frame[6] == 3)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 13;
						fram();
					}

					else if (nowy[0] == 1)
					{
						if (ZaznGuz < 20)
						{
							for (INT i = 0; i < 12; i++)
							{
								if (ZaznGuz == i)
								{
									nowy[1] = i + 1;
									muza.dzwiek_potwierdzenie(glosnosc_efektow);
									ZaznGuz = 20;
								}
							}
						}

						else if (ZaznGuz >= 20)
						{
							ZaznKeys();
							zazn();
						}
					}
				}

				else if (scen[0] == 14)
				{
					if (klik[0] == 0)
					{
						for (INT k = 0; k < 15; k++)
						{
							if (frame[8] == k)
							{
								muza.dzwiek_potwierdzenie(glosnosc_efektow);
								zapis[0] = FALSE;
								pierwszy[k] = ustaw_klaw[k] = 1;
								ustawione = 0;
								klik[0] = 1;
								for (INT kk = 0; kk < 15; kk++)
								{
									if (k == kk)
									{
										for (INT kla = 0; kla < KLA - 25; kla++)
										{
											if (klaw[kla].ustawiony_klaw == k + 1)
											{
												klaw[kla].klawisz = 0;
												klaw[kla].ustawiony_klaw = 0;
											}
										}
									}
								}
							}
							else if (frame[8] != k)
								ustaw_klaw[k] = 0;
						}

						if (frame[8] == 15)
						{
							obr_sciany = klik[0] = 0;
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							for (INT kla = 0; kla < KLA - 25; kla++)
							{
								klaw[kla].klawisz = 0;
								klaw[kla].ustawiony_klaw = 0;
							}

							klaw[32].ustawiony_klaw = 1;
							klaw[46].ustawiony_klaw = 2;
							klaw[45].ustawiony_klaw = 3;
							klaw[47].ustawiony_klaw = 4;
							klaw[73].ustawiony_klaw = 5;
							klaw[74].ustawiony_klaw = 6;
							klaw[75].ustawiony_klaw = 7;
							klaw[76].ustawiony_klaw = 8;
							klaw[57].ustawiony_klaw = 9;
							klaw[33].ustawiony_klaw = 10;
							klaw[31].ustawiony_klaw = 11;
							klaw[71].ustawiony_klaw = 12;
							klaw[30].ustawiony_klaw = 13;
							klaw[84].ustawiony_klaw = 14;
							klaw[85].ustawiony_klaw = 15;

							UstWszKl();

							if (wczyt[0])
								zapis[0] = FALSE;
							else
								zapis[0] = TRUE;

							fram();
						}

						if (frame[8] == 16)
						{
							if (ustaw)
							{
								muza.dzwiek_potwierdzenie(glosnosc_efektow);
								ZapUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0);
								fram();
							}
							else
								scen[0] = scen[2] = 23;
						}

						if (frame[8] == 17)
						{
							obr_sciany = 0;
							muza.dzwiek_potwierdzenie(glosnosc_efektow);

							if (zapis[0])
							{
								scen[0] = scen[2] = 21;
								ustawienia[0] = false;
							}

							if (!zapis[0])
								scen[0] = scen[2] = 18;

							fram();
						}
					}
				}

				else if (scen[0] == 16 && !ustaw_fullekran && !ustaw_rozdzielczosc)
				{
					if (frame[10] == 0)
					{
						ListaRozdz();
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ustaw_rozdzielczosc = true;
						ustaw_fullekran = false;
					}

					if (frame[10] == 1)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ustaw_fullekran = true;
						ustaw_rozdzielczosc = false;
					}

					if (frame[10] == 2 && klik[1] == 0)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						klik[1] = 1;
						ustaw_fullekran = false;
						ustaw_rozdzielczosc = false;
					}

					if (frame[10] == 3)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ustaw_fullekran = false;
						ustaw_rozdzielczosc = false;

						rozdzielczosc_w = 1280.0f;
						rozdzielczosc_h = 600.0f;

						if (roz_w != rozdzielczosc_w || roz_h != rozdzielczosc_h)
							Resizable();

						Tlx[0] = 1.1f;
						jas_sw = 1.3f;
						full = false;
						roz_w = rozdzielczosc_w;
						roz_h = rozdzielczosc_h;

						if (wczyt[3])
							zapis[3] = false;
						else
							zapis[3] = true;

						PozARozdz();

						fram();
					}

					if (frame[10] == 4)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ZapUstGraf();
						ustaw_fullekran = false;
						ustaw_rozdzielczosc = false;
						fram();
					}

					if (frame[10] == 5)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ustaw_fullekran = false;
						ustaw_rozdzielczosc = false;

						if (zapis[3])
						{
							scen[0] = scen[2] = 15;
							ustawienia[3] = false;
						}

						if (!zapis[3])
							scen[0] = scen[2] = 18;

						fram();
					}
				}

				else if (scen[0] == 16 && ustaw_fullekran)
				{
					if (frame[12] == 1)
					{
						if (!full)
						{
							zapis[3] = false;
							full = zmiana_full = true;
						}

						if (zmiana_full && full)
							SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);

						zmiana_full = false;
					}

					if (frame[12] == 2)
					{
						if (full)
						{
							zapis[3] = false;
							full = false;
							zmiana_full = true;
						}

						if (zmiana_full && !full)
							SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);

						zmiana_full = false;
					}
				}

				else if (scen[0] == 16 && ustaw_rozdzielczosc)
				{
					if (frame[13] == 0 && rozdz[lr[0]].ind == 1)
					{
						rozdzielczosc_w = (FLOAT)rozdz[lr[0]].width;
						rozdzielczosc_h = (FLOAT)rozdz[lr[0]].height;
						PozARozdz();

						if (roz_w != rozdzielczosc_w || roz_h != rozdzielczosc_h)
						{
							Resizable();

							roz_w = rozdzielczosc_w;
							roz_h = rozdzielczosc_h;

							zapis[3] = false;
						}
					}

					if (frame[13] == 1 && rozdz[lr[1]].ind == 1)
					{
						rozdzielczosc_w = (FLOAT)rozdz[lr[1]].width;
						rozdzielczosc_h = (FLOAT)rozdz[lr[1]].height;
						PozARozdz();

						if (roz_w != rozdzielczosc_w || roz_h != rozdzielczosc_h)
						{
							Resizable();

							roz_w = rozdzielczosc_w;
							roz_h = rozdzielczosc_h;

							zapis[3] = false;
						}
					}
				}

				else if (scen[0] == 17)
				{
					if (frame[11] == 0 && klik[2] == 0)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						klik[2] = 1;
						klik[3] = 0;
					}

					if (frame[11] == 1 && klik[3] == 0)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						klik[2] = 0;
						klik[3] = 1;
					}

					if (frame[11] == 2)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						Tlx[1] = Tlx[2] = 1.1f;
						glosnosc_tla = glosnosc_efektow = -5000;

						muza.muzyka_tlo_volume(glosnosc_tla);

						if (wczyt[2])
							zapis[2] = false;
						else
							zapis[2] = true;

						fram();
					}

					if (frame[11] == 3)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ZapUstDzw();
						fram();
					}

					if (frame[11] == 4)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);

						if (zapis[2])
						{
							scen[0] = scen[2] = 15;
							ustawienia[2] = false;
						}

						if (!zapis[2])
							scen[0] = scen[2] = 18;

						fram();
					}
				}

				else if (scen[0] == 18)
				{
					if (frame[14] == 0)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);

						if (ustawienia[0])
						{
							if (!zapis[0] && wczyt[0])
								WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);

							if (!wczyt[0])
							{
								for (INT kla = 0; kla < KLA - 25; kla++)
								{
									klaw[kla].klawisz = 0;
									klaw[kla].ustawiony_klaw = 0;
								}

								klaw[32].ustawiony_klaw = 1;
								klaw[46].ustawiony_klaw = 2;
								klaw[45].ustawiony_klaw = 3;
								klaw[47].ustawiony_klaw = 4;
								klaw[73].ustawiony_klaw = 5;
								klaw[74].ustawiony_klaw = 6;
								klaw[75].ustawiony_klaw = 7;
								klaw[76].ustawiony_klaw = 8;
								klaw[57].ustawiony_klaw = 9;
								klaw[33].ustawiony_klaw = 10;
								klaw[31].ustawiony_klaw = 11;
								klaw[71].ustawiony_klaw = 12;
								klaw[30].ustawiony_klaw = 13;
								klaw[84].ustawiony_klaw = 14;
								klaw[85].ustawiony_klaw = 15;
							}

							UstWszKl();

							scen[0] = scen[2] = 21;
						}

						if (ustawienia[1])
						{
							if (!zapis[1] && wczyt[1])
								WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);

							if (!wczyt[1])
							{
								for (INT kla = 86; kla < KLA; kla++)
								{
									klaw[kla].klawisz = 0;
									klaw[kla].ustawiony_klaw = 0;
								}

								klaw[86].ustawiony_klaw = 16;
								klaw[87].ustawiony_klaw = 17;
								klaw[88].ustawiony_klaw = 18;
								klaw[89].ustawiony_klaw = 19;
								klaw[91].ustawiony_klaw = 20;
								klaw[92].ustawiony_klaw = 21;
								klaw[93].ustawiony_klaw = 22;
								klaw[94].ustawiony_klaw = 23;
								klaw[108].ustawiony_klaw = 24;
								klaw[100].ustawiony_klaw = 25;
								klaw[101].ustawiony_klaw = 26;
								klaw[109].ustawiony_klaw = 27;
								klaw[107].ustawiony_klaw = 28;
								klaw[96].ustawiony_klaw = 29;
								klaw[97].ustawiony_klaw = 30;
							}

							UstWszKl();

							scen[0] = scen[2] = 21;
						}

						if (ustawienia[2])
						{
							if (!zapis[2] && wczyt[2])
							{
								WczytUstDzw();
								muza.muzyka_tlo_volume(glosnosc_tla);
							}

							if (!wczyt[2])
							{
								Tlx[1] = Tlx[2] = 1.1f;
								glosnosc_tla = glosnosc_efektow = -5000;
								muza.muzyka_tlo_volume(glosnosc_tla);
							}

							scen[0] = scen[2] = 15;
						}

						if (ustawienia[3])
						{
							if (!zapis[3] && wczyt[3])
							{
								WczytUstGraf();
								SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);
								Resizable();
							}

							if (!zapis[3] && !wczyt[3])
							{
								rozdzielczosc_w = 1280.0f;
								rozdzielczosc_h = 600.0f;
								full = false;
								Tlx[0] = 1.1f;
								jas_sw = 1.3f;
								SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);
								Resizable();
							}

							scen[0] = scen[2] = 15;
						}

						roz_w = rozdzielczosc_w;
						roz_h = rozdzielczosc_h;
						zmiana_full = false;
						fram();
					}

					if (frame[14] == 1)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);

						if (ustawienia[2])
							scen[0] = scen[2] = 17;

						if (ustawienia[3])
							scen[0] = scen[2] = 16;

						if (ustawienia[0])
							scen[0] = scen[2] = 14;

						if (ustawienia[1])
							scen[0] = scen[2] = 22;

						fram();
					}
				}

				else if (scen[0] == 19)
				{
					if (frame[16] != IloscPlikow[0][0] && frame[16] != IloLiter)
					{
						wygrana = FALSE;
						plansza = 5;
						WczytLab("ZapisaneLabirynty\\", pliki2[0][frame[16]]);
						dl_zycia = 0.3f;
						x_dl_zycia = -1.59f;
						strzaly = 0;
						punkty = 0;

						for (INT i = 0, ii = 12; i < 2, ii >= 11; i++, ii--)
						{
							if (wczytyw == i)
							{
								muza.dzwiek_potwierdzenie(glosnosc_efektow);

								if (i == 1)
									pozycja_startowa();

								nowy[0] = nowy[1] = 1;
								for (INT ik = 0; ik < IloElLab; ik++)
								{
									if (tab[i] != 2)
										SingleThink[0] = 0;
									if (tab[i] != 3)
										SingleThink[1] = 0;
									if (tab[i] != 4)
										SingleThink[2] = 0;
									if (tab[i] != 5)
										SingleThink[3] = 0;
									if (tab[i] != 7)
										SingleThink[4] = 0;
									if (tab[i] != 9)
										SingleThink[5] = 0;
									if (tab[i] != 11)
										SingleThink[6] = 0;
								}
								scen[0] = scen[2] = ii;
								fram();
							}
						}
					}

					if (frame[16] == IloscPlikow[0][0])
					{
						for (INT i = 0; i < 2; i++)
						{
							if (wczytyw == i)
							{
								muza.dzwiek_potwierdzenie(glosnosc_efektow);
								scen[0] = scen[2] = i + 12;
								fram();
							}
						}
					}
				}

				else if (scen[0] == 20 && ZapiszGiere % 2 == 1)
				{
					if (frame[15] != 1)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ZapLab(NazwaZapLab);
						NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
						dl = 0;
						for (INT kla = 0; kla < KLA - 25; kla++)
							for (INT i = 0; i < 19; i++)
								klaw[kla].zapis[i] = 0;
						scen[0] = scen[2] = 12;
						fram();
					}

					if (frame[15] == 1)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
						dl = 0;
						for (INT kla = 0; kla < KLA - 25; kla++)
							for (INT i = 0; i < 19; i++)
								klaw[kla].zapis[i] = 0;
						scen[0] = scen[2] = 12;
						fram();
					}
				}

				else if (scen[0] == 21)
				{
					if (frame[17] == 0)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
						if (!wczyt[0])
						{
							for (INT kla = 0; kla < KLA - 25; kla++)
							{
								klaw[kla].klawisz = 0;
								klaw[kla].ustawiony_klaw = 0;
							}

							klaw[32].ustawiony_klaw = 1;
							klaw[46].ustawiony_klaw = 2;
							klaw[45].ustawiony_klaw = 3;
							klaw[47].ustawiony_klaw = 4;
							klaw[73].ustawiony_klaw = 5;
							klaw[74].ustawiony_klaw = 6;
							klaw[75].ustawiony_klaw = 7;
							klaw[76].ustawiony_klaw = 8;
							klaw[57].ustawiony_klaw = 9;
							klaw[33].ustawiony_klaw = 10;
							klaw[31].ustawiony_klaw = 11;
							klaw[71].ustawiony_klaw = 12;
							klaw[30].ustawiony_klaw = 13;
							klaw[84].ustawiony_klaw = 14;
							klaw[85].ustawiony_klaw = 15;
						}
						scen[0] = scen[2] = 14;
						fram();
					}

					if (frame[17] == 1)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
						if (!wczyt[1])
						{
							for (INT kla = 86; kla < KLA; kla++)
							{
								klaw[kla].klawisz = 0;
								klaw[kla].ustawiony_klaw = 0;
							}

							klaw[86].ustawiony_klaw = 16;
							klaw[87].ustawiony_klaw = 17;
							klaw[88].ustawiony_klaw = 18;
							klaw[89].ustawiony_klaw = 19;
							klaw[91].ustawiony_klaw = 20;
							klaw[92].ustawiony_klaw = 21;
							klaw[93].ustawiony_klaw = 22;
							klaw[94].ustawiony_klaw = 23;
							klaw[108].ustawiony_klaw = 24;
							klaw[100].ustawiony_klaw = 25;
							klaw[101].ustawiony_klaw = 26;
							klaw[109].ustawiony_klaw = 27;
							klaw[107].ustawiony_klaw = 28;
							klaw[96].ustawiony_klaw = 29;
							klaw[97].ustawiony_klaw = 30;
						}
						scen[0] = scen[2] = 22;
						fram();
					}

					if (frame[17] == 2 || frame[17] == 3)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 15;
						fram();
					}
				}

				else if (scen[0] == 22 && klik[4] == 0)
				{
					for (INT k = 0; k < 15; k++)
					{
						if (frame[18] == k)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							zapis[1] = FALSE;
							pierwszy[k + 15] = ustaw_klaw[k + 15] = 1;
							ustawione = 0;
							klik[4] = 1;
							for (INT kk = 0; kk < 15; kk++)
							{
								if (k == kk)
								{
									for (INT kla = 86; kla < KLA; kla++)
									{
										if (klaw[kla].ustawiony_klaw == k + 16)
										{
											klaw[kla].klawisz = 0;
											klaw[kla].ustawiony_klaw = 0;
										}
									}
								}
							}
						}

						else if (frame[18] != k)
							ustaw_klaw[k + 15] = 0;
					}

					if (frame[18] == 15)
					{
						obr_sciany = klik[4] = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						for (INT kla = 86; kla < KLA; kla++)
						{
							klaw[kla].klawisz = 0;
							klaw[kla].ustawiony_klaw = 0;
						}

						klaw[86].ustawiony_klaw = 16;
						klaw[87].ustawiony_klaw = 17;
						klaw[88].ustawiony_klaw = 18;
						klaw[89].ustawiony_klaw = 19;
						klaw[91].ustawiony_klaw = 20;
						klaw[92].ustawiony_klaw = 21;
						klaw[93].ustawiony_klaw = 22;
						klaw[94].ustawiony_klaw = 23;
						klaw[108].ustawiony_klaw = 24;
						klaw[100].ustawiony_klaw = 25;
						klaw[101].ustawiony_klaw = 26;
						klaw[109].ustawiony_klaw = 27;
						klaw[107].ustawiony_klaw = 28;
						klaw[96].ustawiony_klaw = 29;
						klaw[97].ustawiony_klaw = 30;

						UstWszKl();

						if (wczyt[1])
							zapis[1] = FALSE;
						else
							zapis[1] = TRUE;

						fram();
					}

					if (frame[18] == 16)
					{
						if (ustaw)
						{
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							ZapUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 1);
							fram();
						}
						else
							scen[0] = scen[2] = 23;
					}

					if (frame[18] == 17)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);

						if (zapis[1])
						{
							scen[0] = scen[2] = 21;
							ustawienia[1] = false;
						}

						if (!zapis[1])
							scen[0] = scen[2] = 18;

						fram();
					}
				}

				else if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(100);
					klik[4] = 0;
				}

				else if (scen[0] == 23)
				{
					if (scen[1] == 1)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 14;
					}

					else if (scen[1] == 4)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = 22;
					}
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_A) && keysPressed[100])
			{
				keysPressed[100] = FALSE;
				klawa_up(100);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_B) && !keysPressed[101])
			{
				keysPressed[101] = TRUE;
				if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					muza.Shutdown();
					exit(0);
				}

				else if ((scen[0] == 1) || (scen[0] == 6) || (scen[0] == 8) || (scen[0] == 10))
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					scen[0] = scen[2] = 3;
					fram();
				}

				else if ((scen[0] == 2) || (scen[0] == 15))
				{
					for (INT i = 1, ii = 4; i >= 0, ii <= 11; i--, ii += 7)
					{
						if (po % 2 == i)
						{
							obr_sciany = reset_czasu = 0;
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							scen[0] = scen[2] = ii;
							fram();
						}
					}
				}

				else if ((scen[0] == 5) || (scen[0] == 13))
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					scen[0] = scen[2] = 6;
					fram();
				}

				else if (scen[0] == 7)
				{
					if (WczytLabiryntu % 2 == 1)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);

						if (WczytGre == 1)
							scen[0] = scen[2] = 8;

						else if (WczytGre == 2)
						{
							scen[0] = scen[2] = 11;
							DystansCam = TPCam.charCamDist;
						}

						fram();
					}

					else if (WczytLabiryntu % 2 == 0)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						WczytLabiryntu = 1;
						fram();
					}
				}

				else if (scen[0] == 9)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					scen[0] = scen[2] = 10;
					fram();
				}

				else if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[101].ustawiony_klaw >= 20 && klaw[101].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[101].ustawiony_klaw >= 20 && klaw[101].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[101].ustawiony_klaw >= 20 && klaw[101].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[101].ustawiony_klaw >= 20 && klaw[101].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[101].ustawiony_klaw >= 16 && klaw[101].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[101].ustawiony_klaw >= 16 && klaw[101].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[101].ustawiony_klaw >= 16 && klaw[101].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[101].ustawiony_klaw >= 16 && klaw[101].ustawiony_klaw <= 19)))
						keysPressed[101] = FALSE;

					klawa(101);

					if (ZapiszGiere % 2 == 0)
						ZapiszGiere++;

					fram();
				}

				else if ((scen[0] == 12 && ZaznGuz >= 0 && ZaznGuz <= 12))
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					frame[6] = 0;
					ZaznGuz = 15;
					fram();
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				else if ((scen[0] == 12 && ZaznGuz == 15))
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					scen[0] = scen[2] = 13;
					fram();
				}

				else if (scen[0] == 12 && ZaznGuz >= 20)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					ZaznGuz = 0;
					fram();
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
					frame[6] = 10;
				}

				else if (scen[0] == 14 && klik[0] == 0)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (zapis[0])
					{
						scen[0] = scen[2] = 21;
						ustawienia[0] = false;
					}

					if (!zapis[0])
						scen[0] = scen[2] = 18;

					fram();
				}

				else if (scen[0] == 16 && !ustaw_fullekran && !ustaw_rozdzielczosc)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (zapis[3])
					{
						scen[0] = scen[2] = 15;
						ustawienia[3] = false;
					}

					if (!zapis[3])
						scen[0] = scen[2] = 18;

					fram();
				}

				else if (scen[0] == 16 && ustaw_fullekran)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					ustaw_fullekran = FALSE;
				}

				else if (scen[0] == 16 && ustaw_rozdzielczosc)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					ustaw_rozdzielczosc = FALSE;
				}

				else if (scen[0] == 17)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (zapis[2])
					{
						scen[0] = scen[2] = 15;
						ustawienia[2] = false;
					}

					if (!zapis[2])
						scen[0] = scen[2] = 18;

					fram();
				}

				else if (scen[0] == 18)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (ustawienia[2] && !ustawienia[3] && !ustawienia[0] && !ustawienia[1])
						scen[0] = scen[2] = 17;

					else if (ustawienia[3] && !ustawienia[2] && !ustawienia[0] && !ustawienia[1])
						scen[0] = scen[2] = 16;

					else if (ustawienia[0] && !ustawienia[2] && !ustawienia[3] && !ustawienia[1])
						scen[0] = scen[2] = 14;

					else if (!ustawienia[0] && !ustawienia[2] && !ustawienia[3] && ustawienia[1])
						scen[0] = scen[2] = 22;

					fram();
				}

				else if (scen[0] == 19)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					for (INT i = 0; i < 2; i++)
					{
						if (wczytyw == i)
							scen[0] = scen[2] = i + 12;
					}

					fram();
				}

				else if (scen[0] == 20)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					dl = 0;
					for (INT kla = 0; kla < KLA - 25; kla++)
						for (INT i = 0; i < 19; i++)
							klaw[kla].zapis[i] = 0;

					scen[0] = scen[2] = 12;
					fram();
				}

				else if (scen[0] == 21)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					scen[0] = scen[2] = 15;
					fram();
				}

				else if (scen[0] == 22 && klik[4] == 0)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (zapis[1])
					{
						scen[0] = scen[2] = 21;
						ustawienia[1] = FALSE;
					}

					if (!zapis[1])
						scen[0] = scen[2] = 18;

					fram();
				}

				else if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(101);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_B) && keysPressed[101])
			{
				keysPressed[101] = FALSE;
				klawa_up(101);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_X) && !keysPressed[102])
			{
				keysPressed[102] = TRUE;

				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[102].ustawiony_klaw >= 20 && klaw[102].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[102].ustawiony_klaw >= 20 && klaw[102].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[102].ustawiony_klaw >= 20 && klaw[102].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[102].ustawiony_klaw >= 20 && klaw[102].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[102].ustawiony_klaw >= 16 && klaw[102].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[102].ustawiony_klaw >= 16 && klaw[102].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[102].ustawiony_klaw >= 16 && klaw[102].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[102].ustawiony_klaw >= 16 && klaw[102].ustawiony_klaw <= 19)))
						keysPressed[102] = FALSE;

					klawa(102);
				}

				if (scen[0] == 12)
				{
					if (nowy[1] > 0)
						odzn();
				}

				else if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(102);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_X) && keysPressed[102])
			{
				keysPressed[102] = FALSE;
				klawa_up(102);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_Y) && !keysPressed[103])
			{
				keysPressed[103] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[103].ustawiony_klaw >= 20 && klaw[103].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[103].ustawiony_klaw >= 20 && klaw[103].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[103].ustawiony_klaw >= 20 && klaw[103].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[103].ustawiony_klaw >= 20 && klaw[103].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[103].ustawiony_klaw >= 16 && klaw[103].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[103].ustawiony_klaw >= 16 && klaw[103].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[103].ustawiony_klaw >= 16 && klaw[103].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[103].ustawiony_klaw >= 16 && klaw[103].ustawiony_klaw <= 19)))
						keysPressed[103] = FALSE;

					klawa(103);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(103);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_Y) && keysPressed[103])
			{
				keysPressed[103] = FALSE;
				klawa_up(103);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_START) && !keysPressed[104])
			{
				keysPressed[104] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[104].ustawiony_klaw >= 20 && klaw[104].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[104].ustawiony_klaw >= 20 && klaw[104].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[104].ustawiony_klaw >= 20 && klaw[104].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[104].ustawiony_klaw >= 20 && klaw[104].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[104].ustawiony_klaw >= 16 && klaw[104].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[104].ustawiony_klaw >= 16 && klaw[104].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[104].ustawiony_klaw >= 16 && klaw[104].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[104].ustawiony_klaw >= 16 && klaw[104].ustawiony_klaw <= 19)))
						keysPressed[104] = FALSE;

					klawa(104);
					po++;
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(104);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_START) && keysPressed[104])
			{
				keysPressed[104] = FALSE;
				klawa_up(104);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_BACK) && !keysPressed[105])
			{
				keysPressed[105] = TRUE;

				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[105].ustawiony_klaw >= 20 && klaw[105].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[105].ustawiony_klaw >= 20 && klaw[105].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[105].ustawiony_klaw >= 20 && klaw[105].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[105].ustawiony_klaw >= 20 && klaw[105].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[105].ustawiony_klaw >= 16 && klaw[105].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[105].ustawiony_klaw >= 16 && klaw[105].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[105].ustawiony_klaw >= 16 && klaw[105].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[105].ustawiony_klaw >= 16 && klaw[105].ustawiony_klaw <= 19)))
						keysPressed[105] = FALSE;

					klawa(105);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(105);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_BACK) && keysPressed[105])
			{
				keysPressed[105] = FALSE;
				klawa_up(105);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) && !keysPressed[106])
			{
				keysPressed[106] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[106].ustawiony_klaw >= 20 && klaw[106].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[106].ustawiony_klaw >= 20 && klaw[106].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[106].ustawiony_klaw >= 20 && klaw[106].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[106].ustawiony_klaw >= 20 && klaw[106].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[106].ustawiony_klaw >= 16 && klaw[106].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[106].ustawiony_klaw >= 16 && klaw[106].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[106].ustawiony_klaw >= 16 && klaw[106].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[106].ustawiony_klaw >= 16 && klaw[106].ustawiony_klaw <= 19)))
						keysPressed[106] = FALSE;

					klawa(106);
				}
				
				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(106);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) && keysPressed[106])
			{
				keysPressed[106] = FALSE;
				klawa_up(106);
			}

			if ((Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && !keysPressed[107])
			{
				keysPressed[107] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[107].ustawiony_klaw >= 20 && klaw[107].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[107].ustawiony_klaw >= 20 && klaw[107].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[107].ustawiony_klaw >= 20 && klaw[107].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[107].ustawiony_klaw >= 20 && klaw[107].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[107].ustawiony_klaw >= 16 && klaw[107].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[107].ustawiony_klaw >= 16 && klaw[107].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[107].ustawiony_klaw >= 16 && klaw[107].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[107].ustawiony_klaw >= 16 && klaw[107].ustawiony_klaw <= 19)))
						keysPressed[107] = FALSE;

					klawa(107);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(107);
					klik[4] = 0;
				}
			}
			else if (!(Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && keysPressed[107])
			{
				keysPressed[107] = FALSE;
				klawa_up(107);
			}

			if (Pad->bLeftTrigger && !keysPressed[108])
			{
				keysPressed[108] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[108].ustawiony_klaw >= 20 && klaw[108].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[108].ustawiony_klaw >= 20 && klaw[108].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[108].ustawiony_klaw >= 20 && klaw[108].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[108].ustawiony_klaw >= 20 && klaw[108].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[108].ustawiony_klaw >= 16 && klaw[108].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[108].ustawiony_klaw >= 16 && klaw[108].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[108].ustawiony_klaw >= 16 && klaw[108].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[108].ustawiony_klaw >= 16 && klaw[108].ustawiony_klaw <= 19)))
						keysPressed[108] = FALSE;

					klawa(108);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(108);
					klik[4] = 0;
				}
			}
			else if (!Pad->bLeftTrigger && keysPressed[108])
			{
				keysPressed[108] = FALSE;
				klawa_up(108);
			}

			if (Pad->bRightTrigger && !keysPressed[109])
			{
				keysPressed[109] = TRUE;
				if (scen[0] == 11)
				{
					if (((klaw[73].ustawiony_klaw >= 5 && klaw[73].ustawiony_klaw <= 8) && (klaw[109].ustawiony_klaw >= 20 && klaw[109].ustawiony_klaw <= 23)) ||
						((klaw[74].ustawiony_klaw >= 5 && klaw[74].ustawiony_klaw <= 8) && (klaw[109].ustawiony_klaw >= 20 && klaw[109].ustawiony_klaw <= 23)) ||
						((klaw[75].ustawiony_klaw >= 5 && klaw[75].ustawiony_klaw <= 8) && (klaw[109].ustawiony_klaw >= 20 && klaw[109].ustawiony_klaw <= 23)) ||
						((klaw[76].ustawiony_klaw >= 5 && klaw[76].ustawiony_klaw <= 8) && (klaw[109].ustawiony_klaw >= 20 && klaw[109].ustawiony_klaw <= 23)) ||
						((klaw[73].ustawiony_klaw >= 1 && klaw[73].ustawiony_klaw <= 4) && (klaw[109].ustawiony_klaw >= 16 && klaw[109].ustawiony_klaw <= 19)) ||
						((klaw[74].ustawiony_klaw >= 1 && klaw[74].ustawiony_klaw <= 4) && (klaw[109].ustawiony_klaw >= 16 && klaw[109].ustawiony_klaw <= 19)) ||
						((klaw[75].ustawiony_klaw >= 1 && klaw[75].ustawiony_klaw <= 4) && (klaw[109].ustawiony_klaw >= 16 && klaw[109].ustawiony_klaw <= 19)) ||
						((klaw[76].ustawiony_klaw >= 1 && klaw[76].ustawiony_klaw <= 4) && (klaw[109].ustawiony_klaw >= 16 && klaw[109].ustawiony_klaw <= 19)))
						keysPressed[109] = FALSE;

					klawa(109);
				}

				if (scen[0] == 22 && klik[4] == 1)
				{
					ustawianie_klawiszy(109);
					klik[4] = 0;
				}
			}
			else if (!Pad->bRightTrigger && keysPressed[109])
			{
				keysPressed[109] = FALSE;
				klawa_up(109);
			}
		}
	}
}

void Renderer::DetectInputKeyboard()
{
	DIKeyboard->GetDeviceState(sizeof(UCHAR[256]), (LPVOID)keys);

	if (KEYDOWN(keys, klaw[1].keysik) && !keysPressed[1])
	{
		keysPressed[1] = TRUE;
		if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
		{
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			muza.Shutdown();
			exit(0);
		}

		else if ((scen[0] == 1) || (scen[0] == 6) || (scen[0] == 8) || (scen[0] == 10))
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			scen[0] = scen[2] = 3;
			fram();
		}

		else if ((scen[0] == 2) || (scen[0] == 15))
		{
			for (INT i = 1, ii = 4; i >= 0, ii <= 11; i--, ii += 7)
			{
				if (po % 2 == i)
				{
					obr_sciany = reset_czasu = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					scen[0] = scen[2] = ii;
					fram();
				}
			}
		}

		else if ((scen[0] == 5) || (scen[0] == 13))
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			scen[0] = scen[2] = 6;
			fram();
		}

		else if (scen[0] == 7)
		{
			if (WczytLabiryntu % 2 == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				if (WczytGre == 1)
					scen[0] = scen[2] = 8;

				else if (WczytGre == 2)
				{
					scen[0] = scen[2] = 11;
					DystansCam = TPCam.charCamDist;
				}
					
				fram();
			}

			else if (WczytLabiryntu % 2 == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytLabiryntu = 1;
				fram();
			}
		}

		else if (scen[0] == 9)
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			scen[0] = scen[2] = 10;
			fram();
		}

		else if (scen[0] == 11)
		{
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			if (ZapiszGiere % 2 == 1)
				po++;

			else if (ZapiszGiere % 2 == 0)
				ZapiszGiere++;

			fram();
		}

		else if ((scen[0] == 12 && ZaznGuz >= 0 && ZaznGuz <= 12))
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			frame[6] = 0;
			ZaznGuz = 15;
			fram();
			CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
		}

		else if ((scen[0] == 12 && ZaznGuz == 15))
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			scen[0] = scen[2] = 13;
			fram();
		}

		else if (scen[0] == 12 && ZaznGuz >= 20)
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			ZaznGuz = 0;
			fram();
			CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
			frame[6] = 10;
		}

		else if (scen[0] == 14)
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);

			if (zapis[0])
			{
				scen[0] = scen[2] = 21;
				ustawienia[0] = FALSE;
			}

			if (!zapis[0])
				scen[0] = scen[2] = 18;

			fram();
		}

		else if (scen[0] == 16 && !ustaw_fullekran && !ustaw_rozdzielczosc)
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);

			if (zapis[3])
			{
				scen[0] = scen[2] = 15;
				ustawienia[3] = FALSE;
			}

			if (!zapis[3])
				scen[0] = scen[2] = 18;

			fram();
		}

		else if (scen[0] == 16 && ustaw_fullekran)
		{
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			ustaw_fullekran = FALSE;
		}

		else if (scen[0] == 16 && ustaw_rozdzielczosc)
		{
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			ustaw_rozdzielczosc = FALSE;
		}

		else if (scen[0] == 17)
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);

			if (zapis[2])
			{
				scen[0] = scen[2] = 15;
				ustawienia[2] = FALSE;
			}

			if (!zapis[2])
				scen[0] = scen[2] = 18;

			fram();
		}

		else if (scen[0] == 18)
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);

			if (ustawienia[2] && !ustawienia[3] && !ustawienia[0] && !ustawienia[1])
				scen[0] = scen[2] = 17;

			else if (ustawienia[3] && !ustawienia[2] && !ustawienia[0] && !ustawienia[1])
				scen[0] = scen[2] = 16;

			else if (ustawienia[0] && !ustawienia[2] && !ustawienia[3] && !ustawienia[1])
				scen[0] = scen[2] = 14;

			else if (!ustawienia[0] && !ustawienia[2] && !ustawienia[3] && ustawienia[1])
				scen[0] = scen[2] = 22;

			fram();
		}

		else if (scen[0] == 19)
		{
			muza.dzwiek_potwierdzenie(glosnosc_efektow);

			for (INT i = 0; i < 2; i++)
			{
				if (wczytyw == i)
					scen[0] = scen[2] = i + 12;
			}

			fram();
		}

		else if (scen[0] == 20)
		{
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			dl = 0;
			for (INT kla = 0; kla < KLA - 25; kla++)
				for (INT i = 0; i < 19; i++)
					klaw[kla].zapis[i] = 0;

			scen[0] = scen[2] = 12;
			fram();
		}

		else if (scen[0] == 21)
		{
			muza.dzwiek_potwierdzenie(glosnosc_efektow);
			scen[0] = scen[2] = 15;
			fram();
		}

		else if (scen[0] == 22)
		{
			obr_sciany = 0;
			muza.dzwiek_potwierdzenie(glosnosc_efektow);

			if (zapis[1])
			{
				scen[0] = scen[2] = 21;
				ustawienia[1] = FALSE;
			}

			if (!zapis[1])
				scen[0] = scen[2] = 18;

			fram();
		}
	}

	else if (!KEYDOWN(keys, klaw[1].keysik) && keysPressed[1])
		keysPressed[1] = FALSE;

	for (INT keyi = 2; keyi < 56; keyi++)
	{
		if (KEYDOWN(keys, klaw[keyi].keysik) && !keysPressed[keyi])
		{
			keysPressed[keyi] = TRUE;
			klawa(keyi);

			if (scen[0] == 12 && keyi == 29)
			{
				if (nowy[1] > 0)
					odzn();
			}
			
			ustawianie_klawiszy(keyi);
			klik[0] = 0;
		}

		else if (!KEYDOWN(keys, klaw[keyi].keysik) && keysPressed[keyi])
		{
			keysPressed[keyi] = FALSE;
			klawa_up(keyi);
		}
	}

	if (KEYDOWN(keys, klaw[56].keysik) && !keysPressed[56])
	{
		keysPressed[56] = TRUE;
		if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
		{
			for (INT i = 0; i < 2; i++)
			{
				if (frame[0] == i)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
					WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
					WczytUstDzw();
					WczytUstGraf();
					scen[0] = scen[2] = i + 1;
					fram();
				}
			}

			if (frame[0] == 2)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				muza.Shutdown();
				exit(0);
			}
		}

		else if ((scen[0] == 1) || (scen[0] == 6) || (scen[0] == 8) || (scen[0] == 10))
		{
			for (INT i = 0, ii = 9; i < 4, ii >= 3; i++, ii -= 2)
			{
				if (frame[1] == i)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					WczytLabiryntu = 1;
					WczytGre = 1;
					scen[0] = scen[2] = ii;
					fram();
				}
			}
		}

		else if ((scen[0] == 2) || (scen[0] == 15))
		{
			if (frame[2] == 0)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 17;
				ustawienia[2] = true;
				zapis[2] = true;
				fram();
			}

			if (frame[2] == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 16;
				ustawienia[3] = TRUE;
				zapis[3] = TRUE;
				fram();
			}

			if (frame[2] == 2)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
				WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
				WczytUstDzw();
				WczytUstGraf();
				scen[0] = scen[2] = 21;
				ustawienia[0] = TRUE;
				zapis[0] = TRUE;
				ustawione = 1;
				fram();
			}

			if (frame[2] == 3)
			{
				for (INT i = 1, ii = 4; i >= 0, ii <= 11; i--, ii += 7)
				{
					if (po % 2 == i)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);

						if (i == 0)
							pozycja_startowa();

						scen[0] = scen[2] = ii;
						fram();
					}
				}
			}
		}

		else if ((scen[0] == 5) || (scen[0] == 13))
		{
			if (frame[3] == 0)
			{
				nowy[0] = 0;
				nowy[1] = 0;
					
				for (INT i = 0; i < 5; i++)
					SingleThink[i] = 0;
				
				obr_sciany = 0;
				for (INT i = 0; i < IloElLab; i++)
					tab[i] = 1;

				for (INT i = 0; i <= IloPunkt; i++)
					poz_scian[i] = 0;

				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 12;
				fram();
			}

			if (frame[3] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				IloscPlikow[0][0] = 0;
				IloscPlikow[0][1] = 1;
				ListaLabiryntow();
				scen[0] = scen[2] = 19;
				fram();
			}

			if (frame[3] == 2)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 6;
				fram();
			}
		}

		else if (scen[0] == 7)
		{
			if (frame[4] == 0 && WczytLabiryntu % 2 == 1)
			{
				obr_sciany = 1;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				IloscPlikow[1][0] = 0;
				IloscPlikow[1][1] = 1;
				WczytLabiryntu++;
				ListaLabiryntow();
				fram();
			}

			if (frame[4] == 1 && WczytLabiryntu % 2 == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				if (WczytGre == 1)
					scen[0] = scen[2] = 8;

				else if (WczytGre == 2)
				{
					scen[0] = scen[2] = 11;
					DystansCam = TPCam.charCamDist;
				}
					
				WczytLabiryntu = 1;
				fram();
			}

			if (WczytLabiryntu % 2 == 0)
			{
				if (frame[16] != IloscPlikow[1][0] && frame[16] != IloLiter)
				{
					wygrana = FALSE;
					WczytLab("ZapisanaGra\\", pliki2[1][frame[16]]);
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					po = 1;
					czekanie = state = 0;
					TempTime *= 1000;
					start = clock() + (-1 * TempTime);
					scen[0] = scen[2] = 11;
					DystansCam = TPCam.charCamDist;
					fram();
				}

				if (frame[16] == IloscPlikow[1][0])
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					WczytLabiryntu = 1;
					fram();
				}
			}
		}

		else if (scen[0] == 9)
		{
			if (frame[5] == 0)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				wygrana = FALSE;
				plansza = 1;
				WczytLab("GotowePlansze\\", "nowaplansza1");
				dl_zycia = 0.3f;
				x_dl_zycia = -1.59f;
				pozycja_startowa();
				punkty = 0;
				strzaly = 0;
				ilo_bron = 0;
				weapons[0] = FALSE;
				weapons[1] = FALSE;
				weapons[2] = FALSE;
				czekanie = state = 0;
				start = clock();
				scen[0] = scen[2] = 11;
				DystansCam = TPCam.charCamDist;
				for (INT ii = 0; ii < MaxCountArrows; ii++)
					ShowArrowOnHedge[ii] = FALSE;

				for (int j = 0; j < IloElLab; j++)
				{
					for (INT ii = 0; ii < MaxCountArrows; ii++)
						ShowArrowOnMonster[j][ii] = FALSE;
				}
				fram();
			}

			if (frame[5] == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 10;
				fram();
			}
		}

		else if (scen[0] == 11)
		{
			keysPressed[56] = FALSE;
			reset_czasu = 0;
			klawa(56);


			if (po % 2 == 0 && ZapiszGiere % 2 == 1)
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				keysPressed[56] = TRUE;
				if (frame[9] == 0)
				{
					po++;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					fram();
				}

				else if (frame[9] == 1)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					bron = czekanie = state = punkty = ilo_bron = strzaly = 0;
					start = clock();
					po = 1;
					ZapiszGiere = 1;
					pozycja_startowa();
					fram();
				}

				else if (frame[9] == 2)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					obr_sciany = 1;
					WczytLabiryntu = 1;
					ZapiszGiere = 1;
					scen[0] = scen[2] = 7;
					fram();
				}

				else if (frame[9] == 3)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					ZapiszGiere++;
					fram();
				}

				else if (frame[9] == 4)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					obr_sciany = 0;
					WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
					WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
					WczytUstDzw();
					WczytUstGraf();
					scen[0] = scen[2] = 15;
					fram();
				}

				else if (frame[9] == 5)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					czekanie = state = scen[0] = scen[2] = obr_sciany = 0;
					obr.x1 = obr.y1 = 0.0f;
					start = clock();
					po = 1;
					WczytGre = 1;
					fram();
				}
			}

			else if (po % 2 == 0 && ZapiszGiere % 2 == 0)
			{
				if (frame[15] != 1)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					ZapLab(NazwaZapLab);
					NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
					dl = 0;
					for (INT kla = 0; kla < KLA - 25; kla++)
						for (INT i = 0; i < 19; i++)
							klaw[kla].zapis[i] = 0;
					ZapiszGiere++;
					fram();
				}

				if (frame[15] == 1)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
					dl = 0;
					for (INT kla = 0; kla < KLA - 25; kla++)
						for (INT i = 0; i < 19; i++)
							klaw[kla].zapis[i] = 0;
					ZapiszGiere++;
					fram();
				}
			}

			else if (po % 2 == 1 && ZapiszGiere % 2 == 1)
				SetCursor(LoadCursor(NULL, NULL));
		}

		else if (scen[0] == 12)
		{
			if (frame[6] == 0)
			{
				nowy[0] = 1;
				nowy[1] = 0; 
				
				for (INT i = 0; i < 7; i++)
					SingleThink[0] = 0; 
					
				ZaznGuz = 0;

				for (INT i = 0; i < IloElLab; i++)
					tab[i] = 0;

				for (INT z = 0; z < IloPunkt; z++)
					poz_scian[z] = 0;

				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				fram();
				frame[6] = 10;
			}

			else if (frame[6] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				IloscPlikow[0][0] = ZaznGuz = 0;
				IloscPlikow[0][1] = 1;
				ListaLabiryntow();
				scen[0] = scen[2] = 19;
				nowy[0] = nowy[1] = 1;
				
				for (INT i = 0; i < IloElLab; i++)
				{
					if (tab[i] != 2)
						SingleThink[0] = 0;
					if (tab[i] != 3)
						SingleThink[1] = 0;
					if (tab[i] != 4)
						SingleThink[2] = 0;
					if (tab[i] != 5)
						SingleThink[3] = 0;
					if (tab[i] != 7)
						SingleThink[4] = 0;
					if (tab[i] != 9)
						SingleThink[5] = 0;
					if (tab[i] != 11)
						SingleThink[6] = 0;
				}
				fram();
				frame[6] = 10;
			}

			else if (frame[6] == 2)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 20;
				ZaznGuz = 0;
				nowy[0] = nowy[1] = 1;
				fram();
			}

			else if (frame[6] == 3)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 13;
				fram();
			}

			else if (nowy[0] == 1)
			{
				if (ZaznGuz < 20)
				{
					for (INT i = 0; i < 12; i++)
					{
						if (ZaznGuz == i)
						{
							nowy[1] = i + 1;
							muza.dzwiek_potwierdzenie(glosnosc_efektow);
							ZaznGuz = 20;
						}
					}
				}

				else if (ZaznGuz >= 20)
				{
					ZaznKeys();
					zazn();
				}
			}
		}

		else if (scen[0] == 14)
		{
			if (klik[0] == 0)
			{
				for (INT k = 0; k < 15; k++)
				{
					if (frame[8] == k)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						zapis[0] = FALSE;
						pierwszy[k] = ustaw_klaw[k] = 1;
						ustawione = 0;
						klik[0] = 1;
						for (INT kk = 0; kk < 15; kk++)
						{
							if (k == kk)
							{
								for (INT kla = 0; kla < KLA - 25; kla++)
								{
									if (klaw[kla].ustawiony_klaw == k + 1)
									{
										klaw[kla].klawisz = 0;
										klaw[kla].ustawiony_klaw = 0;
									}
								}
							}
						}
					}
					else if (frame[8] != k)
						ustaw_klaw[k] = 0;
				}

				if (frame[8] == 15)
				{
					obr_sciany = klik[0] = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					for (INT kla = 0; kla < KLA - 25; kla++)
					{
						klaw[kla].klawisz = 0;
						klaw[kla].ustawiony_klaw = 0;
					}

					klaw[32].ustawiony_klaw = 1;
					klaw[46].ustawiony_klaw = 2;
					klaw[45].ustawiony_klaw = 3;
					klaw[47].ustawiony_klaw = 4;
					klaw[73].ustawiony_klaw = 5;
					klaw[74].ustawiony_klaw = 6;
					klaw[75].ustawiony_klaw = 7;
					klaw[76].ustawiony_klaw = 8;
					klaw[57].ustawiony_klaw = 9;
					klaw[33].ustawiony_klaw = 10;
					klaw[31].ustawiony_klaw = 11;
					klaw[71].ustawiony_klaw = 12;
					klaw[30].ustawiony_klaw = 13;
					klaw[84].ustawiony_klaw = 14;
					klaw[85].ustawiony_klaw = 15;

					UstWszKl();

					if (wczyt[0])
						zapis[0] = FALSE;
					else
						zapis[0] = TRUE;

					fram();
				}

				if (frame[8] == 16)
				{
					if (ustaw)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ZapUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0);
						fram();
					}
					else
						scen[0] = scen[2] = 23;
				}

				if (frame[8] == 17)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (zapis[0])
					{
						scen[0] = scen[2] = 21;
						ustawienia[0] = FALSE;
					}

					if (!zapis[0])
						scen[0] = scen[2] = 18;

					fram();
				}
			}

			else if (klik[0] == 1)
			{
				ustawianie_klawiszy(56);
				klik[0] = 0;
			}
		}

		else if (scen[0] == 16 && !ustaw_fullekran && !ustaw_rozdzielczosc)
		{
			if (frame[10] == 0)
			{
				ListaRozdz();
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ustaw_rozdzielczosc = TRUE;
				ustaw_fullekran = FALSE;
			}

			if (frame[10] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ustaw_fullekran = TRUE;
				ustaw_rozdzielczosc = FALSE;
			}

			if (frame[10] == 2 && klik[1] == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				klik[1] = 1;
				ustaw_fullekran = FALSE;
				ustaw_rozdzielczosc = FALSE;
			}

			if (frame[10] == 3)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ustaw_fullekran = FALSE;
				ustaw_rozdzielczosc = FALSE;

				rozdzielczosc_w = 1280.0f;
				rozdzielczosc_h = 600.0f;

				if (roz_w != rozdzielczosc_w || roz_h != rozdzielczosc_h)
					Resizable();

				Tlx[0] = 1.1f;
				jas_sw = 1.3f;
				full = FALSE;
				roz_w = rozdzielczosc_w;
				roz_h = rozdzielczosc_h;

				if (wczyt[3])
					zapis[3] = FALSE;
				else
					zapis[3] = TRUE;

				PozARozdz();

				fram();
			}

			if (frame[10] == 4)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ZapUstGraf();
				ustaw_fullekran = FALSE;
				ustaw_rozdzielczosc = FALSE;
				fram();
			}

			if (frame[10] == 5)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ustaw_fullekran = FALSE;
				ustaw_rozdzielczosc = FALSE;

				if (zapis[3])
				{
					scen[0] = scen[2] = 15;
					ustawienia[3] = FALSE;
				}

				if (!zapis[3])
					scen[0] = scen[2] = 18;

				fram();
			}
		}

		else if (scen[0] == 16 && ustaw_fullekran)
		{
			if (frame[12] == 1)
			{
				if (!full)
				{
					zapis[3] = FALSE;
					full = zmiana_full = TRUE;
				}

				if (zmiana_full && full)
					SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);

				zmiana_full = FALSE;
			}

			if (frame[12] == 2)
			{
				if (full)
				{
					zapis[3] = FALSE;
					full = FALSE;
					zmiana_full = TRUE;
				}

				if (zmiana_full && !full)
					SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);

				zmiana_full = FALSE;
			}
		}

		else if (scen[0] == 16 && ustaw_rozdzielczosc)
		{
			if (frame[13] == 0 && rozdz[lr[0]].ind == 1)
			{
				rozdzielczosc_w = (FLOAT)rozdz[lr[0]].width;
				rozdzielczosc_h = (FLOAT)rozdz[lr[0]].height;
				PozARozdz();

				if (roz_w != rozdzielczosc_w || roz_h != rozdzielczosc_h)
				{
					Resizable();

					roz_w = rozdzielczosc_w;
					roz_h = rozdzielczosc_h;

					zapis[3] = FALSE;
				}
			}

			if (frame[13] == 1 && rozdz[lr[1]].ind == 1)
			{
				rozdzielczosc_w = (FLOAT)rozdz[lr[1]].width;
				rozdzielczosc_h = (FLOAT)rozdz[lr[1]].height;
				PozARozdz();

				if (roz_w != rozdzielczosc_w || roz_h != rozdzielczosc_h)
				{
					Resizable();

					roz_w = rozdzielczosc_w;
					roz_h = rozdzielczosc_h;

					zapis[3] = FALSE;
				}
			}
		}

		else if (scen[0] == 17)
		{
			if (frame[11] == 0 && klik[2] == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				klik[2] = 1;
				klik[3] = 0;
			}

			if (frame[11] == 1 && klik[3] == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				klik[2] = 0;
				klik[3] = 1;
			}

			if (frame[11] == 2)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				Tlx[1] = Tlx[2] = 1.1f;
				glosnosc_tla = glosnosc_efektow = -5000;

				muza.muzyka_tlo_volume(glosnosc_tla);

				if (wczyt[2])
					zapis[2] = FALSE;
				else
					zapis[2] = TRUE;

				fram();
			}

			if (frame[11] == 3)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ZapUstDzw();
				fram();
			}

			if (frame[11] == 4)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				if (zapis[2])
				{
					scen[0] = scen[2] = 15;
					ustawienia[2] = FALSE;
				}

				if (!zapis[2])
					scen[0] = scen[2] = 18;

				fram();
			}
		}

		else if (scen[0] == 18)
		{
			if (frame[14] == 0)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				if (ustawienia[0])
				{
					if (!zapis[0] && wczyt[0])
						WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);

					if (!wczyt[0])
					{
						for (INT kla = 0; kla < KLA - 25; kla++)
						{
							klaw[kla].klawisz = 0;
							klaw[kla].ustawiony_klaw = 0;
						}

						klaw[32].ustawiony_klaw = 1;
						klaw[46].ustawiony_klaw = 2;
						klaw[45].ustawiony_klaw = 3;
						klaw[47].ustawiony_klaw = 4;
						klaw[73].ustawiony_klaw = 5;
						klaw[74].ustawiony_klaw = 6;
						klaw[75].ustawiony_klaw = 7;
						klaw[76].ustawiony_klaw = 8;
						klaw[57].ustawiony_klaw = 9;
						klaw[33].ustawiony_klaw = 10;
						klaw[31].ustawiony_klaw = 11;
						klaw[71].ustawiony_klaw = 12;
						klaw[30].ustawiony_klaw = 13;
						klaw[84].ustawiony_klaw = 14;
						klaw[85].ustawiony_klaw = 15;
					}

					UstWszKl();

					scen[0] = scen[2] = 21;
				}

				if (ustawienia[2])
				{
					if (!zapis[2] && wczyt[2])
					{
						WczytUstDzw();
						muza.muzyka_tlo_volume(glosnosc_tla);
					}

					if (!wczyt[2])
					{
						Tlx[1] = Tlx[2] = 1.1f;
						glosnosc_tla = glosnosc_efektow = -5000;
						muza.muzyka_tlo_volume(glosnosc_tla);
					}

					scen[0] = scen[2] = 15;
				}

				if (ustawienia[3])
				{
					if (!zapis[3] && wczyt[3])
					{
						WczytUstGraf();
						SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);
						Resizable();
					}

					if (!zapis[3] && !wczyt[3])
					{
						rozdzielczosc_w = 1280.0f;
						rozdzielczosc_h = 600.0f;
						full = FALSE;
						Tlx[0] = 1.1f;
						jas_sw = 1.3f;
						SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);
						Resizable();
					}

					scen[0] = scen[2] = 15;
				}

				if (ustawienia[1])
				{
					if (!zapis[1] && wczyt[1])
					WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);

					if (!wczyt[1])
					{
						for (INT kla = 86; kla < KLA; kla++)
						{
							klaw[kla].klawisz = 0;
							klaw[kla].ustawiony_klaw = 0;
						}

						klaw[86].ustawiony_klaw = 16;
						klaw[87].ustawiony_klaw = 17;
						klaw[88].ustawiony_klaw = 18;
						klaw[89].ustawiony_klaw = 19;
						klaw[91].ustawiony_klaw = 20;
						klaw[92].ustawiony_klaw = 21;
						klaw[93].ustawiony_klaw = 22;
						klaw[94].ustawiony_klaw = 23;
						klaw[108].ustawiony_klaw = 24;
						klaw[100].ustawiony_klaw = 25;
						klaw[101].ustawiony_klaw = 26;
						klaw[109].ustawiony_klaw = 27;
						klaw[107].ustawiony_klaw = 28;
						klaw[96].ustawiony_klaw = 29;
						klaw[97].ustawiony_klaw = 30;
					}

					UstWszKl();

					scen[0] = scen[2] = 21;
				}

				roz_w = rozdzielczosc_w;
				roz_h = rozdzielczosc_h;
				zmiana_full = FALSE;
				fram();
			}

			if (frame[14] == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				if (ustawienia[2])
					scen[0] = scen[2] = 17;

				if (ustawienia[3])
					scen[0] = scen[2] = 16;

				if (ustawienia[0])
					scen[0] = scen[2] = 14;

				if (ustawienia[1])
					scen[0] = scen[2] = 22;

				fram();
			}
		}

		else if (scen[0] == 19)
		{
			if (frame[16] != IloscPlikow[0][0] && frame[16] != IloLiter)
			{
				wygrana = FALSE;
				plansza = 5;
				WczytLab("ZapisaneLabirynty\\", pliki2[0][frame[16]]);
				dl_zycia = 0.3f;
				x_dl_zycia = -1.59f;
				strzaly = 0;
				punkty = 0;

				for (INT i = 0, ii = 12; i < 2, ii >= 11; i++, ii--)
				{
					if (wczytyw == i)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);

						if (i == 1)
							pozycja_startowa();

						nowy[0] = nowy[1] = 1;
				
						for (INT ik = 0; ik < IloElLab; ik++)
						{
							if (tab[i] != 2)
								SingleThink[0] = 0;
							if (tab[i] != 3)
								SingleThink[1] = 0;
							if (tab[i] != 4)
								SingleThink[2] = 0;
							if (tab[i] != 5)
								SingleThink[3] = 0;
							if (tab[i] != 7)
								SingleThink[4] = 0;
							if (tab[i] != 9)
								SingleThink[5] = 0;
							if (tab[i] != 11)
								SingleThink[6] = 0;
						}
						scen[0] = scen[2] = ii;
						fram();
					}
				}
			}

			if (frame[16] == IloscPlikow[0][0])
			{
				for (INT i = 0; i < 2; i++)
				{
					if (wczytyw == i)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = i + 12;
						fram();
					}
				}
			}
		}

		else if (scen[0] == 20 && ZapiszGiere % 2 == 1)
		{
			if (frame[15] != 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ZapLab(NazwaZapLab);
				NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
				dl = 0;
				for (INT kla = 0; kla < KLA - 25; kla++)
					for (INT i = 0; i < 19; i++)
						klaw[kla].zapis[i] = 0;
				scen[0] = scen[2] = 12;
				fram();
			}

			if (frame[15] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
				dl = 0;
				for (INT kla = 0; kla < KLA - 25; kla++)
					for (INT i = 0; i < 19; i++)
						klaw[kla].zapis[i] = 0;
				scen[0] = scen[2] = 12;
				fram();
			}
		}

		else if (scen[0] == 21)
		{
			if (frame[17] == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
				if (!wczyt[0])
				{
					for (INT kla = 0; kla < KLA - 25; kla++)
					{
						klaw[kla].klawisz = 0;
						klaw[kla].ustawiony_klaw = 0;
					}

					klaw[32].ustawiony_klaw = 1;
					klaw[46].ustawiony_klaw = 2;
					klaw[45].ustawiony_klaw = 3;
					klaw[47].ustawiony_klaw = 4;
					klaw[73].ustawiony_klaw = 5;
					klaw[74].ustawiony_klaw = 6;
					klaw[75].ustawiony_klaw = 7;
					klaw[76].ustawiony_klaw = 8;
					klaw[57].ustawiony_klaw = 9;
					klaw[33].ustawiony_klaw = 10;
					klaw[31].ustawiony_klaw = 11;
					klaw[71].ustawiony_klaw = 12;
					klaw[30].ustawiony_klaw = 13;
					klaw[84].ustawiony_klaw = 14;
					klaw[85].ustawiony_klaw = 15;
				}
				scen[0] = scen[2] = 14;
				fram();
			}

			if (frame[17] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
				if (!wczyt[1])
				{
					for (INT kla = 86; kla < KLA; kla++)
					{
						klaw[kla].klawisz = 0;
						klaw[kla].ustawiony_klaw = 0;
					}

					klaw[86].ustawiony_klaw = 16;
					klaw[87].ustawiony_klaw = 17;
					klaw[88].ustawiony_klaw = 18;
					klaw[89].ustawiony_klaw = 19;
					klaw[91].ustawiony_klaw = 20;
					klaw[92].ustawiony_klaw = 21;
					klaw[93].ustawiony_klaw = 22;
					klaw[94].ustawiony_klaw = 23;
					klaw[108].ustawiony_klaw = 24;
					klaw[100].ustawiony_klaw = 25;
					klaw[101].ustawiony_klaw = 26;
					klaw[109].ustawiony_klaw = 27;
					klaw[107].ustawiony_klaw = 28;
					klaw[96].ustawiony_klaw = 29;
					klaw[97].ustawiony_klaw = 30;
				}
				scen[0] = scen[2] = 22;
				fram();
			}

			if (frame[17] == 2 || frame[17] == 3)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 15;
				fram();
			}
		}

		else if (scen[0] == 22)
		{
			if (klik[4] == 0)
			{
				for (INT k = 0; k < 15; k++)
				{
					if (frame[18] == k)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						zapis[1] = FALSE;
						pierwszy[k + 15] = ustaw_klaw[k + 15] = 1;
						ustawione = 0;
						klik[4] = 1;
						for (INT kk = 0; kk < 15; kk++)
						{
							if (k == kk)
							{
								for (INT kla = 86; kla < KLA; kla++)
								{
									if (klaw[kla].ustawiony_klaw == k + 16)
									{
										klaw[kla].klawisz = 0;
										klaw[kla].ustawiony_klaw = 0;
									}
								}
							}
						}
					}

					else if (frame[18] != k)
						ustaw_klaw[k + 15] = 0;
				}

				if (frame[18] == 15)
				{
					obr_sciany = klik[4] = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					for (INT kla = 86; kla < KLA; kla++)
					{
						klaw[kla].klawisz = 0;
						klaw[kla].ustawiony_klaw = 0;
					}

					klaw[86].ustawiony_klaw = 16;
					klaw[87].ustawiony_klaw = 17;
					klaw[88].ustawiony_klaw = 18;
					klaw[89].ustawiony_klaw = 19;
					klaw[91].ustawiony_klaw = 20;
					klaw[92].ustawiony_klaw = 21;
					klaw[93].ustawiony_klaw = 22;
					klaw[94].ustawiony_klaw = 23;
					klaw[108].ustawiony_klaw = 24;
					klaw[100].ustawiony_klaw = 25;
					klaw[101].ustawiony_klaw = 26;
					klaw[109].ustawiony_klaw = 27;
					klaw[107].ustawiony_klaw = 28;
					klaw[96].ustawiony_klaw = 29;
					klaw[97].ustawiony_klaw = 30;

					UstWszKl();

					if (wczyt[1])
						zapis[1] = FALSE;
					else
						zapis[1] = TRUE;

					fram();
				}

				if (frame[18] == 16)
				{
					if (ustaw)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ZapUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 1);
						fram();
					}
					else
						scen[0] = scen[2] = 23;
				}

				if (frame[18] == 17)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (zapis[1])
					{
						scen[0] = scen[2] = 21;
						ustawienia[1] = FALSE;
					}

					if (!zapis[1])
						scen[0] = scen[2] = 18;

					fram();
				}
			}
		}

		else if (scen[0] == 23)
		{
			if (scen[1] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 14;
			}

			else if (scen[1] == 4)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 22;
			}
		}
	}

	else if (!KEYDOWN(keys, klaw[56].keysik) && keysPressed[56])
		keysPressed[56] = FALSE;

	else
		klawa_up(56);

	for (INT keyi = 57; keyi < 73; keyi++)
	{
		if (KEYDOWN(keys, klaw[keyi].keysik) && !keysPressed[keyi])
		{
			keysPressed[keyi] = TRUE;
			klawa(keyi);
			ustawianie_klawiszy(keyi);
			klik[0] = 0;
		}

		else if (!KEYDOWN(keys, klaw[keyi].keysik) && keysPressed[keyi])
		{
			keysPressed[keyi] = FALSE;
			klawa_up(keyi);
		}
	}

	if (KEYDOWN(keys, klaw[73].keysik) && !keysPressed[73])
	{
		keysPressed[73] = TRUE;
		if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
		{
			frame[0]--;
			if (frame[0]<0) frame[0] = 2;
			OdtwDzwMen[0][frame[0]] = Odtw[0][frame[0]] = 1;
		}

		else if ((scen[2] == 1) || (scen[2] == 6) || (scen[2] == 8) || (scen[2] == 10))
		{
			frame[1]--;
			if (frame[1]<0) frame[1] = 3;
			OdtwDzwMen[1][frame[1]] = Odtw[1][frame[1]] = 1;
		}

		else if ((scen[2] == 2) || (scen[2] == 15))
		{
			frame[2]--;
			if (frame[2]<0) frame[2] = 3;
			OdtwDzwMen[2][frame[2]] = Odtw[2][frame[2]] = 1;
		}

		else if ((scen[2] == 5) || (scen[2] == 13))
		{
			frame[3]--;
			if (frame[3]<0) frame[3] = 2;
			OdtwDzwMen[3][frame[3]] = Odtw[3][frame[3]] = 1;
		}

		else if (scen[2] == 7)
		{
			if (WczytLabiryntu % 2 == 1)
			{
				frame[4]--;
				if (frame[4] < 0) frame[4] = 1;
				OdtwDzwMen[4][frame[4]] = Odtw[4][frame[4]] = 1;
			}

			else if (WczytLabiryntu % 2 == 0)
			{
				if (frame[16] == IloLiter)
					frame[16] = 0;

				if (frame[16] < IloscPlikow[1][0])
				{
					if (frame[16] > 0)
						frame[16]--;

					else if (frame[16] <= 0)
					{
						if (IloscPlikow[1][1] > 1)
							IloscPlikow[1][1]--;

						else if (IloscPlikow[1][0] > 16)
						{
							IloscPlikow[1][1] = IloscPlikow[1][0] - 17;
							frame[16] = IloscPlikow[1][0];
						}

						else if (IloscPlikow[1][1] == 1)
							frame[16] = IloscPlikow[1][0];
					}
				}

				else if (frame[16] == IloscPlikow[1][0])
				{
					if (IloscPlikow[1][0] > 16)
						IloscPlikow[1][1] = frame[16] - 17;

					if (IloscPlikow[1][1] > 1)
						frame[16] = frame[16] - IloscPlikow[1][1] - 1;

					else if (IloscPlikow[1][1] == 1)
						frame[16] = IloscPlikow[1][0] - 1;
				}

				OdtwDzwMen[16][0] = Odtw[16][0] = 1;
			}
		}

		else if (scen[2] == 9)
		{
			frame[5]--;
			if (frame[5] < 0) frame[5] = 1;
			OdtwDzwMen[5][frame[5]] = Odtw[5][frame[5]] = 1;
		}

		else if (scen[0] == 11)
		{
			keysPressed[73] = FALSE;
			reset_czasu = 0;
			if (po % 2 == 0 && ZapiszGiere % 2 == 1)
			{
				keysPressed[73] = TRUE;
				frame[9]--;
				if (frame[9]<0) frame[9] = 5;
				OdtwDzwMen[9][frame[9]] = Odtw[9][frame[9]] = 1;
			}

			klawa(73);
		}

		else if ((scen[0] == 12 && ZaznGuz == 0) ||
				(scen[0] == 12 && ZaznGuz == 2) ||
				(scen[0] == 12 && ZaznGuz == 4) ||
				(scen[0] == 12 && ZaznGuz == 6) ||
				(scen[0] == 12 && ZaznGuz == 8) ||
				(scen[0] == 12 && CzerwonyNapis[7][10] == 1 && ZaznGuz == 10))
		{
			frame[7] -= 2;
			ZaznGuz -= 2;
			if (ZaznGuz < 0) ZaznGuz = 10;
			if (frame[7] < 0) frame[7] = 10;

			if (frame[7] == 0)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 2)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 4)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 6)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 8)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 10)
			{
				CzerwonyNapis[7][11] = 0;
				CzerwonyNapis[7][10] = 1;
			}

			OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
		}

		else if ((scen[0] == 12 && ZaznGuz == 1) ||
				(scen[0] == 12 && ZaznGuz == 3) ||
				(scen[0] == 12 && ZaznGuz == 5) ||
				(scen[0] == 12 && ZaznGuz == 7) ||
				(scen[0] == 12 && ZaznGuz == 9) ||
				(scen[0] == 12 && CzerwonyNapis[7][11] == 1 && ZaznGuz == 11))
		{
			frame[7] -= 2;
			ZaznGuz -= 2;
			if (ZaznGuz < 1) ZaznGuz = 11;
			if (frame[7] < 1) frame[7] = 11;

			if (frame[7] == 1)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 3)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 5)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 7)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 9)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 11)
			{
				CzerwonyNapis[7][10] = 0;
				CzerwonyNapis[7][11] = 1;
			}

			OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
		}

		else if (scen[0] == 12 && ZaznGuz == 15)
		{
			frame[6]--;
			if (frame[6]<0) frame[6] = 3;
			OdtwDzwMen[6][frame[6]] = Odtw[6][frame[6]] = 1;
		}

		else if (scen[0] == 12 && ZaznGuz >= 20)
		{
			if (ZaznGuz <= 45)
			{
				if (nowy[1] > 0)
				{
					w -= 26;
					if (w <= 1) w = 1;
				}
			}

			else if(ZaznGuz > 45)
				ZaznGuz -= 26;

			ZaznKeys();
		}

		else if (scen[2] == 14 && klik[0] == 0)
		{
			frame[8]--;
			if (frame[8] < 0) frame[8] = 15;
			OdtwDzwMen[8][frame[8]] = Odtw[8][frame[8]] = 1;
		}

		else if (scen[0] == 14 && klik[0] == 1)
		{
			ustawianie_klawiszy(73);
			klik[0] = 0;
		}

		else if (scen[2] == 16 && !ustaw_rozdzielczosc && !ustaw_fullekran)
		{
			frame[10]--;
			if (frame[10]<0) frame[10] = 3;
			OdtwDzwMen[10][frame[10]] = Odtw[10][frame[10]] = 1;
		}

		else if (scen[2] == 16 && ustaw_fullekran)
		{
			frame[12]--;
			if (frame[12] < 1) frame[12] = 2;
			OdtwDzwMen[12][frame[12]] = Odtw[12][frame[12]] = 1;

			frame[10] = 10;
			frame[13] = 4;
		}

		else if (scen[2] == 16 && ustaw_rozdzielczosc)
		{
			if (frame[13] == 0)
			{
				lr[0]--;
				lr[1]--;
			}

			if (lr[0] <= 0) lr[0] = 0;
			if (lr[1] <= 1) lr[1] = 1;

			rozdz[lr[0]].ind = 1;
			rozdz[lr[1]].ind = 2;

			frame[13]--;
			if (frame[13] < 0) frame[13] = 0;
			OdtwDzwMen[13][frame[13]] = Odtw[13][frame[13]] = 1;

			frame[12] = 4;
			frame[10] = 10;
		}

		else if (scen[2] == 17)
		{
			frame[11]--;
			if (frame[11]<0) frame[11] = 2;
			OdtwDzwMen[11][frame[11]] = Odtw[11][frame[11]] = 1;
		}

		else if (scen[2] == 19)
		{
			if (frame[16] == IloLiter)
				frame[16] = 0;

			if (frame[16] < IloscPlikow[0][0])
			{
				if (frame[16] > 0)
					frame[16]--;

				else if (frame[16] <= 0)
				{
					if (IloscPlikow[0][1] > 1)
						IloscPlikow[0][1]--;

					else if (IloscPlikow[0][0] > 16)
					{
						IloscPlikow[0][1] = IloscPlikow[0][0] - 17;
						frame[16] = IloscPlikow[0][0];
					}

					else if (IloscPlikow[0][1] == 1)
						frame[16] = IloscPlikow[0][0];
				}
			}

			else if (frame[16] == IloscPlikow[0][0])
			{
				if (IloscPlikow[0][0] > 16)
					IloscPlikow[0][1] = frame[16] - 17;

				if (IloscPlikow[0][1] > 1)
					frame[16] = frame[16] - IloscPlikow[0][1] - 1;

				else if (IloscPlikow[0][1] == 1)
					frame[16] = IloscPlikow[0][0] - 1;
			}

			OdtwDzwMen[16][0] = Odtw[16][0] = 1;
		}

		else if (scen[2] == 21)
		{
			if (frame[17] == 0) frame[17] = 2;
			else if (frame[17] == 1) frame[17] = 3;
			else if (frame[17] == 2) frame[17] = 0;
			else if (frame[17] == 3) frame[17] = 1;
			OdtwDzwMen[17][frame[17]] = Odtw[17][frame[17]] = 1;
		}

		else if (scen[2] == 22 && klik[4] == 0)
		{
			frame[18]--;
			if (frame[18] < 0) frame[18] = 15;
			OdtwDzwMen[18][frame[18]] = Odtw[18][frame[18]] = 1;
		}
	}

	else if (!KEYDOWN(keys, klaw[73].keysik) && keysPressed[73])
		keysPressed[73] = FALSE;

	else
		klawa_up(73);

	if (KEYDOWN(keys, klaw[74].keysik) && !keysPressed[74])
	{
		keysPressed[74] = TRUE;
		if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
		{
			frame[0]++;
			if (frame[0]>2) frame[0] = 0;
			OdtwDzwMen[0][frame[0]] = Odtw[0][frame[0]] = 1;
		}

		else if ((scen[2] == 1) || (scen[2] == 6) || (scen[2] == 8) || (scen[2] == 10))
		{
			frame[1]++;
			if (frame[1]>3) frame[1] = 0;
			OdtwDzwMen[1][frame[1]] = Odtw[1][frame[1]] = 1;
		}

		else if ((scen[2] == 2) || (scen[2] == 15))
		{
			frame[2]++;
			if (frame[2]>3) frame[2] = 0;
			OdtwDzwMen[2][frame[2]] = Odtw[2][frame[2]] = 1;
		}

		else if ((scen[2] == 5) || (scen[2] == 13))
		{
			frame[3]++;
			if (frame[3]>2) frame[3] = 0;
			OdtwDzwMen[3][frame[3]] = Odtw[3][frame[3]] = 1;
		}

		else if (scen[2] == 7)
		{
			if (WczytLabiryntu % 2 == 1)
			{
				frame[4]++;
				if (frame[4] > 1) frame[4] = 0;
				OdtwDzwMen[4][frame[4]] = Odtw[4][frame[4]] = 1;
			}

			if (WczytLabiryntu % 2 == 0)
			{
				if (frame[16] == IloLiter)
					frame[16] = 0;

				if (frame[16] < IloscPlikow[1][0])
				{
					if (frame[16] < 16)
						frame[16]++;

					else if (frame[16] == 16 && IloscPlikow[1][1] < IloscPlikow[1][0] - 17)
						IloscPlikow[1][1]++;

					else if (IloscPlikow[1][1] == IloscPlikow[1][0] - 17)
						frame[16] += IloscPlikow[1][1] + 1;
				}

				else if (frame[16] >= IloscPlikow[1][0])
				{
					frame[16] = 0;
					IloscPlikow[1][1] = 1;
				}

				OdtwDzwMen[16][0] = Odtw[16][0] = 1;
			}
		}

		else if (scen[2] == 9)
		{
			frame[5]++;
			if (frame[5] > 1) frame[5] = 0;
			OdtwDzwMen[5][frame[5]] = Odtw[5][frame[5]] = 1;
		}

		else if (scen[0] == 11)
		{
			keysPressed[74] = FALSE;
			reset_czasu = 0;
			if (po % 2 == 0 && ZapiszGiere % 2 == 1)
			{
				keysPressed[74] = TRUE;
				frame[9]++;
				if (frame[9]>5) frame[9] = 0;
				OdtwDzwMen[9][frame[9]] = Odtw[9][frame[9]] = 1;
			}

			klawa(74);
		}

		else if ((scen[0] == 12 && ZaznGuz == 0) ||
				(scen[0] == 12 && ZaznGuz == 2) ||
				(scen[0] == 12 && ZaznGuz == 4) ||
				(scen[0] == 12 && ZaznGuz == 6) ||
				(scen[0] == 12 && ZaznGuz == 8) ||
				(scen[0] == 12 && CzerwonyNapis[7][10] == 1 && ZaznGuz == 10))
		{
			frame[7] += 2;
			ZaznGuz += 2;
			if (ZaznGuz > 10) ZaznGuz = 0;
			if (frame[7] > 10) frame[7] = 0;

			if (frame[7] == 0)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 2)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 4)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 6)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 8)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 10)
			{
				CzerwonyNapis[7][11] = 0;
				CzerwonyNapis[7][10] = 1;
			}

			OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
		}

		else if ((scen[0] == 12 && ZaznGuz == 1) ||
				(scen[0] == 12 && ZaznGuz == 3) ||
				(scen[0] == 12 && ZaznGuz == 5) ||
				(scen[0] == 12 && ZaznGuz == 7) ||
				(scen[0] == 12 && ZaznGuz == 9) ||
				(scen[0] == 12 && CzerwonyNapis[7][11] == 1 && ZaznGuz  == 11))
		{
			frame[7] += 2;
			ZaznGuz += 2;
			if (ZaznGuz > 11) ZaznGuz = 1;
			if (frame[7] > 11) frame[7] = 1;

			if (frame[7] == 1)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 3)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 5)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 7)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 9)
				CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;

			else if (frame[7] == 11)
			{
				CzerwonyNapis[7][10] = 0;
				CzerwonyNapis[7][11] = 1;
			}

			OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
		}

		else if (scen[0] == 12 && ZaznGuz == 15)
		{
			frame[6]++;
			if (frame[6]>3) frame[6] = 0;
			OdtwDzwMen[6][frame[6]] = Odtw[6][frame[6]] = 1;
		}

		else if (scen[0] == 12 && ZaznGuz >= 20)
		{
			if(ZaznGuz < ZaznaczGuziczek)
				ZaznGuz += 26;

			else if (ZaznGuz >= ZaznaczGuziczek)
			{
				if (nowy[1] > 0)
				{
					w += 26;
					if (w >= 781) w = 781;
				}
			}

			ZaznKeys();
		}

		else if (scen[2] == 14 && klik[0] == 0)
		{
			frame[8]++;
			if (frame[8] > 15) frame[8] = 0;
			OdtwDzwMen[8][frame[8]] = Odtw[8][frame[8]] = 1;
		}

		else if (scen[0] == 14 && klik[0] == 1)
		{
			ustawianie_klawiszy(74);
			klik[0] = 0;
		}

		else if (scen[2] == 16 && !ustaw_rozdzielczosc && !ustaw_fullekran)
		{
			frame[10]++;
			if (frame[10]>3) frame[10] = 0;
			OdtwDzwMen[10][frame[10]] = Odtw[10][frame[10]] = 1;
		}

		else if (scen[2] == 16 && ustaw_fullekran)
		{
			frame[12]++;
			if (frame[12] > 2) frame[12] = 1;
			OdtwDzwMen[12][frame[12]] = Odtw[12][frame[12]] = 1;

			frame[10] = 10;
			frame[13] = 4;
		}

		else if (scen[2] == 16 && ustaw_rozdzielczosc)
		{
			if (frame[13] == 1)
			{
				lr[0]++;
				lr[1]++;
			}

			if (lr[0] >= index2 - 2)  lr[0] = index2 - 2;
			if (lr[1] >= index2 - 1)  lr[1] = index2 - 1;

			rozdz[lr[0]].ind = 2;
			rozdz[lr[1]].ind = 1;

			frame[13]++;
			if (frame[13] > 1) frame[13] = 1;

			OdtwDzwMen[13][frame[13]] = Odtw[13][frame[13]] = 1;

			frame[12] = 4;
			frame[10] = 10;
		}

		else if (scen[2] == 17)
		{
			frame[11]++;
			if (frame[11]>2) frame[11] = 0;
			OdtwDzwMen[11][frame[11]] = Odtw[11][frame[11]] = 1;
		}

		else if (scen[2] == 19)
		{
			if (frame[16] == IloLiter)
				frame[16] = 0;

			if (frame[16] < IloscPlikow[0][0])
			{
				if (frame[16] < 16)
					frame[16]++;

				else if (frame[16] == 16 && IloscPlikow[0][1] < IloscPlikow[0][0] - 17)
					IloscPlikow[0][1]++;

				else if (IloscPlikow[0][1] == IloscPlikow[0][0] - 17)
					frame[16] += IloscPlikow[0][1] + 1;
			}

			else if (frame[16] >= IloscPlikow[0][0])
			{
				frame[16] = 0;
				IloscPlikow[0][1] = 1;
			}

			OdtwDzwMen[16][0] = Odtw[16][0] = 1;
		}

		else if (scen[2] == 21)
		{
			if (frame[17] == 0) frame[17] = 2;
			else if (frame[17] == 1) frame[17] = 3;
			else if (frame[17] == 2) frame[17] = 0;
			else if (frame[17] == 3) frame[17] = 1;
			OdtwDzwMen[17][frame[17]] = Odtw[17][frame[17]] = 1;
		}

		else if (scen[2] == 22 && klik[4] == 0)
		{
			frame[18]++;
			if (frame[18] > 15) frame[18] = 0;
			OdtwDzwMen[18][frame[18]] = Odtw[18][frame[18]] = 1;
		}
	}

	else if (!KEYDOWN(keys, klaw[74].keysik) && keysPressed[74])
		keysPressed[74] = FALSE;

	else
		klawa_up(74);

	if (KEYDOWN(keys, klaw[75].keysik) && !keysPressed[75])
	{
		keysPressed[75] = TRUE;

		if (scen[0] == 11 && ZapiszGiere % 2 == 1)
		{
			keysPressed[75] = FALSE;
			klawa(75);
		}

		if (scen[0] == 12 && ZaznGuz <= 19)
		{
			if ((scen[0] == 12 && ZaznGuz == 0) || (scen[0] == 12 && ZaznGuz == 1))
			{
				frame[7]--;
				if (frame[7] < 0) frame[7] = 1;

				if (frame[7] == 0)
				{
					ZaznGuz = 0;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 1)
				{
					ZaznGuz = 1;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 2) || (scen[0] == 12 && ZaznGuz == 3))
			{
				frame[7]--;
				if (frame[7] < 2) frame[7] = 3;

				if (frame[7] == 2)
				{
					ZaznGuz = 2;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 3)
				{
					ZaznGuz = 3;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 4) || (scen[0] == 12 && ZaznGuz == 5))
			{
				frame[7]--;
				if (frame[7] < 4) frame[7] = 5;

				if (frame[7] == 4)
				{
					ZaznGuz = 4;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 5)
				{
					ZaznGuz = 5;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 6) || (scen[0] == 12 && ZaznGuz == 7))
			{
				frame[7]--;
				if (frame[7] < 6) frame[7] = 7;

				if (frame[7] == 6)
				{
					ZaznGuz = 6;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 7)
				{
					ZaznGuz = 7;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 8) || (scen[0] == 12 && ZaznGuz == 9))
			{
				frame[7]--;
				if (frame[7] < 8) frame[7] = 9;

				if (frame[7] == 8)
				{
					ZaznGuz = 8;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 9)
				{
					ZaznGuz = 9;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 10 && CzerwonyNapis[7][10] == 1) || (scen[0] == 12 && ZaznGuz == 11 && CzerwonyNapis[7][11] == 1))
			{
				frame[7]--;
				if (frame[7] < 10) frame[7] = 11;

				if (frame[7] == 10)
				{
					ZaznGuz = 10;
					CzerwonyNapis[7][11] = 0;
					CzerwonyNapis[7][10] = 1;
				}

				if (frame[7] == 11)
				{
					ZaznGuz = 11;
					CzerwonyNapis[7][10] = 0;
					CzerwonyNapis[7][11] = 1;
				}
				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}
		}
		else if (scen[0] == 12 && ZaznGuz >= 20)
		{
			if (ZaznGuz != 20 && ZaznGuz != 46 && ZaznGuz != 72 && ZaznGuz != 98 && ZaznGuz != 124 && ZaznGuz != 150 && ZaznGuz != 176 && ZaznGuz != 202 && ZaznGuz != 228 && ZaznGuz != 254 && ZaznGuz != 280 && ZaznGuz != 306 && ZaznGuz != 332 && ZaznGuz != 358 && ZaznGuz != 384 && ZaznGuz != 410 && ZaznGuz != 436 && ZaznGuz != 462 && ZaznGuz != 488 && ZaznGuz != 514 && ZaznGuz != 540 && ZaznGuz != 566 && ZaznGuz != 592 && ZaznGuz != 618 && ZaznGuz != 644 && ZaznGuz != 670 && ZaznGuz != 696 && ZaznGuz != 722 && ZaznGuz != 748 && ZaznGuz != 774 && ZaznGuz != 800 && ZaznGuz != 826 && ZaznGuz != 852 && ZaznGuz != 878 && ZaznGuz != 904)
				ZaznGuz--;

			ZaznKeys();
		}

		else if (scen[2] == 14 && klik[0] == 0)
		{
			if (frame[8] == 15 || frame[8] == 16 || frame[8] == 17)
			{
				keysPressed[75] = TRUE;
				frame[8]++;
				if (frame[8] > 17) frame[8] = 15;
				OdtwDzwMen[8][frame[8]] = Odtw[8][frame[8]] = 1;
			}
		}

		if (scen[2] == 14 && klik[0] == 1)
		{
			ustawianie_klawiszy(75);
			klik[0] = 0;
		}

		if (scen[0] == 16)
		{
			keysPressed[75] = FALSE;
			if (frame[10] == 2 && klik[1] == 1)
			{
				zapis[3] = FALSE;
				jas_sw -= 0.1f;
				if (jas_sw <= 0.2f) jas_sw = 0.2f;

				if (jas_sw == 0.7f || jas_sw == 0.8f || jas_sw == 1.9f || jas_sw == 1.8f)
					Tlx[0] -= 0.015f;
				else
					Tlx[0] -= 0.03f;

				if (Tlx[0] <= 0.8f) Tlx[0] = 0.8f;
			}

			if (frame[10] == 3 || frame[10] == 4 || frame[10] == 5)
			{
				keysPressed[75] = TRUE;
				frame[10]++;
				if (frame[10] > 5) frame[10] = 3;
				OdtwDzwMen[10][frame[10]] = Odtw[10][frame[10]] = 1;
			}
		}

		else if (scen[0] == 17)
		{
			keysPressed[75] = FALSE;
			if (frame[11] == 0 && klik[2] == 1)
			{
				zapis[2] = FALSE;

				if (glosnosc_tla == -7496)
				{
					Tlx[1] -= 0.015f;
					glosnosc_tla -= 840;
				}

				else if (glosnosc_tla == -7080 || glosnosc_tla == -2504 || glosnosc_tla == -2920)
				{
					Tlx[1] -= 0.015f;
					glosnosc_tla -= 416;
				}

				else
				{
					glosnosc_tla -= 416;
					Tlx[1] -= 0.03f;
				}

				if (glosnosc_tla <= -10000) glosnosc_tla = -10000;
				if (Tlx[1] <= 0.8) Tlx[1] = 0.8f;

				muza.muzyka_tlo_volume(glosnosc_tla);
			}

			if (frame[11] == 1 && klik[3] == 1)
			{
				zapis[2] = FALSE;

				if (glosnosc_efektow == -7496)
				{
					Tlx[2] -= 0.015f;
					glosnosc_efektow -= 840;
				}

				else if (glosnosc_efektow == -7080 || glosnosc_efektow == -2504 || glosnosc_efektow == -2920)
				{
					Tlx[2] -= 0.015f;
					glosnosc_efektow -= 416;
				}

				else
				{
					glosnosc_efektow -= 416;
					Tlx[2] -= 0.03f;
				}

				if (glosnosc_efektow <= -10000) glosnosc_efektow = -10000;
				if (Tlx[2] <= 0.8f) Tlx[2] = 0.8f;

				muza.dzwiek_potwierdzenie(glosnosc_efektow);
			}

			if (frame[11] == 2 || frame[11] == 3 || frame[11] == 4)
			{
				keysPressed[75] = TRUE;
				frame[11]++;
				if (frame[11] > 4) frame[11] = 2;
				OdtwDzwMen[11][frame[11]] = Odtw[11][frame[11]] = 1;
			}
		}

		else if (scen[0] == 18)
		{
			frame[14]--;
			if (frame[14] < 0) frame[14] = 1;
			OdtwDzwMen[14][frame[14]] = Odtw[14][frame[14]] = 1;
		}

		else if ((scen[0] == 20) || (po % 2 == 0 && ZapiszGiere % 2 == 0))
		{
			frame[15]--;
			if (frame[15] < 0) frame[15] = 1;
			OdtwDzwMen[15][frame[15]] = Odtw[15][frame[15]] = 1;
		}

		else if (scen[2] == 21)
		{
			if (frame[17] == 0) frame[17] = 1;
			else if (frame[17] == 1) frame[17] = 0;
			OdtwDzwMen[17][frame[17]] = Odtw[17][frame[17]] = 1;
		}

		else if (scen[2] == 22 && klik[4] == 0)
		{
			if (frame[18] == 15 || frame[18] == 16 || frame[18] == 17)
			{
				keysPressed[75] = TRUE;
				frame[18]++;
				if (frame[18] > 17) frame[18] = 15;
				OdtwDzwMen[18][frame[18]] = Odtw[18][frame[18]] = 1;
			}
		}
	}

	else if (!KEYDOWN(keys, klaw[75].keysik) && keysPressed[75])
		keysPressed[75] = FALSE;

	else
		klawa_up(75);

	if (KEYDOWN(keys, klaw[76].keysik) && !keysPressed[76])
	{
		keysPressed[76] = TRUE;

		if (scen[0] == 11 && ZapiszGiere % 2 == 1)
		{
			keysPressed[76] = FALSE;
			klawa(76);
		}

		else if (scen[0] == 12 && ZaznGuz <= 19)
		{
			if ((scen[0] == 12 && ZaznGuz == 0) || (scen[0] == 12 && ZaznGuz == 1))
			{
				frame[7]++;
				if (frame[7] > 1) frame[7] = 0;

				if (frame[7] == 0)
				{
					ZaznGuz = 0;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 1)
				{
					ZaznGuz = 1;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 2) || (scen[0] == 12 && ZaznGuz == 3))
			{
				frame[7]++;
				if (frame[7] > 3) frame[7] = 2;

				if (frame[7] == 2)
				{
					ZaznGuz = 2;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 3)
				{
					ZaznGuz = 3;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 4) || (scen[0] == 12 && ZaznGuz == 5))
			{
				frame[7]++;
				if (frame[7] > 5) frame[7] = 4;

				if (frame[7] == 4)
				{
					ZaznGuz = 4;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 5)
				{
					ZaznGuz = 5;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 6) || (scen[0] == 12 && ZaznGuz == 7))
			{
				frame[7]++;
				if (frame[7] > 7) frame[7] = 6;

				if (frame[7] == 6)
				{
					ZaznGuz = 6;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 7)
				{
					ZaznGuz = 7;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 8) || (scen[0] == 12 && ZaznGuz == 9))
			{
				frame[7]++;
				if (frame[7] > 9) frame[7] = 8;

				if (frame[7] == 8)
				{
					ZaznGuz = 8;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				if (frame[7] == 9)
				{
					ZaznGuz = 9;
					CzerwonyNapis[7][10] = CzerwonyNapis[7][11] = 0;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if ((scen[0] == 12 && ZaznGuz == 10 && CzerwonyNapis[7][10] == 1) || (scen[0] == 12 && ZaznGuz == 11 && CzerwonyNapis[7][11] == 1))
			{
				frame[7]++;
				if (frame[7] > 11) frame[7] = 10;

				if (frame[7] == 10)
				{
					ZaznGuz = 10;
					CzerwonyNapis[7][11] = 0;
					CzerwonyNapis[7][10] = 1;
				}

				if (frame[7] == 11)
				{
					ZaznGuz = 11;
					CzerwonyNapis[7][10] = 0;
					CzerwonyNapis[7][11] = 1;
				}

				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}

			else if (scen[0] == 12 && CzerwonyNapis[7][10] == 0 && CzerwonyNapis[7][11] == 0)
			{
				frame[7] = 0;
				frame[6] = 10;
				OdtwDzwMen[7][frame[7]] = Odtw[7][frame[7]] = 1;
			}
		}

		else if (scen[0] == 12 && ZaznGuz >= 20)
		{
			if (ZaznGuz != 45 && ZaznGuz != 71 && ZaznGuz != 97 && ZaznGuz != 123 && ZaznGuz != 149 && ZaznGuz != 175 && ZaznGuz != 201 && ZaznGuz != 227 && ZaznGuz != 253 && ZaznGuz != 279 && ZaznGuz != 305 && ZaznGuz != 331 && ZaznGuz != 357 && ZaznGuz != 383 && ZaznGuz != 409 && ZaznGuz != 435 && ZaznGuz != 461 && ZaznGuz != 487 && ZaznGuz != 513 && ZaznGuz != 539 && ZaznGuz != 565 && ZaznGuz != 591 && ZaznGuz != 617 && ZaznGuz != 643 && ZaznGuz != 669 && ZaznGuz != 695 && ZaznGuz != 721 && ZaznGuz != 747 && ZaznGuz != 773 && ZaznGuz != 799 && ZaznGuz != 825 && ZaznGuz != 851 && ZaznGuz != 877 && ZaznGuz != 903 && ZaznGuz != 929 && ZaznGuz != 955 && ZaznGuz != 981 && ZaznGuz != 1007)
				ZaznGuz++;

			ZaznKeys();
		}

		else if (scen[2] == 14 && klik[0] == 0)
		{
			if (frame[8] == 15 || frame[8] == 16 || frame[8] == 17)
			{
				keysPressed[76] = TRUE;
				frame[8]--;
				if (frame[8] < 15) frame[8] = 17;
				OdtwDzwMen[8][frame[8]] = Odtw[8][frame[8]] = 1;
			}
		}

		if (scen[2] == 14 && klik[0] == 1)
		{
			ustawianie_klawiszy(76);
			klik[0] = 0;
		}

		if (scen[0] == 16)
		{
			keysPressed[76] = FALSE;
			if (frame[10] == 2 && klik[1] == 1)
			{
				zapis[3] = FALSE;
				jas_sw += 0.1f;
				if (jas_sw >= 2.4f) jas_sw = 2.4f;

				if (jas_sw == 0.6f || jas_sw == 0.7f || jas_sw == 1.7f || jas_sw == 1.8f)
					Tlx[0] += 0.015f;
				else
					Tlx[0] += 0.03f;

				if (Tlx[0] >= 1.4f) Tlx[0] = 1.4f;
			}

			if (frame[10] == 3 || frame[10] == 4 || frame[10] == 5)
			{
				keysPressed[76] = TRUE;
				frame[10]--;
				if (frame[10] < 3) frame[10] = 5;
				OdtwDzwMen[10][frame[10]] = Odtw[10][frame[10]] = 1;
			}
		}

		else if (scen[0] == 17)
		{
			keysPressed[76] = FALSE;
			if (frame[11] == 0 && klik[2] == 1)
			{
				zapis[2] = FALSE;

				if (glosnosc_tla == -7496)
				{
					Tlx[1] += 0.015f;
					glosnosc_tla += 840;
				}

				else if (glosnosc_tla == -7080 || glosnosc_tla == -2504 || glosnosc_tla == -2920)
				{
					Tlx[1] += 0.015f;
					glosnosc_tla += 416;
				}

				else
				{
					glosnosc_tla += 416;
					Tlx[1] += 0.03f;
				}

				if (glosnosc_tla >= 0) glosnosc_tla = 0;
				if (Tlx[1] >= 1.4f) Tlx[1] = 1.4f;

				muza.muzyka_tlo_volume(glosnosc_tla);
			}

			if (frame[11] == 1 && klik[3] == 1)
			{
				zapis[2] = FALSE;

				if (glosnosc_efektow == -7496)
				{
					Tlx[2] += 0.015f;
					glosnosc_efektow += 840;
				}

				else if (glosnosc_efektow == -7080 || glosnosc_efektow == -2504 || glosnosc_efektow == -2920)
				{
					Tlx[2] += 0.015f;
					glosnosc_efektow += 416;
				}

				else
				{
					glosnosc_efektow += 416;
					Tlx[2] += 0.03f;
				}

				if (glosnosc_efektow >= 0) glosnosc_efektow = 0;
				if (Tlx[2] >= 1.4f) Tlx[2] = 1.4f;

				muza.dzwiek_potwierdzenie(glosnosc_efektow);
			}

			if (frame[11] == 2 || frame[11] == 3 || frame[11] == 4)
			{
				keysPressed[76] = TRUE;
				frame[11]--;
				if (frame[11] < 2) frame[11] = 4;
				OdtwDzwMen[11][frame[11]] = Odtw[11][frame[11]] = 1;
			}
		}

		else if (scen[0] == 18)
		{
			frame[14]++;
			if (frame[14] > 1) frame[14] = 0;
			OdtwDzwMen[14][frame[14]] = Odtw[14][frame[14]] = 1;
		}

		else if ((scen[0] == 20) || (po % 2 == 0 && ZapiszGiere % 2 == 0))
		{
			frame[15]++;
			if (frame[15] > 1) frame[15] = 0;
			OdtwDzwMen[15][frame[15]] = Odtw[15][frame[15]] = 1;
		}

		else if (scen[2] == 21)
		{
			if (frame[17] == 0) frame[17] = 1;
			else if (frame[17] == 1) frame[17] = 0;
			OdtwDzwMen[17][frame[17]] = Odtw[17][frame[17]] = 1;
		}

		else if (scen[2] == 22 && klik[4] == 0)
		{
			if (frame[18] == 15 || frame[18] == 16 || frame[18] == 17)
			{
				keysPressed[76] = TRUE;
				frame[18]--;
				if (frame[18] < 15) frame[18] = 17;
				OdtwDzwMen[18][frame[18]] = Odtw[18][frame[18]] = 1;
			}
		}
	}

	else if (!KEYDOWN(keys, klaw[76].keysik) && keysPressed[76])
		keysPressed[76] = FALSE;

	else
		klawa_up(76);

	for (INT keyi = 77; keyi < 82; keyi++)
	{
		if (KEYDOWN(keys, klaw[keyi].keysik) && !keysPressed[keyi])
		{
			keysPressed[keyi] = TRUE;
			klawa(keyi);
			ustawianie_klawiszy(keyi);
			klik[0] = 0;
		}

		else if (!KEYDOWN(keys, klaw[keyi].keysik) && keysPressed[keyi])
		{
			keysPressed[keyi] = FALSE;
			klawa_up(keyi);
		}
	}
}

void Renderer::DetectInputMouse()
{
	DIMouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);

	mouseLastState.lY += mouse_state.lY;
	mouseLastState.lX += mouse_state.lX;

	if (BUTTONDOWN(mouse_state, 0) && MouseButton[0])
	{
		MouseButton[0] = FALSE;

		if ((scen[0] == 0) || (scen[0] == 3) || (scen[0] == 4))
		{
			for (INT i = 0; i < 2; i++)
			{
				if (frame[0] == i)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
					WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
					WczytUstDzw();
					WczytUstGraf();
					scen[0] = scen[2] = i + 1;
					fram();
				}
			}

			if (frame[0] == 2)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				exit(0);
			}
		}

		else if ((scen[0] == 1) || (scen[0] == 6) || (scen[0] == 8) || (scen[0] == 10))
		{
			for (INT i = 0, ii = 9; i < 4, ii >= 3; i++, ii -= 2)
			{
				if (frame[1] == i)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					WczytGre = 1;
					WczytLabiryntu = 1;
					scen[0] = scen[2] = ii;
					fram();
				}
			}
		}

		else if ((scen[0] == 2) || (scen[0] == 15))
		{
			if (frame[2] == 0)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytUstDzw();
				ustawienia[2] = TRUE;
				scen[0] = scen[2] = 17;
				zapis[2] = TRUE;
				fram();
			}

			if (frame[2] == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytUstGraf();
				ustawienia[3] = TRUE;
				scen[0] = scen[2] = 16;
				zapis[3] = TRUE;
				fram();
			}

			if (frame[2] == 2)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
				WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
				WczytUstDzw();
				WczytUstGraf();
				scen[0] = scen[2] = 21;
				ustawione = 1;
				fram();
			}

			if (frame[2] == 3)
			{
				for (INT i = 1, ii = 4; i >= 0, ii <= 11; i--, ii += 7)
				{
					if (po % 2 == i)
					{
						obr_sciany = 0;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = ii;
						fram();
					}
				}
			}
		}

		else if ((scen[0] == 5) || (scen[0] == 13))
		{
			if (frame[3] == 0)
			{
				nowy[0] = 0;
				nowy[1] = 0;

				for (INT i = 0; i < 7; i++)
					SingleThink[i] = 0;
				
				for (INT i = 0; i <= IloElLab; i++)
					tab[i] = 0;

				for (INT i = 0; i < IloPunkt; i++)
					poz_scian[i] = 0;

				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 12;
				fram();
			}

			if (frame[3] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				IloscPlikow[0][0] = 0;
				IloscPlikow[0][1] = 1;
				ListaLabiryntow();
				scen[0] = scen[2] = 19;
				fram();
			}

			if (frame[3] == 2)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 6;
				fram();
			}
		}

		else if (scen[0] == 7)
		{
			if (frame[4] == 0 && WczytLabiryntu % 2 == 1)
			{
				obr_sciany = 1;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				IloscPlikow[1][0] = 0;
				IloscPlikow[1][1] = 1;
				WczytLabiryntu++;
				ListaLabiryntow();
				fram();
			}

			if (frame[4] == 1 && WczytLabiryntu % 2 == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				if (WczytGre == 1)
					scen[0] = scen[2] = 8;

				else if (WczytGre == 2)
				{
					scen[0] = scen[2] = 11;
					DystansCam = TPCam.charCamDist;
				}

				WczytLabiryntu = 1;
				fram();
			}

			if (WczytLabiryntu % 2 == 0)
			{
				if (frame[16] != IloscPlikow[1][0] && frame[16] != IloLiter)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					wygrana = FALSE;
					WczytLab("ZapisanaGra\\", pliki2[1][frame[16]]);
					po = 1;
					czekanie = state = 0;
					TempTime *= 1000;
					start = clock() + (-1 * TempTime);
					scen[0] = scen[2] = 11;
					DystansCam = TPCam.charCamDist;
					fram();
				}

				if (frame[16] == IloscPlikow[1][0])
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					WczytLabiryntu = 1;
					fram();
				}
			}
		}

		else if (scen[0] == 9)
		{
			if (frame[5] == 0)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				wygrana = FALSE;
				plansza = 1;
				WczytLab("GotowePlansze\\", "nowaplansza1");
				dl_zycia = 0.3f;
				x_dl_zycia = -1.59f;
				pozycja_startowa();
				punkty = 0;
				strzaly = 0;
				ilo_bron = 0;
				weapons[0] = FALSE;
				weapons[1] = FALSE;
				weapons[2] = FALSE;
				czekanie = state = 0;
				start = clock();
				scen[0] = scen[2] = 11;
				DystansCam = TPCam.charCamDist;
				for (INT ii = 0; ii < MaxCountArrows; ii++)
					ShowArrowOnHedge[ii] = FALSE;

				for (int j = 0; j < IloElLab; j++)
				{
					for (INT ii = 0; ii < MaxCountArrows; ii++)
						ShowArrowOnMonster[j][ii] = FALSE;
				}
				fram();
			}

			if (frame[5] == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 10;
				fram();
			}
		}

		else if (scen[0] == 11)
		{
			if (po % 2 == 1 && ZapiszGiere % 2 == 1)
				klawa(82);

			else if (po % 2 == 0 && ZapiszGiere % 2 == 1)
			{
				if (frame[9] == 0)
				{
					po++;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					fram();
				}

				else if (frame[9] == 1)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					czekanie = state = 0;
					start = clock();
					punkty = 0;
					po = ZapiszGiere = 1;
					bron = ilo_bron = strzaly = 0;
					pozycja_startowa();
					fram();
				}

				else if (frame[9] == 2)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					obr_sciany = 1;
					WczytLabiryntu = 1;
					ZapiszGiere = 1;
					scen[0] = scen[2] = 7;
					fram();
				}

				else if (frame[9] == 3)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					ZapiszGiere++;
					fram();
				}

				else if (frame[9] == 4)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
					WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
					WczytUstDzw();
					WczytUstGraf();
					scen[0] = scen[2] = 15;
					fram();
				}

				else if (frame[9] == 5)
				{
					czekanie = state = scen[0] = scen[2] = obr_sciany = 0;
					obr.x1 = obr.y1 = 0.0f;
					start = clock();
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					po = 1;
					ZapiszGiere = 1;
					fram();
				}
			}

			else if (po % 2 == 0 && ZapiszGiere % 2 == 0)
			{
				if (frame[15] != 1)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					ZapLab(NazwaZapLab);
					NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
					dl = 0;
					for (INT kla = 0; kla < KLA - 25; kla++)
						for (INT i = 0; i < 19; i++)
							klaw[kla].zapis[i] = 0;
					ZapiszGiere++;
					fram();
				}

				if (frame[15] == 1)
				{
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
					dl = 0;
					for (INT kla = 0; kla < KLA - 25; kla++)
						for (INT i = 0; i < 19; i++)
							klaw[kla].zapis[i] = 0;
					ZapiszGiere++;
					fram();
				}
			}
		}

		else if (scen[0] == 12)
		{
			if (frame[6] == 0)
			{
				nowy[0] = 1;
				nowy[1] = 0;

				for (INT i = 0; i < 7; i++)
					SingleThink[0] = 0;
					
				ZaznGuz = 0;

				for (INT i = 0; i < IloElLab; i++)
					tab[i] = 0;

				for (INT z = 0; z < IloPunkt; z++)
					poz_scian[z] = 0;

				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				fram();
				frame[6] = 10;
			}

			else if (frame[6] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				IloscPlikow[0][0] = ZaznGuz = 0;
				IloscPlikow[0][1] = 1;
				ListaLabiryntow();
				scen[0] = scen[2] = 19;
				nowy[0] = nowy[1] = 1;
				for (INT i = 0; i < IloElLab; i++)
				{
					if (tab[i] != 2)
						SingleThink[0] = 0;
					if (tab[i] != 3)
						SingleThink[1] = 0;
					if (tab[i] != 4)
						SingleThink[2] = 0;
					if (tab[i] != 5)
						SingleThink[3] = 0;
					if (tab[i] != 7)
						SingleThink[4] = 0;
					if (tab[i] != 9)
						SingleThink[5] = 0;
					if (tab[i] != 11)
						SingleThink[6] = 0;
				}
				fram();
				frame[6] = 10;
			}

			else if (frame[6] == 2)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 20;
				ZaznGuz = 0;
				nowy[0] = nowy[1] = 1;
				fram();
			}

			else if (frame[6] == 3)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 13;
				fram();
			}

			else if (nowy[0] == 1)
			{
				zazn();

				for (INT i = 0; i < 12; i++)
				{
					if (ZaznGuz == i && frejmik[i])
					{
						nowy[1] = i + 1;
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						SetCursorPos((INT)(rozdzielczosc_w / 2), (INT)(rozdzielczosc_h / 2));
						ZaznGuz = 20;
					}
				}
			}
		}

		else if (scen[0] == 14)
		{
			if (klik[0] == 1)
			{
				ustawianie_klawiszy(82);
				klik[0] = 0;
			}

			else if (klik[0] == 0)
			{
				for (INT k = 0; k < 15; k++)
				{
					if (frame[8] == k)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						zapis[0] = FALSE;
						pierwszy[k] = ustaw_klaw[k] = 1;
						ustawione = 0;
						klik[0] = 1;
						for (INT kk = 0; kk < 15; kk++)
						{
							if (k == kk)
							{
								for (INT kla = 0; kla < KLA - 25; kla++)
								{
									if (klaw[kla].ustawiony_klaw == k + 1)
									{
										klaw[kla].klawisz = 0;
										klaw[kla].ustawiony_klaw = 0;
									}
								}
							}
						}
					}
					else if (frame[8] != k)
						ustaw_klaw[k] = 0;
				}

				if (frame[8] == 15)
				{
					obr_sciany = 0;
					klik[0] = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					for (INT kla = 0; kla < KLA - 25; kla++)
					{
						klaw[kla].klawisz = 0;
						klaw[kla].ustawiony_klaw = 0;
					}

					klaw[32].ustawiony_klaw = 1;
					klaw[46].ustawiony_klaw = 2;
					klaw[45].ustawiony_klaw = 3;
					klaw[47].ustawiony_klaw = 4;
					klaw[73].ustawiony_klaw = 5;
					klaw[74].ustawiony_klaw = 6;
					klaw[75].ustawiony_klaw = 7;
					klaw[76].ustawiony_klaw = 8;
					klaw[57].ustawiony_klaw = 9;
					klaw[33].ustawiony_klaw = 10;
					klaw[31].ustawiony_klaw = 11;
					klaw[71].ustawiony_klaw = 12;
					klaw[30].ustawiony_klaw = 13;
					klaw[84].ustawiony_klaw = 14;
					klaw[85].ustawiony_klaw = 15;

					UstWszKl();

					if (wczyt[0])
						zapis[0] = FALSE;
					else
						zapis[0] = TRUE;

					fram();
				}

				if (frame[8] == 16)
				{
					if (ustaw)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ZapUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0);
						fram();
					}
					else
						scen[0] = scen[2] = 23;
				}

				if (frame[8] == 17)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (zapis[0])
					{
						scen[0] = scen[2] = 21;
						ustawienia[0] = FALSE;
					}

					if (!zapis[0])
						scen[0] = scen[2] = 18;

					fram();
				}
			}
		}

		else if (scen[0] == 16)
		{
			if (frame[10] == 0)
			{
				ListaRozdz();

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ustaw_rozdzielczosc = TRUE;
				ustaw_fullekran = FALSE;
			}

			else if (frame[10] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				ustaw_fullekran = TRUE;
				ustaw_rozdzielczosc = FALSE;
			}

			else if (frame[10] == 2 && klik[1] == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				klik[1] = 1;
				ustaw_fullekran = FALSE;
				ustaw_rozdzielczosc = FALSE;
			}

			else if (frame[10] == 3)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				ustaw_fullekran = FALSE;
				ustaw_rozdzielczosc = FALSE;

				rozdzielczosc_w = 1280.0f;
				rozdzielczosc_h = 600.0f;

				Tlx[0] = 1.1f;
				jas_sw = 1.3f;
				full = FALSE;

				if (zmiana_full)
					SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);

				if (rozdzielczosc_w != roz_w || rozdzielczosc_h != roz_h)
					Resizable();

				roz_w = rozdzielczosc_w;
				roz_h = rozdzielczosc_h;

				if (wczyt[3])
					zapis[3] = FALSE;
				else
					zapis[3] = TRUE;

				PozARozdz();

				fram();
			}

			else if (frame[10] == 4)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				ZapUstGraf();
				ustaw_fullekran = FALSE;
				ustaw_rozdzielczosc = FALSE;
				fram();
			}

			else if (frame[10] == 5)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				ustaw_fullekran = FALSE;
				ustaw_rozdzielczosc = FALSE;

				if (zapis[3])
				{
					scen[0] = scen[2] = 15;
					ustawienia[3] = FALSE;
				}

				if (!zapis[3])
					scen[0] = scen[2] = 18;

				fram();
			}


			if (ustaw_fullekran)
			{
				if (frame[12] == 1)
				{
					if (!full)
					{
						zapis[3] = FALSE;
						full = TRUE;
						zmiana_full = TRUE;
					}

					if (zmiana_full && full)
						SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);

					zmiana_full = FALSE;
				}

				if (frame[12] == 2)
				{
					if (full)
					{
						zapis[3] = FALSE;
						full = FALSE;
						zmiana_full = TRUE;
					}

					if (zmiana_full && !full)
						SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);

					zmiana_full = FALSE;
				}
			}

			if (ustaw_rozdzielczosc)
			{
				if (frame[13] == 0 && rozdz[lr[0]].ind == 1)
				{
					rozdzielczosc_w = (FLOAT)rozdz[lr[0]].width;
					rozdzielczosc_h = (FLOAT)rozdz[lr[0]].height;
					PozARozdz();

					if (roz_w != rozdzielczosc_w || roz_h != rozdzielczosc_h)
					{
						Resizable();

						roz_w = rozdzielczosc_w;
						roz_h = rozdzielczosc_h;

						zapis[3] = FALSE;
					}
				}

				if (frame[13] == 1 && rozdz[lr[1]].ind == 1)
				{
					rozdzielczosc_w = (FLOAT)rozdz[lr[1]].width;
					rozdzielczosc_h = (FLOAT)rozdz[lr[1]].height;
					PozARozdz();

					if (roz_w != rozdzielczosc_w || roz_h != rozdzielczosc_h)
					{
						Resizable();

						roz_w = rozdzielczosc_w;
						roz_h = rozdzielczosc_h;

						zapis[3] = FALSE;
					}
				}
			}
		}

		else if (scen[0] == 17)
		{
			if (frame[11] == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				klik[2] = 1;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = FALSE;
			}

			if (frame[11] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				klik[3] = 1;

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = FALSE;
			}

			if (frame[11] == 2)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				Tlx[1] = Tlx[2] = 1.1f;

				glosnosc_tla = glosnosc_efektow = -5000;

				muza.muzyka_tlo_volume(glosnosc_tla);

				if (wczyt[2])
					zapis[2] = FALSE;
				else
					zapis[2] = TRUE;

				fram();

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = FALSE;
			}

			if (frame[11] == 3)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ZapUstDzw();

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				fram();
			}

			if (frame[11] == 4)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				for (INT i = 0; i < 4; i++)
					klik[i] = 0;

				if (zapis[2])
				{
					scen[0] = scen[2] = 15;
					ustawienia[2] = FALSE;
				}

				if (!zapis[2])
					scen[0] = scen[2] = 18;

				fram();

				for (INT fr = 0; fr < 1400; fr++)
					frejmik[fr] = FALSE;
			}
		}

		else if (scen[0] == 18)
		{
			if (frame[14] == 0)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				if (ustawienia[0])
				{
					if (!zapis[0] && wczyt[0])
						WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);

					if (!wczyt[0])
					{
						for (INT kla = 0; kla < KLA - 25; kla++)
						{
							klaw[kla].klawisz = 0;
							klaw[kla].ustawiony_klaw = 0;
						}

						klaw[32].ustawiony_klaw = 1;
						klaw[46].ustawiony_klaw = 2;
						klaw[45].ustawiony_klaw = 3;
						klaw[47].ustawiony_klaw = 4;
						klaw[73].ustawiony_klaw = 5;
						klaw[74].ustawiony_klaw = 6;
						klaw[75].ustawiony_klaw = 7;
						klaw[76].ustawiony_klaw = 8;
						klaw[57].ustawiony_klaw = 9;
						klaw[33].ustawiony_klaw = 10;
						klaw[31].ustawiony_klaw = 11;
						klaw[71].ustawiony_klaw = 12;
						klaw[30].ustawiony_klaw = 13;
						klaw[84].ustawiony_klaw = 14;
						klaw[85].ustawiony_klaw = 15;
					}

					UstWszKl();

					scen[0] = scen[2] = 21;
				}

				if (ustawienia[2])
				{
					if (!zapis[2] && wczyt[2])
					{
						WczytUstDzw();
						muza.muzyka_tlo_volume(glosnosc_tla);
					}

					if (!wczyt[2])
					{
						Tlx[1] = Tlx[2] = 1.1f;
						glosnosc_tla = glosnosc_efektow = -5000;
						muza.muzyka_tlo_volume(glosnosc_tla);
					}

					scen[0] = scen[2] = 15;
				}

				if (ustawienia[3])
				{
					if (!zapis[3] && wczyt[3])
					{
						WczytUstGraf();
						SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);
						Resizable();
					}

					if (!zapis[3] && !wczyt[3])
					{
						rozdzielczosc_w = 1280.0f;
						rozdzielczosc_h = 600.0f;
						full = FALSE;
						Tlx[0] = 1.1f;
						jas_sw = 1.3f;
						SetFullscreen((UINT)rozdzielczosc_w, (UINT)rozdzielczosc_h, full);
						Resizable();
					}

					scen[0] = scen[2] = 15;
				}

				if (ustawienia[1])
				{
					if (!zapis[1] && wczyt[1])
						WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);

					if (!wczyt[1])
					{
						for (INT kla = 86; kla < KLA; kla++)
						{
							klaw[kla].klawisz = 0;
							klaw[kla].ustawiony_klaw = 0;
						}

						klaw[86].ustawiony_klaw = 16;
						klaw[87].ustawiony_klaw = 17;
						klaw[88].ustawiony_klaw = 18;
						klaw[89].ustawiony_klaw = 19;
						klaw[91].ustawiony_klaw = 20;
						klaw[92].ustawiony_klaw = 21;
						klaw[93].ustawiony_klaw = 22;
						klaw[94].ustawiony_klaw = 23;
						klaw[108].ustawiony_klaw = 24;
						klaw[100].ustawiony_klaw = 25;
						klaw[101].ustawiony_klaw = 26;
						klaw[109].ustawiony_klaw = 27;
						klaw[107].ustawiony_klaw = 28;
						klaw[96].ustawiony_klaw = 29;
						klaw[97].ustawiony_klaw = 30;
					}

					UstWszKl();

					scen[0] = scen[2] = 21;
				}

				roz_w = rozdzielczosc_w;
				roz_h = rozdzielczosc_h;
				zmiana_full = FALSE;
				fram();
			}

			if (frame[14] == 1)
			{
				obr_sciany = 0;
				muza.dzwiek_potwierdzenie(glosnosc_efektow);

				if (ustawienia[2])
					scen[0] = scen[2] = 17;

				if (ustawienia[3])
					scen[0] = scen[2] = 16;

				if (ustawienia[0])
					scen[0] = scen[2] = 14;

				if (ustawienia[1])
					scen[0] = scen[2] = 22;

				fram();
			}
		}

		else if (scen[0] == 19)
		{
			if (frame[16] != IloscPlikow[0][0] && frame[16] != IloLiter)
			{
				wygrana = FALSE;
				plansza = 5;
				WczytLab("ZapisaneLabirynty\\", pliki2[0][frame[16]]);
				dl_zycia = 0.3f;
				x_dl_zycia = -1.59f;
				strzaly = 0;
				punkty = 0;

				for (INT i = 0, ii = 12; i < 2, ii >= 11; i++, ii--)
				{
					if (wczytyw == i)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						nowy[0] = nowy[1] = 1;
						for (INT ik = 0; ik < IloElLab; ik++)
						{
							if (tab[i] != 2)
								SingleThink[0] = 0;
							if (tab[i] != 3)
								SingleThink[1] = 0;
							if (tab[i] != 4)
								SingleThink[2] = 0;
							if (tab[i] != 5)
								SingleThink[3] = 0;
							if (tab[i] != 7)
								SingleThink[4] = 0;
							if (tab[i] != 9)
								SingleThink[5] = 0;
							if (tab[i] != 11)
								SingleThink[6] = 0;
						}
						scen[0] = scen[2] = ii;
						fram();
					}
				}
			}

			if (frame[16] == IloscPlikow[0][0])
			{
				for (INT i = 0; i < 2; i++)
				{
					if (wczytyw == i)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						scen[0] = scen[2] = i + 12;
						fram();
					}
				}
			}
		}

		else if (scen[0] == 20 && ZapiszGiere % 2 == 1)
		{
			if (frame[15] == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				ZapLab(NazwaZapLab);
				NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
				dl = 0;
				for (INT kla = 0; kla < KLA - 25; kla++)
					for (INT i = 0; i < 19; i++)
						klaw[kla].zapis[i] = 0;
				scen[0] = scen[2] = 12;
				fram();
			}

			if (frame[15] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				NazwaZapLab = NazwaZapLab.substr(0, NazwaZapLab.length() - dl);
				dl = 0;
				for (INT kla = 0; kla < KLA; kla++)
					for (INT i = 0; i < 19; i++)
						klaw[kla].zapis[i] = 0;
				scen[0] = scen[2] = 12;
				fram();
			}
		}

		else if (scen[0] == 21)
		{
			if (frame[17] == 0)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
				if (!wczyt[0])
				{
					for (INT kla = 0; kla < KLA - 25; kla++)
					{
						klaw[kla].klawisz = 0;
						klaw[kla].ustawiony_klaw = 0;
					}

					klaw[32].ustawiony_klaw = 1;
					klaw[46].ustawiony_klaw = 2;
					klaw[45].ustawiony_klaw = 3;
					klaw[47].ustawiony_klaw = 4;
					klaw[73].ustawiony_klaw = 5;
					klaw[74].ustawiony_klaw = 6;
					klaw[75].ustawiony_klaw = 7;
					klaw[76].ustawiony_klaw = 8;
					klaw[57].ustawiony_klaw = 9;
					klaw[33].ustawiony_klaw = 10;
					klaw[31].ustawiony_klaw = 11;
					klaw[71].ustawiony_klaw = 12;
					klaw[30].ustawiony_klaw = 13;
					klaw[84].ustawiony_klaw = 14;
					klaw[85].ustawiony_klaw = 15;
				}
				scen[0] = scen[2] = 14;
				fram();
			}

			if (frame[17] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
				if (!wczyt[1])
				{
					for (INT kla = 86; kla < KLA; kla++)
					{
						klaw[kla].klawisz = 0;
						klaw[kla].ustawiony_klaw = 0;
					}

					klaw[86].ustawiony_klaw = 16;
					klaw[87].ustawiony_klaw = 17;
					klaw[88].ustawiony_klaw = 18;
					klaw[89].ustawiony_klaw = 19;
					klaw[91].ustawiony_klaw = 20;
					klaw[92].ustawiony_klaw = 21;
					klaw[93].ustawiony_klaw = 22;
					klaw[94].ustawiony_klaw = 23;
					klaw[108].ustawiony_klaw = 24;
					klaw[100].ustawiony_klaw = 25;
					klaw[101].ustawiony_klaw = 26;
					klaw[109].ustawiony_klaw = 27;
					klaw[107].ustawiony_klaw = 28;
					klaw[96].ustawiony_klaw = 29;
					klaw[97].ustawiony_klaw = 30;
				}
				scen[0] = scen[2] = 22;
				fram();
			}

			if (frame[17] == 2 || frame[17] == 3)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 15;
				fram();
			}
		}

		else if (scen[0] == 22)
		{
			if (klik[4] == 0)
			{
				for (INT k = 0; k < 15; k++)
				{
					if (frame[18] == k)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						zapis[1] = FALSE;
						pierwszy[k + 15] = ustaw_klaw[k + 15] = 1;
						ustawione = 0;
						klik[4] = 1;
						for (INT kk = 0; kk < 15; kk++)
						{
							if (k == kk)
							{
								for (INT kla = 86; kla < KLA; kla++)
								{
									if (klaw[kla].ustawiony_klaw == k + 16)
									{
										klaw[kla].klawisz = 0;
										klaw[kla].ustawiony_klaw = 0;
									}
								}
							}
						}
					}

					else if (frame[18] != k)
						ustaw_klaw[k + 15] = 0;
				}

				if (frame[18] == 15)
				{
					obr_sciany = klik[4] = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);
					for (INT kla = 86; kla < KLA; kla++)
					{
						klaw[kla].klawisz = 0;
						klaw[kla].ustawiony_klaw = 0;
					}

					klaw[86].ustawiony_klaw = 16;
					klaw[87].ustawiony_klaw = 17;
					klaw[88].ustawiony_klaw = 18;
					klaw[89].ustawiony_klaw = 19;
					klaw[91].ustawiony_klaw = 20;
					klaw[92].ustawiony_klaw = 21;
					klaw[93].ustawiony_klaw = 22;
					klaw[94].ustawiony_klaw = 23;
					klaw[108].ustawiony_klaw = 24;
					klaw[100].ustawiony_klaw = 25;
					klaw[101].ustawiony_klaw = 26;
					klaw[109].ustawiony_klaw = 27;
					klaw[107].ustawiony_klaw = 28;
					klaw[96].ustawiony_klaw = 29;
					klaw[97].ustawiony_klaw = 30;

					UstWszKl();

					if (wczyt[1])
						zapis[1] = FALSE;
					else
						zapis[1] = TRUE;

					fram();
				}

				if (frame[18] == 16)
				{
					if (ustaw)
					{
						muza.dzwiek_potwierdzenie(glosnosc_efektow);
						ZapUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 1);
						fram();
					}
					else
						scen[0] = scen[2] = 23;
				}

				if (frame[18] == 17)
				{
					obr_sciany = 0;
					muza.dzwiek_potwierdzenie(glosnosc_efektow);

					if (zapis[1])
					{
						scen[0] = scen[2] = 21;
						ustawienia[1] = FALSE;
					}

					if (!zapis[1])
						scen[0] = scen[2] = 18;

					fram();
				}
			}
		}

		else if (scen[0] == 23 && frami)
		{
			if (scen[1] == 1)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 14;
			}

			else if (scen[1] == 4)
			{
				muza.dzwiek_potwierdzenie(glosnosc_efektow);
				scen[0] = scen[2] = 22;
			}
		}
	}
	else if (!BUTTONDOWN(mouse_state, 0) && !MouseButton[0]) MouseButton[0] = TRUE;

	if (BUTTONDOWN(mouse_state, 1) && MouseButton[1])
	{
		MouseButton[1] = TRUE;

		if (scen[0] == 11)
			klawa(83);

		else if (scen[0] == 12)
			odzn();

		if (scen[0] == 14 && klik[0] == 1)
		{
			ustawianie_klawiszy(83);
			klik[0] = 0;
		}
	}
	else if (!BUTTONDOWN(mouse_state, 1) && !MouseButton[1]) MouseButton[1] = TRUE;
}

void Renderer::suwak(Vect Tb, Vect Tl, XMMATRIX &mProj, XMMATRIX &mView, FLOAT ScSuw)
{
	Objekt(md3dImmediateContext, jas_sw, jas_diff, mProj, mView, { 0.1f + ScSuw, 0.2f, 0.08f + ScSuw }, Tl, Objects::suwak, mDirLights, false);
	Objekt(md3dImmediateContext, jas_sw, jas_diff, mProj, mView, { 1.0f + ScSuw, 0.2f, 0.15f + ScSuw }, Tb, linia_suwak, mDirLights, true);
}

void Renderer::BeginD3D()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const FLOAT*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::EndD3D()
{
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);

	ID3D11ShaderResourceView* nullSRV[16] = { 0 };
	md3dImmediateContext->PSSetShaderResources(0, 16, nullSRV);

	HR(mSwapChain->Present(0, 0));
}

void Renderer::EndzikD3D(XMMATRIX &proj, XMMATRIX &view)
{
	mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);
	napis.KlawMenu(frame, CzerwonyNapis);
	muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
	EndD3D();
}

void Renderer::DrawScene(FLOAT dt)
{
	if (scen[0] == 0)
	{
		WczytUstSter("zapisane_ustawienia_klawiatury.txt", 0, KLA - 25, 0, 16, 0);
		WczytUstSter("zapisane_ustawienia_gamepada.txt", 86, KLA, 16, 31, 1);
		WczytUstDzw();
		WczytUstGraf();

		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		weapons[1] = weapons[2] = weapons[0] = false;
		obr.x1 = 0;

		for (INT i = 0; i < 3; i++)
		{
			nap[0][i].x1 = nap[0][i].x2;
			nap[0][i].y1 = nap[0][i].y2;
		}

		md3dImmediateContext->RSSetState(0);
		md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
		mSsao->SetNormalDepthRenderTarget(mDepthStencilView);
		DrawSsao(md3dImmediateContext, proj, view, TPCam, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		mSsao->ComputeSsao(TPCam);

		ID3D11RenderTargetView* renderTargets[1] = { mRenderTargetView };
		md3dImmediateContext->OMSetRenderTargets(1, renderTargets, mDepthStencilView);
		md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
		md3dImmediateContext->OMSetDepthStencilState(RenderStates::EqualsDSS, 0);

		Effects::BasicFX->SetDirLights(mDirLights);
		Effects::BasicFX->SetEyePosW(TPCam.CamPosition);
		Effects::BasicFX->SetCubeMap(mSky->CubeMapSRV());
		Effects::BasicFX->SetSsaoMap(mSsao->AmbientSRV());

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 1)
	{
		if (obr_sciany == 0)
		{
			obr.x1 = 0;

			for (INT i = 0; i < 3; i++)
			{
				nap[0][i].x1 = nap[0][i].x2;
				nap[0][i].y1 = nap[0][i].y2;
			}

			for (INT i = 0; i < 5; i++)
			{
				nap[1][i].x1 = 5.0f;
				nap[1][i].y1 = nap[1][i].y2;
			}

			while (obr.x1 <= 1.40625 && nap[0][0].x1 >= -5.0f && nap[0][1].x1 >= -5.0f && nap[0][2].x1 >= -5.0f && nap[1][0].x1 >= nap[1][0].x2 && nap[1][1].x1 >= nap[1][1].x2 && nap[1][2].x1 >= nap[1][2].x2 && nap[1][3].x1 >= nap[1][3].x2 && nap[1][4].x1 >= nap[1][4].x2)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				obr.x1 += 0.015625;

				nap[0][0].x1 -= 0.050f;
				nap[0][1].x1 -= 0.052f;
				nap[0][2].x1 -= 0.050f;

				nap[1][0].x1 -= 0.058f;
				nap[1][1].x1 -= 0.060f;
				nap[1][2].x1 -= 0.061f;
				nap[1][3].x1 -= 0.065f;
				nap[1][4].x1 -= 0.063f;

				EndzikD3D(proj, view);
			}
			obr_sciany = 1;
		}

		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		for (INT i = 0; i < 5; i++)
			nap[1][i].x1 = nap[1][i].x2;

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 2)
	{
		if (obr_sciany == 0)
		{
			obr.x1 = 0;

			for (INT i = 0; i < 3; i++)
			{
				nap[0][i].x1 = nap[0][i].x2;
				nap[0][i].y1 = nap[0][i].y2;
			}
			
			for (INT i = 0; i < 5; i++)
			{
				nap[2][i].x1 = -5.0f;
				nap[2][i].y1 = nap[2][i].y2;
			}

			while (obr.x1 >= -1.40625 && nap[0][0].x1 <= 5.0f && nap[0][1].x1 <= 5.0f && nap[0][2].x1 <= 5.0f  && nap[2][0].x1 <= nap[2][0].x2 && nap[2][1].x1 <= nap[2][1].x2 && nap[2][2].x1 <= nap[2][2].x2 && nap[2][3].x1 <= nap[2][3].x2 && nap[2][4].x1 <= nap[2][4].x2)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.x1 -= 0.015625;

				nap[2][0].x1 += 0.046f;
				nap[2][1].x1 += 0.052f;
				nap[2][2].x1 += 0.046f;
				nap[2][3].x1 += 0.045f;
				nap[2][4].x1 += 0.050f;

				nap[0][0].x1 += 0.060f;
				nap[0][1].x1 += 0.059f;
				nap[0][2].x1 += 0.060f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		for (INT i = 0; i < 5; i++)
			nap[2][i].x1 = nap[2][i].x2;

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 3)
	{
		if (obr_sciany == 0)
		{
			obr.x1 = 1.40625;

			for (INT i = 0; i < 3; i++)
			{
				nap[0][i].x1 = -5.0f;
				nap[0][i].y1 = nap[0][i].y2;
			}

			for (INT i = 0; i < 5; i++)
			{
				nap[1][i].x1 = nap[1][i].x2;
				nap[1][i].y1 = nap[1][i].y2;
			}

			while (obr.x1 >= 0 && nap[0][0].x1 <= nap[0][0].x2 && nap[0][1].x1 <= nap[0][1].x2 && nap[0][2].x1 <= nap[0][2].x2 && nap[1][0].x1 <= 5.0f && nap[1][1].x1 <= 5.0f && nap[1][2].x1 <= 5.0f && nap[1][3].x1 <= 5.0f && nap[1][4].x1 <= 5.0f)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.x1 -= 0.015625;

				nap[0][0].x1 += 0.050f;
				nap[0][1].x1 += 0.052f;
				nap[0][2].x1 += 0.050f;

				nap[1][0].x1 += 0.051f;
				nap[1][1].x1 += 0.054f;
				nap[1][2].x1 += 0.056f;
				nap[1][3].x1 += 0.059f;
				nap[1][4].x1 += 0.063f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		for (INT i = 0; i < 3; i++)
			nap[0][i].x1 = nap[0][i].x2;

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 4)
	{
		if (obr_sciany == 0)
		{
			obr.x1 = -1.40625;

			for (INT i = 0; i < 3; i++)
			{
				nap[0][i].x1 = 5.0f;
				nap[0][i].y1 = nap[0][i].y2;
			}
			
			for (INT i = 0; i < 5; i++)
			{
				nap[2][0].x1 = nap[2][0].x2;
				nap[2][4].y1 = nap[2][4].y2;
			}

			while (obr.x1 <= 0 && nap[0][0].x1 >= nap[0][0].x2 && nap[0][1].x1 >= nap[0][1].x2 && nap[0][2].x1 >= nap[0][2].x2 && nap[2][0].x1 >= -5.0f && nap[2][1].x1 >= -5.0f && nap[2][2].x1 >= -5.0f && nap[2][3].x1 >= -5.0f && nap[2][4].x1 >= -5.0f)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.x1 += 0.015625;

				nap[0][0].x1 -= 0.055f;
				nap[0][1].x1 -= 0.052f;
				nap[0][2].x1 -= 0.050f;

				nap[2][0].x1 -= 0.046f;
				nap[2][1].x1 -= 0.052f;
				nap[2][2].x1 -= 0.046f;
				nap[2][3].x1 -= 0.045f;
				nap[2][4].x1 -= 0.050f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		for (INT i = 0; i < 3; i++)
			nap[0][i].x1 = nap[0][i].x2;

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 5)
	{
		wczytyw = 1;
		if (obr_sciany == 0)
		{
			obr.y1 = 0;

			for (INT i = 0; i < 5; i++)
			{
				nap[1][i].x1 = nap[1][i].x2;
				nap[1][i].y1 = nap[1][i].y2;
			}
			
			for (INT i = 0; i < 4; i++)
			{
				nap[3][i].y1 = 5.0f;
				nap[3][i].x1 = nap[3][i].x2;
			}

			while (obr.y1 <= 1.40625 && nap[3][0].y1 >= nap[3][0].y2 && nap[3][1].y1 >= nap[3][1].y2 && nap[3][2].y1 >= nap[3][2].y2 && nap[3][3].y1 >= nap[3][3].y2 && nap[1][0].y1 >= -5.0f && nap[1][1].y1 >= -5.0f && nap[1][2].y1 >= -5.0f && nap[1][3].y1 >= -5.0f && nap[1][4].y1 >= -5.0f)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.y1 += 0.015625;

				nap[3][0].y1 -= 0.051f;
				nap[3][1].y1 -= 0.055f;
				nap[3][2].y1 -= 0.058f;
				nap[3][3].y1 -= 0.063f;

				nap[1][0].y1 -= 0.060f;
				nap[1][1].y1 -= 0.057f;
				nap[1][2].y1 -= 0.055f;
				nap[1][3].y1 -= 0.052f;
				nap[1][4].y1 -= 0.048f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		nowy[0] = 0;

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		for (INT i = 0; i < 4; i++)
			nap[3][i].y1 = nap[3][i].y2;


		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 6)
	{
		if (obr_sciany == 0)
		{
			obr.y1 = 1.40625;

			for (INT i = 0; i < 5; i++)
			{
				nap[1][i].x1 = nap[1][i].x2;
				nap[1][i].y1 = -5.0f;
			}

			for (INT i = 0; i < 4; i++)
			{
				nap[3][i].x1 = nap[3][i].x2;
				nap[3][i].y1 = nap[3][i].y2;
			}

			while (obr.y1 >= 0 && nap[1][0].y1 <= nap[1][0].y2 && nap[1][1].y1 <= nap[1][1].y2 && nap[1][2].y1 <= nap[1][2].y2 && nap[1][3].y1 <= nap[1][3].y2 && nap[1][4].y1 <= nap[1][4].y2 && nap[3][0].y1 <= 5.0f && nap[3][1].y1 <= 5.0f && nap[3][2].y1 <= 5.0f && nap[3][3].y1 <= 5.0f)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.y1 -= 0.015625;

				nap[1][0].y1 += 0.060f;
				nap[1][1].y1 += 0.057f;
				nap[1][2].y1 += 0.055f;
				nap[1][3].y1 += 0.052f;
				nap[1][4].y1 += 0.048f;

				nap[3][0].y1 += 0.051f;
				nap[3][1].y1 += 0.055f;
				nap[3][2].y1 += 0.058f;
				nap[3][3].y1 += 0.063f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		for (INT i = 0; i < 5; i++)
			nap[1][i].y1 = nap[1][i].y2;

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 7)
	{
		if (obr_sciany == 0)
		{
			obr.y1 = 0;

			for (INT i = 0; i < 5; i++)
			{
				nap[1][i].x1 = nap[1][i].x2;
				nap[1][i].y1 = nap[1][i].y2;
			}

			for (INT i = 0; i < 3; i++)
			{
				nap[4][i].x1 = nap[4][i].x2;
				nap[4][i].y1 = 5.0f;
			}

			while (obr.y1 >= -1.40625 && nap[1][0].y1 >= -5.0f && nap[1][1].y1 >= -5.0f && nap[1][2].y1 >= -5.0f && nap[1][3].y1 >= -5.0f && nap[1][4].y1 >= -5.0f && nap[4][0].y1 >= nap[4][0].y2 && nap[4][1].y1 >= nap[4][1].y2 && nap[4][2].y1 >= nap[4][2].y2)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.y1 -= 0.015625;

				nap[4][0].y1 -= 0.051f;
				nap[4][1].y1 -= 0.056f;
				nap[4][2].y1 -= 0.063f;

				nap[1][0].y1 -= 0.060f;
				nap[1][1].y1 -= 0.057f;
				nap[1][2].y1 -= 0.055f;
				nap[1][3].y1 -= 0.052f;
				nap[1][4].y1 -= 0.048f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		for (INT i = 0; i < 3; i++)
			nap[4][i].y1 = nap[4][i].y2;

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		if(WczytLabiryntu % 2 == 0)
		{
			for (INT framik = 0; framik <= IloscPlikow[1][0]; framik++)
				if (frame[16] == framik)
					CzerwonyPliki[1][framik] = 1;
				else if (frame[16] != framik)
					CzerwonyPliki[1][framik] = 0;

			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 1.2f, 0.2f, 1.4f }, { 0.0f, -1.97f, 0.0f }, ogien, mDirLights, false);
			muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
		}
		
		EndzikD3D(proj, view);
	}

	if (scen[0] == 8)
	{
		if (obr_sciany == 0)
		{
			obr.y1 = -1.40625;

			for (INT i = 0; i < 5; i++)
			{
				nap[1][i].x1 = nap[1][i].x2;
				nap[1][i].y1 = 5.0f;
			}

			for (INT i = 0; i < 3; i++)
			{
				nap[4][i].x1 = nap[4][i].x2;
				nap[4][i].y1 = nap[4][i].y2;
			}

			while (obr.y1 <= 0 && nap[1][0].y1 >= nap[1][0].y2 && nap[1][1].y1 >= nap[1][1].y2 && nap[1][2].y1 >= nap[1][2].y2 && nap[1][3].y1 >= nap[1][3].y2 && nap[1][4].y1 >= nap[1][4].y2 && nap[4][0].y1 >= -5.0f && nap[4][1].y1 >= -5.0f && nap[4][2].y1 >= -5.0f)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.y1 += 0.015625;

				nap[1][0].y1 -= 0.051f;
				nap[1][1].y1 -= 0.054f;
				nap[1][2].y1 -= 0.056f;
				nap[1][3].y1 -= 0.059f;
				nap[1][4].y1 -= 0.063f;

				nap[4][0].y1 -= 0.060f;
				nap[4][1].y1 -= 0.055f;
				nap[4][2].y1 -= 0.048f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		for (INT i = 0; i < 5; i++)
			nap[1][i].y1 = nap[1][i].y2;

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 9)
	{
		if (obr_sciany == 0)
		{
			obr.x1 = 1.40625;

			for (INT i = 0; i < 5; i++)
			{
				nap[1][i].x1 = nap[1][i].x2;
				nap[1][i].y1 = nap[1][i].y2;
			}

			for (INT i = 0; i < 3; i++)
			{
				nap[5][i].x1 = 5.0f;
				nap[5][i].y1 = nap[5][i].y2;
			}

			while (obr.x1 <= 2.8125 && nap[5][0].x1 >= nap[5][0].x2 && nap[5][1].x1 >= nap[5][1].x2 && nap[5][2].x1 >= nap[5][2].x2 && nap[1][0].x1 >= -5.0f && nap[1][1].x1 >= -5.0f && nap[1][2].x1 >= -5.0f && nap[1][3].x1 >= -5.0f && nap[1][4].x1 >= -5.0f)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.x1 += 0.015625;

				nap[5][0].x1 -= 0.073f;
				nap[5][1].x1 -= 0.060f;
				nap[5][2].x1 -= 0.063f;

				nap[1][0].x1 -= 0.053f;
				nap[1][1].x1 -= 0.051f;
				nap[1][2].x1 -= 0.050f;
				nap[1][3].x1 -= 0.046f;
				nap[1][4].x1 -= 0.048f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		for (INT i = 0; i < 3; i++)
			nap[5][i].x1 = nap[5][i].x2;

		EndzikD3D(proj, view);
	}

	if (scen[0] == 10)
	{
		if (obr_sciany == 0)
		{
			obr.x1 = 2.8125;

			for (INT i = 0; i < 5; i++)
			{
				nap[1][i].x1 = -5.0f;
				nap[1][i].y1 = nap[1][i].y2;
			}

			for (INT i = 0; i < 3; i++)
			{
				nap[5][i].x1 = nap[5][i].x2;
				nap[5][i].y1 = nap[5][i].y2;
			}

			while (obr.x1 >= 1.40625 && nap[1][0].x1 <= nap[1][0].x2 && nap[1][1].x1 <= nap[1][1].x2 && nap[1][2].x1 <= nap[1][2].x2 && nap[1][3].x1 <= nap[1][3].x2 && nap[1][4].x1 <= nap[1][4].x2 && nap[5][0].x1 <= 5.0f && nap[5][1].x1 <= 5.0f && nap[5][2].x1 <= 5.0f)
			{
				BeginD3D();

				XMMATRIX view = XMLoadFloat4x4(&mView);
				XMMATRIX proj = XMLoadFloat4x4(&mProj);

				Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

				mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

				obr.x1 -= 0.015625;

				nap[1][0].x1 += 0.053f;
				nap[1][1].x1 += 0.051f;
				nap[1][2].x1 += 0.050f;
				nap[1][3].x1 += 0.046f;
				nap[1][4].x1 += 0.048f;

				nap[5][0].x1 += 0.073f;
				nap[5][1].x1 += 0.060f;
				nap[5][2].x1 += 0.063f;

				EndD3D();
			}
			obr_sciany = 1;
		}
		BeginD3D();

		XMMATRIX view = XMLoadFloat4x4(&mView);
		XMMATRIX proj = XMLoadFloat4x4(&mProj);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		for (INT i = 0; i < 5; i++)
			nap[1][i].x1 = nap[1][i].x2;

		EndzikD3D(proj, view);
	}

	if (scen[0] == 11)
	{
		muza.m_secondaryBuffer3->Stop();

		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		if (bron == 0 || (bron == 1 && !weapons[0]) || (bron == 2 && !weapons[1]) || (bron == 3 && !weapons[2]))
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.2f, 0.2f, 0.2f }, { xBroniki, -2.0f, 0.7f }, texture[0], mDirLights, false);

		else if (bron == 1 && weapons[0])
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.2f, 0.2f, 0.2f }, { xBroniki, -2.0f, 0.7f }, texture[1], mDirLights, false);

		else if (bron == 2 && weapons[1])
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.2f, 0.2f, 0.2f }, { xBroniki, -2.0f, 0.7f }, texture[2], mDirLights, false);

		else if (bron == 3 && weapons[2])
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.2f, 0.2f, 0.2f }, { xBroniki, -2.0f, 0.7f }, texture[3], mDirLights, false);

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { dl_zycia, 0.2f, 0.01f }, { x_dl_zycia, -2.0f, 0.77f }, ogien, mDirLights, false);

		if (!wygrana && po % 2 == 1 && Anim[0] != 14)
		{
			KlawiszologiaKamery(dt);
			KlawiszologiaPostaci(dt, TPCam);
		}

		Kolizje(dt);
		pierw = 0;

		stringstream spunkty, sstrzaly;
		spunkty << punkty; sstrzaly << strzaly;
		punkty_string2 = spunkty.str(); strzaly_string2 = sstrzaly.str();

		czas();

		INT sekundys[3] = { sekundy / 3600, (sekundy / 60) % 60, sekundy % 60 };
		stringstream ssekundy[3];
		for (int i = 0; i < 3; i++)
		{
			ssekundy[i] << sekundys[i];
			sekundy_s[i] = ssekundy[i].str();
		}

		for (INT i = 0; i < IloElLab; i++)
		{
			if (po % 2 == 1 && ZapiszGiere % 2 == 1)
			{
				for (int mons = 0; mons < 4; mons++)
				{
					if (tab[i] == mons + 8)
						AkcjaPost(i, Potwor[mons]);
				}
			}
		}

		if (po % 2 == 1 && ZapiszGiere % 2 == 1)
			SetCursorPos((INT)(rozdzielczosc_w / 2), (INT)(rozdzielczosc_h / 2));

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		Skydome->Draw(md3dImmediateContext, TPCam.CamProjection, TPCam.CamView, 0, 0, 10000.0f, XMVectorGetX(TPCam.CamPosition), XMVectorGetY(TPCam.CamPosition), XMVectorGetZ(TPCam.CamPosition));

		if (Postac.Ins[14].TimePos >= Postac.Ins[14].Model->SkinnedData.GetClipEndTime(Postac.Ins[14].ClipName) && Anim[0] == 14 && po % 2 == 1)
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 1.0f, 0.2f, 0.6f }, { 0.0f, -2.0f, 0.0f }, ogien, mDirLights, false);

		if (wygrana && po % 2 == 1)
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 1.8f, 0.2f, 0.7f }, { 0.0f, -2.0f, 0.0f }, ogien, mDirLights, false);

		if (po % 2 == 0 && ZapiszGiere % 2 == 1)
		{
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.5f, 0.2f, 0.7f }, { 0.0f, -2.0f, 0.0f }, ogien, mDirLights, false);
			napis.KlawMenu(frame, CzerwonyNapis);
			muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
		}

		else if (po % 2 == 0 && ZapiszGiere % 2 == 0)
		{
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 2.0f, 0.2f, 0.8f }, { 0.0f, -1.99f, -0.1f }, ogien, mDirLights, false);

			napis.KlawMenu(frame, CzerwonyNapis);
			muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
		}

		if (Anim[0] == 0)
			TPCam.SpeedCamera = 0.0f;

		if (klawisze[12] && bron == 1)
		{
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.03f, 0.2f, 0.03f }, { 0.05f, -2.001f, 0.4f }, Celownik, mDirLights, true);
			TPCam.WalkForward(dt);
		}
		
		KolKamZZyw();
		TPCam.Update(TPCam.charCamDist);

		EndD3D();
	}

	if (scen[0] == 12)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		wczytyw = 0;
		
		ObjektyScen12(proj, view, ObjektyScen12X, ScaleGuzik);

		napis.KlawMenu(frame, CzerwonyNapis);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);

		EndD3D();
	}

	if (scen[0] == 13)
	{
		nowy[0] = 0;
		nowy[1] = 0;
		for(INT i = 0; i < 5; i++)
			SingleThink[i] = 0;
		
		wczytyw = 1;

		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		for (INT i = 0; i < 4; i++)
		{
			nap[3][i].x1 = nap[3][i].x2;
			nap[3][i].y1 = nap[3][i].y2;
		}

		EndzikD3D(proj, view);
	}

	if (scen[0] == 14)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		scen[1] = 1;
		ustawienia[0] = TRUE;
		ustawienia[2] = FALSE;
		ustawienia[3] = FALSE;
		ustawienia[1] = FALSE;
			
		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 4.0f, 0.2f, 4.0f }, { 0.0f, -2.0f, 0.0f }, ziemia, mDirLights, false);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
		napis.KlawMenu(frame, CzerwonyNapis);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndD3D();
	}

	if (scen[0] == 15)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		for (INT i = 0; i < 5; i++)
		{
			nap[2][i].x1 = nap[2][i].x2;
			nap[2][i].y1 = nap[2][i].y2;
		}

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndzikD3D(proj, view);
	}

	if (scen[0] == 16)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		ustawienia[0] = FALSE;
		ustawienia[2] = FALSE;
		ustawienia[3] = TRUE;
		ustawienia[1] = FALSE;

		scen[1] = 2;

		stringstream OpcjeRozdzielczosc1, OpcjeRozdzielczosc2, OpcjeRozdzielczosc3, OpcjeRozdzielczosc4;
		OpcjeRozdzielczosc1 << rozdz[lr[0]].width;
		OpcjeRozdzielczosc2 << rozdz[lr[0]].height;
		OpcjeRozdzielczosc3 << rozdz[lr[1]].width;
		OpcjeRozdzielczosc4 << rozdz[lr[1]].height;

		OpRozdz[0][0] = OpcjeRozdzielczosc1.str();
		OpRozdz[0][1] = OpcjeRozdzielczosc2.str();
		OpRozdz[1][0] = OpcjeRozdzielczosc3.str();
		OpRozdz[1][1] = OpcjeRozdzielczosc4.str();

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 4.0f, 0.2f, 4.0f }, { 0.0f, -2.0f, 0.0f }, ziemia, mDirLights, false);

		if (ustaw_rozdzielczosc || ustaw_fullekran)
			Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 0.6f, 0.2f, 0.4f }, { 0.0f, -1.998f, 0.0f }, ogien, mDirLights, false);

		suwak({ 1.1f + PosSuwakX, -1.9999f, -0.2f }, { Tlx[0] + PosSuwakX, -1.9998f, -0.205f }, proj, view, ScaleSuwak);

		napis.KlawMenu(frame, CzerwonyNapis);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
		EndD3D();
	}

	if (scen[0] == 17)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		ustawienia[0] = FALSE;
		ustawienia[2] = TRUE;
		ustawienia[3] = FALSE;
		ustawienia[1] = FALSE;

		scen[1] = 3;

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 4.0f, 0.2f, 4.0f }, { 0.0f, -2.0f, 0.0f }, ziemia, mDirLights, false);
		napis.KlawMenu(frame, CzerwonyNapis);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
		suwak({ 1.1f + PosSuwakX, -1.9999f, 0.0f }, { Tlx[1] + PosSuwakX, -1.9998f, -0.005f }, proj, view, ScaleSuwak);
		suwak({ 1.1f + PosSuwakX, -1.9999f, -0.2f }, { Tlx[2] + PosSuwakX, -1.9998f, -0.205f }, proj, view, ScaleSuwak);

		EndD3D();
	}

	if (scen[0] == 18)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 4.0f, 0.2f, 4.0f }, { 0.0f, -2.0f, 0.0f }, ziemia, mDirLights, false);

		if (scen[1] == 2)
			suwak({ 1.1f, -1.9999f, -0.2f }, { Tlx[0], -1.999f, -0.205f }, proj, view, ScaleSuwak);

		else if (scen[1] == 3)
		{
			suwak({ 1.1f, -1.9999f, 0.0f }, { Tlx[1], -1.999f, -0.005f }, proj, view, ScaleSuwak);
			suwak({ 1.1f, -1.9999f, -0.2f }, { Tlx[2], -1.999f, -0.205f }, proj, view, ScaleSuwak);
		}

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 2.0f, 0.2f, 0.6f }, { 0.0f, -1.989f, -0.1f }, ogien, mDirLights, false);
		napis.KlawMenu(frame, CzerwonyNapis);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);

		EndD3D();
	}

	if (scen[0] == 19)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		if (wczytyw == 0)
			ObjektyScen12(proj, view, ObjektyScen12X, ScaleGuzik);

		else if (wczytyw == 1)
		{
			mSky->Draw(md3dImmediateContext, proj, view, obr.x1, obr.y1, 20.0f, 0.0f, 0.0f, 5.0f);

			for (INT i = 0; i < 4; i++)
			{
				nap[3][i].x1 = nap[3][i].x2;
				nap[3][i].y1 = nap[3][i].y2;
			}
		}

		for (INT framik = 0; framik <= IloscPlikow[0][0]; framik++)
			if (frame[16] == framik)
				CzerwonyPliki[0][framik] = 1;
			else if (frame[16] != framik)
				CzerwonyPliki[0][framik] = 0;

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 1.2f, 0.2f, 1.4f }, { 0.0f, -1.97f, 0.0f }, ogien, mDirLights, false);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);

		EndD3D();
	}

	if (scen[0] == 20)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		ObjektyScen12(proj, view, ObjektyScen12X, ScaleGuzik);

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 2.0f, 0.2f, 0.8f }, { 0.0f, -1.99f, -0.1f }, ogien, mDirLights, false);

		napis.KlawMenu(frame, CzerwonyNapis);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);

		EndD3D();
	}

	if (scen[0] == 21)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		ustawienia[0] = TRUE;
		zapis[0] = TRUE;
		ustawienia[1] = TRUE;
		zapis[1] = TRUE;
		ustawienia[2] = FALSE;
		ustawienia[3] = FALSE;

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 4.0f, 0.2f, 4.0f }, { 0.0f, -2.0f, 0.0f }, ziemia, mDirLights, false);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
		napis.KlawMenu(frame, CzerwonyNapis);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndD3D();
	}

	if (scen[0] == 22)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		ustawienia[1] = TRUE;
		ustawienia[0] = FALSE;
		ustawienia[2] = FALSE;
		ustawienia[3] = FALSE;

		scen[1] = 4;

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 4.0f, 0.2f, 4.0f }, { 0.0f, -2.0f, 0.0f }, ziemia, mDirLights, false);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);
		napis.KlawMenu(frame, CzerwonyNapis);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		EndD3D();
	}

	if (scen[0] == 23)
	{
		BeginD3D();

		XMMATRIX proj = XMLoadFloat4x4(&mProj);
		XMMATRIX view = XMLoadFloat4x4(&mView);

		Draw(md3dImmediateContext, mLightView, mLightProj, mDepthStencilView, mScreenViewport, mRenderTargetView, proj, view, TPCam, Skydome, pierw, scen[0], obr_sciany, punkty_string2, strzaly_string2, sekundy_s, po, nowy, poz_scian, kla, klaw[kla].nazwa_klawisza, pierwszy, ustaw_klaw, ustaw_rozdzielczosc, OpRozdz, rozdzielczosc_w, rozdzielczosc_h, full, fullekr, ustaw_fullekran, scen[1], wczytyw, IloscPlikow, pliki, pliki2, zapxx, CzerwonyNapis, CzerwonyPliki, dt, mDirLights, jas_sw, jas_diff, Anim, weapons, strzaly, ScaleMenu4, vGraNapisy, MenuStworzLabX, OpcjeNapX, Grafika1Pos, OpcjeDzwiekuX, ZywoplotT);

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 4.0f, 0.2f, 4.0f }, { 0.0f, -2.0f, 0.0f }, ziemia, mDirLights, false);

		Objekt(md3dImmediateContext, jas_sw, jas_diff, proj, view, { 2.0f, 0.2f, 0.6f }, { 0.0f, -1.989f, -0.1f }, ogien, mDirLights, false);
		napis.KlawMenu(frame, CzerwonyNapis);
		muza.dzwiek_menu(OdtwDzwMen, Odtw, glosnosc_efektow);

		EndD3D();
	}
}