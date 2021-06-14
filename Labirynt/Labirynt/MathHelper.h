#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <Windows.h>
#include <FLOAT.h>
#include <cmath>
#include "FX11\DirectX\Include\xnamath.h"

class MathHelper
{
public:

	static FLOAT RandF();
	static FLOAT RandF(FLOAT a, FLOAT b);
	template<typename T>static T Min(const T& a, const T& b);
	template<typename T>static T Max(const T& a, const T& b);

	static XMMATRIX InverseTranspose(CXMMATRIX M);

	static const FLOAT Infinity;
	static const FLOAT Pi;
};

template<typename T>inline T MathHelper::Min(const T & a, const T & b)
{
	return a < b ? a : b;
}

template<typename T>inline T MathHelper::Max(const T & a, const T & b)
{
	return a > b ? a : b;
}

#endif