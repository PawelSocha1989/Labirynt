#include "MathHelper.h"

const FLOAT MathHelper::Infinity = FLT_MAX;
const FLOAT MathHelper::Pi = 3.1415926535f;

FLOAT MathHelper::RandF()
{
	return (FLOAT)(rand()) / (FLOAT)RAND_MAX;
}

FLOAT MathHelper::RandF(FLOAT a, FLOAT b)
{
	return a + RandF()*(b - a);
}

XMMATRIX MathHelper::InverseTranspose(CXMMATRIX M)
{
	XMMATRIX A = M;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));
}

