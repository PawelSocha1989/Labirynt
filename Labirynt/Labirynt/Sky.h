#ifndef SKY_H
#define SKY_H

#include "d3dUtil.h"
#include "GeometryGenerator.h"
#include "Vertex.h"
#include "Effects.h"

class Sky
{
public:
	Sky(ID3D11Device* device, const std::wstring& cubemapFilename);
	~Sky();

	ID3D11ShaderResourceView* CubeMapSRV();

	void Draw(ID3D11DeviceContext* dc, XMMATRIX &mProj, XMMATRIX &mView, FLOAT obrx, FLOAT obry, FLOAT Scalef, FLOAT Tx, FLOAT Ty, FLOAT Tz);

private:
	Sky(const Sky& rhs);

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11ShaderResourceView* mCubeMapSRV;

	UINT mIndexCount;
};

#endif