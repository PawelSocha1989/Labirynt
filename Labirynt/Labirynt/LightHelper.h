#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <Windows.h>
#include "FX11\DirectX\Include\xnamath.h"

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	FLOAT Pad;
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; 
	XMFLOAT4 Reflect;
};

#endif